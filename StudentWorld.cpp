#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_peach = nullptr;
    actors.clear();
}

int StudentWorld::init()
{
    levelComplete = false;
    
    Level lev(assetPath());
    string level_file;
    if (getLevel() < 10) {
        level_file = "level0" + to_string(getLevel()) + ".txt";
      }
    else if (getLevel() < 100) {
        level_file = "level" + to_string(getLevel()) + ".txt";
      }
    else {
        return GWSTATUS_PLAYER_WON;
      }
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        Level::GridEntry ge;
        for (int lx = 0; lx<32; lx++) {
            for (int ly = 0; ly<32; ly++) {
                ge = lev.getContentsOf(lx, ly);
                double x = lx*SPRITE_WIDTH;
                double y = ly*SPRITE_HEIGHT;
                switch (ge) {
                    case Level::empty:
                        break;
                    case Level::peach:
                        m_peach = new Peach (this, x, y);
                        break;
                    case Level::goomba:
                        actors.push_back(new Goomba(this, x, y));
                        break;
                    case Level::koopa:
                        actors.push_back(new Koopa(this, x, y));
                        break;
                    case Level::piranha:
                        actors.push_back(new Piranha(this, x, y));
                        break;
                    case Level::block:
                        actors.push_back(new Block(this, x, y));
                        break;
                    case Level::star_goodie_block:
                        actors.push_back(new Block(this, "star", x, y));
                        break;
                    case Level::mushroom_goodie_block:
                        actors.push_back(new Block(this, "mushroom", x, y));
                        break;
                    case Level::flower_goodie_block:
                        actors.push_back(new Block(this, "flower", x, y));
                        break;
                    case Level::pipe:
                        actors.push_back(new Pipe(this, x, y));
                        break;
                    case Level::flag:
                        actors.push_back(new Flag(this, x, y));
                        break;
                    case Level::mario:
                        actors.push_back(new Mario(this, x, y));
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}
    

int StudentWorld::move()
{
    m_peach->doSomething();
    
    for (int i=0; i<actors.size(); i++) {
        if (!actors[i]->isDead()) {
            actors[i]->doSomething();
        
            if (m_peach->isDead()) {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if (levelComplete) {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
            if (gameWon) {
                playSound(SOUND_GAME_OVER);
                return GWSTATUS_PLAYER_WON;
            }
            
        }
    }
    
    for (auto it = actors.begin(); it != actors.end();) {
        if ((*it)->isDead()) {
            delete *it;
            actors.erase(it);
        }
        else
            it++;
    }
    
    ostringstream oss;
    
    oss << "Lives: " << getLives() << "  ";
    oss << "Level: " << getLevel() << "  ";
    
    oss.fill('0');
    oss << "Points: ";
    oss << setw(6) << getScore() << "  ";
    
    oss << m_peach->printPowers() << endl;
    
    setGameStatText(oss.str());
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (Actor* a: actors)
        delete a;
    actors.clear();

    delete m_peach;
    m_peach = nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

void StudentWorld::finishLev() { levelComplete = true; }

void StudentWorld::winGame() {
    gameWon = true;
    playSound(SOUND_GAME_OVER);
}

// ////////////
// Accessors //
// ////////////

bool StudentWorld::isBlockingObjectAt(int x, int y) {
    for(vector<Actor*>::iterator a = actors.begin(); a!=actors.end(); a++) {
        if ((*a)->getX() == (x) && (*a)->getY() == (y)) {
            if((*a)->isSolid())
                return true;
        }
    }
    return false;
}

bool StudentWorld::intersect(double x1, double y1, double x2, double y2) const {
    if (x1 >= x2+SPRITE_WIDTH-1 || y1 >= y2+SPRITE_HEIGHT-1)
        return false;
    if (x1+SPRITE_WIDTH-1 <= x2 || y1+SPRITE_WIDTH-1 <= y2)
        return false;
    return true;
}

bool StudentWorld::overlap(const Actor *a, const Actor* b) const{
    return intersect(a->getX(), a->getY(), b->getX(), b->getY());
}

bool StudentWorld::overlapPeach(Actor* src) {
    return overlap(m_peach, src);
}

double StudentWorld::getPeachX() { return m_peach->getX(); }

double StudentWorld::getPeachY() { return m_peach->getY(); }

Actor* StudentWorld::overlappingActor(Actor* src) {
    for(vector<Actor*>::iterator a = actors.begin(); a!=actors.end(); a++) {
        if (overlap(src, (*a)) && (*a)!=m_peach) {
            return (*a);
        }
    }
    return nullptr;
}
// ///////////
// Mutators //
// ///////////

void StudentWorld::addStar(int x, int y) {
    actors.push_back(new Star(this, x, y+8));
}

void StudentWorld::addFlower(int x, int y) {
    actors.push_back(new Flower(this, x, y+8));
}

void StudentWorld::addMushroom(int x, int y) {
    actors.push_back(new Mushroom(this, x, y+8));
}

void StudentWorld::addPiranhaFire(int x, int y, int dir) {
    actors.push_back(new PiranhaFireball(this, x, y, dir));
}

void StudentWorld::addPeachFire(int x, int y, int dir) {
    actors.push_back(new PeachFireball(this, x, y, dir));
}

void StudentWorld::addShell(int x, int y, int dir) {
    actors.push_back(new Shell(this, x, y, dir));
}

void StudentWorld::peachMushroom() { m_peach->getMushroom(); }

void StudentWorld::peachStar() { m_peach->getStar(); }

void StudentWorld::peachFlower() { m_peach->getFlower(); }

void StudentWorld::bonkActorAt(int x, int y)  {
    for(vector<Actor*>::iterator a = actors.begin(); a!=actors.end(); a++) {
        if ((*a)->getX() == (x) && (*a)->getY() == (y))
            (*a) -> bonk();
    }
}

void StudentWorld::bonkPeach() {
    m_peach->bonk();
}





