#pragma once

enum class CellType {
	SPACE,
	WALL,
	START,
	TARGET,
	BLACK,
	FRINGE,
	PATH,
};

enum class BFS {
	NO_BFS,
	FIRST_BFS,
	SECOND_BFS
};

class Cell
{

private:
	BFS currentBFS;
	int row, col;
	CellType type;
	Cell* parent;


public:
	Cell(); //constructor

	int getRow();
	void setRow(int row);
	int getCol();
	void setCol(int col);
	CellType getType();
	void setType(CellType type);
	Cell* getParent();
	void setParent(Cell* parent);
	BFS getBFS();
	void setBFS(BFS currentBFS);
};

