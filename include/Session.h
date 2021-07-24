#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include <json.hpp>
#include <fstream>
#include "Watchable.h"
#include "User.h"
#include <sstream>
#include <iostream>
using namespace std;



class User;
class Watchable;
class BaseAction;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    Session(Session& other);
    Session(Session&&other);
    Session &operator=(const Session&other);
    Session &operator=(Session&&other);


    void start();
    //Sets
    void setUserMap(User *userToAdd);
    void setActiveUser(User* user);
    void deleteUser(User *user);
    //Gets
    std::unordered_map<std::string,User*> & getUserMap();
    User* getActiveUser() const;
    std::vector<Watchable*>& getContent();
    vector<BaseAction*>& getActionLog();

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    void loop();
    void exit();
    void copy(const Session & other);
    void copyAndDelete(Session & other);
    void clean();
    void loopExit();


};
#endif
