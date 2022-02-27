#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
int randomDirection() {
    int x = randInt(0,1);
    if (x == 0)
        return 0;
    else
        return 180;
}

Actor::Actor(StudentWorld *world, int imageID, int startX, int startY, int dir, int depth, double size):
GraphObject(imageID, startX, startY, dir, depth, size) {
    m_dead = false;
    m_world = world;
};

bool Actor::canGoLeft() {
    int curY = getY();
    int mod = curY%SPRITE_HEIGHT;
    int curX = getX();
    if (curX%2 == 0) {
        if (mod == 0) {
            if (getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH, curY))
                return false;
            else
                return true;
        }
        if (getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH, curY-mod))
            return false;
        else if (getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH, curY+(SPRITE_HEIGHT-mod)))
            return false;
        else
            return true;
    }
    else {
        if (mod == 0) {
            if (getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH-1, curY))
                return false;
            else
                return true;
        }
        if (getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH-1, curY-mod))
            return false;
        else if (getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH-1, curY+(SPRITE_HEIGHT-mod)))
            return false;
        else
            return true;
    }
}

bool Actor::canGoRight() {
    int curY = getY();
    int mod = curY%SPRITE_HEIGHT;
    int curX = getX();
    if (curX%2 == 0) {
        if (mod == 0) {
            if (getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH, curY))
                return false;
            else
                return true;
        }
        if (getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH, curY-mod))
            return false;
        else if (getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH, curY+(SPRITE_HEIGHT-mod)))
            return false;
        else
            return true;
    }
    else {
        if (mod == 0) {
            if (getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH+1, curY))
                return false;
            else
                return true;
        }
        if (getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH+1, curY-mod))
            return false;
        else if (getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH+1, curY+(SPRITE_HEIGHT-mod)))
            return false;
        else
            return true;
    }
}

bool Actor::canGoUp() {
    int curX = getX();
    int mod = curX%SPRITE_WIDTH;
    if (mod == 0) {
        if (getWorld()->isBlockingObjectAt(curX, getY()+SPRITE_HEIGHT))
            return false;
        else return true;
    }
    if (getWorld()->isBlockingObjectAt(curX - mod, getY()+SPRITE_HEIGHT))
        return false;
    if (getWorld()->isBlockingObjectAt(curX + (SPRITE_WIDTH-mod), getY()+SPRITE_HEIGHT))
        return false;
    return true;
}

bool Actor::canGoDown() {
    int curX = getX();
    int mod = curX%SPRITE_WIDTH;
    if (mod == 0) {
        if (getWorld()->isBlockingObjectAt(curX, getY()-SPRITE_HEIGHT))
            return false;
        else return true;
    }
    if (getWorld()->isBlockingObjectAt(curX - mod, getY()-SPRITE_HEIGHT))
        return false;
    if (getWorld()->isBlockingObjectAt(curX + (SPRITE_WIDTH-mod), getY()-SPRITE_HEIGHT))
        return false;
    return true;
}

bool Actor::isDead() { return m_dead; }

void Actor::setDead() { m_dead = true; }

StudentWorld* Actor::getWorld() { return m_world; }

// ////////
// Peach //
// ////////

Peach::Peach(StudentWorld* world, int startX, int startY):
Actor(world, IID_PEACH, startX, startY, 0, 0, 1.0) {
    invincibleTime = 0;
    m_invincible = false;
    TinvincibleTime = 0;
    t_invincible = false;
    m_health = 1;
    hasJumpPower = false;
    hasShootPower = false;
    remainingJumpDistance = 0;
    time_to_recharge_before_next_fire = 0;
}

bool Peach::isInvincible() { return m_invincible; }

