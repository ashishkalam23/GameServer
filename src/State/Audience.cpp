#include "State/Audience.h"
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
/**
 * Testing Sphinx
 * @return audienceName
 */
// Get the name of the audience
std::string Audience::getName() const {
    return audienceName;  // Return the audience member's name
}
// Add a comment with the current timestamp
void Audience::addComment(const std::string& comment) {
    std::time_t now = std::time(nullptr);  // Get the current time
    comments.push_back(comment);
    timestamps.push_back(now);  // Add the timestamp
}

// Get the list of comments
const std::vector<std::string>& Audience::getComments() const {
    return comments;
}

// Clear all comments
void Audience::clearComments() {
    comments.clear();
    timestamps.clear();  // Clear timestamps as well
}

// Get the number of comments
int Audience::getCommentCount() const {
    return comments.size();
}

// Display comments along with timestamps
void Audience::displayComments() const {
    for (size_t i = 0; i < comments.size(); ++i) {
        std::cout << formatTime(timestamps[i]) << ": " << comments[i] << std::endl;
    }
}

// Format time for display
std::string Audience::formatTime(std::time_t time) const {
    std::tm* localTime = std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}