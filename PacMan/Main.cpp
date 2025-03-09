#include <GL/freeglut.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>

#include "Map.h"
#include "Sphere.h"
#include "Light.h"
#include "CollisionDetector.h"
#include "CollisionHandler.h"

const int FPS = 60;
int sTime = 0;
int eTime = 0;
int stageClearTime = 0;
int gameOverTime = 0;
bool displayStageClear = false;
bool displayGameOver = false;
std::string userInput;
bool cursorVisible = true;
int lastTime = 0;

enum GameState {
    MENU,
    PLAYING1,
    PLAYING2,
    GETNAME,
    SCOREBOARD
};

GameState currentState = MENU; // init

Light light(BOUNDARY_X, BOUNDARY_Y, BOUNDARY_X / 2, GL_LIGHT0);

Map map0;
Map map1;
Map map2;

PacMan pacman(BLOCK_SIZE / 2.0f, 20, 20, false);
Ghost blinky(BLOCK_SIZE / 2.0f, 20, 20, -250, 280, Ghost::STATE::CHASE, Ghost::STATE::SCATTER);
Ghost pinky(BLOCK_SIZE / 2.0f, 20, 20, -250, -280, Ghost::STATE::CHASE, Ghost::STATE::SCATTER);
Ghost inky(BLOCK_SIZE / 2.0f, 20, 20, 250, 280, Ghost::STATE::CHASE, Ghost::STATE::SCATTER);
Ghost clyde(BLOCK_SIZE / 2.0f, 20, 20, 250, -280, Ghost::STATE::CHASE, Ghost::STATE::SCATTER);

std::vector<Ghost*> Ghosts;

CollisionHandler colhandler;

int Life = 2;

int numSmallCoins = 0;
int numBigCoins = 0;
int score = 0;

using namespace std;

