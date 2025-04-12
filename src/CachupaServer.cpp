#include "CachupaServer.h"
#include <sstream>
using networking::Connection;
using networking::Message;
using networking::Server;
using std::function, std::bind, std::optional, std::deque, 
std::vector, std::string;
#define startOrJoinRoomUserPrompt "Please enter a room ID to join an already existing room or create a new room(type !newroom). Type !help to display the possible commands :"
#define newRoomCreated "Your room was created successfully. Please use the room ID (case-sensitive) given below to invite players to your room. :)"
#define helpMSG "!newroom: create a new room \n !exitroom: exit room \n !newUserName: change your user name \n!roomID: get ID of the current room \n!roomPasscode: get passcode for the current room \n!help: show all the commands \n"

std::string CachupaServer::getHTTPMessage(const char *htmlLocation)
{
    if (access(htmlLocation, R_OK) != -1)
    {
        std::ifstream infile{htmlLocation};
        return std::string{std::istreambuf_iterator<char>(infile),
                           std::istreambuf_iterator<char>()};
    }

    std::cerr << "Unable to open HTML index file:\n"
              << htmlLocation << "\n";
    std::exit(-1);
}

CachupaServer::CachupaServer(int port, const char *httpFile)
{
    auto onConnect = [this](Connection c){ 
        std::cout << "Connected:" << c.id << std::endl; 
        SendMsgClient(c,startOrJoinRoomUserPrompt);
    };
    auto onDisconnect = [](Connection c)
    { std::cout << "Disconnected:" << c.id << std::endl; };
    srvr = std::make_unique<Server>(port, getHTTPMessage(httpFile), onConnect
    , onDisconnect);
    playersNames = std::make_unique<unordered_map<uintptr_t, PlayerName>>();
    mainLoop();
}

void CachupaServer::mainLoop()
{
    int i = 0;
    while (true)
    {
        srvr->update();
        const auto incoming = srvr->receive();
        auto incomingResult = processIncoming(incoming);
        // Shut down message
        sleep(1);
        ++i;
    }
}
void CachupaServer::handleRoomInput(Room& room, const Connection& client, const std::string& input, std::vector<Event>& events) {
    if (room.getOwner() == client && input == "startgame") {
        // Send message to all clients in the room
        SendMsgClient(room.getClientList(), "The game is starting!");

        // Create an event for GAME_STARTED
        Event event(EventType::PLAYED_TURN);
        event.setEventDataField("roomID", room.getRoomID());
        event.setEventDataField("action", "startgame");
        events.push_back(event);
    } else {
        // Broadcast the input as a chat message to the room
        SendMsgClient(room.getClientList(), input);

        // Add a chat message event
        Event event(EventType::PLAYED_TURN);
        event.setEventDataField("roomID", room.getRoomID());
        event.setEventDataField("message", input);
        event.setEventDataField("sender", std::to_string(client.id));
        events.push_back(event);
    }
}

Room* CachupaServer::findRoomByConnection(const Connection& connection) {
    for (auto& room : roomList) {
        // Check if the connection exists in the room's client list
        const auto& clients = room.getClientList();
        if (std::find(clients.begin(), clients.end(), connection) != clients.end()) {
            return &room; // Return pointer to the room
        }
    }
    return nullptr; // Return nullptr if connection is not found in any room
}
vector<Event> CachupaServer::processIncoming(const deque<Message> &incoming)
{    vector<Event> events;

    for (const auto& message : incoming) {
        const auto& client = message.connection;
        const auto& text = message.text;

        if (text.empty()) {
            // Skip empty messages
            std::cerr << "Received empty message from connection " << client.id << std::endl;
            continue;
        }

        if (text.substr(0, 1) == "!") { // Command
            // Process commands (e.g., !newroom, !joinroom)
            Event commandEvent = commandHandler(message);
            events.push_back(commandEvent);
        } else {
            // Non-command input (e.g., game-related input or chat messages)
            Room* room = findRoomByConnection(client);

            if (room) {
                // Handle game-related or room-specific input
                handleRoomInput(*room, client, text, events);
            } else {
                // Client is not in a room
                std::cerr << "Message received from client not in a room: " << client.id << std::endl;
                SendMsgClient(client, "You are not in a room. Use !joinroom or !newroom to join/create one.");
            }
        }
    }

    return events;

}

void CachupaServer::SendMsgClient(Connection recipient, const string &log)
{
    deque<Message> outgoing;
    outgoing.push_back({recipient, log});
    srvr->send(outgoing);
}

