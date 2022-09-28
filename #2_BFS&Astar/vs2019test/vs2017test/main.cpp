#include "glew.h"
#include "glut.h"
#include <time.h>
#include <iostream>

#include "maze.h"
#include "gbfs.h"
#include "astar.h"
//207331182
using namespace std;


Maze maze;

AlgoGBFS gbfs;
bool shouldRunGBFS;

AlgoAStar astar;
bool shouldRunAStar;

static Tile& getRandomTile() {
    size_t row = rand() % Maze::SIZE;
    size_t col = rand() % Maze::SIZE;

    return maze.tiles[row][col];
}

void init()
{
    glClearColor(0.8, 0.7, 0.5, 0);// color of window background
    glOrtho(-1, 1, -1, 1, 1, -1);

    srand(time(nullptr));

    generateMaze(maze);

    auto& start = getRandomTile();
    start.type = TileType::START;
    auto& target = getRandomTile();
    target.type = TileType::TARGET;

    std::cout << "Start ("
              << start.row << ", " << start.col << ")"
              << std::endl;
    std::cout << "Target ("
              << target.row << ", " << target.col << ")"
              << std::endl;

    gbfsInit(gbfs, maze, start, target);
    shouldRunGBFS = false;

    astarInit(astar, maze, start, target);
    shouldRunAStar = false;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
    drawMaze(maze);
	glutSwapBuffers(); // show all
}

void idle()
{
    if (shouldRunGBFS) {
        if (gbfsIteration(gbfs)) {
            shouldRunGBFS = false;

            if (nullptr == gbfs.foundNode) {
                std::cout << "GBFS algorithm didn't find a node" << std::endl;
            }

            gbfsFinished(gbfs);
        }
    }
    if (shouldRunAStar) {
        if (astarIteration(astar)) {
            shouldRunAStar = false;

            if (nullptr == astar.foundNode) {
                std::cout << "AStar algorithm didn't find a node" << std::endl;
            }

            astarFinished(astar);
        }
    }

	glutPostRedisplay(); // indirect call to display
}

void menu(int choice)
{
	switch (choice)
	{
	case 1:
        shouldRunGBFS = true;
		break;
	case 2:
        shouldRunAStar = true;
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("First Example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("greedyBestFS", 1);
	glutAddMenuEntry("AStar", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();

    return 0;
}