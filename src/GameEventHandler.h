#pragma once
#include "SkseMessagingListener.h"

namespace betteralttab {
    class GameEventHandler : public SkseMessagingListener {
        private:
            GameEventHandler() = default;
            GameEventHandler(const GameEventHandler&) = delete;
            GameEventHandler& operator=(const GameEventHandler&) = delete;

        public:
            [[nodiscard]] static GameEventHandler& getInstance() {
                static GameEventHandler instance;
                return instance;
            }

            void onLoad() override;
            void onDataLoaded() override;
    };

}  // namespace betteralttab
