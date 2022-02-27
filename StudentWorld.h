#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld();
    void finishLev();
    void winGame();
    
// Accessors
    
    bool isBlockingObjectAt(int x, int y);
    bool overlap(const Actor* a1, const Actor* a2) const;
    bool intersect(double x1, double y1, double x2, double y2) const;
    bool overlapPeach(Actor* src);
    double getPeachX();
    double getPeachY();
    Peach* getPeach() { return m_peach; }
    Actor* overlappingActor(Actor* src);
    

// Mutators
    void addStar(int x, int y);
    void addFlower(int x, int y);
    void addMushroom(int x, int y);
    void addPiranhaFire(int x, int y, int dir);
    void addPeachFire(int x, int y, int dir);
    void addShell(int x, int y, int dir);
    
    void peachMushroom();
    void peachStar();
    void peachFlower();
    void bonkActorAt(int x, int y);
    void bonkPeach();
    
    

    
private:
    Peach* m_peach;
    vector<Actor*> actors;
    bool levelComplete = false;
    bool gameWon = false;
};

#endif // STUDENTWORLD_H_
