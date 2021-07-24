
#include "../include/User.h"
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "Session.h"
#include "Watchable.h"
#include <cmath>

#include <algorithm>
#include <bits/stdc++.h>
using namespace std;
//////////////////////////////////////////////////
///////          User
/////////////////////////////////////////////////
//Constructor
    User::User(const std::string& name): name(name), history()
    {

    }

    //Destructor
    User::~User() {
        history.clear();
    }

    //Copy Constructor
    User::User(User& other): name(other.name)
    {
        copy(other);
    }

    //Move Constructor
    User::User(User&& other): name(other.name)
    {
        copy(other);
        copyAndClean(other);
    }

    void User:: copy(const User& other)
    {
        history.clear();
        for (int i = 0; i < other.history.size(); ++i) {
            this->history.push_back(other.get_history().at(i));
        }
    }

    void User:: copyAndClean(User& other)
    {
        history.clear();
        for (int i = 0; i < other.history.size(); ++i) {
            this->history.push_back(other.history.at(i));
        }

        other.history.clear();
    }

    //Operator Assignment
    User & User::operator=(const User &other)
    {
        if(this != &other)
        {

            //TDL: How to assign const name
            this->name=name;
            copy(other);
        }
        return *this;
    }

    //Operator Move Assignment
    User & User::operator=(User &&other)
    {
        if(this != &other)
        {

            //TDL: How to assign const name?!?!?!?!?
            copyAndClean(other);
        }
        return *this;
    }


    std::string User:: getName() const
    {
        return this->name;
    }

    std::vector<Watchable*> User :: get_history() const
    {
        return this->history;
    }

    Watchable* User::getRecommendation(Session &s) {}

    void User::setHistory(Watchable *toAdd) {history.push_back(toAdd);}

    void User::setName(std::string &newName) {name=newName;}

    const std::string User::getRec() {
        return rec;
    }

//////////////////////////////////////////////////
///////          LengthRecommenderUser
/////////////////////////////////////////////////

    //Constructor
    LengthRecommenderUser::LengthRecommenderUser (const std::string& name):User(name)
    {
        rec="len";
    }

    //Destructor
    LengthRecommenderUser::~LengthRecommenderUser(){
        history.clear();
    }

    //Copy Constructor
    LengthRecommenderUser::LengthRecommenderUser (LengthRecommenderUser& other):User(other.getName())
    {
        copy(other);
    }


    //Move Constructor
    LengthRecommenderUser::LengthRecommenderUser (LengthRecommenderUser&& other):User(other.getName())
    {
        copyAndClean(other);
    }

    //Operator Assignment
    LengthRecommenderUser & LengthRecommenderUser::operator=(const LengthRecommenderUser &other)
    {
        if(this != &other)
        {

            //TDL: How to assign const name

            this->copy(other);
        }
        return *this;
    }

    //Operator Move Assignment
    LengthRecommenderUser & LengthRecommenderUser::operator=(LengthRecommenderUser &&other)
    {
        if(this != &other)
        {

            //TDL: How to assign const name?!?!?!?!?
            copyAndClean(other);
        }
        return *this;
    }

    Watchable* LengthRecommenderUser::getRecommendation(Session &s) {

        int distanceFromLength=INT32_MAX;
        Watchable* theClosest= nullptr;
        Watchable* lastViewed=s.getActiveUser()->get_history().at(s.getActiveUser()->get_history().size()-1);
        int next=lastViewed->getReccomendationOfContent();
        if(next>=0)
        {
            //cout<<"series algo"<<endl;
            return s.getContent().at(next-1);
        } else {


            for (int i = 0; i < s.getContent().size(); ++i) {
                Watchable *ptr = s.getContent().at(i);
                int diff = std::abs(ptr->getLength() - lastViewed->getLength());
                bool foundInHistory = false;
                if (diff < distanceFromLength) {
                    for (int j = 0; !foundInHistory && j < history.size(); ++j) {
                        if (history.at(j)->getID() == (ptr->getID())) {
                            foundInHistory = true;
                        }
                    }
                    if (!foundInHistory) {
                        distanceFromLength = diff;
                        theClosest = s.getContent().at(i);
                    }
                }
                ptr = nullptr;

            }

            return theClosest;
        }

    }

void LengthRecommenderUser:: copy(const LengthRecommenderUser& other)
{
    history.clear();
//    for (int i = 0; i < other.history.size(); ++i) {
//        this->history.push_back(other.history.at(i));
//    }
}

void LengthRecommenderUser:: copyAndClean(LengthRecommenderUser& other)
{
    history.clear();
//    for (int i = 0; i < other.history.size(); ++i) {
//        this->history.push_back(other.history.at(i));
//    }

    other.history.clear();
}

