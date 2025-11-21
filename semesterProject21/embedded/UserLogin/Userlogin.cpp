#include "Userlogin.h"
#include <vector>
#include <string>


UserLogin::UserLogin()
{
    fetchUsernames();
}
// Here connect to pico api to fetch usernames to /api/users
bool UserLogin::fetchUsernames() {
    usernames.clear();
    // Example: fetched from HTTP response JSON
    usernames.push_back("Alice");
    usernames.push_back("Bob");
    usernames.push_back("Carol");
    usernames.push_back("Lucifer");
    usernames.push_back("Spider Man");
    usernames.push_back("Bojack");
    
    return true;
}
// login through the api by sending the username to /api/login
bool UserLogin::loginUser(const std::string& username) {
    // For now, just print or simulate a login.
    // Later you can replace this with your real POST API request logic.
    printf("Logging in user: %s\n", username.c_str());
    currentUser = username;

    return true; // pretend it succeeded
}

// logout through the api by sending the username to /api/logout
bool UserLogin::logoutUser() {
    // For now, just print or simulate a login.
    // Later you can replace this with your real POST API request logic.
    printf("Logging out user: %s\n", currentUser.c_str());
    return true; // pretend it succeeded
}