void CachupaServer::SendMsgClient(const vector<Connection> &recipients
, const string &log)
{
    deque<Message> outgoing;
    for (auto recp : recipients)
    {
        outgoing.push_back({recp, log});
    }
    srvr->send(outgoing);
}

/**
 * @brief send a message to all client in the room using SendMsgClient
 * @param roomID to identify the room
 * @param message being sent to all clients
*/
void CachupaServer::sendMessageToRoom(std::string roomID, const std::string& message) {
    // Lambda to find the client list for a given roomID
    auto findClientList = [](const std::vector<Room>& roomList, const std::string& roomID) -> std::vector<Connection> {
        auto it = std::find_if(roomList.begin(), roomList.end(), [&roomID](const Room& room) {
            return room.getRoomID() == roomID;
        });
        if (it != roomList.end()) {
            return it->getClientList();
        }
        return {}; // Return empty client list if roomID is not found
    };

    // Find the client list for the given roomID
    std::vector<Connection> clientList = findClientList(roomList, roomID);
    // Match to the multiple target send message function
    SendMsgClient(clientList, message);
}
/**
 * @brief create a room with a client and return the ID of the room
 * @param client to hold the room
*/
std::string CachupaServer::createNewRoom(std::string password, Connection owner){
    Room newRoom("Room" + std::to_string(roomList.size() + 1 ), password, owner);
    newRoom.addClient(owner);
    roomList.push_back(newRoom);
    return newRoom.getRoomID();
}
vector<string> parseCommand(string str){
    vector<string> v;
 
    string s;
 
    // constructing stream from the string
    std::stringstream ss(str);
 
    // using while loop until the getline condition is
    // satisfied
    // ' ' represent split the string whenever a space is
    // found in the original string
    while (getline(ss, s, ' ')) {
        // store token string in the vector
        v.push_back(s);
    }
    return v;
}
Event CachupaServer::commandHandler(const Message msg){
    std::string command = msg.text;
    vector<string> parsedcommand = parseCommand(command);
    Connection client = msg.connection;
    if ( parsedcommand[0] == "!newroom"){
        std::string returnMSG = "Room ID: " + createNewRoom("random",client);
        SendMsgClient(client,returnMSG);
        Event event = Event{EventType::JOINED_GAME,unordered_map<string,string>{{"",""}}};
        return event;
    }
    else if ( parsedcommand[0] == "!shutdown")  { 
        //Send shutdown signal
        std::string returnMSG = "Shutting down server.";
        SendMsgClient(client,returnMSG);
        Event event = Event{EventType::SHUTDOWN,unordered_map<string,string>{{"",""}}};
        return event;
    }
    else if (parsedcommand[0] == "!help"){
        // Print commands
        std::string returnMSG = helpMSG;
        SendMsgClient(client,returnMSG);
        Event event = Event{EventType::HELP,unordered_map<string,string>{{"",""}}};
        return event;
    }
    else if (parsedcommand[0] == "!exitroom"){
        // Remove client from their room and print welcome prompt again
        std::string returnMSG = "Exiting room.";
        SendMsgClient(client,returnMSG);
        Event event = Event{EventType::SHUTDOWN,unordered_map<string,string>{{"",""}}};
        return event;
    }
    else if( parsedcommand[0] == "!roomID"){
        // send message consist of room ID
        Room* rm = findRoomByConnection(client);
        std::string returnMSG;
        if (rm != nullptr)
            returnMSG = "Room ID is " + rm->getRoomID();
        else 
            returnMSG = "You are not in a room";
        SendMsgClient(client,returnMSG);
        Event event = Event{EventType::HELP,unordered_map<string,string>{{"",""}}};
        return event;
    }
    else if (parsedcommand[0] == "!roomPasscode"){
        // send message consist of room passcode, fail if client has no room
        Room* rm = findRoomByConnection(client);
        std::string returnMSG;
        if (rm != nullptr)
            returnMSG = "Room ID is " + rm->getPassword().pw;
        else 
            returnMSG = "You are not in a room";
        SendMsgClient(client,returnMSG);
        Event event = Event{EventType::HELP,unordered_map<string,string>{{"",""}}};
        return event;
    }
    else if ( parsedcommand[0] == "!joinroom"){
        Event event = Event{EventType::NOT_FOUND,unordered_map<string,string>{{"",""}}};
        return event;
    }
    else {
        std::string returnMSG = "Command not found.";
        SendMsgClient(client,returnMSG);
        Event event = Event{EventType::NOT_FOUND,unordered_map<string,string>{{"",""}}};
        return event;
    }
}