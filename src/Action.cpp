#include <string>
#include "../include/Action.h"

using namespace std;
//Constructor
BaseAction::BaseAction() {
    status=PENDING;

}

void BaseAction::complete() {status=COMPLETED; errorMsg="No Error";}

BaseAction::~BaseAction() {}

BaseAction::BaseAction(BaseAction &other) {this->status=other
.status; this->errorMsg=other.errorMsg;}

BaseAction::BaseAction(BaseAction &&other) {this->status=other
            .status; this->errorMsg=other.errorMsg;}
BaseAction& BaseAction::operator=(const BaseAction &other) {
    if(this!=&other)
    {
        this->status=other.status;
        this->errorMsg=other.errorMsg;
    }
    return *this;

}

BaseAction& BaseAction::operator=(BaseAction &&other) {
    if(this!=&other)
    {
        this->status=other.status;
        this->errorMsg=other.errorMsg;
    }
    return *this;

}


ActionStatus BaseAction::getStatus() const { return status;}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::error(const std::string &errorMsg) {
    status=ERROR;
    this->errorMsg=errorMsg;
    std::cout<<"Error - "<<errorMsg<<std::endl;
}
const string BaseAction::getMsg()
{
    return errorMsg;
}
///////////////////////////////////////////////////////////////////
///////////Create User
///////////////////////////////////////////////////////////////////

CreateUser::CreateUser(string& name, string &recommendationStr ) :BaseAction(),NewUsersName(name),NewUserRec(recommendationStr){

}

CreateUser::CreateUser(CreateUser &other):BaseAction(other),NewUserRec(other.NewUserRec),NewUsersName(other.NewUsersName){

}
CreateUser::~CreateUser(){}
CreateUser::CreateUser(CreateUser&&other):BaseAction(other),NewUserRec(other.NewUserRec),NewUsersName(other.NewUsersName){}

CreateUser* CreateUser::clone() {
    return new CreateUser(*this);
}

void CreateUser::act(Session &sess) {
    if(sess.getUserMap().size()==0||sess.getUserMap().find(NewUsersName)==sess.getUserMap().end())
    {
        //Correct
        if(this->NewUserRec.compare("len")==0)
        {
            LengthRecommenderUser *lenUser= new LengthRecommenderUser(this->NewUsersName);
            sess.setUserMap(lenUser);
            lenUser= nullptr;
            complete();
        }
        else if(this->NewUserRec.compare("rer")==0)
        {
            RerunRecommenderUser *rerUser= new RerunRecommenderUser(this->NewUsersName);
            sess.setUserMap(rerUser);
            rerUser= nullptr;
            complete();
        }
        else if(this->NewUserRec.compare("gen")==0)
        {
            GenreRecommenderUser *genUser= new GenreRecommenderUser(this->NewUsersName);
            sess.setUserMap(genUser);
            genUser= nullptr;
            complete();
        }
        else
        {
            error("The user's recommendation string input is invalid");
        }
    }
    else
    {
        //Error
        error("The user name you want to create exits in the system");
    }
}

std::string CreateUser::toString() const {return "CreateUser";}
std::string ChangeActiveUser::toString() const {return "ChangeActiveUser";}
std::string DeleteUser::toString() const {return "DeleteUser";}
std::string DuplicateUser::toString() const {return "DuplicateUser";}
std::string PrintContentList::toString() const {return "PrintContentList";}
std::string PrintWatchHistory::toString() const {return "PrintWatchHistory";}
std::string Watch::toString() const {return "Watch";}
std::string PrintActionsLog::toString() const {return "PrintActionsLog";}
std::string Exit::toString() const {return "Exit";}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
Watch::Watch(const long &id) :BaseAction(),id(id){}
Watch::Watch(Watch &other) :BaseAction(other), id(other.id){}
Watch* Watch::clone() {
    return new Watch(*this);
}
//Always remember id is one bigger then it's locating in the vector
void Watch::act(Session &sess) {
    if(id<0 ||sess.getContent().size()<id)
    {
        error("invalid id");
        return;
    }
    Watchable *watching=sess.getContent().at(id);
    cout<<"Watching "<<watching->toString()<<endl;
    /////////
    User *u=sess.getActiveUser();
    u->setHistory(watching);
    u= nullptr;
    watching= nullptr;
    complete();

}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
DuplicateUser::DuplicateUser(string &originName, string &newName) :BaseAction(),originalName(originName),newName(newName){

}
//TDL: add toString for all actions
void DuplicateUser::act(Session &sess) {
    if(sess.getUserMap().size()>0 &&
    sess.getUserMap().find(originalName)!=sess.getUserMap().end()
    && sess.getUserMap().find(newName)==sess.getUserMap().end())
    {
        //TDL:think if possible to do it better
        User *newUser = sess.getUserMap()[originalName];
        string newName_l= newName;
        string newRec_l=newUser->getRec();
        CreateUser *creatingNewUserAction= new CreateUser( newName_l, newRec_l);
        creatingNewUserAction->act(sess);
        delete(creatingNewUserAction);
        //newUser->setName(newName);

        complete();
        //originalUser = nullptr;
        newUser = nullptr;
    }
    else
    {
        if(sess.getUserMap().size()==0 )
            error("no users to copy");
        else if(sess.getUserMap().find(originalName)==sess.getUserMap().end())
            error("user not found");
        else if(sess.getUserMap().find(newName)!=sess.getUserMap().end())
            error("new user already exists");
    }

}
DuplicateUser::DuplicateUser(DuplicateUser &other) : BaseAction(other),originalName(other.originalName),newName(other.newName){

}
DuplicateUser* DuplicateUser::clone() {
    return new DuplicateUser(*this);
}

