#include <Windows.h>
#include <imm.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>


#ifndef IMC_GETCONVERSIONMODE
#define IMC_GETCONVERSIONMODE 0x0001
#endif
#ifndef IMC_SETCONVERSIONMODE
#define IMC_SETCONVERSIONMODE 0x0002
#endif

// Debug log to file (next to the exe)
static std::ofstream& logFile() {
  static std::ofstream f;
  if (!f.is_open()) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string p(path);
    p = p.substr(0, p.rfind('\\') + 1) + "imetool_debug.log";
    f.open(p, std::ios::app);
  }
  return f;
}

static void log(const std::string& msg) {
  auto& f = logFile();
  time_t t = time(nullptr);
  char buf[64];
  strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&t));
  f << "[" << buf << "] " << msg << std::endl;
}

void GetImeMode() {
  HWND hwnd = GetForegroundWindow();
  if (!hwnd) {
    log("GetImeMode: GetForegroundWindow() returned NULL");
    std::cout << "en" << std::endl;
    return;
  }

  char title[256] = {};
  GetWindowTextA(hwnd, title, sizeof(title));
  log(std::string("GetImeMode: foreground window='") + title + "' hwnd=0x" +
      std::to_string((uintptr_t)hwnd));

  HWND hIme = ImmGetDefaultIMEWnd(hwnd);
  if (!hIme) {
    log("GetImeMode: ImmGetDefaultIMEWnd() returned NULL");
    std::cout << "en" << std::endl;
    return;
  }

  LRESULT mode = SendMessage(hIme, WM_IME_CONTROL, IMC_GETCONVERSIONMODE, 0);
  log("GetImeMode: mode=" + std::to_string(mode) +
      " NATIVE=" + std::to_string((mode & IME_CMODE_NATIVE) ? 1 : 0));

  if (mode & IME_CMODE_NATIVE) {
    std::cout << "zh" << std::endl;
  } else {
    std::cout << "en" << std::endl;
  }
}

void SetImeMode(bool toChinese) {
  HWND hwnd = GetForegroundWindow();
  if (!hwnd) {
    log("SetImeMode: GetForegroundWindow() returned NULL");
    return;
  }

  char title[256] = {};
  GetWindowTextA(hwnd, title, sizeof(title));
  log(std::string("SetImeMode(") + (toChinese ? "zh" : "en") +
      "): foreground window='" + title + "'");

  HWND hIme = ImmGetDefaultIMEWnd(hwnd);
  if (!hIme) {
    log("SetImeMode: ImmGetDefaultIMEWnd() returned NULL");
    return;
  }

  LRESULT currentMode =
      SendMessage(hIme, WM_IME_CONTROL, IMC_GETCONVERSIONMODE, 0);
  LRESULT newMode = currentMode;

  if (toChinese) {
    newMode |= IME_CMODE_NATIVE;
  } else {
    newMode &= ~IME_CMODE_NATIVE;
  }

  log("SetImeMode: currentMode=" + std::to_string(currentMode) +
      " newMode=" + std::to_string(newMode));

  SendMessage(hIme, WM_IME_CONTROL, IMC_SETCONVERSIONMODE, newMode);
}

void ToggleImeMode() {
  HWND hwnd = GetForegroundWindow();
  if (!hwnd) return;
  HWND hIme = ImmGetDefaultIMEWnd(hwnd);
  if (!hIme) return;

  LRESULT currentMode =
      SendMessage(hIme, WM_IME_CONTROL, IMC_GETCONVERSIONMODE, 0);
  LRESULT newMode = currentMode ^ IME_CMODE_NATIVE;
  SendMessage(hIme, WM_IME_CONTROL, IMC_SETCONVERSIONMODE, newMode);
}

void PrintHelp() {
  std::cout << "Usage:\n"
            << "  imetool        -> Get current mode (outputs 'en' or 'zh')\n"
            << "  imetool en     -> Set to English mode\n"
            << "  imetool zh     -> Set to Chinese mode\n"
            << "  imetool toggle -> Toggle between English and Chinese\n";
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
  } else if (arg == "toggle") {
    ToggleImeMode();
  } else {
    PrintHelp();
  }

  return 0;
}
