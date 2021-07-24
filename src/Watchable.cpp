#include "../include/Watchable.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "../include/User.h"
#include "../include/Session.h"

#include <vector>
using namespace std;
//Constructor
Watchable::Watchable():id(0),length(0),tags() {}
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags):id (id),length(length),tags() {
    //Receiving a vector of strings on stack- copying it to a vector on the heap
    copy(tags);
}
//////////////////////////////////////
///Rule of 5
/////////////////////////////////////
//Destructor
Watchable::~Watchable() {
    //TDL: If the tags vector will be copied, needs to delete evert string in the vector
    clean();

}

void Watchable:: clean()
{

    this->tags.clear();
    this->length=0;
}
//Copy Construcor
Watchable::Watchable(const Watchable& other):id (other.id),length(other.length) ,tags(other.tags){
    copy(other.tags);
}

void Watchable::DeleteOther (Watchable& other){
    for (unsigned int i = 0; i < this->tags.size(); i++) {
        string *st=&other.tags.at(i);
        delete(&st);
        other.tags.at(i)= nullptr;
    }
    other.tags.clear();
}

void Watchable::copy(const std::vector<std::string> &OtherTags) {
    //Clearing the vector from previous objects
    for (unsigned int j=0;j<tags.size();j++){
        string *st=&tags.at(j);
        delete(&st);
        st= nullptr;
    }

    for (unsigned int j=0;j<OtherTags.size();j++){

        tags.push_back(OtherTags.at(j));

    }
}


//Move Constructor
Watchable::Watchable(Watchable &&other):id (other.id),length(other.length),tags() {
    copy(tags);
}

//Operator Copy assignment
Watchable &Watchable::operator=(const Watchable &other) {
    //TDL:Check about const id, cant be coppied


    if (this != &other)
    {
        this->clean();
        this->setLength(other.length);
        copy(other.getTags());
    }
    return *this;

}

Watchable &Watchable::operator=(Watchable &&other) {
    //TDL:Check about const id, cant be coppied


    if (this != &other)
    {
        this->clean();
        this->setLength(other.length);
        copy(other.getTags());
        DeleteOther(other);
    }
    return *this;

}

Watchable* Watchable:: getNextWatchable(Session& sess) const
{
    //User* activeUser= sess.getActiveUser();
    User* activeUser= nullptr;
    Watchable* nextToWatch= (*activeUser).getRecommendation(sess);
    return nextToWatch;

}

//getters
const std::vector<std::string>& Watchable::getTags() const {return this->tags;}


const long Watchable::getID() const {return this->id;}
const int Watchable::getLength() const {return this->length;}


//Setters
void Watchable::setLength(int length) {this->length=length;}
//TDL:Move assignment


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Watchable::getReccomendationOfContent() { return -99;}



/////////////////////////////////////////////////////////////////////



//Constructor
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags): Watchable(id,length,tags),name(name){

}

//Copy Constructor
Movie::Movie(const Movie& other):Watchable(other.getID(),other.getLength(),other.getTags()),name(other.name){

}

//Mover Constructor
Movie::Movie(Movie &&other):Movie(other.getID(),other.name,other.getLength(),other.getTags()) {

}

Movie::~Movie()
{
    clean();
}
//CopyAssignment
Movie & Movie:: operator=(const Movie &other){
    if(this!=&other) {
        this->Watchable::clean();
        this->setLength(other.getLength());
        this->copy(other.getTags());
        this->name=other.name;
    }
    //TDL:copyingID

    return *this;
}

//MoveAssigment
Movie &Movie::operator=(Movie &&other) {
    //do we need to make sure to delete other? is it this class's responsibility?
    this->operator=(other);
    this->DeleteOther(other);
    return *this;
}

std::string Movie::toString() const {
    return this->name;
}

Watchable* Movie::getNextWatchable(Session &session) const {
    //TDL: User* activeUser= session.getActiveUser();
    User* activeUser= nullptr;
    Watchable* nextToWatch= (*activeUser).getRecommendation(session);
    return nextToWatch;
}

int Movie::getReccomendationOfContent() {
    return -99;
}




Movie* Movie::clone() {
    return new Movie(*this);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Constructor
Episode::Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags):Watchable(id,length,tags) ,seriesName(seriesName),season(season),episode(episode),nextEpisodeId(-99)
{

}
//destructor
Episode::~Episode(){
    clean();
}
//Copy Constructor
Episode::Episode(const Episode& other):Watchable(other.getID(),other.getLength(),other.getTags()) ,seriesName(other.seriesName),season(other.season),episode(other.episode),nextEpisodeId(other.nextEpisodeId)
{
}

//Mover Constructor
Episode::Episode(Episode &&other):Episode(other.getID(),other.seriesName,other.getLength(),other.season,other.episode,other.getTags()) {

}

//CopyAssignment
Episode & Episode:: operator=(const Episode &other){
    if(this!=&other) {
        this->Watchable::clean();
        this->setLength(other.getLength());
        this->copy(other.getTags());
        this->episode=other.episode;
        this->season=other.season;
        this->seriesName=other.seriesName;
    }
    //TDL:copyingID
    return *this;
}

//MoveAssigment
Episode &Episode::operator=(Episode &&other) {
    //do we need to make sure to delete other? is it this class's responsibility?

    this->operator=(other);
    this->DeleteOther(other);
    return *this;
}

std::string Episode::toString() const {
    std::stringstream st;
    st<<this->seriesName +" S"<<(this->season)<<+"E"<<(this->episode);
    std::string s=st.str();
    return  s;
}

Watchable* Episode::getNextWatchable(Session &session) const {
    //User* activeUser= session.getActiveUser();
    User* activeUser= nullptr;
    if(this->nextEpisodeId!=0)
    {

    }
    Watchable* nextToWatch= (*activeUser).getRecommendation(session);
    return nextToWatch;
}

void Episode::setNextID(long next_id)
{
    this->nextEpisodeId=next_id;
}

int Episode::getReccomendationOfContent() {
    if(nextEpisodeId>=0)
    {
        return nextEpisodeId;
    }
    else
    {
        return -99;
    }
}


//TDL: setNextID, nextEpisode

Episode* Episode::clone() {
    return new Episode(*this);
}