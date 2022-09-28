#include "cell.h"

Cell::Cell()
	: row(0)
	, col(0)
	, type(CellType::SPACE)
	, parent(nullptr)
	, currentBFS(BFS::NO_BFS)
{
}

int Cell::getRow()
{
	return row;
}

void Cell::setRow(int row) {
	this->row = row;
}

int Cell::getCol()
{
	return col;
}

void Cell::setCol(int col) {
	this->col = col;
}

CellType Cell::getType()
{
	return type;
}

void Cell::setType(CellType type)
{
	this->type = type;
}

Cell* Cell::getParent()
{
	return parent;
}

void Cell::setParent(Cell* parent) {
	this->parent = parent;
}

BFS Cell::getBFS()
{
	return currentBFS;
}

void Cell::setBFS(BFS currentBFS)
{
	this->currentBFS = currentBFS;
}


