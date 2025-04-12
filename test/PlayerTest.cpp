#include <gtest/gtest.h>
#include "State/Player.h"

// Test for connection state change to "connected"
TEST(PlayerTest, ConnectionTest) {
    Player player;
    player.connect();
    EXPECT_EQ(player.getState(), "connected");
}

// Test for connection state change to "disconnected"
TEST(PlayerTest, DisconnectionTest) {
    Player player;
    player.connect();  // Ensure initial state is "connected"
    player.disconnect();
    EXPECT_EQ(player.getState(), "disconnected");
}

// Test setting and getting the player name
TEST(PlayerTest, SetNameTest) {
    Player player;
    player.setName("Player1");
    EXPECT_EQ(player.getName(), "Player1");
}

// Test setting an empty name
TEST(PlayerTest, SetEmptyNameTest) {
    Player player;
    player.setName("");
    EXPECT_EQ(player.getName(), "");
}

// // Test setting and getting the player ID
// TEST(PlayerTest, SetIdTest) {
//     Player player;
//     player.setId(1);
//     EXPECT_EQ(player.getId(), 1);
// }

// // Test setting a negative player ID
// TEST(PlayerTest, SetNegativeIdTest) {
//     Player player;
//     player.setId(-1);
//     EXPECT_EQ(player.getId(), -1);
// }

// Test setting and getting player variables
TEST(PlayerTest, SetVariableTest) {
    Player player;
    player.setVariable("score", "100");
    EXPECT_EQ(player.getVariable("score"), "100");
}

// Test getting a variable that hasn't been set
TEST(PlayerTest, GetUnsetVariableTest) {
    Player player;
    EXPECT_EQ(player.getVariable("unknown"), "");  // Should return an empty string
}

// Test sending a message
TEST(PlayerTest, SendMessageTest) {
    Player player;
    testing::internal::CaptureStdout();  // Capture console output
    player.sendMessage("Hello, World!");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Player tester sent message to server: Hello, World!\n");
}

// Test receiving a message
TEST(PlayerTest, GetMessageTest) {
    Player player;
    testing::internal::CaptureStdout();  // Capture console output
    player.getMessage();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Player tester received message from server\n");
}

// Test joining the audience
// TEST(PlayerTest, JoinAudienceTest) {
//     Player player;
//     player.joinAudience("Spectators");
//     // No explicit expectation since joinAudience() doesn't return a value or change state
// }
