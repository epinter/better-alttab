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

    void WindowProc::hideCursor() {
        if (!Config::getInstance().isCursorEnabled())
            return;
        started = true;

        CURSORINFO cursorInfo{};
        cursorInfo.cbSize = sizeof(CURSORINFO);
        GetCursorInfo(&cursorInfo);
        logger::debug("cursor state - IsHCursorNull:{}; visible:{};", cursorInfo.hCursor == nullptr, cursorInfo.flags);
        if (cursorInfo.hCursor != nullptr) {
            SetCursor(NULL);
            logger::debug("cursor set to null and hidden");
        }
        if (cursorInfo.flags == CURSOR_SHOWING) {
            int cursorCount = ShowCursor(false);
            logger::debug("ShowCursor count:{};", cursorCount);
        }
    }

    void WindowProc::showCursor(bool foreground) {
        if (!Config::getInstance().isCursorEnabled() || foreground || !started)
            return;

        CURSORINFO cursorInfo{};
        cursorInfo.cbSize = sizeof(CURSORINFO);
        GetCursorInfo(&cursorInfo);

        if (cursorInfo.hCursor == nullptr) {
            SetCursor(static_cast<HCURSOR>(LoadImage(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)));
            logger::debug("cursor set to arrow and visible");
        }
        if (cursorInfo.flags == 0) {
            int cursorCount = ShowCursor(true);
            logger::debug("ShowCursor count:{};", cursorCount);
        }
    }

    LRESULT __fastcall WindowProc::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_ACTIVATEAPP) {
            using namespace RE::BSGraphics;
            WORD active = LOWORD(wParam);
            if (Config::getInstance().isCursorEnabled() && Renderer::GetSingleton() != nullptr &&
                Renderer::GetSingleton()->data.fullScreen) {
                logger::info("fullscreen detected, disabling cursor feature");
                Config::getInstance().setCursorEnabled(false);
            }

            if (active == 1) {
                logger::debug("window active");
                WindowProc::getInstance().hideCursor();
                Xaudio::getInstance().startEngine();
            } else {
                logger::debug("window inactive");
                Xaudio::getInstance().stopEngine();
            }
            logger::debug("hwnd:{}; uMsg:{}; wParam:{}; lParam:{};", reinterpret_cast<std::uintptr_t>(hwnd), uMsg, wParam, lParam);
        } else if (uMsg == WM_SETCURSOR) {
            WindowProc::getInstance().showCursor(GetForegroundWindow() == hwnd);
        }

        return original(hwnd, uMsg, wParam, lParam);
    }

    bool WindowProc::installHook() {
        REL::RelocationID function = REL::RelocationID(WindowProc::OFFSET_FUNC_SE, WindowProc::OFFSET_FUNC_AE);
        original = function.address();

        if (DetourTransactionBegin() == NO_ERROR && DetourUpdateThread(GetCurrentThread()) == NO_ERROR &&
            DetourAttach(&reinterpret_cast<PVOID &>(original), windowProc) == NO_ERROR && DetourTransactionCommit() == NO_ERROR) {
            logger::info("WindowProc hook installed at address 0x{:08X}", function.offset());
            return true;
        }

        logger::error("Failed to hook WindowProc at address 0x{:08X}", function.offset());
        return false;
    }
}  // namespace betteralttab