LengthRecommenderUser* LengthRecommenderUser::clone() {
        return new LengthRecommenderUser(*this);
    }

//////////////////////////////////////////////////
///////          RerunRecommenderUser
/////////////////////////////////////////////////
//Constructor
RerunRecommenderUser::RerunRecommenderUser (const std::string& name):User(name)
{
    rec="rer";
    lastRecommendedIndex=0;
    isFirst=true;
}

//Destructor
RerunRecommenderUser::~RerunRecommenderUser(){
    history.clear();
}

//Copy Constructor
RerunRecommenderUser::RerunRecommenderUser (RerunRecommenderUser& other):User(other.getName())
{
    lastRecommendedIndex=other.lastRecommendedIndex;
    copy(other);
}


//Move Constructor
RerunRecommenderUser::RerunRecommenderUser (RerunRecommenderUser&& other):User(other.getName())
{
    lastRecommendedIndex=other.lastRecommendedIndex;
    copyAndClean(other);
}

//Operator Assignment
RerunRecommenderUser & RerunRecommenderUser::operator=(const RerunRecommenderUser &other)
{
    if(this != &other)
    {
        //TDL: How to assign const name
        this->copy(other);
        lastRecommendedIndex=other.lastRecommendedIndex;
    }
    return *this;
}

//Operator Move Assignment
RerunRecommenderUser & RerunRecommenderUser::operator=(RerunRecommenderUser &&other)
{
    if(this != &other)
    {

        //TDL: How to assign const name?!?!?!?!?
        copyAndClean(other);
        lastRecommendedIndex=other.lastRecommendedIndex;
    }
    return *this;
}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    Watchable* lastViewed=s.getActiveUser()->get_history().at(s.getActiveUser()->get_history().size()-1);
    int next=lastViewed->getReccomendationOfContent();
    if(next>=0)
    {
        return s.getContent().at(next-1);
    } else {
        if(isFirst)
        {
            isFirst=false;
            return this->history.at(0);
        }

        else if(history.size()>0) {
            int i = this->lastRecommendedIndex;
            int n=history.size();


            int NewIndex = (i+1 ) % (n);
            lastRecommendedIndex = NewIndex;
            return this->history.at(NewIndex);
        }
    }
}

void RerunRecommenderUser:: copy(const RerunRecommenderUser& other)
{
    history.clear();

//    for (int i = 0; i < other.history.size(); ++i) {
//        this->history.push_back(other.history.at(i));
//    }
    this->lastRecommendedIndex=other.lastRecommendedIndex;
    this->isFirst=other.isFirst;


}

void RerunRecommenderUser:: copyAndClean(RerunRecommenderUser& other)
{
    history.clear();
//    for (int i = 0; i < other.history.size(); ++i) {
//        this->history.push_back(other.history.at(i));
//    }

    other.history.clear();
}

RerunRecommenderUser* RerunRecommenderUser::clone() {
        return new RerunRecommenderUser(*this);
    }

//////////////////////////////////////////////////
///////          GenreRecommenderUser
/////////////////////////////////////////////////
//Constructor
GenreRecommenderUser::GenreRecommenderUser (const std::string& name):User(name)
{
    rec="gen";
}

//Destructor
GenreRecommenderUser::~GenreRecommenderUser(){
    history.clear();
    for (int i = 0; i < histogramPopularity.size(); ++i) {
        //delete(histogramPopularity.at(i));
        histogramPopularity.at(i)= nullptr;
    }
    histogramPopularity.clear();

}

//Copy Constructor
GenreRecommenderUser::GenreRecommenderUser (GenreRecommenderUser& other):User(other.getName())
{
    copy(other);
    copyHistogram(other.histogramPopularity);

}

void GenreRecommenderUser::copyHistogram(std::vector<TagPopularity*> popTags)
{
    for (int i = 0; i < histogramPopularity.size(); ++i) {
        //delete(histogramPopularity.at(i));
        histogramPopularity.at(i)= nullptr;
    }
    histogramPopularity.clear();
    for (int i = 0; i <popTags.size(); ++i) {
        this->histogramPopularity.push_back(popTags.at(i));
    }
}


//Move Constructor
GenreRecommenderUser::GenreRecommenderUser (GenreRecommenderUser&& other):User(other.getName())
{

    copyAndClean(other);
    copyHistogram(other.histogramPopularity);
}

//Operator Assignment
GenreRecommenderUser & GenreRecommenderUser::operator=(const GenreRecommenderUser &other)
{
    if(this != &other)
    {
        //TDL: How to assign const name
        this->copy(other);
        copyHistogram(other.histogramPopularity);
    }
    return *this;
}

