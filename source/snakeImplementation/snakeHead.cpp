#include "snakeHead.h"

#include <iostream>

using namespace std;

SnakeHead::SnakeHead(Board* board, glm::vec3 startingPos, glm::ivec3 startingMovementDirection, glm::ivec3 startingCompensationVector ) : worldPosition(startingPos), board(board), movementDirection(startingMovementDirection), movementDirectionRequested(startingMovementDirection), compensationVector(startingCompensationVector)
{
	snakeVelocity = 2;
//	boardPositionPixels = worldPosition - board->getBoardPositionInWorld();
	calculateMovementDirectionMultipliedWithVelocity();
	boardPositionCoords = calculateBoardPosition( worldPosition );
}


void SnakeHead::calculateMovementDirectionMultipliedWithVelocity()
{
	movementDirectionMultipliedWithVelocity = snakeVelocity * static_cast<glm::vec3>(movementDirection);
}

glm::ivec3 SnakeHead::calculateBoardPosition(glm::vec3 worldPosition)
{
	worldPosition = worldPosition - board->getBoardPositionInWorld();
	
	glm::ivec3 boardCoords;

	boardCoords.x = worldPosition.x / FIELD_SIZE_X;
	boardCoords.y = worldPosition.y / FIELD_SIZE_Y;
	boardCoords.z = worldPosition.z / FIELD_SIZE_Z;

	return boardCoords;
}

Field SnakeHead::move( float deltaTime, SnakeTail& tail)
{
	Field collisionField = Field::EMPTY;

	glm::vec3 movement = deltaTime * movementDirectionMultipliedWithVelocity;

	worldPosition += movement;

	glm::ivec3 newBoardPositionCoords = calculateBoardPosition(worldPosition);

	newBoardPositionCoords += compensationVector;

	if (newBoardPositionCoords != this->boardPositionCoords) // snake entered new field
	{
		tail.setMovementBlocked(false);

		if (movementDirection != movementDirectionRequested) // we must change direction
		{
			worldPosition.x = newBoardPositionCoords.x * FIELD_SIZE_X;
			worldPosition.y = newBoardPositionCoords.y * FIELD_SIZE_Y;
			worldPosition.z = newBoardPositionCoords.z * FIELD_SIZE_Z;

			movementDirection = movementDirectionRequested;
			compensationVector = compensationVectorRequested;
			calculateMovementDirectionMultipliedWithVelocity();
		}

		// we say to snakeTail where it should go
		tail.getQueue().push(movementDirection);

		// check if we are colliding with anything
		collisionField = board->getFieldState( newBoardPositionCoords + movementDirection );

		// flag field as traversed by snake
		board->setFieldState(newBoardPositionCoords, Field::SNAKE);


	}	

	this->boardPositionCoords = newBoardPositionCoords;

	//cout << "----------------------\n";
	//cout << "World coords: " << worldPosition.x << " " << worldPosition.y << " " << worldPosition.z << " \n" ;
	//cout << "Board coords: " << boardPositionCoords.x << " " << boardPositionCoords.y << " " << boardPositionCoords.z << " \n" ;
	//cout << deltaTime << "\n";
	//cout << "----------------------" << endl;

	return collisionField;
}

void SnakeHead::requestDirectionChange(glm::ivec3 movementDirection, glm::ivec3 compensationVector)
{
	if ( movementDirection != -this->movementDirection )
	{
		this->movementDirectionRequested = movementDirection;
		this->compensationVectorRequested = compensationVector;
	}
}

void SnakeHead::draw(Cube& cube, ShaderProgram& shader)
{
	cube.setPosition(worldPosition);
	cube.draw(shader);
}

void SnakeHead::setSnakeVelocity(float velocity)
{
	this->snakeVelocity = velocity;
	calculateMovementDirectionMultipliedWithVelocity();
}

//void SnakeHead::setCompensationVector(glm::ivec3 compensationVector)
//{
//	this->compensationVector = compensationVector;
//}