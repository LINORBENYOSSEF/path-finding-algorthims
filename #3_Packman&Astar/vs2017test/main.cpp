#include "glew.h"
#include "glut.h"

#include <time.h>
#include <iostream>
#include <chrono>

#include "maze.h"
#include "characters.h"
//207331182
using namespace std;

class Game {
public:
    static constexpr size_t GHOST_COUNT = 2;
    static constexpr size_t UPDATE_INTERVAL_MS = 100;

    Game()
        : maze()
        , packman(maze, *maze.getRandomTileOfType(TileType::SPACE))
        , ghosts()
        , ended(false)
        , lastUpdateMs(0)
    {
        for (int i = 0; i < GHOST_COUNT; ++i) {
            ghosts.emplace_back(maze, packman, *maze.getRandomTileOfType(TileType::SPACE));
        }
    }

    void draw()
    {
        maze.draw();
    }

    void update()
    {
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (lastUpdateMs == 0 || now - lastUpdateMs >= UPDATE_INTERVAL_MS) {
            lastUpdateMs = now;
            doUpdate();
        }
    }

private:
    void doUpdate()
    {
        if (ended) return;

        packman.update();

        for (auto& ghost : ghosts) {
            ghost.update();
            if (ghost.currentPosition() == packman.currentPosition()) {
                ended = true;
                std::cout << "ATE PACKMAN" << std::endl;
                return;
            }
        }
    }

    Maze maze;
    Packman packman;
    std::vector<Ghost> ghosts;
    bool ended;
    time_t lastUpdateMs;
};

Game* game = nullptr;

void init()
{
    glClearColor(0.8, 0.7, 0.5, 0);// color of window background
    glOrtho(-1, 1, -1, 1, 1, -1);

    srand(time(nullptr));

    game = new Game;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
    game->draw();
	glutSwapBuffers(); // show all
}

void idle()
{
    game->update();
	glutPostRedisplay(); // indirect call to display
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

	init();

	glutMainLoop();

    if (nullptr != game) {
        delete game;
    }

    return 0;
}