void Peach::doSomething() {
    if (isDead())
        return;
    if (m_invincible) {
        invincibleTime--;
        if (invincibleTime == 0) {
            m_invincible = false;
            powers.erase("Star Power!");
        }
    }
    if (t_invincible) {
        TinvincibleTime--;
        if (TinvincibleTime == 0)
            t_invincible = false;
    }
    if (remainingJumpDistance > 0) {
        if (!canGoUp()) {
            int curX = getX();
            if (curX%SPRITE_WIDTH == 0) {
                getWorld()->bonkActorAt(getX(), getY()+SPRITE_HEIGHT);
                remainingJumpDistance = 0;
            }
            else {
                if (getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH/2, getY()+SPRITE_HEIGHT)) {
                    getWorld()->bonkActorAt(getX()-SPRITE_WIDTH/2, getY()+SPRITE_HEIGHT);
                    remainingJumpDistance = 0;
                }
                if (getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH/2, getY()+SPRITE_HEIGHT)) {
                    getWorld()->bonkActorAt(getX()+SPRITE_WIDTH/2, getY()+SPRITE_HEIGHT);
                    remainingJumpDistance = 0;
                }
            }
        }
        else {
            moveTo(getX(), getY()+4);
            remainingJumpDistance--;
        }
    }
    if (canGoDown() && remainingJumpDistance == 0) {
        moveTo(getX(), getY()-4);
    }
    if (time_to_recharge_before_next_fire > 0)
        time_to_recharge_before_next_fire--;
    
    Actor* targ = getWorld()->overlappingActor(this);
    if (targ) {
        targ->bonk();
    }
    
    int ch;
    if (getWorld()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_LEFT:
                setDirection(180);
                if (canGoLeft())
                    moveTo(getX()-4, getY());
                else {
                    int cY = getY();
                    if (cY%SPRITE_HEIGHT == 0) {
                        getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH, getY());
                        getWorld()->bonkActorAt(getX()-SPRITE_WIDTH, getY());
                    }
                    else {
                        if (getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH, getY()+SPRITE_HEIGHT/2)) {
                            getWorld()->bonkActorAt(getX()-SPRITE_WIDTH, getY()+SPRITE_HEIGHT/2);
                        }
                        if (getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH, getY()-SPRITE_HEIGHT/2)) {
                            getWorld()->bonkActorAt(getX()-SPRITE_WIDTH, getY()-SPRITE_HEIGHT/2);
                        }
                    }
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                if (canGoRight())
                    moveTo(getX()+4, getY());
                else {
                    int cY = getY();
                    if (cY%SPRITE_HEIGHT == 0) {
                        getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH, getY());
                        getWorld()->bonkActorAt(getX()+SPRITE_WIDTH, getY());
                    }
                    else {
                        if (getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT/2)) {
                            getWorld()->bonkActorAt(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT/2);
                        }
                        if (getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH, getY()-SPRITE_HEIGHT/2)) {
                            getWorld()->bonkActorAt(getX()+SPRITE_WIDTH, getY()-SPRITE_HEIGHT/2);
                        }
                    }
                }
                break;
            case KEY_PRESS_UP:
                if (canGoDown())
                    break;
                if (canGoUp()) {
                    if (hasJumpPower)
                        remainingJumpDistance = 12;
                    else
                        remainingJumpDistance = 8;
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                    moveTo(getX(), getY()+4);
                }
                break;
            case KEY_PRESS_SPACE:
                if (!hasShootPower ||  time_to_recharge_before_next_fire > 0)
                    break;
                else {
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    time_to_recharge_before_next_fire = 8;
                    if (getDirection() == 0 && canGoRight()) {
                        getWorld()->addPeachFire(getX() + 4, getY(), getDirection());
                    }
                    if (getDirection() == 180 && canGoLeft()) {
                        getWorld()->addPeachFire(getX() - 4, getY(), getDirection());
                    }
                    break;
                }
            default:
                break;
        }
    }
}

void Peach::getMushroom() {
    getWorld()->increaseScore(75);
    powers.insert("Jump Power!");
    hasJumpPower = true;
    m_health = 2;
    getWorld()->playSound(SOUND_PLAYER_POWERUP);
}

void Peach::getFlower() {
    getWorld()->increaseScore(50);
    powers.insert("Shoot Power!");
    hasShootPower = true;
    m_health = 2;
    getWorld()->playSound(SOUND_PLAYER_POWERUP);
}

void Peach::getStar() {
    getWorld()->increaseScore(100);
    powers.insert("Star Power!");
    m_invincible = true;
    invincibleTime = 150;
    m_health = 2;
    getWorld()->playSound(SOUND_PLAYER_POWERUP);
}

