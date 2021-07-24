#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual ~User();
    User(User& other);
    User & operator=(const User &other); //Q
    User & operator=(User &&other); //Q
    User(User&& other);

    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual void setHistory(Watchable* toAdd); //Q
    void setName(std::string &newName);
    virtual User* clone()=0;
    const std::string getRec();
    std::string toString();
    // Compares two intervals according to staring times.

protected:
    std::vector<Watchable*> history;
    virtual void copy(const User& other);
    virtual void copyAndClean(User& other);
    std::string rec;

private:
    std::string name; //TDL: delete the const all over


};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    LengthRecommenderUser (LengthRecommenderUser& other);
    LengthRecommenderUser (LengthRecommenderUser&& other);
    virtual ~LengthRecommenderUser();
    LengthRecommenderUser & operator=(const LengthRecommenderUser &other); //Q
    LengthRecommenderUser & operator=(LengthRecommenderUser &&other); //Q
    virtual Watchable* getRecommendation(Session&s);
    LengthRecommenderUser* clone();
private:
    virtual void copyAndClean(LengthRecommenderUser& other);
    virtual void copy(const LengthRecommenderUser& other);
};

class RerunRecommenderUser : public User {
public:

    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser (RerunRecommenderUser& other);
    RerunRecommenderUser (RerunRecommenderUser&& other);
    virtual ~RerunRecommenderUser();
    RerunRecommenderUser & operator=(const RerunRecommenderUser &other); //Q
    RerunRecommenderUser & operator=(RerunRecommenderUser &&other); //Q
    virtual Watchable* getRecommendation(Session& s);
    RerunRecommenderUser* clone();
private:
    int lastRecommendedIndex;
    bool isFirst;
    virtual void copyAndClean(RerunRecommenderUser& other);
    virtual void copy(const RerunRecommenderUser& other);

};
class TagPopularity
{
public:

    TagPopularity(std::string tag);
    TagPopularity (TagPopularity& other);
    TagPopularity (TagPopularity&& other);
    virtual ~TagPopularity();
    TagPopularity & operator=(const TagPopularity &other); //Q
    TagPopularity & operator=(TagPopularity &&other); //Q
    void increment();
    void zero();

    std::string getTag();
    int getPopularity();

private:
    std::string tag;
    int popularity;



};
class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);

    GenreRecommenderUser (GenreRecommenderUser& other);
    GenreRecommenderUser (GenreRecommenderUser&& other);
    virtual ~GenreRecommenderUser();
    GenreRecommenderUser & operator=(const GenreRecommenderUser &other); //Q
    GenreRecommenderUser & operator=(GenreRecommenderUser &&other); //Q
    virtual Watchable* getRecommendation(Session& s);
    void setHistory(Watchable* toAdd);
    GenreRecommenderUser* clone();

private:
    std::vector<TagPopularity*> histogramPopularity;
    void copyHistogram(std::vector<TagPopularity*> popTags);
    virtual void copyAndClean(GenreRecommenderUser& other);
    virtual void copy(const GenreRecommenderUser& other);
    void loopByMostPopularToLastTag();
    void loopByAlphaBet();
    std::map<std::string,int> history_map;//only for tags
    Watchable* findByPopularity(Session &s,int& pop);
    Watchable* lookForInContent(Session &s,std::string tagToFind);
    void old1();
    void old2();
    int findBiggest();

};


#endif