void initialize() {
    if ((currentState == PLAYING1)|| (currentState == PLAYING2)) {
        // Light
        light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
        light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
        light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

        // PacMan
        Material mtl;	// basic material
        mtl.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
        mtl.setAmbient(0.6f, 0.6f, 0.0f, 1.0f);
        mtl.setDiffuse(0.8f, 0.8f, 0.0f, 1.0f);
        mtl.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
        mtl.setShininess(30.0f);

        pacman.setIndexPosition(14, 23);
        pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
        pacman.setMTL(mtl);

        // Ghost: blinky
        mtl.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
        mtl.setAmbient(0.6f, 0.0f, 0.0f, 1.0f);
        mtl.setDiffuse(0.8f, 0.0f, 0.0f, 1.0f);
        mtl.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
        mtl.setShininess(30.0f);

        blinky.setIndexPosition(1, 1);
        blinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
        blinky.setMTL(mtl);

        // Ghost: pinky
        mtl.setEmission(1.0f, 0.2f, 0.8f, 1.0f);
        mtl.setAmbient(0.5f, 0.1f, 0.4f, 1.0f);
        mtl.setDiffuse(1.0f, 0.2f, 0.8f, 1.0f);
        mtl.setSpecular(1.0f, 0.6f, 0.9f, 1.0f);
        mtl.setShininess(30.0f);

        pinky.setIndexPosition(1, NUM_COL - 2); // NUM_ROW - 2 ,1
        pinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
        pinky.setMTL(mtl);

        // Ghost: inky
        mtl.setEmission(0.0f, 0.2f, 0.2f, 1.0f);
        mtl.setAmbient(0.0f, 0.3f, 0.3f, 1.0f);
        mtl.setDiffuse(0.0f, 0.8f, 0.8f, 1.0f);
        mtl.setSpecular(0.0f, 0.8f, 0.8f, 1.0f);
        mtl.setShininess(90.0f);

        inky.setIndexPosition(NUM_ROW - 2, 1);
        inky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
        inky.setMTL(mtl);

        // Ghost: clyde
        mtl.setEmission(0.0f, 0.1f, 0.0f, 1.0f);
        mtl.setAmbient(0.0f, 0.2f, 0.0f, 1.0f);
        mtl.setDiffuse(0.0f, 0.8f, 0.0f, 1.0f);
        mtl.setSpecular(0.5f, 1.0f, 0.5f, 1.0f);
        mtl.setShininess(80.0f);

        clyde.setIndexPosition(NUM_ROW - 2, NUM_COL - 2);
        clyde.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
        clyde.setMTL(mtl);

        //put ghosts in the ghosts vector
        Ghosts.push_back(&blinky);
        Ghosts.push_back(&pinky);
        Ghosts.push_back(&inky);
        Ghosts.push_back(&clyde);
    }
    if (currentState == PLAYING2) {
        std::array<std::array<tileType, MAP_WIDTH>, MAP_HEIGHT> idxMap1 = { {
            {w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w},
            {w, o, o, o, o, o, o, o, o, o, o, o, o, w, w, o, o, o, o, o, o, o, o, o, o, o, o, w},
            {w, o, w, w, w, w, o, w, w, w, w, w, o, w, w, o, w, w, w, w, w, o, w, w, w, w, o, w},
            {w, O, w, w, w, w, o, w, w, w, w, w, o, w, w, o, w, w, w, w, w, o, w, w, w, w, O, w},
            {w, o, w, w, w, w, o, w, w, w, w, w, o, w, w, o, w, w, w, w, w, o, w, w, w, w, o, w},
            {w, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, w},
            {w, o, w, w, w, w, o, w, w, o, w, w, w, w, w, w, w, w, o, w, w, o, w, w, w, w, o, w},
            {w, o, w, w, w, w, o, w, w, o, w, w, w, w, w, w, w, w, o, w, w, o, w, w, w, w, o, w},
            {w, o, o, o, o, o, o, w, w, o, o, o, o, w, w, o, o, o, o, w, w, o, o, o, o, o, o, w},
            {w, w, w, w, w, w, o, w, w, w, w, w, p, w, w, p, w, w, w, w, w, o, w, w, w, w, w, w},
            {w, w, w, w, w, w, o, w, w, w, w, w, p, w, w, p, w, w, w, w, w, o, w, w, w, w, w, w},
            {w, w, w, w, w, w, o, w, w, p, p, p, p, p, p, p, p, p, p, w, w, o, w, w, w, w, w, w},
            {w, w, w, w, w, w, o, w, w, p, w, w, w, h, h, w, w, w, p, w, w, o, w, w, w, w, w, w},
            {w, w, w, w, w, w, o, w, w, p, w, p, p, p, p, p, p, w, p, w, w, o, w, w, w, w, w, w},
            {p, p, p, p, p, p, o, p, p, p, w, p, p, p, p, p, p, w, p, p, p, o, p, p, p, p, p, p}, // center
            {w, w, w, w, w, w, o, w, w, p, w, p, p, p, p, p, p, w, p, w, w, o, w, w, w, w, w, w},
            {w, w, w, w, w, w, o, w, w, p, w, w, w, w, w, w, w, w, p, w, w, o, w, w, w, w, w, w},
            {w, w, w, w, w, w, o, w, w, p, p, p, p, p, p, p, p, p, p, w, w, o, w, w, w, w, w, w},
            {w, w, w, w, w, w, o, w, w, p, w, w, w, w, w, w, w, w, p, w, w, o, w, w, w, w, w, w},
            {w, w, w, w, w, w, o, w, w, p, w, w, w, w, w, w, w, w, p, w, w, o, w, w, w, w, w, w},
            {w, o, o, o, o, o, o, o, o, o, o, o, o, w, w, o, o, o, o, o, o, o, o, o, o, o, o, w},
            {w, o, w, w, w, w, o, w, w, w, w, w, o, w, w, o, w, w, w, w, w, o, w, w, w, w, o, w},
            {w, o, w, w, w, w, o, w, w, w, w, w, o, w, w, o, w, w, w, w, w, o, w, w, w, w, o, w},
            {w, O, o, o, w, w, o, o, o, o, o, o, o, p, p, o, o, o, o, o, o, o, w, w, o, o, O, w},
            {w, w, w, o, w, w, o, w, w, o, w, w, w, w, w, w, w, w, o, w, w, o, w, w, o, w, w, w},
            {w, w, w, o, w, w, o, w, w, o, w, w, w, w, w, w, w, w, o, w, w, o, w, w, o, w, w, w},
            {w, o, o, o, o, o, o, w, w, o, o, o, o, w, w, o, o, o, o, w, w, o, o, o, o, o, o, w},
            {w, o, w, w, w, w, w, w, w, w, w, w, o, w, w, o, w, w, w, w, w, w, w, w, w, w, o, w},
            {w, o, w, w, w, w, w, w, w, w, w, w, o, w, w, o, w, w, w, w, w, w, w, w, w, w, o, w},
            {w, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, o, w},
            {w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w}
        } };
        map1.createMap(idxMap1);

        numSmallCoins = map1.smallCoins.coins.size();
        numBigCoins = map1.bigCoins.coins.size();

        map0 = map1;
    }
    else if (currentState == PLAYING1) {
        std::array<std::array<tileType, MAP_WIDTH>, MAP_HEIGHT> idxMap2 = { {
            {w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w},
            {w, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, w},
            {w, p, w, w, w, w, p, w, w, w, w, w, p, w, w, p, w, w, w, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, w, w, w, p, w, w, p, w, w, w, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, w, w, w, p, w, w, p, w, w, w, w, w, p, w, w, w, w, p, w},
            {w, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, w},
            {w, p, w, w, w, w, p, w, w, p, w, w, w, w, w, w, w, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, p, w, w, w, w, w, w, w, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, p, p, p, p, p, w, w, p, p, p, p, w, w, p, p, p, p, w, w, p, p, p, p, p, p, w},
            {w, p, w, w, w, w, p, w, w, w, w, w, p, w, w, p, w, w, w, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, w, w, w, p, w, w, p, w, w, w, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, p, w, w, w, h, h, w, w, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, p, w, p, p, p, p, p, p, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, p, p, p, p, p, p, p, p, w, p, p, p, p, p, p, w, p, p, p, p, p, p, p, p, p, w}, // center
            {w, p, w, w, w, w, p, w, w, p, w, p, p, p, p, p, p, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, p, w, w, w, w, w, w, w, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, p, p, p, p, p, p, p, p, p, p, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, p, w, w, w, w, w, w, w, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, p, w, w, w, w, w, w, w, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, p, p, p, p, p, p, p, p, p, p, O, O, O, O, p, p, p, p, p, p, p, p, p, p, p, w},
            {w, p, w, w, w, w, p, w, w, w, w, w, o, w, w, o, w, w, w, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, w, w, w, o, w, w, o, w, w, w, w, w, p, w, w, w, w, p, w},
            {w, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, w},
            {w, p, w, w, w, w, p, w, w, p, w, w, w, w, w, w, w, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, w, w, w, w, p, w, w, p, w, w, w, w, w, w, w, w, p, w, w, p, w, w, w, w, p, w},
            {w, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, w},
            {w, p, w, w, w, w, w, w, w, w, w, w, p, w, w, p, w, w, w, w, w, w, w, w, w, w, p, w},
            {w, p, w, w, w, w, w, w, w, w, w, w, p, w, w, p, w, w, w, w, w, w, w, w, w, w, p, w},
            {w, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, w},
            {w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w}
        } };
        map2.createMap(idxMap2);

        numSmallCoins = map2.smallCoins.coins.size();
        numBigCoins = map2.bigCoins.coins.size();

        map0 = map2;
    }
}

