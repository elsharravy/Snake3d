#include "camera.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos) : view( glm::mat4(1.0f)), yaw(-90.0f), pitch(0.0f), pitchSensitivity(0.1f), yawSensitivity(0.1f), angleSensitivity(0.10f), angleYSensitivity(0.05f), speed(0.1f), radius( 20.0f ), angle( 90.0f ), angleY(45.0f)
{
	position = pos;

	pointWeAreLookingAt = glm::vec3(5, 5, 5);
	center = glm::vec3(5, 5, 5);

	up = glm::vec3(0.0f, 1.0f, 0.0f);

	calculatePosition();
//	direction = glm::vec3(0.0f , -1.0f , - 0.7f );
	direction = glm::normalize(center - position);

//	pitch = glm::degrees(glm::asin(direction.y));

	view = glm::lookAt(position, center, up);
}

Camera::~Camera()
{

}

void Camera::calculatePosition()
{
	float radians = glm::radians(angle);
	float radiansY = glm::radians(angleY);
//	position.y = 5;
	position.y = center.y + radius * glm::sin(radiansY);
	position.x = center.x + radius * glm::cos(radians) * glm::cos(radiansY);
	position.z = center.z + radius * glm::sin(radians) * glm::cos(radiansY);

	view = glm::lookAt(position, center, up);
}

//void Camera::updateDirection()
//{
//	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//	direction.y = sin(glm::radians(pitch));
//	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//	direction = glm::normalize(direction);
//	
//	view = glm::lookAt(position, position + direction, up);
//}


glm::vec3 Camera::getPosition()
{
	return position;
}

glm::mat4 Camera::getView()
{
	return view;
}
/*
void Camera::move(glm::vec3 moveVector)
{
	position += moveVector;
	direction = glm::normalize(pointWeAreLookingAt - position);

	view = glm::lookAt(position, pointWeAreLookingAt, up);
}

void Camera::moveForward()
{
	move(direction * speed);
}

void Camera::moveBackward()
{
	move(-direction * speed);
}
*/
//void Camera::updatePitchAndYaw(float pitchChange, float yawChange)
//{
//	pitch += (pitchChange * pitchSensitivity);
//	yaw += (yawChange * yawSensitivity);
//
//	if(pitch > 89.0f)
//		pitch = 89.0f;
//	if (pitch < -89.0f)
//		pitch = -89.0f;
//
//	updateDirection();
//}

void Camera::setAngle(float angle)
{
	this->angle = angle;

	calculatePosition();
}

void Camera::addAngle(float angle)
{
	this->angle += (angleSensitivity * angle) ;

//	std::cout << angle << std::endl;

	calculatePosition();
}

void Camera::addAngleY(float angleY)
{
	this->angleY += (angleYSensitivity * angleY);

	if (this->angleY > 89)
	{
		this->angleY = 89;
	}
	else if (this->angleY < -89)
	{
		this->angleY = -89;
	}

//	std::cout << this->angleY << std::endl;

	calculatePosition();
}

void Camera::addRadius(float radius)
{
	this->radius += radius;

	calculatePosition();
}

float Camera::getAngle()
{
	return angle;
}

float Camera::getAngleY()
{
	return angleY;
}