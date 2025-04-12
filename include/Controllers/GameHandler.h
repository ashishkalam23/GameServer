#pragma once
#include "Event.h"
#include <vector>
#include "../State/Game.h"

using std::vector;

class GameEngine {
    public:
        // Handle all the events raised by the server
        void handleEvents(vector<Event> events);
    
    private:
        vector<Game> games; 
};