void revival() {

    sTime = eTime;
    pacman.setIndexPosition(14, 23);
    pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    pacman.setCurrentDirection(Sphere::DIRECTION::NONE);
    pacman.setNextDirection(Sphere::DIRECTION::NONE);
    pacman.setCenter(10.0f, -160.0f, 0.0f);

    blinky.setIndexPosition(1, 1);
    blinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    blinky.setCurrentDirection(Sphere::DIRECTION::NONE);
    blinky.setCenter(-250.0f, 280.0f, 0.0f);
    blinky.setState(Ghost::STATE::CHASE);
    blinky.setSaveState(Ghost::STATE::SCATTER);
    blinky.setpTime(0);
    blinky.setdTime(eTime);

    
    pinky.setIndexPosition(1, NUM_COL - 2);
    pinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    pinky.setCurrentDirection(Sphere::DIRECTION::NONE);
    pinky.setCenter(-250.0f, -280.0f, 0.0f);
    pinky.setState(Ghost::STATE::CHASE);
    pinky.setSaveState(Ghost::STATE::SCATTER);
    pinky.setpTime(0);
    pinky.setdTime(eTime);

    inky.setIndexPosition(NUM_ROW - 2, 1);
    inky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    inky.setCurrentDirection(Sphere::DIRECTION::NONE);
    inky.setCenter(250.0f, 280.0f, 0.0f);
    inky.setState(Ghost::STATE::CHASE);
    inky.setSaveState(Ghost::STATE::SCATTER);
    inky.setpTime(0);
    inky.setdTime(eTime);

    clyde.setIndexPosition(NUM_ROW - 2, NUM_COL - 2);
    clyde.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    clyde.setCurrentDirection(Sphere::DIRECTION::NONE);
    clyde.setCenter(250.0f, -280.0f, 0.0f);
    clyde.setState(Ghost::STATE::CHASE);
    clyde.setSaveState(Ghost::STATE::SCATTER);
    clyde.setpTime(0);
    clyde.setdTime(eTime);
}

void updateDirectionOfPacMan() {

    switch (pacman.getNextDirection())
    {
    case Sphere::DIRECTION::NONE:
        break;
    case Sphere::DIRECTION::LEFT:
        if (map0.getBlock(pacman.getYIndex() % NUM_COL, (pacman.getXIndex() - 1 + NUM_ROW) % NUM_ROW).isPassable())
            pacman.updateDirection();
        break;
    case Sphere::DIRECTION::RIGHT:
        if (map0.getBlock(pacman.getYIndex() % NUM_COL, (pacman.getXIndex() + 1) % NUM_ROW).isPassable())
            pacman.updateDirection();
        break;
    case Sphere::DIRECTION::UP:
        if (map0.getBlock((pacman.getYIndex() - 1 + NUM_COL) % NUM_COL, pacman.getXIndex()% NUM_ROW).isPassable())
            pacman.updateDirection();
        break;
    case Sphere::DIRECTION::DOWN:
        if (!map0.getBlock((pacman.getYIndex() + 1) % NUM_COL, pacman.getXIndex() % NUM_ROW).isHalfWall() && map0.getBlock((pacman.getYIndex() + 1) % NUM_COL, pacman.getXIndex() % NUM_ROW).isPassable())
            pacman.updateDirection();
        break;
    }
}