//Operator Move Assignment
GenreRecommenderUser & GenreRecommenderUser::operator=(GenreRecommenderUser &&other)
{
    if(this != &other)
    {

        //TDL: How to assign const name?!?!?!?!?
        copyAndClean(other);
        copyHistogram(other.histogramPopularity);
    }
    return *this;
}

void GenreRecommenderUser::setHistory(Watchable *toAdd) {


        history.push_back(toAdd);

        for (int j = 0; j < toAdd->getTags().size(); ++j) {
            bool found = false;

            if(history_map.find(toAdd->getTags().at(j))!=history_map.end())
            {
                history_map[toAdd->getTags().at(j)]++;
            }
            if(!found) {
                history_map.insert(make_pair(toAdd->getTags().at(j),1));
                TagPopularity tag(toAdd->getTags().at(j));
                //histogramPopularity.push_back(TagPopularity(toAdd->getTags().at(j)));
                histogramPopularity.push_back(&tag);
            }
        }
}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    Watchable *lastViewed = s.getActiveUser()->get_history().at(s.getActiveUser()->get_history().size() - 1);
    int next = lastViewed->getReccomendationOfContent();
    if (next >= 0) {
        return s.getContent().at(next-1);
    } else {
        Watchable *currentMovieEpisode = nullptr;
        if (histogramPopularity.size() > 0) {
            bool notFound = true;
            int max=findBiggest();

            while (currentMovieEpisode== nullptr && max>0)
            {
                currentMovieEpisode=findByPopularity(s,max);
                max--;
            }
            ///TDL:check about delete pinter currentMovieEpisode
            return currentMovieEpisode;
        }
    }
}

int GenreRecommenderUser::findBiggest()
{
        int max=0;

    for (auto x: history_map) {
        if(x.second>max)
        {
            max=x.second;
        }

    }
    //cout<<"finding biggest"<<max<<endl;
    return max;
}

Watchable* GenreRecommenderUser:: findByPopularity(Session &s, int& pop)
{
    Watchable* currentMovieEpisode= nullptr;
    for (auto x: history_map) {
        if(x.second==pop)
        {
            currentMovieEpisode=lookForInContent(s,x.first);
            if(currentMovieEpisode!= nullptr) {
                break;
            }
        }

    }
    //cout<<"finding popularity in historymap: "<<pop<<endl;
    return currentMovieEpisode;
}

Watchable* GenreRecommenderUser::lookForInContent(Session &s, string tagToFind)
{
    Watchable *currentMovieEpisode= nullptr;
        bool foundInContent= false;
        bool foundInHistory=false;
    for (int idxOfContent = 0; !foundInContent && idxOfContent < s.getContent().size(); ++idxOfContent) {
        //running on content
        currentMovieEpisode = s.getContent().at(idxOfContent);
        for (int idxOfTagsInMovieEpisode = 0; !foundInContent && idxOfTagsInMovieEpisode <
                                                          currentMovieEpisode->getTags().size(); ++idxOfTagsInMovieEpisode) {
            foundInHistory=false;
            //running on content's tags
            if (currentMovieEpisode->getTags().at(idxOfTagsInMovieEpisode).compare(tagToFind) == 0) {

                for (int idxInHistory = 0;!foundInHistory&& idxInHistory < this->history.size(); ++idxInHistory) {
                    if (history.at(idxInHistory) == currentMovieEpisode) {
                        foundInHistory=true;
                    }
                }
                if(!foundInHistory)
                {
                    foundInContent=true;
                }
            }
        }
    }
    if(foundInContent && !foundInHistory)
    {
        //cout<<"finding tags in content: "<<tagToFind<<endl;
        return currentMovieEpisode;
    } else
    {
        //cout<<"finding tags in content: "<<tagToFind<<" and nullptr"<<endl;
        return nullptr;
    }

}







void GenreRecommenderUser:: copy(const GenreRecommenderUser& other)
{
    history.clear();
//    for (int i = 0; i < other.history.size(); ++i) {
//        this->history.push_back(other.history.at(i));
//    }

}

void GenreRecommenderUser:: copyAndClean(GenreRecommenderUser& other)
{
    history.clear();
//    for (int i = 0; i < other.history.size(); ++i) {
//        this->history.push_back(other.history.at(i));
//    }

    this->copyHistogram(other.histogramPopularity);

    other.history.clear();
}

GenreRecommenderUser* GenreRecommenderUser::clone() {
        return new GenreRecommenderUser(*this);
    }


   // int maxTag=histogramPopularity.at(0)->getPopularity();
   // TagPopularity *maxTag_ptr= histogramPopularity.at(0);
   // for (int i = 1; i < histogramPopularity.size(); ++i) {
   //     if(histogramPopularity.at(i)->getPopularity()>maxTag)
   //     {
   //         maxTag=histogramPopularity.at(i)->getPopularity();
   //         maxTag_ptr=histogramPopularity.at(i);
   //     }
   // }
   // std::


