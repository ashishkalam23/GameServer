// daw10
// 10/04/2024

#include <gtest/gtest.h>
#include "Network/Server.h"  
#include <deque>
#include <string>

using namespace networking;  // Ensure this matches your project namespace

// Testable subclass of the Server class
class TestableServer : public Server {
public:
    TestableServer(unsigned short port, const std::string& httpMessage,
                   std::function<void(Connection)> onConnect,
                   std::function<void(Connection)> onDisconnect)
        : Server(port, httpMessage, onConnect, onDisconnect),
          disconnectCalledCount(0) { }

    // Mock for Disconnect
    void mockDisconnect(Connection connection) {
        disconnectCalledCount++;
        lastConnection = connection;
    }

    // Disconnect tracking
    int disconnectCalledCount;
    Connection lastConnection;
};

// Mock Connection class for testing purposes
struct MockConnection {
    int id;

    bool operator==(const MockConnection& other) const {
        return id == other.id;
    }
};

// Mock Message class using MockConnection
struct MockMessage {
    MockConnection connection;
    std::string text;
};

// Mock ServerImpl class to simulate internal behavior
class MockServerImpl {
public:
    void sendMessages(const std::deque<MockMessage>& messages) {
        for (const auto& msg : messages) {
            sentMessages.push_back(msg);
        }
    }

    std::deque<MockMessage> sentMessages;
};

// Test fixture for server
class ServerTest : public ::testing::Test {
protected:
    MockServerImpl* mockImpl;
    TestableServer* testServer;

    void SetUp() override {
        mockImpl = new MockServerImpl();
        testServer = new TestableServer(
            0,  // Use 0 to allow OS to select an available port
            "<html></html>",
            [](Connection) { /* Mock onConnect */ },
            [](Connection) { /* Mock onDisconnect */ }
        );
    }

    void TearDown() override {
        delete mockImpl;
        delete testServer;
    }

    // Helper function to simulate sending messages
    void simulateSend(Server& server, const std::deque<MockMessage>& messages) {
        mockImpl->sendMessages(messages);
    }
};

// Test case for mockDisconnect function
TEST(ServerTests, DisconnectTest) {
    // TestableServer instance with mock handlers
    TestableServer testServer(
        0,  // Use 0 to allow OS to select an available port
        "<html></html>",
        [](Connection) { /* Mock */ },
        [](Connection) { /* Mock */ }
    );

    // Create test connections
    Connection testConnection1{1234};
    Connection testConnection2{5678};

    // Call mockDisconnect
    testServer.mockDisconnect(testConnection1);

    // Check if disconnect was called correctly
    ASSERT_EQ(testServer.disconnectCalledCount, 1);
    ASSERT_EQ(testServer.lastConnection.id, testConnection1.id);

    testServer.mockDisconnect(testConnection2);
    ASSERT_EQ(testServer.disconnectCalledCount, 2);
    ASSERT_EQ(testServer.lastConnection.id, testConnection2.id);
}

// Test case for sending messages
TEST_F(ServerTest, SendMessagesTest) {
    // Create a deque of Mock messages
    std::deque<MockMessage> messagesToSend = {
        { MockConnection{1}, "Hello" },
        { MockConnection{2}, "World" },
    };

    // Simulate a send
    simulateSend(*testServer, messagesToSend);

    // Check if messages were sent correctly
    ASSERT_EQ(mockImpl->sentMessages.size(), 2);
    ASSERT_EQ(mockImpl->sentMessages[0].text, "Hello");
    ASSERT_EQ(mockImpl->sentMessages[1].text, "World");
    ASSERT_EQ(mockImpl->sentMessages[0].connection.id, 1);
    ASSERT_EQ(mockImpl->sentMessages[1].connection.id, 2);
}