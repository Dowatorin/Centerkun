#include <windows.h>
#include <iostream>

HHOOK keyboardHook;

// Function to move the active window to the center of the screen
void MoveActiveWindowToCenter() {
    HWND hwnd = GetForegroundWindow(); // Get the currently active window
    if (hwnd == NULL) {
        std::cout << "No active window found!\n";
        return;
    }

    RECT rect;
    GetWindowRect(hwnd, &rect);
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int xPos = (screenWidth - windowWidth) / 2;
    int yPos = (screenHeight - windowHeight) / 2;

    MoveWindow(hwnd, xPos, yPos, windowWidth, windowHeight, TRUE);
}

// Hook procedure to capture keyboard events
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKbdStruct = (KBDLLHOOKSTRUCT*)lParam;

        // Check for Ctrl + M (VK_CONTROL + 'M')
        if ((wParam == WM_KEYDOWN) && (pKbdStruct->vkCode == 'M') && (GetKeyState(VK_CONTROL) & 0x8000)) {
            MoveActiveWindowToCenter();
            return 1; // Prevent further processing of this key
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main() {
    // Set the keyboard hook
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
    if (keyboardHook == NULL) {
        std::cerr << "Failed to install hook!" << std::endl;
        return 1;
    }

    // Message loop to keep the program running
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unhook when done
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main();  // Calls your actual main() function
}