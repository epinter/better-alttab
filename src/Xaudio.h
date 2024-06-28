#pragma once
#include <dx2010/XAudio2.h>

/*
Copyright (c) 2024 Emerson Pinter

This file is part of Better AltTab for Skyrim <https://github.com/epinter/better-alttab>.
Licensed under LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.txt>.
*/

namespace betteralttab {
    class Xaudio {
        private:
            IXAudio2* pXAudio2;
            bool xaudioInitialized = false;
            static constexpr int OFFSET_FUNC_SE = 66746;
            static constexpr int OFFSET_FUNC_AE = 67952;
            static constexpr int OFFSET_INST_SE = 0x44;
            static constexpr int OFFSET_INST_AE = 0xb7;
            static constexpr const SKSE::stl::nttp::string PATTERN_AE = "33 D2 48 ?? ?? ?? ?? ?? ?? FF 15 ?? ?? ?? ?? 8B F0";
            static constexpr int PATTERN_AE_OFF = -9;
            static constexpr const SKSE::stl::nttp::string PATTERN_SE = "33 D2 44 ?? ?? ?? FF 15 ?? ?? ?? ?? 85 C0";
            static constexpr int PATTERN_SE_OFF = -6;

            Xaudio() = default;
            Xaudio(const Xaudio&) = delete;
            Xaudio& operator=(const Xaudio&) = delete;
            void setXAudio2(IXAudio2* p);
            void setXaudioInitialized(bool b);

            static HRESULT __fastcall xaudioCreate(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv);
            static inline REL::Relocation<decltype(xaudioCreate)> originalXaudioCreate;

        public:
            [[nodiscard]] static Xaudio& getInstance();
            void installHook();
            void startEngine() const;
            void stopEngine() const;
    };
}  // namespace betteralttab