#include <time.h>
#include <queue>
#include <iostream>

#include "glut.h"
#include "cell.h"


#define GLCLAMPF(x) (static_cast<GLclampf>(x))

using namespace std;

const int MAZE_SIZE = 50; // maze size

Cell mazeCells[MAZE_SIZE][MAZE_SIZE];
bool BFSIsOn = false;

queue<Cell*> graysFirst; // queue of gray cells from type Cell
queue<Cell*> graysSecond;


void initMaze() {
	int i, j;

	for (i = 0; i < MAZE_SIZE; i++) {
		for (j = 0; j < MAZE_SIZE; j++) {
			mazeCells[i][j].setRow(i);
			mazeCells[i][j].setCol(j);
		}
	}

	//set margins set CellType::WALLs
	for (i = 0; i < MAZE_SIZE; i++) {
		mazeCells[0][i].setType(CellType::WALL);
		mazeCells[MAZE_SIZE - 1][i].setType(CellType::WALL);
		mazeCells[i][0].setType(CellType::WALL);
		mazeCells[i][MAZE_SIZE - 1].setType(CellType::WALL);
	}
	for (i = 1; i < MAZE_SIZE - 1; i++) //way init passage built from CellType::WALLs and CellType::SPACEs *build maze*
	{
		for (j = 1; j < MAZE_SIZE - 1; j++) {
			if (i % 2 == 1)  //most CellType::SPACE passage-first row was CellType::WALL NOT put CellType::WALLs side by side
			{
				if (rand() % 100 < 10) //10% CellType::WALLS to get a way to pass
					mazeCells[i][j].setType(CellType::WALL);
				else
					mazeCells[i][j].setType(CellType::SPACE);
			}
			else  // is mostly CellType::WALL
			{
				if (rand() % 100 < 40) //40% CellType::SPACE
					mazeCells[i][j].setType(CellType::SPACE);
				else
					mazeCells[i][j].setType(CellType::WALL);
			}
		}
	}

	int randStartSecondBFS = rand() % MAZE_SIZE;

	Cell& startPointFirstBFS = mazeCells[MAZE_SIZE / 2][MAZE_SIZE / 2];
	startPointFirstBFS.setType(CellType::START);
	graysFirst.push(&startPointFirstBFS); // add first Cell to grays push_back: add to the end first BFS

	Cell& startPointSecondBfs = mazeCells[randStartSecondBFS][randStartSecondBFS];
	startPointSecondBfs.setType(CellType::TARGET);
	graysSecond.push(&startPointSecondBfs); // add first Cell to grays push_back: add to the end second BFS
}

void init()
{
	glClearColor(GLCLAMPF(0.8), GLCLAMPF(0.7), GLCLAMPF(0.5), GLCLAMPF(0));// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1); // limits

	srand(static_cast<unsigned int>(time(0)));
	initMaze(); //random
}

void drawMaze()
{
	int i, j;
	double sizeX, sizeY; // Cell size
	double x, y; // 2 *( j / (MSZ - 1))

	sizeX = 2.0 / MAZE_SIZE; // from -1 to 1 is 2 and maze is size 100 so each Cell is in size 2 / 100 = MSZ
	sizeY = 2.0 / MAZE_SIZE;

	for (i = 0; i < MAZE_SIZE; i++)
		for (j = 0; j < MAZE_SIZE; j++)
		{
			switch (mazeCells[i][j].getType()) {
			case CellType::SPACE: //white
				glColor3d(1, 1, 1); // set white color
				break;
			case CellType::WALL:
				glColor3d(0.7, 0.4, 0); // set dark orange color
				break;
			case CellType::START:
				glColor3d(0, 1, 1); // set cyan blue color
				break;
			case CellType::TARGET:
				glColor3d(1, 0, 0); // set red color
				break;
			case CellType::FRINGE:
				glColor3d(1, 1, 0); // set yellow color
				break;
			case CellType::BLACK:
				glColor3d(0.8, 1, 0.8); // set green color
				break;
			case CellType::PATH:
				glColor3d(0.8, 0.4, 1); // set purple color
				break;

			}
			//draw sqaure maze[i][j]
			x = 2 * (j / (double)MAZE_SIZE) - 1;
			y = 2 * (i / (double)MAZE_SIZE) - 1;

			glBegin(GL_POLYGON); //fill up cells
			glVertex2d(x, y); //each vetex in Cell
			glVertex2d(x, y + sizeY);
			glVertex2d(x + sizeX, y + sizeY);
			glVertex2d(x + sizeX, y);
			glEnd();

		}
}

void display() //what to show all time
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	drawMaze(); //show maze
	glutSwapBuffers(); // show all
}

void setPathOnMaze(Cell* end) {
	while (end->getParent() != nullptr)
	{
		end->setType(CellType::PATH);
		end = end->getParent();
	}
}

void advanceBFSToNextCells(queue<Cell*>& graysNum, BFS currentBFS)  //dynamic array for cells
{
	if (graysNum.empty()) {
		cout << "no more grays....no solution\n";
		BFSIsOn = false;
	}
	else {
		Cell* current = graysNum.front();
		graysNum.pop(); //remove the first elemnt from queue and draw in black
		int currRow = current->getRow();
		int currCol = current->getCol();

		if (current->getType() != CellType::START && current->getType() != CellType::TARGET) {
			current->setType(CellType::BLACK);
			current->setBFS(currentBFS);
		}

		int points[][2] = {
			{currRow + 1, currCol},
			{currRow - 1, currCol},
			{currRow, currCol + 1},
			{currRow, currCol - 1},
		};

		for (int* point : points) {
			int row = point[0];
			int col = point[1];

			BFS otherBFSNum = static_cast<BFS>((static_cast<int>(currentBFS) % 2) + 1);
			if (mazeCells[row][col].getBFS() == otherBFSNum) { //check if intercasting between cells
				BFSIsOn = false;
				setPathOnMaze(current);
				setPathOnMaze(&mazeCells[row][col]);
				break;
			}
			else if (mazeCells[row][col].getType() == CellType::SPACE) {//add to GRAYS and paint it gray
				mazeCells[row][col].setType(CellType::FRINGE);

				Cell& next = mazeCells[row][col];
				next.setParent(current);
				next.setBFS(currentBFS);
				graysNum.push(&next);
			}
		}
	}
}

void idle()
{
	if (BFSIsOn) {
		//on each iteration of BFS change the color of one of the cells 'grey all Cell neighbors and black the Cell itself
		advanceBFSToNextCells(graysFirst, BFS::FIRST_BFS);

		if (BFSIsOn) {
			advanceBFSToNextCells(graysSecond, BFS::SECOND_BFS);
		}
	}

	glutPostRedisplay();
}

void menu(int choice) {
	switch (choice) {
	case 1: //BFS
		BFSIsOn = true;
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); //general init
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //window init
	glutInitWindowSize(600, 600); //size
	glutInitWindowPosition(200, 100); //position
	glutCreateWindow("First Example"); //open window

	glutDisplayFunc(display); //refresh window
	glutIdleFunc(idle); //runs when nothing happen

	// menu
	glutCreateMenu(menu); // show menu of choose
	glutAddMenuEntry("Bidirectional BFS", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON); //right click in mouse when run program will show us the menu and got an option to use

	init();

	glutMainLoop();
	return 0;
}