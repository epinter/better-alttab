#include "Config.h"
#include "SimpleIni.h"

namespace betteralttab {
    Config::Config() {
        CSimpleIni ini;
        ini.SetQuotes(true);
        ini.SetAllowKeyOnly(false);
        ini.SetUnicode(true);
        ini.SetMultiKey(false);
        std::string fileName = std::string("Data\\SKSE\\Plugins\\").append(CONFIG_FILE);

        bool save = false;
        if (ini.LoadFile(fileName.c_str()) == 0) {
            debugEnabled = ini.GetBoolValue(INI_SECTION, OPT_DEBUG, debugEnabled);
            cursorEnabled = ini.GetBoolValue(INI_SECTION, OPT_CURSOR, cursorEnabled);
            audioEnabled = ini.GetBoolValue(INI_SECTION, OPT_AUDIO, audioEnabled);
        } else {
            logger::error("Can't open config file '{}'. Trying to create a new.", CONFIG_FILE);
        }

        if (!ini.KeyExists(INI_SECTION, OPT_DEBUG)) {
            ini.SetBoolValue(INI_SECTION, OPT_DEBUG, debugEnabled, nullptr, false);
            save = true;
        }
        if (!ini.KeyExists(INI_SECTION, OPT_CURSOR)) {
            ini.SetBoolValue(INI_SECTION, OPT_CURSOR, cursorEnabled, nullptr, true);
            save = true;
        }
        if (!ini.KeyExists(INI_SECTION, OPT_AUDIO)) {
            ini.SetBoolValue(INI_SECTION, OPT_AUDIO, audioEnabled, nullptr, true);
            save = true;
        }

        if (save) {
            ini.SaveFile(fileName.c_str());
        }

        logger::info("Config: {}='{}'; {}='{}'; {}='{}';", OPT_DEBUG, debugEnabled, OPT_CURSOR, cursorEnabled, OPT_AUDIO, audioEnabled);
    }

    Config& Config::getInstance() {
        static Config instance;
        return instance;
    }

    bool Config::isDebugEnabled() {
        return debugEnabled;
    }

    bool Config::isCursorEnabled() {
        return cursorEnabled;
    }

    void Config::setCursorEnabled(bool b) {
        cursorEnabled = b;
    }

    bool Config::isAudioEnabled() {
        return audioEnabled;
    }

}  // namespace betteralttab