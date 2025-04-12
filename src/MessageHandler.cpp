#include "MessageHandler.h"
#include <iostream>
#include <jsoncpp/json/json.h>  // jsoncpp header
#include "State/Player.h"
#include "Network/Server.h"

// MessageHandler.cpp
// Intial setup for a JSON-message converter, so we can handle SocialGaming code within messages in the future.

MessageHandler::MessageHandler() {
    // For debugging purposes
    std::cout << "MessageHandler initialized." << std::endl;
    // Constructor here for initialization
}

MessageHandler::~MessageHandler() {
    // For debugging purposes
    std::cout << "MessageHandler destroyed." << std::endl;
    // Destructor here for memory cleanup
}

// Parse & convert incoming message from a raw string into a JSON object using jsoncpp
Json::Value MessageHandler::parseMessage(const std::string& rawMessage) {
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errs;

    // Basic error handling, secures it from possibly crashing

    try {
        std::istringstream ss(rawMessage);

        bool parsingSuccessful = Json::parseFromStream(builder, ss, &root, &errs);
        
        if (!parsingSuccessful) {
            throw std::runtime_error("Failed to parse message: " + errs);
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;

        // Handle error appropriately, possibly return an empty 
        // JSON object or a specific error JSON
        // Returning an empty JSON object for now
        return Json::Value();  
    }

    return root;
}

// Convert JSON object into a string to be sent using jsoncpp
std::string MessageHandler::createMessage(const Json::Value& messageData) {
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, messageData);  
}

void MessageHandler::processMessage(const Json::Value& message) {
    // Handle different message types here
    if (message.isMember("type")) {
        std::string messageType = message["type"].asString();

        if (messageType == "chat") {
            handleChatMessage(message);
        }
        else if (messageType == "gameAction") {
            handleGameAction(message);
        }
        else if (messageType == "playerJoin") {
            handleJoinRoom(message);
        }
        else {
            std::cerr << "Unknown message type: " << messageType << std::endl;
        }
    }
    else {
        std::cout << "Message format missing 'type' " << std::endl;
    }
}

// Prepare JSON message with a specific type and payload using jsoncpp
std::string MessageHandler::prepareMessage(const std::string& messageType, const Json::Value& payload) {
    Json::Value message;
    message["type"] = messageType;
    message["data"] = payload;
    
    return createMessage(message);
}

void MessageHandler::handleChatMessage(const Json::Value& message) {
    std::cout << message["data"]["text"].asString() << std::endl;
}

void MessageHandler::handleGameAction(const Json::Value& message) {
    std::cout << "Handling game action: " << message["data"].toStyledString() << std::endl;

    // This might involve updating game state, which would require interaction with GameServer or Player classes
}

void MessageHandler::handleJoinRoom(const Json::Value& message) {
    std::string playerID = message["data"]["playerID"].asString();
    std::string roomID = message["data"]["roomID"].asString();
    
    std::cout << "Player " << playerID << " is attempting to join room: " << roomID << std::endl;

    // ONLY if player join needs to be handled here
    // Obtain the GameServer instance and add the player to the room
    // std::shared_ptr<GameServer> gameServer = /* logic to get game server instance */;
    // if (gameServer) {
    //     gameServer->joinRoom(/*playerWebSocket*/, roomID);
    // } else {
    //     std::cerr << "GameServer instance not found!" << std::endl;
    // }
}

// void MessageHandler::broadcastMessageToRoom(const std::string& roomID, const std::string& message) {
//     std::shared_ptr<Room> room = /* code to get room instance by roomID */ NULL;
    
//     if (room) {
//         //Will be changed later
//         for (const auto& player : room->getPlayers()) {

//             player->sendMessage(message);
//         }
//         std::cout << "Broadcasted message to room " << roomID << ": " << message << std::endl;
//     } else {
//         std::cerr << "Room " << roomID << " not found." << std::endl;
//     }
// }

// Experimental
// void MessageHandler::handlePlayerReconnection(const std::string& playerID, const std::string& roomID) {
//     // Assuming we have access to the GameServer instance and Player instance
//     std::shared_ptr<Player> player = /* code to get player instance by playerID */;
//     std::shared_ptr<GameServer> gameServer = /* code to get game server instance */;

//     if (player) {
//         player->setState("connected");
//         std::cout << "Player " << playerID << " reconnected." << std::endl;

//         if (gameServer) {
//             // Use the player's WebSocket stream to rejoin the room
//             gameServer->joinRoom(player->getSocket(), roomID);
//             std::cout << "GameServer notified of player " << playerID << " reconnection." << std::endl;
//         } else {
//             std::cerr << "GameServer instance not found." << std::endl;
//         }
//     } else {
//         std::cerr << "Player " << playerID << " not found." << std::endl;
//     }
// }
