#include <Windows.h>
#include <imm.h>

#include <iostream>
#include <string>

#ifndef IMC_GETCONVERSIONMODE
#define IMC_GETCONVERSIONMODE 0x0001
#endif

// Get the current IME conversion mode for the foreground window.
// Returns true if NATIVE (Chinese) mode is active.
bool IsChineseMode() {
  HWND hwnd = GetForegroundWindow();
  if (!hwnd) return false;

  HWND hIme = ImmGetDefaultIMEWnd(hwnd);
  if (!hIme) return false;

  LRESULT mode = SendMessage(hIme, WM_IME_CONTROL, IMC_GETCONVERSIONMODE, 0);
  return (mode & IME_CMODE_NATIVE) != 0;
}

// Simulate a Shift key press to toggle Microsoft Pinyin's Chinese/English mode.
// This is the only reliable method for Electron/Chromium apps (e.g. VSCode)
// because WM_IME_CONTROL + IMC_SETCONVERSIONMODE is ignored by TSF-based apps.
void SimulateShiftToggle() {
  INPUT inputs[2] = {};

  // Key down
  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.wVk = VK_SHIFT;

  // Key up
  inputs[1].type = INPUT_KEYBOARD;
  inputs[1].ki.wVk = VK_SHIFT;
  inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(2, inputs, sizeof(INPUT));
}

void GetImeMode() { std::cout << (IsChineseMode() ? "zh" : "en") << std::endl; }

void SetImeMode(bool toChinese) {
  bool currentIsChinese = IsChineseMode();
  // Only toggle if the current mode differs from the desired mode
  if (currentIsChinese != toChinese) {
    SimulateShiftToggle();
  }
}

void PrintHelp() {
  std::cout << "Usage:\n"
            << "  imetool        -> Get current mode (outputs 'en' or 'zh')\n"
            << "  imetool en     -> Set to English mode\n"
            << "  imetool zh     -> Set to Chinese mode\n";
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    GetImeMode();
    return 0;
  }

  std::string arg = argv[1];

  if (arg == "en") {
    SetImeMode(false);
  } else if (arg == "zh") {
    SetImeMode(true);
  } else {
    PrintHelp();
  }

  return 0;
}
