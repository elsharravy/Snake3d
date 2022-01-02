#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#include "../shaders/shaderProgram.h"

class Camera
{
	float radius;
	float angle;
	float angleY;

	glm::vec3 center;

	glm::mat4 view;

	glm::vec3 position;
	glm::vec3 direction;	// direction we are looking at
	glm::vec3 up;			// up vector for camera
	
	glm::vec3 pointWeAreLookingAt;

	float pitch;
	float yaw;

	float pitchSensitivity;
	float yawSensitivity;

	float angleSensitivity;
	float angleYSensitivity;

	float speed;

//	void updateDirection();

	void calculatePosition();

public:
	Camera( glm::vec3 pos = glm::vec3(0) );
	~Camera();

	glm::mat4 getView( );
	glm::vec3 getPosition();

//	void move(glm::vec3 moveVector);
	void moveBackward();
	void moveForward();
//	void updatePitchAndYaw(float pitchChange, float yawChange);

	void setAngle(float angle);
	void addAngle(float angle);
	void addAngleY(float angleY);

	void addRadius(float radius);
	float getAngle();
	float getAngleY();
};

#endif
