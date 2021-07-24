#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    //Constructor
    Watchable();
    Watchable(long id, int length, const std::vector<std::string>& tags); //D
    //Copy Constructor
    Watchable(const Watchable& other); //D
    //Destructor
    virtual ~Watchable(); //Q
    //Move Constructor
    Watchable(Watchable &&other); //Q
    //Move Assignment
    Watchable & operator=(const Watchable &other); //Q
    Watchable &operator=(Watchable &&other);
    virtual Watchable* clone()=0;
    virtual std::string toString() const = 0; //D
    virtual Watchable* getNextWatchable(Session&) const = 0; //D

    const long getID() const;
    const int getLength() const;
    const std::vector<std::string>& getTags() const;
    virtual int getReccomendationOfContent();
    //std::string toString();

protected:
    void clean();
    void setLength(int length);
    virtual void copy(const std::vector<std::string> &OtherTags);
    void DeleteOther (Watchable& other);
private:

    const long id;
    int length;
    std::vector<std::string> tags;



};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    ~Movie();
    Movie(const Movie& other);
    Movie(Movie &&other);
    Movie & operator=(const Movie &other);
    Movie& operator=(Movie &&other);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual int getReccomendationOfContent();
    std::string toString();
    Movie* clone();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    ~Episode();
    Episode(const Episode& other);
    Episode(Episode &&other);
    Episode& operator=(const Episode &other);
    Episode& operator=(Episode &&other);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    void setNextID(long next_id);
    virtual int getReccomendationOfContent();
    std::string toString();
    Episode* clone();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
