#include "Xaudio.h"
#include "Config.h"

/*
Copyright (c) 2024 Emerson Pinter

This file is part of Better AltTab for Skyrim <https://github.com/epinter/better-alttab>.
Licensed under LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.txt>.
*/

namespace betteralttab {
    Xaudio& Xaudio::getInstance() {
        static Xaudio instance;
        return instance;
    }

    void Xaudio::setXAudio2(IXAudio2* p) {
        pXAudio2 = p;
    }

    void Xaudio::startEngine() const {
        if (xaudioInitialized && pXAudio2) {
            logger::debug("XAudio2: starting engine");
            HRESULT res = pXAudio2->StartEngine();
            if (res != S_OK) {
                logger::error("Error starting XAudio2 audio processing thread.");
            }
        }
    }

    void Xaudio::stopEngine() const {
        if (xaudioInitialized && pXAudio2) {
            logger::debug("XAudio2: stopping engine");
            pXAudio2->StopEngine();
        }
    }

    HRESULT __fastcall Xaudio::xaudioCreate(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv) {
        HRESULT hr = originalXaudioCreate(rclsid, pUnkOuter, dwClsContext, riid, ppv);

        if (SUCCEEDED(hr)) {
            Xaudio::getInstance().setXaudioInitialized(true);
            Xaudio::getInstance().setXAudio2(reinterpret_cast<IXAudio2*>(*ppv));
            logger::debug("XAudio2 initialized");
        } else {
            logger::error("XAudio2 initialization failed");
        }
        return hr;
    }

    void Xaudio::setXaudioInitialized(bool b) {
        xaudioInitialized = b;
    }

    void Xaudio::installHook() {
        if (!Config::getInstance().isAudioEnabled())
            return;

        REL::RelocationID function = REL::RelocationID(Xaudio::OFFSET_FUNC_SE, Xaudio::OFFSET_FUNC_AE);
        int instOffset = REL::Relocate(Xaudio::OFFSET_INST_SE, Xaudio::OFFSET_INST_AE);
        uintptr_t call = function.address() + instOffset;

        if (REL::Module::IsAE() && !REL::make_pattern<PATTERN_AE>().match(function.address() + instOffset + PATTERN_AE_OFF)) {
            logger::error("XAudio2 hook not installed, address didn't match (address 0x{:08X}, offset 0x{:04X})", function.offset(),
                          instOffset);
            return;
        } else if (REL::Module::IsSE() && !REL::make_pattern<PATTERN_SE>().match(call + PATTERN_SE_OFF)) {
            logger::error("XAudio2 hook not installed, address didn't match (address 0x{:08X}, offset 0x{:04X})", function.offset(),
                          instOffset);
            return;
        }

        auto& trampoline = SKSE::GetTrampoline();
        trampoline.create(64);

        originalXaudioCreate = *reinterpret_cast<uintptr_t*>(trampoline.write_call<6>(call, xaudioCreate));
        logger::info("XAudio2 hook installed at address 0x{:08X}, offset 0x{:04X}", function.offset(), instOffset);
    }
}  // namespace betteralttab