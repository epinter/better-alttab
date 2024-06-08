#include "WindowProc.h"
#include <detours/detours.h>
#include "Xaudio.h"
#include "Config.h"

/*
Copyright (c) 2024 Emerson Pinter

This file is part of Better AltTab for Skyrim <https://github.com/epinter/better-alttab>.
Licensed under LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.txt>.
*/

namespace betteralttab {
    WindowProc &WindowProc::getInstance() {
        static WindowProc instance;
        return instance;
    }

    HWND WindowProc::getHwnd() {
        return hwnd;
    }

    LRESULT CALLBACK WindowProc::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        LRESULT res = CallWindowProc(reinterpret_cast<WNDPROC>(original), hwnd, uMsg, wParam, lParam);

        if (uMsg == WM_ACTIVATEAPP) {
            WORD active = LOWORD(wParam);
            if (active == 1) {
                logger::debug("window active");
                Xaudio::getInstance().startEngine();
            } else {
                logger::debug("window inactive");
                Xaudio::getInstance().stopEngine();
            }
            logger::debug("hwnd:{}; uMsg:{}; wParam:{}; lParam:{};", reinterpret_cast<std::uintptr_t>(hwnd), uMsg, wParam, lParam);
        }

        return res;
    }

    void WindowProc::toggleCursor(uintptr_t param1, bool state) {
        // logger::debug("toggleCursor param1:{} state:{}", *reinterpret_cast<int32_t *>(param1 + 0x2c), state);
        if (GetForegroundWindow() == WindowProc::getInstance().getHwnd()) {
            state = false;
        } else {
            state = true;
        }
        originalToggleCursor(param1, state);
    }

    bool WindowProc::installToggleCursorHook() {
        if (!Config::getInstance().isCursorEnabled()) {
            return false;
        }

        REL::RelocationID function = REL::RelocationID(80429, 82541);
        auto &trampoline = SKSE::GetTrampoline();
        trampoline.create(64);

        originalToggleCursor = reinterpret_cast<originalToggleCursorFn>(function.address());
        if (DetourTransactionBegin() == NO_ERROR && DetourUpdateThread(GetCurrentThread()) == NO_ERROR &&
            DetourAttach(&reinterpret_cast<PVOID &>(originalToggleCursor), toggleCursor) == NO_ERROR &&
            DetourTransactionCommit() == NO_ERROR) {
            logger::info("ToggleCursor hook installed at address 0x{:08X}", function.offset());
        } else {
            logger::error("Failed to install toggleCursor hook");
        }
        return true;
    }

    bool WindowProc::installWndProcHook(HWND h) {
        hwnd = h;
        original = SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(windowProc));

        if (original != 0) {
            logger::info("WindowProc hook installed");
            return true;
        }

        logger::error("Failed to hook WindowProc");
        return false;
    }
}  // namespace betteralttab