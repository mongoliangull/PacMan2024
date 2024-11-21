#include <GL/freeglut.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Map.h"
#include "Sphere.h"
#include "Light.h"
#include "CollisionDetector.h"
#include "CollisionHandler.h"

const int FPS = 60;
int sTime = 0;
int eTime = 0;
int dTime = 0;
int pTime = 0;

Light light(BOUNDARY_X, BOUNDARY_Y, BOUNDARY_X / 2, GL_LIGHT0);

Map map1;
Map map;

PacMan pacman(BLOCK_SIZE / 2.0f, 20, 20, false);
Ghost blinky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::STATE::CHASE, Ghost::STATE::SCATTER);

CollisionHandler colhandler;

int score = 1234;

using namespace std;

void initialize() {
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

    map = map1;
}

void updateDirectionOfPacMan() {

    switch (pacman.getNextDirection())
    {
    case Sphere::DIRECTION::NONE:
        break;
    case Sphere::DIRECTION::LEFT:
        if (pacman.getCurrentDirection() == Sphere::DIRECTION::RIGHT)
            break;
        if (map.getBlock(pacman.getYIndex(), pacman.getXIndex() - 1).isPassable())
            pacman.updateDirection();
        break;
    case Sphere::DIRECTION::RIGHT:
        if (pacman.getCurrentDirection() == Sphere::DIRECTION::LEFT)
            break;
        if (map.getBlock(pacman.getYIndex(), pacman.getXIndex() + 1).isPassable())
            pacman.updateDirection();
        break;
    case Sphere::DIRECTION::UP:
        if (pacman.getCurrentDirection() == Sphere::DIRECTION::DOWN)
            break;
        if (map.getBlock(pacman.getYIndex() - 1, pacman.getXIndex()).isPassable())
            pacman.updateDirection();
        break;
    case Sphere::DIRECTION::DOWN:
        if (pacman.getCurrentDirection() == Sphere::DIRECTION::UP)
            break;
        if (map.getBlock(pacman.getYIndex() + 1, pacman.getXIndex()).isPassable())
            pacman.updateDirection();
        break;
    }
}

