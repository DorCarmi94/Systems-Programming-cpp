#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "../include/User.h"
#include "Session.h"
using namespace std;
class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
	//Constructor
    BaseAction();
	//Destructor
	virtual ~BaseAction();
	//CopyConstructor
	BaseAction(BaseAction& other);
	//MoveConstructor
	BaseAction(BaseAction&& other);
	//Operator Assignment
    BaseAction & operator=(const BaseAction &other); //Q
    BaseAction & operator=(BaseAction &&other); //Q


	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
	const string getMsg();
	virtual BaseAction* clone()=0;
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    CreateUser(std::string& name, std::string &recommendationStr);
    CreateUser(CreateUser &other);
    virtual ~CreateUser();
    CreateUser(CreateUser&&other);
	virtual void act(Session& sess);
	virtual std::string toString() const;
    CreateUser* clone();
private:
    const std::string NewUsersName;
    const std::string NewUserRec;
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(string &newUserName);
    ChangeActiveUser(ChangeActiveUser& other);
	virtual void act(Session& sess);
	virtual std::string toString() const;
    ChangeActiveUser* clone();
private:
    std::string userToChangeName;
};

class DeleteUser : public BaseAction {
public:
    DeleteUser(std::string&userToDelete);
    DeleteUser(DeleteUser &other);
	virtual void act(Session & sess);
	virtual std::string toString() const;
    DeleteUser* clone();
private:
    std::string userToDelete;
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser(string&originName, string&newName);
    DuplicateUser(DuplicateUser &other);
	virtual void act(Session & sess);
	virtual std::string toString() const;
    DuplicateUser* clone();
private:
    std::string originalName;
    std::string newName;

};

class PrintContentList : public BaseAction {
public:
    PrintContentList();
    PrintContentList(PrintContentList& other);
	virtual void act (Session& sess);
	virtual std::string toString() const;
    PrintContentList* clone();

};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
    PrintWatchHistory(PrintWatchHistory &other);
	virtual void act (Session& sess);
	virtual std::string toString() const;
	PrintWatchHistory* clone();
};


class Watch : public BaseAction {
public:
    Watch(const long&id);
    Watch(Watch& other);
	virtual void act(Session& sess);
	virtual std::string toString() const;
    Watch* clone();
private:
    const long id;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    PrintActionsLog(PrintActionsLog&other);
	virtual void act(Session& sess);
	virtual std::string toString() const;
	PrintActionsLog* clone();
};

class Exit : public BaseAction {

public:
    Exit(Exit & other);
    Exit();
	virtual void act(Session& sess);
	virtual std::string toString() const;
	Exit* clone();
};
#endif