void updateDirectionOfGhost(Ghost& ghost, int targetX, int targetY) {
    /*std::cout << "updateDirectionOfGhost" << std::endl;*/
    //std::cout << "targetX: " << targetX << " targetY: " << targetY << std::endl;
    /*std::cout << "curridxX: " << ghost.getXIndex() << "curridxY: " << ghost.getYIndex() << std::endl;*/
    int idx[2] = { ghost.getXIndex(), ghost.getYIndex() };

    int lIdx[2] = { (idx[0] - 1 + NUM_ROW) % NUM_ROW, idx[1] % NUM_COL };
    int tIdx[2] = { idx[0] % NUM_ROW , (idx[1] - 1 + NUM_COL) % NUM_COL };
    int rIdx[2] = { (idx[0] + 1) % NUM_ROW , idx[1] % NUM_COL};
    int bIdx[2] = { idx[0] % NUM_ROW , (idx[1] + 1) % NUM_COL};
    /*std::cout << "ltrb" << std::endl;*/
    const Block& lBlock = map0.getBlock(lIdx[1], lIdx[0]);
    const Block& tBlock = map0.getBlock(tIdx[1], tIdx[0]);
    const Block& rBlock = map0.getBlock(rIdx[1], rIdx[0]);
    const Block& bBlock = map0.getBlock(bIdx[1], bIdx[0]);
    int arr[4] = { 0,0,0,0 };

    Sphere::DIRECTION arrD[4] = { Sphere::DIRECTION::LEFT, Sphere::DIRECTION::UP, Sphere::DIRECTION::RIGHT, Sphere::DIRECTION::DOWN };

    std::vector<Block> blocks;
    blocks.push_back(lBlock);
    blocks.push_back(tBlock);
    blocks.push_back(rBlock);
    blocks.push_back(bBlock);
    arr[0] = ((lIdx[0] - NUM_ROW / 2) * BLOCK_SIZE - targetX) * ((lIdx[0] - NUM_ROW / 2) * BLOCK_SIZE - targetX) + ((NUM_COL / 2 - lIdx[1]) * BLOCK_SIZE - targetY) * ((NUM_COL / 2 - lIdx[1]) * BLOCK_SIZE - targetY);
    arr[1] = ((tIdx[0] - NUM_ROW / 2) * BLOCK_SIZE - targetX) * ((tIdx[0] - NUM_ROW / 2) * BLOCK_SIZE - targetX) + ((NUM_COL / 2 - tIdx[1]) * BLOCK_SIZE - targetY) * ((NUM_COL / 2 - tIdx[1]) * BLOCK_SIZE - targetY);
    arr[2] = ((rIdx[0] - NUM_ROW / 2) * BLOCK_SIZE - targetX) * ((rIdx[0] - NUM_ROW / 2) * BLOCK_SIZE - targetX) + ((NUM_COL / 2 - rIdx[1]) * BLOCK_SIZE - targetY) * ((NUM_COL / 2 - rIdx[1]) * BLOCK_SIZE - targetY);
    arr[3] = ((bIdx[0] - NUM_ROW / 2) * BLOCK_SIZE - targetX) * ((bIdx[0] - NUM_ROW / 2) * BLOCK_SIZE - targetX) + ((NUM_COL / 2 - bIdx[1]) * BLOCK_SIZE - targetY) * ((NUM_COL / 2 - bIdx[1]) * BLOCK_SIZE - targetY);

    Sphere::DIRECTION CurrDir = ghost.getCurrentDirection();
    Sphere::DIRECTION OppDir = Sphere::DIRECTION::NONE;

    if (CurrDir == Sphere::DIRECTION::LEFT)
        OppDir = Sphere::DIRECTION::RIGHT;
    else if (CurrDir == Sphere::DIRECTION::RIGHT)
        OppDir = Sphere::DIRECTION::LEFT;
    else if (CurrDir == Sphere::DIRECTION::UP)
        OppDir = Sphere::DIRECTION::DOWN;
    else if (CurrDir == Sphere::DIRECTION::DOWN)
        OppDir = Sphere::DIRECTION::UP;
    /*std::cout << "oppdir" << std::endl;*/
    Sphere::DIRECTION newDir = Sphere::DIRECTION::NONE;
    int MinDistSquare = -1;
    int MaxDistSquare = -1;
    if ((ghost.getState() == Ghost::STATE::CHASE) || (ghost.getState() == Ghost::STATE::SCATTER) || (ghost.getState() == Ghost::STATE::EATEN)) {
        for (int i = 0; i < 4; i++) {
            /*cout << blocks[i].isPassable() << endl;*/
            /*cout << "ditance square: " << arr[i] << endl;*/
            if (blocks[i].isPassable() && arrD[i] != OppDir) {
                if (MinDistSquare == -1) {
                    MinDistSquare = arr[i];
                    newDir = arrD[i];
                }
                else {
                    if (arr[i] < MinDistSquare) {
                        MinDistSquare = arr[i];
                        newDir = arrD[i];
                    }
                }
            }
        }
    }
    else if (ghost.getState() == Ghost::STATE::FRIGHTENED) {
        for (int i = 0; i < 4; i++) {
            /*cout << blocks[i].isPassable() << endl;*/
            /*cout << "ditance square: " << arr[i] << endl;*/
            if (blocks[i].isPassable() && arrD[i] != OppDir) {
                if (MaxDistSquare == -1) {
                    MaxDistSquare = arr[i];
                    newDir = arrD[i];
                }
                else {
                    if (arr[i] > MaxDistSquare) {
                        MaxDistSquare = arr[i];
                        newDir = arrD[i];
                    }
                }
            }
        }
    }
    /*cout << "blinky's ndwDir" << newDir << endl;*/
    ghost.setNextDirection(newDir);
    ghost.updateDirection();
    ghost.updateVelocity();
}

