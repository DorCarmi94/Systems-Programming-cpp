#include "../include/Session.h"
using namespace std;

//Constructor
Session::Session(const std::string &configFilePath) {
    using json=nlohmann::json;
    //string configFilePath2="../include/config1.json";
    ifstream inStream(configFilePath);
    json jFile;
    inStream>>jFile;

    long counter=0;
    for (; counter <jFile["movies"].size() ; ++counter) {
        Watchable *newMovie=new Movie(counter+1,
                                      jFile["movies"][counter]["name"],
                                      jFile["movies"][counter]["length"],
                                      jFile["movies"][counter]["tags"]);
        content.push_back(newMovie);
    }


    for (int j=0; j <jFile["tv_series"].size() ; ++j) {
        //Counts each series
        Episode* LastInSeason;
        for (int seasonCount = 0; seasonCount < jFile["tv_series"][j]["seasons"].size(); ++seasonCount) {
            //Counts each season
            Episode* previousEpisode;

            for (int epiPerSeason = 0; epiPerSeason < jFile["tv_series"][j]["seasons"][seasonCount]; ++epiPerSeason) {
                //Counts each episode


                Episode *newEpisode=new Episode(counter+1,
                                                jFile["tv_series"][j]["name"],
                                                jFile["tv_series"][j]["episode_length"],
                                                seasonCount+1,
                                                epiPerSeason+1,
                                                jFile["tv_series"][j]["tags"]);
                if(epiPerSeason>0) {
                    previousEpisode->setNextID(newEpisode->getID());
                }
                if(epiPerSeason==0 && seasonCount>0)
                {
                    LastInSeason->setNextID(newEpisode->getID());
                }
                previousEpisode=newEpisode;
                LastInSeason=newEpisode;
                content.push_back(newEpisode);
                counter++;
            }
        }
        LastInSeason->setNextID(-99);
    }
}

//Descructor
Session::~Session() {
    clean();
}

void Session::clean()
{
    //delete users

    for(auto it2:userMap)
    {

        delete(it2.second);

    }
    userMap.clear();
    //delete actions
    for (int i = 0; i < actionsLog.size(); ++i) {
        delete (actionsLog.at(i));
        //actionsLog.at(i)= nullptr;
    }
    actionsLog.clear();
    //delete content
    for (int j = 0; j < content.size(); ++j) {
        delete(content.at(j));
        //content.at(j)= nullptr;
    }
    content.clear();
    activeUser= nullptr;

}

//Copy Constructor
Session::Session(Session &other) {
    copy(other);
}

void Session::copy(const Session &other)  {

    for (int i = 0; i < other.content.size(); ++i) {
        this->content.push_back(other.content.at(i)->clone());
    }

    for (int j = 0; j < other.actionsLog.size(); ++j) {
        this->actionsLog.push_back(other.actionsLog.at(j)->clone());
    }

    for(auto& it:other.userMap)
    {
        string x=it.first;
        User *adress=it.second;

        this->userMap.insert(make_pair(x,adress->clone()));
        for (int i = 0; i < adress->get_history().size(); ++i) {
            bool found=false;
            for (int j = 0;!found&& j < this->content.size(); ++j) {
                if(content.at(j)->getID()==adress->get_history().at(i)->getID())
                {
                    found=true;
                    this->userMap[x]->setHistory(this->content.at(j));
                }
            }

        }
        adress= nullptr;
    }
    string CurrentUserName=other.getActiveUser()->getName();
    this->setActiveUser(this->userMap[CurrentUserName]);

}

void Session::copyAndDelete(Session &other) {
    this->activeUser=other.activeUser;
    for (int i = 0; i < other.content.size(); ++i) {
        this->content.push_back(other.content.at(i));
        delete(other.content.at(i));
        other.content.at(i)= nullptr;
    }

    for (int j = 0; j < other.actionsLog.size(); ++j) {
        this->actionsLog.push_back(other.actionsLog.at(j));
        delete(other.actionsLog.at(j));
        other.actionsLog.at(j)= nullptr;
    }

    for(auto& it:userMap)
    {
        auto x=it.first;
        this->userMap.insert(make_pair(x,other.userMap[x]));
        delete(other.userMap[x]);
        other.userMap[x]= nullptr;
        other.userMap.erase(x);
    }
    other.actionsLog.clear();
    other.userMap.clear();
    other.content.clear();
    other.activeUser= nullptr;
}

//Move Constructor
Session::Session(Session &&other) {
    copyAndDelete(other);
}

//Operator Assignment
Session& Session:: operator=(const Session& other)
{
    if(&other!=this)
    {
        clean();
        copy(other);
    }
    return *this;
}

//Mover operator Assignment
Session& Session:: operator=(Session&& other)
{
    if(&other!=this)
    {
        clean();
        copyAndDelete(other);
    }
    return *this;
}

vector<BaseAction*>& Session:: getActionLog()
{
    return this->actionsLog;
}


void Session::start() {
    cout<<"SPLFLIX is now on!"<<endl;
    string name="default";
    string rec="len";
    if(this->userMap.find("default")!=userMap.end())
    {
        activeUser=userMap["default"];
    }
    else {
        CreateUser *defaultUserCreation = new CreateUser(name, rec);
        defaultUserCreation->act(*this);
        actionsLog.push_back(defaultUserCreation);
        this->activeUser = userMap[name];
        defaultUserCreation = nullptr;
    }
    loop();
}

