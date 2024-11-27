#include <GL/freeglut.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

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

enum GameState {
    MENU,
    PLAYING1,
    PLAYING2
};

GameState currentState = MENU; // init

Light light(BOUNDARY_X, BOUNDARY_Y, BOUNDARY_X / 2, GL_LIGHT0);

Map map;
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
    if (currentState != MENU) {
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

        map = map1;
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

        map = map2;
    }
}

void revival() {

    sTime = eTime;
    pacman.setIndexPosition(14, 23);
    pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    pacman.setCurrentDirection(Sphere::DIRECTION::NONE);
    pacman.setCenter(10.0f, -160.0f, 0.0f);

    blinky.setIndexPosition(1, 1);
    blinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    blinky.setCurrentDirection(Sphere::DIRECTION::NONE);
    blinky.setCenter(-250.0f, 280.0f, 0.0f);
    blinky.setState(Ghost::STATE::CHASE);
    blinky.setpTime(0);
    blinky.setdTime(eTime);

    pinky.setIndexPosition(1, NUM_COL - 2);
    pinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    pinky.setCurrentDirection(Sphere::DIRECTION::NONE);
    pinky.setCenter(-250.0f, -280.0f, 0.0f);
    blinky.setState(Ghost::STATE::CHASE);
    pinky.setpTime(0);
    pinky.setdTime(eTime);

    inky.setIndexPosition(NUM_ROW - 2, 1);
    inky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    inky.setCurrentDirection(Sphere::DIRECTION::NONE);
    inky.setCenter(250.0f, 280.0f, 0.0f);
    inky.setState(Ghost::STATE::CHASE);
    inky.setpTime(0);
    inky.setdTime(eTime);

    clyde.setIndexPosition(NUM_ROW - 2, NUM_COL - 2);
    clyde.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    clyde.setCurrentDirection(Sphere::DIRECTION::NONE);
    clyde.setCenter(250.0f, -280.0f, 0.0f);
    clyde.setState(Ghost::STATE::CHASE);
    clyde.setpTime(0);
    clyde.setdTime(eTime);
}

void updateDirectionOfPacMan() {

    switch (pacman.getNextDirection())
    {
    case Sphere::DIRECTION::NONE:
        break;
    case Sphere::DIRECTION::LEFT:
        if (map.getBlock(pacman.getYIndex() % NUM_COL, (pacman.getXIndex() - 1 + NUM_ROW) % NUM_ROW).isPassable())
            pacman.updateDirection();
        break;
    case Sphere::DIRECTION::RIGHT:
        if (map.getBlock(pacman.getYIndex() % NUM_COL, (pacman.getXIndex() + 1) % NUM_ROW).isPassable())
            pacman.updateDirection();
        break;
    case Sphere::DIRECTION::UP:
        if (map.getBlock((pacman.getYIndex() - 1 + NUM_COL) % NUM_COL, pacman.getXIndex()% NUM_ROW).isPassable())
            pacman.updateDirection();
        break;
    case Sphere::DIRECTION::DOWN:
        if (!map.getBlock((pacman.getYIndex() + 1) % NUM_COL, pacman.getXIndex() % NUM_ROW).isHalfWall() && map.getBlock((pacman.getYIndex() + 1) % NUM_COL, pacman.getXIndex() % NUM_ROW).isPassable())
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
    const Block& lBlock = map.getBlock(lIdx[1], lIdx[0]);
    const Block& tBlock = map.getBlock(tIdx[1], tIdx[0]);
    const Block& rBlock = map.getBlock(rIdx[1], rIdx[0]);
    const Block& bBlock = map.getBlock(bIdx[1], bIdx[0]);
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
        colhandler(pacman, map);
    }

    pacman.move();
}