void updatePacMan() {
    // update PacMan
    bool bNoDir = pacman.getCurrentDirection() == Sphere::DIRECTION::NONE;

    pacman.updateCheck();

    bool bIdxPosUpdated = pacman.isIndexPositionUpdated();

    if (bNoDir || bIdxPosUpdated) {
        updateDirectionOfPacMan();
        colhandler(pacman, map0);
    }

    pacman.move();
}

void updateGhost() {
    for (int i = 0; i < 4; i++) {
            bool bNoDir = Ghosts[i]->getCurrentDirection() == Sphere::DIRECTION::NONE;
            Ghosts[i]->updateCheck();
            bool bIdxPosUpdated = Ghosts[i]->isIndexPositionUpdated();
            if ((bNoDir || bIdxPosUpdated) && (!displayStageClear)) {
                int targetX = 0;
                int targetY = 0;
                // set target
                if (Ghosts[i]->getState() == Ghost::STATE::EATEN) {
                    targetX = 0;
                    targetY = 20;
                }
                else if (Ghosts[i]->getCenter()[0] >= -60.0f && Ghosts[i]->getCenter()[0] <= 60.0f && Ghosts[i]->getCenter()[1] >= -10.0f && Ghosts[i]->getCenter()[1] <= 70.0f) { // If ghost is in Ghost roo
                    targetX = 0;
                    targetY = 90.0f;
                }
                else if (Ghosts[i]->getCenter()[0] >= -60.0f && Ghosts[i]->getCenter()[0] <= 60.0f && Ghosts[i]->getCenter()[1] >= 70.0f && Ghosts[i]->getCenter()[1] <= 100.0f) {
                    targetX = (Ghosts[i]->getCurrentDirection() == Sphere::DIRECTION::LEFT) ? -70.0f : 70.0f;
                    targetY = 90.0f;
                }
                else if (Ghosts[i]->getState() == Ghost::STATE::CHASE) {
                    if (i == 0) {
                        targetX = pacman.getCenter()[0];
                        targetY = pacman.getCenter()[1];
                    }
                    else if (i == 1) {
                        if (pacman.getCurrentDirection() == Sphere::DIRECTION::LEFT) {
                            targetX = pacman.getCenter()[0] - BLOCK_SIZE * 4;
                            targetY = pacman.getCenter()[1];
                        }
                        else if (pacman.getCurrentDirection() == Sphere::DIRECTION::RIGHT) {
                            targetX = pacman.getCenter()[0] + BLOCK_SIZE * 4;
                            targetY = pacman.getCenter()[1];
                        }
                        else if (pacman.getCurrentDirection() == Sphere::DIRECTION::UP) {
                            targetX = pacman.getCenter()[0];
                            targetY = pacman.getCenter()[1] + BLOCK_SIZE * 4;
                        }
                        else if (pacman.getCurrentDirection() == Sphere::DIRECTION::DOWN) {
                            targetX = pacman.getCenter()[0];
                            targetY = pacman.getCenter()[1] - BLOCK_SIZE * 4;
                        }
                        else {
                            targetX = pacman.getCenter()[0];
                            targetY = pacman.getCenter()[1];
                        }
                    }
                    else if (i == 2) {
                        if (pacman.getCurrentDirection() == Sphere::DIRECTION::LEFT) {
                            targetX = (pacman.getCenter()[0] - BLOCK_SIZE * 2 - blinky.getCenter()[0]) + pacman.getCenter()[0] - BLOCK_SIZE * 2;
                            targetY = (pacman.getCenter()[1] - blinky.getCenter()[1]) + pacman.getCenter()[1];
                        }
                        else if (pacman.getCurrentDirection() == Sphere::DIRECTION::RIGHT) {
                            targetX = (pacman.getCenter()[0] + BLOCK_SIZE * 2 - blinky.getCenter()[0]) + pacman.getCenter()[0] + BLOCK_SIZE * 2;
                            targetY = (pacman.getCenter()[1] - blinky.getCenter()[1]) + pacman.getCenter()[1];
                        }
                        else if (pacman.getCurrentDirection() == Sphere::DIRECTION::UP) {
                            targetX = (pacman.getCenter()[0] - blinky.getCenter()[0]) + pacman.getCenter()[0];
                            targetY = (pacman.getCenter()[1] + BLOCK_SIZE * 2 - -blinky.getCenter()[1]) + pacman.getCenter()[1] + BLOCK_SIZE * 2;
                        }
                        else if (pacman.getCurrentDirection() == Sphere::DIRECTION::DOWN) {
                            targetX = (pacman.getCenter()[0] - blinky.getCenter()[0]) + pacman.getCenter()[0];
                            targetY = (pacman.getCenter()[1] - BLOCK_SIZE * 2 - -blinky.getCenter()[1]) + pacman.getCenter()[1] - BLOCK_SIZE * 2;;
                        }
                        else {
                            targetX = pacman.getCenter()[0];
                            targetY = pacman.getCenter()[1];
                        }
                    }
                    else if (i == 3) {
                        int PCDistanceSquare = (Ghosts[i]->getCenter()[0] - pacman.getCenter()[0]) * (Ghosts[i]->getCenter()[0] - pacman.getCenter()[0]) + (Ghosts[i]->getCenter()[1] - pacman.getCenter()[1]) * (Ghosts[i]->getCenter()[1] - pacman.getCenter()[1]);
                        if (PCDistanceSquare <= (BLOCK_SIZE * 8) * (BLOCK_SIZE * 8)) {
                            targetX = Ghosts[i]->getOriginX();
                            targetY = Ghosts[i]->getOriginY();
                        }
                        else {
                            targetX = pacman.getCenter()[0];
                            targetY = pacman.getCenter()[1];
                        }
                    }
                }
                else if (Ghosts[i]->getState() == Ghost::STATE::SCATTER) {
                    targetX = Ghosts[i]->getOriginX();
                    targetY = Ghosts[i]->getOriginY();
                }
                else if (Ghosts[i]->getState() == Ghost::STATE::FRIGHTENED) { // Frightened �϶��� ��� �����ϰ� �Ѹ� ��ġ�� �����ؼ� �Ѹǿ��� �־�������
                    targetX = pacman.getCenter()[0];
                    targetY = pacman.getCenter()[1];
                }
                updateDirectionOfGhost(*Ghosts[i], targetX, targetY);
            }
            Ghosts[i]->move();
    }
}

