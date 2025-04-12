#include <iostream>
#include <memory>
#include <string>
#include "GameServer.h"

// Helper functions to simulate checking test results
void assertEqual(const std::string& testName, const std::string& expected, const std::string& actual) {
    if (expected == actual) {
        std::cout << testName << " passed!" << std::endl;
    } else {
        std::cout << testName << " failed! Expected: " << expected << ", but got: " << actual << std::endl;
    }
}

void assertTrue(const std::string& testName, bool condition) {
    if (condition) {
        std::cout << testName << " passed!" << std::endl;
    } else {
        std::cout << testName << " failed!" << std::endl;
    }
}

// Mock WebSocket class that mimics boost::beast::websocket::stream behavior
class MockWebSocket : public websocket::stream<tcp::socket> {
public:
    MockWebSocket(net::io_context& ioc) : websocket::stream<tcp::socket>(ioc) {}

    void write(const boost::asio::const_buffer& buffer) {
        last_message_ = std::string(boost::asio::buffer_cast<const char*>(buffer), boost::asio::buffer_size(buffer));
    }

    std::string getLastMessage() const {
        return last_message_;
    }

private:
    std::string last_message_;
};

// Test: Room Creation
void testCreateRoom(GameServer& server) {
    auto ws = std::make_shared<MockWebSocket>(server.getIoContext());
    server.createRoom(ws);

    // Check that a room was created
    assertTrue("testCreateRoom - Room Created", server.roomCount() == 1);
}

// Test: Player Joins Room
void testJoinRoom(GameServer& server) {
    auto ws1 = std::make_shared<MockWebSocket>(server.getIoContext());
    auto ws2 = std::make_shared<MockWebSocket>(server.getIoContext());

    server.createRoom(ws1);
    server.joinRoom(ws2, "room1");

    // Get the room and check if the second player was added
    auto room = server.getRoom("room1");
    assertTrue("testJoinRoom - Players Added", room->playerCount() == 2);
}

// Test: Start Game in a Room
void testStartGame(GameServer& server) {
    auto ws = std::make_shared<MockWebSocket>(server.getIoContext());
    server.createRoom(ws);

    auto room = server.getRoom("room1");
    server.startGame(*room);

    // Ensure the game is started
    assertTrue("testStartGame - Game Started", room->isGameStarted());
    // Assuming the game logic sets currentRound_ to totalRounds_ after initGame
    assertTrue("testStartGame - First Round Started", room->getCurrentRound() == 3);
}

// Test: Handle "create room" message
void testHandleCreateRoomMessage(GameServer& server) {
    auto ws = std::make_shared<MockWebSocket>(server.getIoContext());

    // Simulate receiving "create room" message
    ws->write(net::buffer("create room"));
    server.createRoom(ws);

    // Ensure a room was created and message is returned correctly
    assertTrue("testHandleCreateRoomMessage - Room Created", server.roomCount() == 1);
    assertEqual("testHandleCreateRoomMessage - Message Sent", "Room created with ID: room1", ws->getLastMessage());
}

// Test: Server Shutdown
void testShutdownServer(GameServer& server) {
    auto ws1 = std::make_shared<MockWebSocket>(server.getIoContext());
    auto ws2 = std::make_shared<MockWebSocket>(server.getIoContext());

    server.createRoom(ws1);
    server.joinRoom(ws2, "room1");

    // Simulate shutdown
    server.shutdown();

    // Check that all clients were disconnected
    assertTrue("testShutdownServer - Clients Cleared", server.clientCount() == 0);
}

int main() {
    net::io_context ioc;
    tcp::endpoint endpoint(tcp::v4(), 8080);
    GameServer server(ioc, endpoint);

    // Run the tests
    testCreateRoom(server);
    testJoinRoom(server);
    testStartGame(server);
    testHandleCreateRoomMessage(server);
    testShutdownServer(server);

    return 0;
}
