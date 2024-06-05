#pragma once

/*
Copyright (c) 2024 Emerson Pinter

This file is part of Better AltTab for Skyrim <https://github.com/epinter/better-alttab>.
Licensed under LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.txt>.
*/

namespace betteralttab {
    class WindowProc {
    private:
        static constexpr int OFFSET_FUNC_SE = 35635;
        static constexpr int OFFSET_FUNC_AE = 36649;

        bool started = false;
        static LRESULT __fastcall windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static inline REL::Relocation<decltype(windowProc)> original;

    public:
        [[nodiscard]] static WindowProc& getInstance();
        static bool installHook();
        void hideCursor();
        void showCursor(bool isBackground);
    };
}  // namespace betteralttab