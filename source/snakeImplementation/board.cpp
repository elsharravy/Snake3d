#include "board.h"


Board::Board() : boardValues(BOARD_FIELDS , 0), boardPositionInWorld(0,0,0)
{

}

Field Board::getFieldState(int x, int y, int z) const
{
	return static_cast<Field>(boardValues.at(y * FLOOR_FIELDS + z * BOARD_SIDE_SIZEX + x));
}

Field Board::getFieldState(glm::vec3 pos) const
{
	return static_cast<Field>(boardValues.at(pos.y * FLOOR_FIELDS + pos.z * BOARD_SIDE_SIZEX + pos.x));
}

glm::vec3 Board::calculateOffset(int x, int y, int z) const
{
	return glm::vec3( x * FIELD_SIZE_X , y * FIELD_SIZE_Y , z * FIELD_SIZE_Z);
}

void Board::setFieldState(int x, int y, int z, Field state)
{
	boardValues.at(y * FLOOR_FIELDS + z * BOARD_SIDE_SIZEX + x) = state;
}

void Board::setFieldState(glm::ivec3 pos, Field state)
{
	boardValues.at(pos.y * FLOOR_FIELDS + pos.z * BOARD_SIDE_SIZEX + pos.x) = state;
}

void Board::setBoardInWorldPosition(glm::vec3 boardPositionInWorld)
{
	this->boardPositionInWorld = boardPositionInWorld;
}

glm::vec3 Board::getBoardPositionInWorld()
{
	return boardPositionInWorld;
}