void updateGhost() {
    /*cout << "updateGhost" << endl;*/
    for (int i = 0; i < 4; i++) {
        bool bNoDir = Ghosts[i]->getCurrentDirection() == Sphere::DIRECTION::NONE;
        /*cout << endl;
        cout << endl;
        cout << "bNoDir : " << bNoDir << endl;*/
        Ghosts[i]->updateCheck();
        bool bIdxPosUpdated = Ghosts[i]->isIndexPositionUpdated();
        /*cout << "bdixposupdate : " << bIdxPosUpdated << endl;*/
        if (bNoDir || bIdxPosUpdated) {
            //cout << "set New Target" << endl;
            //cout << endl;
            //cout << "ith : " << i << endl;
            int targetX = 0;
            int targetY = 0;
            // set target
            if (Ghosts[i]->getState() == Ghost::STATE::EATEN) { //이거도 전부 동일
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
            else if (Ghosts[i]->getState() == Ghost::STATE::FRIGHTENED) { // Frightened 일때는 모두 동일하게 팩맨 위치로 설정해서 팩맨에서 멀어지도록
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
    if (currentState != MENU) {
        float spf = 1000.0f / FPS;
        eTime = glutGet(GLUT_ELAPSED_TIME);

        if (eTime - sTime > spf) {
            /* Implement: update direction and move Pac-Man */
            updatePacMan();
            updateGhost();
            for (auto ghost : Ghosts) {
                if (ghost->getState() != Ghost::STATE::EATEN) {
                    colhandler(pacman, *ghost);
                    if (ghost->getState() == Ghost::STATE::FRIGHTENED)
                        pacman.setCollided(false);
                }
            }

            if (pacman.getCollided()) {
                if (Life) {
                    revival();
                    Life--;
                    pacman.setCollided(false);
                }
                else {
                    gameOverTime = eTime;
                    displayGameOver = true;
                }
            }

            updateState();

            sTime = eTime;
            glutPostRedisplay();
        }
    }
}

void displayCharacters(void* font, string str, float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for (int i = 0; i < str.size(); i++)
        glutBitmapCharacter(font, str[i]);
}

void displayMenu() {
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
    glRasterPos2f(-50, 0); // Position for the text
    std::string title = "PAC-MAN";
    for (char c : title) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(-85, -50);
    std::string startPrompt = "Press ENTER to Start";
    for (char c : startPrompt) {
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
    //gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (currentState == MENU) {
        displayMenu();
    }
    else {
        // 2D draw
        map.draw();

        colhandler(pacman, map.smallCoins);
        if (numSmallCoins > map.smallCoins.coins.size()) {
            numSmallCoins--;
            score += 10;
        }
        colhandler(pacman, map.bigCoins);
        if (numBigCoins > map.bigCoins.coins.size()) {
            numBigCoins--;
            score += 50;

            for (auto ghost : Ghosts) {
                if ((ghost->getState() == Ghost::STATE::CHASE) || (ghost->getState() == Ghost::STATE::SCATTER)) {
                    ghost->saveState();
                    ghost->setState(Ghost::STATE::FRIGHTENED); //Frightened로 바뀔 때, 이전 상태를 저장, state 전환, 흘러간 시간을 pTime에 저장, dTime 초기화
                    ghost->setpTime(eTime - ghost->getdTime());
                    ghost->setdTime(eTime);
                }
            }
        }
        /*
        if (numSmallCoins == 0 && numBigCoins == 0) {
            currentState = static_cast<GameState>(static_cast<int>(currentState) + 1);
            initialize();
            revival();
        }
        */

        if (blinky.getState() == Ghost::STATE::CHASE)
            displayCharacters(GLUT_BITMAP_HELVETICA_18, "key 1: change ghost's state (Chase)", -270, -340);
        else if (blinky.getState() == Ghost::STATE::SCATTER)
            displayCharacters(GLUT_BITMAP_HELVETICA_18, "key 1: change ghost's state (Scatter)", -270, -340);
        else if (blinky.getState() == Ghost::STATE::FRIGHTENED)
            displayCharacters(GLUT_BITMAP_HELVETICA_18, "key 2: change ghost's state (Frightened)", -270, -340);
        else if (blinky.getState() == Ghost::STATE::EATEN)
            displayCharacters(GLUT_BITMAP_HELVETICA_18, "key 2: change ghost's state (Eaten)", -270, -340);

        // 3D draw
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(light.getID());

        light.draw();

        map.drawCoins();
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
        std::string name = "HIGH SCORE";                            // High score print
        glRasterPos2f(-40, 350);
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
            displayCharacters(GLUT_BITMAP_HELVETICA_18, "STAGE CLEAR!", -60, 0);
            if (elapsed > 5000) { // 5 seconds
                displayStageClear = false;
                currentState = static_cast<GameState>(static_cast<int>(currentState) + 1);
                initialize();
                revival();
                // Reset the game or go to next stage if applicable
            }
        }
        else if (displayGameOver) {
            int elapsed = eTime - gameOverTime;
            std::cout << elapsed << std::endl;
            displayCharacters(GLUT_BITMAP_HELVETICA_18, "GAME OVER", -60, 0);
            if (elapsed > 5000) { // 5 seconds
                exit(0); // Game over, exit to menu or close window
            }
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
}

void keyboardDown(unsigned char key, int x, int y) {
    switch (key)
    {
    case 27:
        exit(0);
    case '1':
        for (auto ghost : Ghosts) {
            if ((ghost->getState() == Ghost::STATE::CHASE) || (ghost->getState() == Ghost::STATE::SCATTER)) {
                ghost->saveState();
                ghost->setState(Ghost::STATE::FRIGHTENED); //Frightened로 바뀔 때, 이전 상태를 저장, state 전환, 흘러간 시간을 pTime에 저장, dTime 초기화
                ghost->setpTime(eTime - ghost->getdTime());
                ghost->setdTime(eTime);
            }
        }
    case 13: // ENTER
        if (currentState == MENU) {
            currentState = PLAYING1;
            initialize();
        }
    default:
        break;
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