//////////////////////////////////////////////////
///////          TagPopularity
/////////////////////////////////////////////////

TagPopularity::TagPopularity(std::string tag):tag(tag) {popularity=1;}
void TagPopularity::increment() {popularity++;}
void TagPopularity::zero() {popularity=0;}

//Destructor
TagPopularity::~TagPopularity(){}



//Copy Constructor
TagPopularity::TagPopularity (TagPopularity& other):tag(other.tag), popularity(other.popularity){}



//Move Constructor
TagPopularity::TagPopularity (TagPopularity&& other):tag(other.tag), popularity(other.popularity){}


//Operator Assignment
TagPopularity & TagPopularity::operator=(const TagPopularity &other)
{
    if(this != &other)
    {
        tag=other.tag;
        popularity=other.popularity;
    }
    return *this;
}

//Operator Move Assignment
TagPopularity & TagPopularity::operator=(TagPopularity &&other)
{
    if(this != &other)
    {
        tag=other.tag;
        popularity=other.popularity;
    }
    return *this;
}
int TagPopularity::getPopularity() { return popularity;}
std::string TagPopularity::getTag() {return tag;}



void GenreRecommenderUser::old1() {
    /*
history.push_back(toAdd);
  //find if tag exists in histogram
//            for (int i = 0; !found && i < histogramPopularity.size(); ++i) {
//                if (histogramPopularity.at(i)->getTag().compare(toAdd->getTags().at(j)) == 0) {
//                    found = true;
//                    histogramPopularity.at(i)->increment();
//
//                }
//            }
//for each tag of new to watchable
for (int j = 0; j < toAdd->getTags().size(); ++j) {
bool found= false;
//find if tag exists in histogram
for (int i = 0; !found && i < histogramPopularity.size(); ++i) {
  if (histogramPopularity.at(i)->getTag().compare(toAdd->getTags().at(j))==0)
  {
      found=true;
      histogramPopularity.at(i)->increment();
      indexOfFoundedTag=i;
  }
}

//adding new value to history
bool notGreater=true;

int idx;
if(found)
{
  idx=indexOfFoundedTag;
}
if(!found) {

  histogramPopularity.push_back(new TagPopularity(toAdd->getTags().at(j)));
  idx = histogramPopularity.size()-1;
}


while(notGreater && idx>0)
{
  if(histogramPopularity.at(idx)->getPopularity()==histogramPopularity.at(idx-1)->getPopularity())
  {
      if(histogramPopularity.at(idx)->getTag().compare(histogramPopularity.at(idx-1)->getTag())<0)
      {
          TagPopularity* temp=histogramPopularity.at(idx);
          histogramPopularity.at(idx)=histogramPopularity.at(idx-1);
          histogramPopularity.at(idx-1)=temp;
      } else
          {
          notGreater=false;
          }
  } else{
      notGreater= false;
  }
}
}
*/
    }
    void GenreRecommenderUser::old2() {
//        //sort(histogramPopularity.begin(), histogramPopularity.end(), compareInterval);
//        std::sort(histogramPopularity.begin(), histogramPopularity.end(), []( TagPopularity& a, TagPopularity& b)
//        { return a.getPopularity() < b.getPopularity(); });
//
//        //histogramPopularity.sort
//        for (int idxOfHistogramTags = 0;
//             notFound && idxOfHistogramTags < this->histogramPopularity.size(); ++idxOfHistogramTags) {
//
//
//            for (int idxOfHistogramTags = 0;
//                 notFound && idxOfHistogramTags < this->histogramPopularity.size(); ++idxOfHistogramTags) {
//                std::string tagToFind = histogramPopularity.at(idxOfHistogramTags)->getTag();
//                for (int idxOfContent = 0; notFound && idxOfContent < s.getContent().size(); ++idxOfContent) {
//                    currentMovieEpisode = s.getContent().at(idxOfContent);
//                    for (int idxOfTagsInMovieEpisode = 0; notFound && idxOfTagsInMovieEpisode <
//                                                                      currentMovieEpisode->getTags().size(); ++idxOfTagsInMovieEpisode) {
//                        if (currentMovieEpisode->getTags().at(idxOfTagsInMovieEpisode).compare(tagToFind) == 0) {
//                            for (int idxInHistory = 0; idxInHistory < this->history.size(); ++idxInHistory) {
//                                if (history.at(idxInHistory) != currentMovieEpisode) {
//                                    notFound = false;
//                                }
//                            }
//                        }
//                    }
//                }
//
//            }
//        }
    }
