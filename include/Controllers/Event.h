#pragma once
#include "../Support.h"
#include <string>
#include <memory>
#include <unordered_map>

using std::unordered_map,std::string;

enum EventType {
    JOINED_GAME = 1,
    EXITED_GAME ,
    PLAYED_TURN ,
    SELECTED_GAME,
    SHUTDOWN,
    NOT_FOUND,
    HELP
};

class Event {
public:
    Event(EventType et,const unordered_map<string,string>& data):et(et){event_data = std::make_unique<unordered_map<string,string>>(data);}
    Event(EventType et):et(et){event_data = std::make_unique<unordered_map<string,string>>();}
    Event(const Event &ev):et(ev.et) {
        event_data = std::make_unique<unordered_map<string,string>>();
        std::copy(ev.event_data->begin(),ev.event_data->end(),std::inserter(*event_data,event_data->end()));
    }
    EventType getEventType() const {return et;}
    void setEventDataField(string key,string val) { (*event_data)[key] = val; }
    string getEventDataField(string key) {return (*event_data)[key];}
private:
    EventType et;
    std::unique_ptr<unordered_map<string,string>> event_data;
};