void updateState() {
    for (auto ghost : Ghosts) {
        if ((ghost->getState() == Ghost::STATE::CHASE) && (eTime - ghost->getdTime()) >= 10000 - ghost->getpTime()) {
            ghost->setState(Ghost::STATE::SCATTER);
            ghost->setdTime(eTime);
            ghost->setpTime(0);
        }
        else if ((ghost->getState() == Ghost::STATE::SCATTER) && (eTime - ghost->getdTime()) >= 10000 - ghost->getpTime()) {
            ghost->setState(Ghost::STATE::CHASE);
            ghost->setdTime(eTime);
            ghost->setpTime(0);
        }
        else if ((ghost->getState() == Ghost::STATE::FRIGHTENED) && (eTime - ghost->getdTime()) >= 5000) {
            ghost->setState(ghost->getprevState());
            ghost->setdTime(eTime);
        }
        else if ((ghost->getState() == Ghost::STATE::EATEN) && (ghost->getXIndex() == 13) && (ghost->getYIndex() == 13)) {
            ghost->setState(ghost->getprevState());
            ghost->setdTime(eTime);
        }
    }

}

void idle() {
    eTime = glutGet(GLUT_ELAPSED_TIME);
    if ((currentState == PLAYING1)|| (currentState == PLAYING2)) {
        float spf = 1000.0f / FPS;
        

        if (eTime - sTime > spf) {
            if (!(displayGameOver || displayStageClear)) {
                updatePacMan();
                updateGhost();
                for (auto ghost : Ghosts) {
                    if (ghost->getState() != Ghost::STATE::EATEN) {
                        colhandler(pacman, *ghost);
                        if (ghost->getState() == Ghost::STATE::FRIGHTENED)
                            pacman.setCollided(false);
                    }
                }
            }

            if (pacman.getCollided()) {
                if (Life) {
                    revival();
                    Life--;
                    pacman.setCollided(false);
                }
                else {
                    if(gameOverTime == 0)
                        gameOverTime = eTime;
                    displayGameOver = true;
                }
            }

            updateState();

            sTime = eTime;
            glutPostRedisplay();
        }
    }
    if (displayGameOver) {
        int elapsed = eTime - gameOverTime;
        if (elapsed > 5000) {
            currentState = GETNAME;
            displayGameOver = false;
        }
        glutPostRedisplay();

    }
    //if (displayGameOver || displayStageClear) {
    //    for (auto ghost : Ghosts) {
    //        ghost->setVelocity(Vector3f(0.0f, 0.0f, 0.0f));

    //    }
    //}
}