void updateDirectionOfGhost(Ghost& ghost, int targetX, int targetY) {
    std::cout << "updateDirectionOfGhost" << std::endl;
    std::cout << "targetX: " << targetX << " targetY: " << targetY << std::endl;
    int idx[2] = { ghost.getXIndex(), ghost.getYIndex() };

    int lIdx[2] = { idx[0] - 1, idx[1] };
    int tIdx[2] = { idx[0] , idx[1] - 1 };
    int rIdx[2] = { idx[0] + 1 , idx[1] };
    int bIdx[2] = { idx[0] , idx[1] + 1 };

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

    arr[0] = ((lIdx[0] - NUM_ROW / 2) * 40 - targetX) * ((lIdx[0] - NUM_ROW / 2) * 40 - targetX) + ((NUM_COL / 2 - lIdx[1]) * 40 - targetY) * ((NUM_COL / 2 - lIdx[1]) * 40 - targetY);
    arr[1] = ((tIdx[0] - NUM_ROW / 2) * 0 - targetX) * ((tIdx[0] - NUM_ROW / 2) * 40 - targetX) + ((NUM_COL / 2 - tIdx[1]) * 40 - targetY) * ((NUM_COL / 2 - tIdx[1]) * 40 - targetY);
    arr[2] = ((rIdx[0] - NUM_ROW / 2) * 40 - targetX) * ((rIdx[0] - NUM_ROW / 2) * 40 - targetX) + ((NUM_COL / 2 - rIdx[1]) * 40 - targetY) * ((NUM_COL / 2 - rIdx[1]) * 40 - targetY);
    arr[3] = ((bIdx[0] - NUM_ROW / 2) * 40 - targetX) * ((bIdx[0] - NUM_ROW / 2) * 40 - targetX) + ((NUM_COL / 2 - bIdx[1]) * 40 - targetY) * ((NUM_COL / 2 - bIdx[1]) * 40 - targetY);

    Sphere::DIRECTION CurrDir = blinky.getCurrentDirection();
    Sphere::DIRECTION OppDir = Sphere::DIRECTION::NONE;

    if (CurrDir == Sphere::DIRECTION::LEFT)
        OppDir = Sphere::DIRECTION::RIGHT;
    else if (CurrDir == Sphere::DIRECTION::RIGHT)
        OppDir = Sphere::DIRECTION::LEFT;
    else if (CurrDir == Sphere::DIRECTION::UP)
        OppDir = Sphere::DIRECTION::DOWN;
    else if (CurrDir == Sphere::DIRECTION::DOWN)
        OppDir = Sphere::DIRECTION::UP;
    cout << "opposite direction" << OppDir << endl;

    Sphere::DIRECTION newDir = Sphere::DIRECTION::NONE;
    int MinDistSquare = -1;
    int MaxDistSquare = -1;
    if ((blinky.getState() == Ghost::STATE::CHASE) || (blinky.getState() == Ghost::STATE::SCATTER) || (blinky.getState() == Ghost::STATE::EATEN)) {
        for (int i = 0; i < 4; i++) {
            cout << blocks[i].isPassable() << endl;
            cout << "ditance square: " << arr[i] << endl;
            if (blocks[i].isPassable() && arrD[i] != OppDir) {
                if (MinDistSquare == -1) {
                    cout << "yes" << endl;
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
    else if (blinky.getState() == Ghost::STATE::FRIGHTENED) {
        for (int i = 0; i < 4; i++) {
            cout << blocks[i].isPassable() << endl;
            cout << "ditance square: " << arr[i] << endl;
            if (blocks[i].isPassable() && arrD[i] != OppDir) {
                if (MaxDistSquare == -1) {
                    cout << "yes" << endl;
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
    cout << "blinky's ndwDir" << newDir << endl;
    blinky.setNextDirection(newDir);
    blinky.updateDirection();
    blinky.updateVelocity();
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
    cout << "updateGhost" << endl;
    bool bNoDir = blinky.getCurrentDirection() == Sphere::DIRECTION::NONE;
    blinky.updateCheck();
    bool bIdxPosUpdated = blinky.isIndexPositionUpdated();
    cout << "bdixposupdate" << bIdxPosUpdated << endl;
    if (bNoDir || bIdxPosUpdated) {
        cout << "set New Target" << endl;
        int targetX = 0;
        int targetY = 0;
        // set target
        if (blinky.getState() == Ghost::STATE::CHASE) {
            targetX = pacman.getCenter()[0];
            targetY = pacman.getCenter()[1];
        }
        else if (blinky.getState() == Ghost::STATE::SCATTER) {
            targetX = -120;
            targetY = 120;
        }
        else if (blinky.getState() == Ghost::STATE::FRIGHTENED) {
            targetX = pacman.getCenter()[0];
            targetY = pacman.getCenter()[1];
        }
        else if (blinky.getState() == Ghost::STATE::EATEN) {
            targetX = 0;
            targetY = 0;
        }

        updateDirectionOfGhost(blinky, targetX, targetY);
    }
    blinky.move();
}

void updateState() {
    if ((blinky.getState() == Ghost::STATE::CHASE) && (eTime - dTime) >= 10000 - pTime) {
        blinky.setState(Ghost::STATE::SCATTER);
        dTime = eTime;
        pTime = 0;
    }
    else if ((blinky.getState() == Ghost::STATE::SCATTER) && (eTime - dTime) >= 10000 - pTime) {
        blinky.setState(Ghost::STATE::CHASE);
        dTime = eTime;
        pTime = 0;
    }
    else if ((blinky.getState() == Ghost::STATE::FRIGHTENED) && (eTime - dTime) >= 5000) {
        blinky.setState(blinky.getprevState());
        dTime = eTime;
    }
    else if ((blinky.getState() == Ghost::STATE::EATEN) && (blinky.getXIndex() == NUM_COL / 2) && (blinky.getYIndex() == NUM_ROW / 2)) {
        blinky.setState(blinky.getprevState());
        dTime = eTime;
    }

}

void idle() {
    float spf = 1000.0f / FPS;
    eTime = glutGet(GLUT_ELAPSED_TIME);

    if (eTime - sTime > spf) {
        /* Implement: update direction and move Pac-Man */
        updatePacMan();
        updateGhost();
        if (blinky.getState() != Ghost::STATE::EATEN)
            colhandler(pacman, blinky);
        updateState();

        sTime = eTime;
        glutPostRedisplay();
    }
}

void displayCharacters(void* font, string str, float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for (int i = 0; i < str.size(); i++)
        glutBitmapCharacter(font, str[i]);
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

	// 2D draw
    map1.draw();

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

    map1.drawCoins();
    pacman.draw();
    blinky.draw();

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

	glutSwapBuffers();
}

void keyboardDown(unsigned char key, int x, int y) {
    switch (key)
    {
    case 27:
        exit(0);
    case '1':
        if ((blinky.getState() == Ghost::STATE::CHASE) || (blinky.getState() == Ghost::STATE::SCATTER)) {
            blinky.saveState();
            blinky.setState(Ghost::STATE::FRIGHTENED); //Frightened로 바뀔 때, 이전 상태를 저장, state 전환, 흘러간 시간을 pTime에 저장, dTime 초기화
            pTime = (eTime - dTime);
            dTime = eTime;
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