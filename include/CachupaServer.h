#pragma once

#include "Network/Server.h"
#include "Support.h"
#include <numeric>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <memory>
#include <string>
#include <optional>
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include "Controllers/Event.h"
#include "Room.h"

using networking::Connection;
using networking::Message;
using networking::Server;
using std::optional, std::bind, std::queue,std::deque, std::vector, std::string,std::unordered_map;
using RoomId = StrongAlias<string, struct RoomIdTag>;

using Client = StrongAlias<Connection, struct ClientTag>;
using PlayerName = StrongAlias<string, struct PlayerNameTag>;
using ClientList = vector<Client>;

//! A class for the server
class CachupaServer
{
public:
  // Public wrappers
    void sendMessageToClient(Connection recipient, const string &message) {
        SendMsgClient(recipient, message);
    }

    void sendMessageToClients(const vector<Connection> &recipients, const string &message) {
        SendMsgClient(recipients, message);
    }

    void sendRoomMessage(const std::string &roomID, const std::string &message) {
        sendMessageToRoom(roomID, message);
    }

    Room* getRoomByConnection(const Connection &connection) {
        return findRoomByConnection(connection);
    }
    const std::vector<Room>& getRooms() const {
        return roomList;
    }
private:
    std::unique_ptr<Server> srvr;
    std::unique_ptr<unordered_map<uintptr_t,PlayerName>> playersNames;

    /*! A list of rooms */
    std::vector<Room> roomList;
    //! Room related methods
    void sendMessageToRoom(std::string RoomID, const std::string& message);
    std::string createNewRoom(std::string password, Connection owner);

    //! Input handlers
    Event commandHandler(const Message msg);

    std::string getHTTPMessage(const char *htmlLocation);
    void mainLoop();

    void handleRoomInput(Room &room, const Connection &client, const std::string &input, std::vector<Event> &events);

    Room *findRoomByConnection(const Connection &connection);

    // Sending msg to a single client
    void SendMsgClient(Connection recipient, const string &log);
    // Multiple clients
    void SendMsgClient(const vector<Connection> &recipients, const string &log);
    vector<Event> processIncoming(const deque<Message> &incoming);

public:
    CachupaServer(int port, const char *httpFile);
    bool isRunning() { return true; } // TODO: Add a mechanism for server to shutdown gracefully after 
    //all users disconnect and host shuts it down
};