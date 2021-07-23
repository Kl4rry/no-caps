#include <windows.h>
#include <stdio.h>
#include <ctype.h>

void press_caps() {
    INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = 0x14;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

    ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) (lParam);

	if(wParam == WM_KEYDOWN || wParam  == WM_SYSKEYDOWN) {
        if(p->vkCode == 0x14) {
            if ((GetKeyState(VK_CAPITAL) & 0x0001) == 0) {
                press_caps();
            } else {
                return 0;
            }
        }
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
}



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, hInstance, 0);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(keyboardHook);
	return 0;
}