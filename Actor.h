#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>
#include <string>
#include <set>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;


class Actor: public GraphObject {
public:
    Actor(StudentWorld *world, int imageID, int startX, int startY, int dir, int depth, double size);
    bool isDead();
    virtual void doSomething() = 0;
    virtual void bonk() = 0;
    void setDead();
    StudentWorld* getWorld();
    virtual bool isDamageable() { return false; }
    virtual bool isSolid() { return false; }
    virtual void getDamaged() { return; }
    bool canGoLeft();
    bool canGoRight();
    bool canGoDown();
    bool goLeft();
    bool canGoUp();
    
private:
    StudentWorld *m_world;
    bool m_dead;
};

class Peach: public Actor {
public:
    Peach(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    bool isInvincible();
    virtual void bonk();
    virtual bool isDamageable() { return true; }
    void getMushroom();
    void getFlower();
    void getStar();
    std::string printPowers();
    
private:
    int m_health;
    int invincibleTime;
    bool m_invincible;
    int TinvincibleTime;
    bool t_invincible;
    std::set<std::string> powers;
    bool hasJumpPower;
    bool hasShootPower;
    int time_to_recharge_before_next_fire;
    int remainingJumpDistance;
    
    
};

class Mario: public Actor {
public:
    Mario(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual void bonk();
};

class Enemy: public Actor {
public:
    Enemy(StudentWorld* world, int imageID, int startX, int startY);
    virtual void doSomething();
    virtual bool isDamageable() { return true; }
    virtual void bonk();
    virtual void getDamaged();
    bool extends();
    
    
};

class Goomba: public Enemy {
public:
    Goomba(StudentWorld* world, int startX, int startY);
};

class Koopa: public Enemy {
public:
    Koopa(StudentWorld* world, int startX, int startY);
    virtual void getDamaged();
};

class Piranha: public Enemy {
public:
    Piranha(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
private:
    int firingDelay;
    
};

class Projectile: public Actor {
public:
    Projectile(StudentWorld* world, int imageID, int startX, int startY, int dir);
    virtual void doSomething();
    virtual void bonk() { return; }
};

class PiranhaFireball: public Projectile {
public:
    PiranhaFireball(StudentWorld* world, int startX, int startY, int dir);
    virtual void doSomething();
};

class PeachFireball: public Projectile {
public:
    PeachFireball(StudentWorld* world, int startX, int startY, int dir);
};

class Shell: public Projectile {
public:
    Shell(StudentWorld* world, int startX, int startY, int dir);
};


class Block: public Actor {
public:
    Block(StudentWorld* world, int startX, int startY);
    Block(StudentWorld* world, std::string blockTyp, int startX, int startY);
    virtual void doSomething();
    virtual void bonk();
    virtual bool isSolid() { return true; }
    
private:
    bool released;
    bool hasGoodie;
    std::string blockType;
};

class Pipe: public Actor {
public:
    Pipe(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual void bonk();
    virtual bool isSolid() { return true; }
};

class Goodie: public Actor {
public:
    Goodie(StudentWorld* world, int imageID, int startX, int startY);
    virtual void bonk() { return; }
};

class Mushroom: public Goodie {
public:
    Mushroom(StudentWorld* world, int startX, int startY);
    virtual void doSomething();

};

class Flower: public Goodie {
public:
    Flower(StudentWorld* world, int startX, int startY);
    virtual void doSomething();

};

class Star: public Goodie {
public:
    Star(StudentWorld* world, int startX, int startY);
    virtual void doSomething();

};

class Flag: public Actor {
public:
    Flag(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual void bonk() { return; };
};



#endif // ACTOR_H_
