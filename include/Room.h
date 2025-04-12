#pragma once
#include "State/GameState.h"
#include "Network/Server.h"
#include <string>
using networking::Connection;
using networking::Server;
//! A Room class.
/*!
    The room class store clients Connections and map their ID to game state
*/
//! Password Struct
struct Password {
    std::string pw;
    Password(const std::string& password = "") : pw(password) {}

    bool verifyPW(const std::string& attempt) const {
        return pw == attempt;
    }
};

//! An enum for the status of methods
enum methodStatus{
    SUCCESS,
    FAILURE,
    ALREADY_EXIST,
    NON_EXISTENCE
};
class Room{
public:
    //! Constructor for Room
    Room(const std::string id, const std::string& password = "", const Connection& owner = Connection()) 
    : roomID(id), pw(password), owner(owner) {}
    /*! 
        Constructor takes in a string for identifying and password struct for security
    */
   //! Adding client method
    methodStatus addClient(const Connection& connection){
        for ( const auto& client : clientList) {
            if(client == connection) /*! The client already exists*/
                return ALREADY_EXIST;
        }
        clientList.push_back(connection);
        return SUCCESS; /*! Successfully added client*/
    }
    //! Removing client method
    methodStatus removeClient(const Connection& connection) {
        auto it = std::find(clientList.begin(), clientList.end(), connection);
        if (it != clientList.end()) {
            clientList.erase(it); // Erase using the iterator
            return SUCCESS; // Successfully removed
        }
        return NON_EXISTENCE; // Client does not exist
    }


    //! Verify password to enter
    bool verifyPassword(const std::string& attempt){
        return pw.verifyPW(attempt);
    }
    //! Getter methods
    //! Getter method for getting clientList
    const std::vector<Connection>& getClientList() const {
        return clientList;
    }
    //! Get owner Connection
    const Connection& getOwner() const {
        return owner;
    }
    //! Getter method for room ID 
    std::string getRoomID() const {
        return roomID;
    }
    //! Getter method for returning password, check if client is in room
    Password getPassword() const {
        return pw;
    }

private: 
    /*! Room id */
    std::string roomID;
    /*! Password */
    Password pw;
    /*! List of clients*/
    std::vector<Connection> clientList; 
    /*! Owner of the room*/
    Connection owner;
    /*! Gamestate of Room*/
    // GameState gamestate;
 //    std::unordered_map<Connection, std::queue<std::string>> inputQueues;
};