ChangeActiveUser::ChangeActiveUser(ChangeActiveUser& other):BaseAction(other),userToChangeName(other.userToChangeName) {

}
ChangeActiveUser::ChangeActiveUser(string &newUserName):BaseAction(),userToChangeName(newUserName) {

}
ChangeActiveUser* ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}

void ChangeActiveUser::act(Session &sess) {
    if(sess.getUserMap().size()>0&&sess.getUserMap().find(userToChangeName)!=sess.getUserMap().end())
    {

        sess.setActiveUser(sess.getUserMap()[userToChangeName]);
        complete();
    }
    else{
        if(sess.getUserMap().size()==0)
        {
            error("no users to change to");
        }
        if(sess.getUserMap().find(userToChangeName)==sess.getUserMap().end())
        {
            error("user doesn't exist");
        }
    }

}

DeleteUser::DeleteUser(DeleteUser &other):BaseAction(other),userToDelete(other.userToDelete) {

}

DeleteUser::DeleteUser(string &userToDelete):BaseAction(),userToDelete(userToDelete) {

}

DeleteUser* DeleteUser::clone() {
    return new DeleteUser(*this);
}
void DeleteUser::act(Session & sess)
{
    if(sess.getUserMap().size()>0&&sess.getUserMap().find(userToDelete)!=sess.getUserMap().end())
    {
        if(sess.getUserMap()[userToDelete]==sess.getActiveUser())
        {
            error("ha ha ha ha staying alive staying alive");
        }
        sess.deleteUser(sess.getUserMap()[userToDelete]);
        complete();
    }
    else{
        if(sess.getUserMap().size()==0)
        {
            error("no users to delete");
        }
        if(sess.getUserMap().find(userToDelete)==sess.getUserMap().end())
        {
            error("user doesn't exist");
        }
    }
}

void PrintContentList::act(Session &sess) {
    cout<<"Watch history for "<<sess.getActiveUser()->getName()<<endl;
    for (int i = 0; i < sess.getContent().size(); ++i) {
        cout<<i+1<<". "<<sess.getContent().at(i)->toString()<<" "
        <<sess.getContent().at(i)->getLength()<<" minutes [";
        for (int j = 0; j < sess.getContent().at(i)->getTags().size(); ++j) {
            if(j==sess.getContent().at(i)->getTags().size()-1)
            {
                cout << sess.getContent().at(i)->getTags().at(j);
            }
            else {
                cout << sess.getContent().at(i)->getTags().at(j) << ", ";
            }
        }
        cout<<"]"<<endl;

    }
    complete();
}

PrintContentList::PrintContentList(PrintContentList &other) :BaseAction(other){

}

PrintContentList::PrintContentList() :BaseAction(){}
PrintContentList* PrintContentList::clone() {
    return new PrintContentList(*this);
}

void PrintWatchHistory::act(Session &sess) {
    User *active=sess.getActiveUser();
    for (int i = 0; i < active->get_history().size(); ++i) {
        cout<<i+1<<". "<<active->get_history().at(i)->toString()<<endl;
    }
    complete();
}
PrintWatchHistory::PrintWatchHistory() :BaseAction(){}
PrintWatchHistory::PrintWatchHistory(PrintWatchHistory &other):BaseAction(other) {

}

PrintWatchHistory* PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}

PrintActionsLog::PrintActionsLog(PrintActionsLog &other) :BaseAction(other){}
PrintActionsLog::PrintActionsLog() :BaseAction(){}
PrintActionsLog* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

void PrintActionsLog::act(Session &sess) {
    for (int i =sess.getActionLog().size()-1 ; i >=0; --i) {
        if(sess.getActionLog().at(i)->getStatus()==ERROR)
        {

            cout << sess.getActionLog().at(i)->toString() <<" ERROR: "<< sess.getActionLog().at(i)->getMsg() << endl;
        }
        else {
            cout << sess.getActionLog().at(i)->toString() <<" COMPLETED"<<endl;
        }
    }
}

Exit::Exit(Exit &other):BaseAction(other) {}

Exit* Exit::clone() {
    return new Exit(*this);
}

Exit::Exit()
{

}
void Exit::act(Session &sess) {


}


