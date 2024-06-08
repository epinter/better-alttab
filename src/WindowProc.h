#pragma once

/*
Copyright (c) 2024 Emerson Pinter

This file is part of Better AltTab for Skyrim <https://github.com/epinter/better-alttab>.
Licensed under LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.txt>.
*/

namespace betteralttab {
    class WindowProc {
    private:
        bool loading = true;
        HWND hwnd = 0;
        static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static inline LONG_PTR original;

    public:
        bool installToggleCursorHook();
        static void __fastcall toggleCursor(uintptr_t param1, bool state);
        using originalToggleCursorFn = decltype(&toggleCursor);
        static inline originalToggleCursorFn originalToggleCursor;

        [[nodiscard]] static WindowProc& getInstance();
        bool installWndProcHook(HWND h);
        HWND getHwnd();
    };
}  // namespace betteralttab