void Session::loop() {
    bool exit=false;

    while(!exit)
    {
        string inputStr;
        string command;
        bool valid=false;
        getline(cin, inputStr);

        if(inputStr.find(' ')==string::npos)
        {
            //no spcae
            if (inputStr.compare("exit")==0) {
                exit = true;
                valid=true;
                Exit *e= new Exit();
                e->act(*this);
                actionsLog.push_back(e);
                e= nullptr;

                //TDL action exit
            }
            ///TDL: delete later
            if (inputStr.compare("userdetails")==0) {
                cout<<this->activeUser->getName()<<endl;
            }
            else if(inputStr.compare("content")==0)
            {
                PrintContentList *printContentListAction= new PrintContentList();
                printContentListAction->act(*this);
                this->actionsLog.push_back(printContentListAction);
                printContentListAction= nullptr;
                valid= true;
            }
            else if(inputStr.compare("watchhist")==0)
            {
                PrintWatchHistory *printWatchHistoryAction= new PrintWatchHistory();
                printWatchHistoryAction->act(*this);
                this->actionsLog.push_back(printWatchHistoryAction);
                printWatchHistoryAction= nullptr;
                valid=true;
            }
            else if(inputStr.compare("log")==0)
            {
                PrintActionsLog *printActionsLogAction= new PrintActionsLog();
                printActionsLogAction->act(*this);
                this->actionsLog.push_back(printActionsLogAction);
                printActionsLogAction= nullptr;
                valid=true;
            }
            else
            {
                if(!valid) {

                    cout << "invalid input, type again" << endl;
                }
            }
        }
        else {
            //lets split by spaces

            istringstream iss(inputStr);
            vector<string> results((istream_iterator<string>(iss)),istream_iterator<string>());

            if(results.size()==0 || results.size()>3)
            {
                cout<<"invalid input, type again1"<<endl;
            }
            else {


                command = results.at(0);

                if (command == "watch") {
                    string numberOfContent;
                    if(results.size()==2)
                    {
                        numberOfContent = results.at(1);
                        long id = stol(results.at(1), nullptr, 10);
                        Watch *wa = new Watch(id - 1);//

                        wa->act(*this);
                        this->actionsLog.push_back(wa);

                        wa = nullptr;
                        valid = true;
                        bool keepAsking=true;


                        while(keepAsking) {
                            Watchable* nextToWatch=activeUser->getRecommendation(*this);
                            if(nextToWatch== nullptr)
                            {
                                keepAsking=false;
                            }
                            else if (nextToWatch != nullptr) {
                                cout << "We recommend watching " << nextToWatch->toString() << ", continue watching? [y/n]"<<endl;
                                //cout <<nextToWatch->getID()<<endl;

                                string yesOrNo;
                                getline(cin, yesOrNo);

                                if (yesOrNo.compare("y") == 0) {
                                    Watch *nextOfferdAction = new Watch(nextToWatch->getID()-1);
                                    nextOfferdAction->act(*this);
                                    actionsLog.push_back(nextOfferdAction);
                                    nextOfferdAction = nullptr;
                                    nextToWatch = nullptr;
                                    keepAsking=true;
                                }
                                else if(yesOrNo.compare("n") == 0)
                                {
                                    keepAsking=false;
                                }
                                else
                                {
                                    cout<<"invalid input"<<endl;

                                }
                            }
                        }






                    }
                }
                else if (command == "createuser"&&results.size()==3) {
//                    string userName=results.at(1);
//                    string userRec=results.at(2);
//                    CreateUser *newUserAction= new CreateUser(userName,userRec);

                    CreateUser *newUserAction= new CreateUser(results.at(1),results.at(2));
                    newUserAction->act(*this);
                    this->actionsLog.push_back(newUserAction);
                    newUserAction= nullptr;
                    valid=true;

                }

                else if (command == "dupuser" && results.size()==3) {

                    string originalname=results.at(1);
                    string newname=results.at(2);

                    DuplicateUser *dupUserAction = new DuplicateUser(originalname,newname);
                    dupUserAction->act(*this);
                    actionsLog.push_back(dupUserAction);
                    dupUserAction= nullptr;
                    valid=true;
                }

                else if (command == "changeuser"&& results.size()==2) {

                    string userToChange=results.at(1);
                    ChangeActiveUser *changeActiveUserAction= new ChangeActiveUser(userToChange);
                    changeActiveUserAction->act(*this);
                    actionsLog.push_back(changeActiveUserAction);
                    changeActiveUserAction= nullptr;
                    valid=false;
                }

                else if (command == "deleteuser" && results.size()==2) {
                    string userToDelete=results.at(1);
                    DeleteUser *deleteActiveUserAction= new DeleteUser(userToDelete);
                    deleteActiveUserAction->act(*this);
                    actionsLog.push_back(deleteActiveUserAction);
                    deleteActiveUserAction= nullptr;
                    valid=true;

                }
                else
                {
                    if(!valid) {

                        cout << "invalid input, type again" << endl;
                    }
                }
            }

        }

    }
}



void Session::setActiveUser(User *user) {

    this->activeUser=user;
}

void Session::deleteUser(User *user) {

    userMap.erase(user->getName());
    delete(user);
    //user->~User();
    user=nullptr;
}

unordered_map<std::string,User*>& Session:: getUserMap()
{
    return userMap;
}
vector<Watchable*> & Session::getContent()
{
    return this->content;
}
User* Session::getActiveUser() const {
    return this->activeUser;
}

void Session::setUserMap(User *userToAdd) {
    this->userMap.insert((make_pair(userToAdd->getName(),userToAdd)));
}