void displayCharacters(void* font, string str, float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for (int i = 0; i < str.size(); i++)
        glutBitmapCharacter(font, str[i]);
}

void displayMenu() {
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
    glRasterPos2f(-43, 0); // Position for the text
    std::string title = "PAC-MAN";
    for (char c : title) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(-85, -50);
    std::string startPrompt = "Press ENTER to Start";
    for (char c : startPrompt) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(-84, -230);
    std::string scoreboardPrompt = "Scoreboard : press s";
    for (char c : scoreboardPrompt) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glutSwapBuffers();
}

void renderText(const std::string& text, float x, float y) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

void timer(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - lastTime >= 500) {
        cursorVisible = !cursorVisible;
        lastTime = currentTime;
        glutPostRedisplay();
    }
    glutTimerFunc(100, timer, 0);
}


void displayNameInput() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
    //gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    std::string displayText = "Player: " + userInput;
    if (cursorVisible) {
        displayText += "|"; 
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(displayText, -40.0f, 0.0f);

    std::ostringstream oss;
    oss << std::setw(6) << std::setfill('0') << score;
    std::string scoreString = oss.str();                        // High score print
    glRasterPos2f(-20.0f, -30.0f);
    for (int i = 0; i < scoreString.size(); i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, scoreString[i]);

    glutSwapBuffers();
}

void displayScoreBoard() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
    //gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    std::string filename = "scoreBoard.txt";

    std::multimap<std::string, std::string, std::greater<string>> scoreBoard;


    std::ifstream inputFile(filename);

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string key, name;

        if (iss >> key >> name) {
            scoreBoard.insert({ key, name });
        }
    }

    inputFile.close();

    float i = 1;
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "SCORE BOARD", -65.0f, +30.0f);
    for (const auto& entry : scoreBoard) {
        std::stringstream ss;
        ss << i;
        std::string str = ss.str();
        std::string displayText = str + " : " + entry.first + " : " + entry.second;
        glColor3f(1.0f, 1.0f, 1.0f); 
        renderText(displayText, -80.0f, 0.0f - i*30.0f);
        i += 1;
        if (i == 6)
            break;
    }

    glRasterPos2f(-108, -230);
    std::string menuPrompt = "Menu : press SPACE BAR";
    for (char c : menuPrompt) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glutSwapBuffers();
}

