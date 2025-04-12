#include <string>
#include <jsoncpp/json/json.h>
// JSON message conversion using jsoncpp
class MessageHandler {
public:
    MessageHandler();
    ~MessageHandler();
    
    Json::Value parseMessage(const std::string& rawMessage);
    
    std::string createMessage(const Json::Value& messageData);

    void processMessage(const Json::Value& message);
    std::string prepareMessage(const std::string& messageType, const Json::Value& payload);

private:
    void handleJoinRoom(const Json::Value& message);
    void handleChatMessage(const Json::Value& message);
    void handleGameAction(const Json::Value& message);

    void handlePlayerJoin(const Json::Value &message);
    void broadcastMessageToRoom(const std::string &roomID, const std::string &message);
    // Experimental
    // void handlePlayerReconnection(const std::string &playerID, const std::string &roomID);
};