#include <gmock.h>
#include <gtest.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "GameServer.h"

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace websocket = beast::websocket;

// Mock WebSocket stream
class MockWebSocket : public websocket::stream<tcp::socket> {
public:
    MockWebSocket(net::io_context& ioc) : websocket::stream<tcp::socket>(ioc) {}

    MOCK_METHOD(void, async_read, (beast::flat_buffer&, std::function<void(const beast::error_code&, std::size_t)>), (override));
    MOCK_METHOD(void, async_write, (const beast::flat_buffer&, std::function<void(const beast::error_code&, std::size_t)>), (override));
    MOCK_METHOD(void, close, (websocket::close_code), (override));
};

// Unit Test Class
class GameServerTest : public ::testing::Test {
protected:
    net::io_context ioc;
    tcp::endpoint endpoint{tcp::v4(), 8080};
    GameServer server{ioc, endpoint};

    std::shared_ptr<MockWebSocket> createMockWebSocket() {
        return std::make_shared<MockWebSocket>(ioc);
    }
};

// Test: Room Creation
TEST_F(GameServerTest, CreateRoomTest) {
    auto ws = createMockWebSocket();

    EXPECT_CALL(*ws, async_write(testing::_, testing::_))  // Simulate write when room is created
        .Times(1);

    server.createRoom(ws);

    // Check that a room was created
    EXPECT_EQ(server.rooms_.size(), 1);
    EXPECT_NE(server.rooms_.find("room1"), server.rooms_.end());
}

// Test: Player Joins a Room
TEST_F(GameServerTest, JoinRoomTest) {
    auto ws1 = createMockWebSocket();
    auto ws2 = createMockWebSocket();

    // Create a room and simulate another player joining it
    server.createRoom(ws1);
    std::string roomID = "room1";

    EXPECT_CALL(*ws2, async_write(testing::_, testing::_))  // Simulate write when joining room
        .Times(1);

    server.joinRoom(ws2, roomID);

    // Check if the player has been added to the room
    auto room = server.rooms_[roomID];
    EXPECT_EQ(room->players_.size(), 2);  // Two players should be in the room
}

// Test: Start Game in a Room
TEST_F(GameServerTest, StartGameTest) {
    auto ws = createMockWebSocket();
    server.createRoom(ws);

    auto room = server.rooms_["room1"];

    // Simulate starting a game in the room
    server.startGame(*room);

    // Check that the game has started and the rounds have initialized
    EXPECT_TRUE(room->gameStarted_);
    EXPECT_EQ(room->currentRound_, 3);  // Expect game to have 3 rounds, as set in startGame
}

// Test: Handle "create room" Message in doRead
TEST_F(GameServerTest, HandleCreateRoomMessage) {
    auto ws = createMockWebSocket();
    std::string message = "create room";

    // Mock receiving a message
    EXPECT_CALL(*ws, async_read(testing::_, testing::_))
        .WillOnce([message](beast::flat_buffer& buffer, auto callback) {
            buffer.commit(boost::asio::buffer_copy(buffer.prepare(message.size()), boost::asio::buffer(message)));
            callback({}, message.size());
        });

    // Call the method to read and process the message
    server.doRead(ws);

    // Ensure a room was created as a result of the message
    EXPECT_EQ(server.rooms_.size(), 1);
}

// Test: Server Shutdown
TEST_F(GameServerTest, ShutdownServerTest) {
    auto ws1 = createMockWebSocket();
    auto ws2 = createMockWebSocket();

    server.createRoom(ws1);
    server.joinRoom(ws2, "room1");

    EXPECT_CALL(*ws1, close(websocket::close_code::normal))
        .Times(1);
    EXPECT_CALL(*ws2, close(websocket::close_code::normal))
        .Times(1);

    // Shutdown the server
    server.shutdown();

    // Check that the server stopped accepting new connections
    EXPECT_TRUE(server.clients_.empty());
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
