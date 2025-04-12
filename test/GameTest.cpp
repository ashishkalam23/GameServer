#include <gtest/gtest.h>
#include "State/Game.h"
#include "State/Player.h"
#include "State/Audience.h"
#include "Network/Server.h"  // Assuming the Connection type is defined here

// Mock Connection for testing purposes
struct MockConnection : public networking::Connection {
    explicit MockConnection(unsigned long id) {
        this->id = id;  // Initialize the Connection ID
    }
};

// Test fixture for Game tests
class GameTest : public ::testing::Test {
protected:
    Game* game;
    MockConnection conn1{1};
    MockConnection conn2{2};

    void SetUp() override {
        // Initialize the game with the initial host (player)
        game = new Game("Test Game", 5, conn1, "HostPlayer");
    }

    void TearDown() override {
        delete game;
    }
};

// Test starting the game
TEST_F(GameTest, StartGameTest) {
    game->startGame();
    EXPECT_EQ(game->isGameOver(), false);  // Game should not be over
}

// Test advancing rounds
TEST_F(GameTest, AdvanceRoundTest) {
    game->startGame();
    bool result = game->advanceRound();
    EXPECT_TRUE(result);  // Round should advance successfully
    EXPECT_FALSE(game->isGameOver());  // Game should not be over

    // Advance to the final round
    for (int i = 1; i < 5; ++i) {
        game->advanceRound();
    }
    EXPECT_TRUE(game->isGameOver());  // Game should be over now
}

// Test advancing rounds without starting the game
TEST_F(GameTest, AdvanceRoundWithoutStartingTest) {
    bool result = game->advanceRound();
    EXPECT_FALSE(result);  // Should not be able to advance without starting
}

// Test adding a player
TEST_F(GameTest, AddPlayerTest) {
    game->addPlayer(conn2, "Player1");
    EXPECT_EQ(game->getPlayerScore(conn2), 0);  // Initial score should be 0
}

// Test adding an audience member
TEST_F(GameTest, AddAudienceTest) {
    game->addAudience(conn2, "Audience1");
    // Ensure that addAudience() works without crashing
}

// Test removing a player
TEST_F(GameTest, RemovePlayerTest) {
    game->addPlayer(conn2, "Player1");
    bool removed = game->removePlayer(conn2);
    EXPECT_TRUE(removed);  // Player should be removed
    EXPECT_EQ(game->getPlayerScore(conn2), -1);  // Score should be -1 after removal
}

// Test updating player score
TEST_F(GameTest, UpdatePlayerScoreTest) {
    game->addPlayer(conn2, "Player1");
    game->updatePlayerScore(conn2, 10);
    EXPECT_EQ(game->getPlayerScore(conn2), 10);  // Score should be updated
}

// Test host functionality
TEST_F(GameTest, HostTest) {
    EXPECT_TRUE(game->isHost(conn1));  // conn1 should be the host
    EXPECT_EQ(game->getHostName(), "HostPlayer");  // Host's name should match
}

// Test ending the game
TEST_F(GameTest, EndGameTest) {
    game->startGame();
    game->endGame();
    EXPECT_FALSE(game->isGameOver());  // Game should not be over after ending
}