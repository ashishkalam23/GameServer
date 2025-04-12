#pragma once

#include "../Network/Server.h"  // Assumes Connection is defined here

#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <iostream>

using std::string, std::vector, networking::Connection;
/**
 * Testing Sphinx
 * @class Audience
 */
class Audience {
public:
    // Constructor initializes the connection and name
    inline Audience(Connection conn, const string& name)
        : c(conn), audienceName(name) {}

    // Returns the name of the audience member
    string getName() const;

    // Returns the Connection object for the audience member
    Connection getConnection() const {
        return c;
    }

    // Possibly some kind of audience chat functionality (?)
    void addComment(const string& comment);
    const vector<string>& getComments() const;

    // Clear all comments
    void clearComments();

    // Get total number of comments
    int getCommentCount() const;

    // Display all comments with timestamps
    void displayComments() const;

private:
    string audienceName;
    Connection c;  // Stores the Connection object
    vector<string> comments;
    vector<std::time_t> timestamps; // Store timestamps

    // To format timestamps
    string formatTime(std::time_t time) const;
};