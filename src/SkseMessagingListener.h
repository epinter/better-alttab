

class SkseMessagingListener {
    protected:
        SkseMessagingListener() = default;

    private:
        SkseMessagingListener(const SkseMessagingListener&) = delete;
        SkseMessagingListener& operator=(const SkseMessagingListener&) = delete;

    public:
        ~SkseMessagingListener() = default;

        // This plugin is finishing load.
        virtual void onLoad(){};

        // All plugins have finished running SKSEPlugin_Load.
        virtual void onPostLoad(){};

        // All kPostLoad message handlers have run.
        virtual void onPostPostLoad(){};

        // All game data has been found.
        virtual void onInputLoaded(){};

        // All ESM/ESL/ESP plugins have loaded, main menu is now active.
        virtual void onDataLoaded(){};

        // Player starts a new game from main menu.
        virtual void onNewGame(){};

        // Player selected a game to load, but it hasn't loaded yet.
        virtual void onPreLoadGame(){};

        // Player selected save game has finished loading.
        virtual void onPostLoadGame(){};

        // Player has saved a game.
        virtual void onSaveGame(){};

        // Player deleted a saved game from within the load menu.
        virtual void onDeleteGame(){};

        template <class T>
        auto registerListener() {
            if (!SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
                    switch (message->type) {
                        case SKSE::MessagingInterface::kPostLoad:
                            T::getInstance().onPostLoad();
                            break;
                        case SKSE::MessagingInterface::kPostPostLoad:
                            T::getInstance().onPostPostLoad();
                            break;
                        case SKSE::MessagingInterface::kInputLoaded:
                            T::getInstance().onInputLoaded();
                            break;
                        case SKSE::MessagingInterface::kDataLoaded:
                            T::getInstance().onDataLoaded();
                            break;
                        case SKSE::MessagingInterface::kNewGame:
                            T::getInstance().onNewGame();
                            break;
                        case SKSE::MessagingInterface::kPreLoadGame:
                            T::getInstance().onPreLoadGame();
                            break;
                        case SKSE::MessagingInterface::kPostLoadGame:
                            T::getInstance().onPostLoadGame();
                            break;
                        case SKSE::MessagingInterface::kSaveGame:
                            T::getInstance().onSaveGame();
                            break;
                        case SKSE::MessagingInterface::kDeleteGame:
                            T::getInstance().onDeleteGame();
                            break;
                    }
                })) {
                SKSE::stl::report_and_fail("Unable to register message listener.");
            }
        }
};
