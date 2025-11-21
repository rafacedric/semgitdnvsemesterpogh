#ifndef USERLOGIN_H
#define USERLOGIN_H
#endif
#include <vector>
#include <string>

class UserLogin {
public:
    UserLogin();
    bool fetchUsernames();             // Fetch list from the server
    bool loginUser(const std::string& username);
    bool logoutUser();

    const std::vector<std::string>& getUsernames() const { return usernames; }

private:
    std::vector<std::string> usernames;
    std::string currentUser="";
};