void display() {

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (currentState == MENU) {
        displayMenu();
    }
    else if((currentState == PLAYING1)||(currentState == PLAYING2)){
        // 2D draw
        map0.draw();

        colhandler(pacman, map0.smallCoins);
        if (numSmallCoins > map0.smallCoins.coins.size()) {
            numSmallCoins--;
            score += 10;
        }
        colhandler(pacman, map0.bigCoins);
        if (numBigCoins > map0.bigCoins.coins.size()) {
            numBigCoins--;
            score += 50;

            for (auto ghost : Ghosts) {
                if ((ghost->getState() == Ghost::STATE::CHASE) || (ghost->getState() == Ghost::STATE::SCATTER)) {
                    ghost->saveState();
                    ghost->setState(Ghost::STATE::FRIGHTENED); //Frightened�� �ٲ� ��, ���� ���¸� ����, state ��ȯ, �귯�� �ð��� pTime�� ����, dTime �ʱ�ȭ
                    ghost->setpTime(eTime - ghost->getdTime());
                    ghost->setdTime(eTime);
                }
            }
        }

        // 3D draw
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(light.getID());

        light.draw();

        map0.drawCoins();
        pacman.draw();
        for (auto ghost : Ghosts) {
            ghost->draw();
        }

        for (int i = 0; i <= Life; i++) {
            GLfloat ambient[4]{ 0.6f, 0.6f, 0.0f, 1.0f };
            GLfloat emission[4]{ 0.2f, 0.2f, 0.2f, 1.0f };
            GLfloat diffuse[4]{ 0.8f, 0.8f, 0.0f, 1.0f };
            GLfloat specular[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
            GLfloat shininess[1]{ 30.0f };
            glPushMatrix();
            glTranslatef(270 - i * 22, -340, 0);
            glShadeModel(GL_SMOOTH);
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
            glutSolidSphere(10, 20, 20);
            glPopMatrix();
        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(light.getID());

        // draw text
        glColor3f(1.0f, 1.0f, 1.0f);
        std::string name = "PAC-MAN";                            // High score print
        glRasterPos2f(-33, 350);
        for (int i = 0; i < name.size(); i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, name[i]);

        std::ostringstream oss;
        oss << std::setw(6) << std::setfill('0') << score;
        std::string scoreString = oss.str();                        // High score print
        glRasterPos2f(-270, 320);
        for (int i = 0; i < scoreString.size(); i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, scoreString[i]);


        // stage clear && game over
        if (displayStageClear) {
            int elapsed = eTime - stageClearTime;
            displayCharacters(GLUT_BITMAP_HELVETICA_18, "STAGE CLEAR!", -65, 23);
            if (currentState == PLAYING2) {
                glRasterPos2f(-52, -3);
                std::string startPrompt = "Click to END";
                for (char c : startPrompt) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
                }
                currentState = static_cast<GameState>(static_cast<int>(currentState) + 1);
            }
            else if (elapsed > 5000) { // 5 seconds
                displayStageClear = false;
                currentState = static_cast<GameState>(static_cast<int>(currentState) + 1);
                initialize();
                revival();
                // Reset the game or go to next stage if applicable
            }
        }
        else if (displayGameOver) {
            displayCharacters(GLUT_BITMAP_HELVETICA_18, "GAME OVER", -55, 23);
        }
        else {
            if (numSmallCoins == 0 && numBigCoins == 0 && !displayStageClear) {
                // Player has cleared the stage
                stageClearTime = eTime; // Set the time to show stage clear message
                displayStageClear = true;
                // You might want to reset the game state or go to the next level here
            }
        }

        glutSwapBuffers();
    }
    else if(currentState ==GETNAME) {
        displayNameInput();
        glutTimerFunc(100, timer, 0);
    }
    else {
        displayScoreBoard();
    }
}

void keyboardDown(unsigned char key, int x, int y) {
    
    if ((currentState == MENU)) {
        switch (key)
        {
        case 27:
            exit(0);
        case 's':
            currentState = SCOREBOARD;
            break;
        case 13: // ENTER
            currentState = PLAYING1;
            pacman.setCollided(false);
            score = 0;

            stageClearTime = 0;
            gameOverTime = 0;
            displayStageClear = false;
            displayGameOver = false;
            userInput ="";
            cursorVisible = true;
            lastTime = 0;
            
            Life = 2;
            initialize();
            revival();
            break;
        default:
            break;
        }
    }
    else if (currentState == GETNAME) {
        if (key == 8) {
            if (!userInput.empty()) {
                userInput.pop_back();
            }
        }
        else if (key == 13) {
            std::cout << "Final Input: " << userInput << std::endl;
            ofstream scoreBoard("scoreBoard.txt", ios::app);
            std::ostringstream oss;
            oss << std::setw(6) << std::setfill('0') << score;
            std::string scoreString = oss.str();
            scoreBoard << scoreString + ' ' + userInput << endl;
            scoreBoard.close();
            userInput.clear();
            currentState = static_cast<GameState>(static_cast<int>(currentState) + 1);
        }
        else if (key >= 32 && key <= 126) {
            userInput += key;
        }
    }
    else {
        if (key == 13) {
            exit(0);
        }
        else if (key == ' ') {

            currentState = MENU;
        }
    }

    glutPostRedisplay();
}

void specialKeyDown(int key, int x, int y) {
    /* Implement: set next directions */
    switch (key)
    {
    case GLUT_KEY_LEFT:
        pacman.setNextDirection(Sphere::DIRECTION::LEFT);
        break;
    case GLUT_KEY_RIGHT:
        pacman.setNextDirection(Sphere::DIRECTION::RIGHT);
        break;
    case GLUT_KEY_UP:
        pacman.setNextDirection(Sphere::DIRECTION::UP);
        break;
    case GLUT_KEY_DOWN:
        pacman.setNextDirection(Sphere::DIRECTION::DOWN);
        break;
    }
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitWindowSize(WINDOW_W, WINDOW_H); //MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE 
	glutCreateWindow("PacMan");

	initialize();

	// register callbacks
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboardDown);
    glutSpecialFunc(specialKeyDown);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}