void Peach::bonk() {
    if (t_invincible || m_invincible)
        return;
    m_health--;
    TinvincibleTime = 10;
    t_invincible = true;
    hasShootPower = false;
    hasJumpPower = false;
    powers.clear();
    if (m_health > 0)
        getWorld()->playSound(SOUND_PLAYER_HURT);
    if (m_health == 0) {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
}

std::string Peach::printPowers() {
    std::string returnThis;
    for (set<std::string>::iterator it = powers.begin(); it!=powers.end(); it++) {
        returnThis = returnThis + (*it) + " ";
    }
    return returnThis;
}
// ////////
// Mario //
// ////////

Mario::Mario(StudentWorld* world, int startX, int startY):
Actor(world, IID_MARIO, startX, startY, 0, 1, 1) {
}

void Mario::doSomething() {
    if(isDead())
        return;
    if (getWorld()->overlapPeach(this)) {
        getWorld()->increaseScore(1000);
        setDead();
        getWorld()->winGame();
    }
}

void Mario::bonk() { return; }

// ////////
// Block //
// ////////
Block::Block(StudentWorld* world, int startX, int startY):
Actor(world, IID_BLOCK, startX, startY, 0, 2, 1) {
    released = false;
    blockType = "normal";
    hasGoodie = false;
}

Block::Block(StudentWorld* world, std::string blockTyp, int startX, int startY):
Actor(world, IID_BLOCK, startX, startY, 0, 2, 1) {
    released = false;
    blockType = blockTyp;
    hasGoodie = true;
}

void Block::doSomething() { return; }

void Block::bonk() {
    if (!hasGoodie || released) {
        getWorld()->playSound(SOUND_PLAYER_BONK);
        return;
    }
    if (hasGoodie && !released) {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        if (blockType == "star") {
            getWorld()->addStar(getX(), getY());
            released = true;
            hasGoodie = false;
        }
        if (blockType == "mushroom") {
            getWorld()->addMushroom(getX(), getY());
            released = true;
            hasGoodie = false;
        }
        if(blockType == "flower") {
            getWorld()->addFlower(getX(), getY());
            released = true;
            hasGoodie = false;
        }
    }
}

Pipe::Pipe(StudentWorld* world, int startX, int startY):
Actor(world, IID_PIPE, startX, startY, 0, 2, 1) {}

void Pipe::doSomething() { return; }

void Pipe::bonk() { return; }

// //////////
// Enemies //
// //////////
Enemy::Enemy(StudentWorld* world, int imageID, int startX, int startY):
Actor(world, imageID, startX, startY, randomDirection(), 0, 1) { // change direction to random 1 or 180
}

bool Enemy::extends() {
    int curX = getX();
    if (getDirection() == 0) {
        curX = curX - curX%SPRITE_WIDTH;
        if (!getWorld()->isBlockingObjectAt(curX + SPRITE_WIDTH, getY() - SPRITE_HEIGHT))
            return true;
    }
    if (getDirection() == 180) {
        curX = curX + (SPRITE_WIDTH - curX%SPRITE_WIDTH);
        if (!getWorld()->isBlockingObjectAt(curX - SPRITE_WIDTH, getY() - SPRITE_HEIGHT))
            return true;
    }
    return false;
}

void Enemy::doSomething() {
    if (isDead())
        return;
    if(getWorld()->overlapPeach(this)) {
        getWorld()->bonkPeach();
        return;
    }
    if (getDirection() == 0) {
        if(!canGoRight() || extends()) {
            setDirection(180);
            return;
        }
        else {
            moveTo(getX()+1, getY());
        }
    }
    if (getDirection() == 180) {
        if (!canGoLeft() || extends()) {
            setDirection(0);
            return;
        }
        else {
            moveTo(getX()-1, getY());
        }
    }
}

void Enemy::bonk() {
    if (getWorld()->getPeach()->isInvincible()) {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getDamaged();
    }
    return;
}

void Enemy::getDamaged() {
    getWorld()->increaseScore(100);
    setDead();
}

Goomba::Goomba(StudentWorld* world, int startX, int startY):
Enemy(world, IID_GOOMBA, startX, startY) {}

Koopa::Koopa(StudentWorld* world, int startX, int startY):
Enemy(world, IID_KOOPA, startX, startY) {}

void Koopa::getDamaged() {
    getWorld()->increaseScore(100);
    setDead();
    getWorld()->addShell(getX(), getY(), getDirection());
    return;
}

Piranha::Piranha(StudentWorld* world, int startX, int startY):
Enemy(world, IID_PIRANHA, startX, startY) {
    firingDelay = 0;
}
void Piranha::doSomething() {
    if (isDead())
        return;
    increaseAnimationNumber();
    if (getWorld()->overlapPeach(this)) {
        getWorld()->bonkPeach();
        return;
    }
    if (getWorld()->getPeachY() <= 1.5*SPRITE_HEIGHT+getY() && getWorld()->getPeachY() >= getY()-1.5*SPRITE_HEIGHT) {
        if (getWorld()->getPeachX() > getX()) {
            setDirection(0);
        }
        else if (getWorld()->getPeachX() < getX()) {
            setDirection(180);
        }
        if (firingDelay > 0) {
            firingDelay--;
            return;
        }
        if (firingDelay == 0) {
            if (getWorld()->getPeachX() < 8*SPRITE_WIDTH + getX() && getWorld()->getPeachX() > getX() - 8*SPRITE_HEIGHT) {
                getWorld()->addPiranhaFire(getX(), getY(), getDirection());
                firingDelay = 40;
            }
        }
    }
    return;
}
// //////////////
// Projectiles //
// //////////////

Projectile::Projectile(StudentWorld* world, int imageID, int startX, int startY, int dir):
Actor(world, imageID, startX, startY, dir, 1, 1) {}

void Projectile::doSomething() {
    if (isDead())
        return;
    Actor* target = getWorld()->overlappingActor(this);
    if (target && !target->isDead() && target->isDamageable()) {
        target->getDamaged();
        setDead();
        return;
    }
    
    if (canGoDown()) {
        moveTo(getX(), getY()-2);
    }
    
    if (getDirection() == 0) {
        if(!canGoRight()) {
            setDead();
            return;
        }
        else {
            moveTo(getX()+2, getY());
            return;
        }
    }
    if (getDirection() == 180) {
        if(!canGoLeft()) {
            setDead();
            return;
        }
        else {
            moveTo(getX()-2, getY());
            return;
        }
    }
    
}

PiranhaFireball::PiranhaFireball(StudentWorld* world, int startX, int startY, int dir):
Projectile(world, IID_PIRANHA_FIRE, startX, startY, dir) {}


void PiranhaFireball::doSomething() {
    if (getWorld()->overlapPeach(this)) {
        getWorld()->bonkPeach();
        setDead();
        return;
    }
    if (canGoDown()) {
        moveTo(getX(), getY()-2);
    }
    if (getDirection() == 0) {
        if(!canGoRight()) {
            setDead();
            return;
        }
        else {
            moveTo(getX()+2, getY());
        }
    }
    if (getDirection() == 180) {
        if(!canGoLeft()) {
            setDead();
            return;
        }
        else {
            moveTo(getX()-2, getY());
        }
    }
    return;
}


PeachFireball::PeachFireball(StudentWorld* world, int startX, int startY, int dir):
Projectile(world, IID_PEACH_FIRE, startX, startY, dir) {}

Shell::Shell(StudentWorld* world, int startX, int startY, int dir):
Projectile(world, IID_SHELL, startX, startY, dir) {}

// //////////
// Goodies //
// //////////

Goodie::Goodie(StudentWorld* world, int imageID, int startX, int startY):
Actor(world, imageID, startX, startY, 0, 1, 1) {}

Mushroom::Mushroom(StudentWorld* world, int startX, int startY):
Goodie(world, IID_MUSHROOM, startX, startY) {}

void Mushroom::doSomething() {
    if (getWorld()->overlapPeach(this)) {
        getWorld()->peachMushroom();
        setDead();
        return;
    }
    if (canGoDown())
        moveTo(getX(), getY()-2);
    
    if (getDirection() == 0) {
        if(!canGoRight()) {
            setDirection(180);
            return;
        }
        else {
            moveTo(getX()+2, getY());
        }
    }
    if (getDirection() == 180) {
        if(!canGoLeft()) {
            setDirection(0);
            return;
        }
        else {
            moveTo(getX()-2, getY());
        }
    }
}


Flower::Flower(StudentWorld* world, int startX, int startY):
Goodie(world, IID_FLOWER, startX, startY) {}

void Flower::doSomething() {
    if (getWorld()->overlapPeach(this)) {
        getWorld()->peachFlower();
        setDead();
    }
    if (canGoDown()) {
        moveTo(getX(), getY()-2);
    }
    if (getDirection() == 0) {
        if(!canGoRight()) {
            setDirection(180);
            return;
        }
        else {
            moveTo(getX()+2, getY());
        }
    }
    if (getDirection() == 180) {
        if(!canGoLeft()) {
            setDirection(0);
            return;
        }
        else {
            moveTo(getX()-2, getY());
        }
    }
    
}

Star::Star(StudentWorld* world, int startX, int startY):
Goodie(world, IID_STAR, startX, startY) {}

void Star::doSomething() {
    if (getWorld()->overlapPeach(this)) {
        getWorld()->peachStar();
        setDead();
    }
    if (canGoDown()) {
        moveTo(getX(), getY()-2);
    }
    else {
        if (getDirection() == 0) {
            if(!canGoRight()) {
                setDirection(180);
                return;
            }
            else {
                moveTo(getX()+2, getY());
            }
        }
        if (getDirection() == 180) {
            if(!canGoLeft()) {
                setDirection(0);
                return;
            }
            else {
                moveTo(getX()-2, getY());
            }
        }
    }
}

// ///////
// Flag //
// ///////

Flag::Flag(StudentWorld* world, int startX, int startY):
Actor(world, IID_FLAG, startX, startY, 0, 1, 1) {}

void Flag::doSomething() {
    if(isDead())
        return;
    if(getWorld()->overlapPeach(this)) {
        getWorld()->increaseScore(1000);
        setDead();
        getWorld()->finishLev();
    }
}
