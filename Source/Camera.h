/*
  ==============================================================================

	Camera2D.h
	Created: 23 Sep 2019 4:36:59pm
	Author:  user

  ==============================================================================
*/

#pragma once

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

//Camera class for 2D games
class Camera
{
public:
	Camera() {
		updateCameraVectors();
	};
	~Camera() {};


	glm::mat4 getViewMat_()  // for circle demo
	{
		auto view = glm::lookAt(_cameraPos,
			_targetPos,
			_Up);

		return view;
	}


	glm::mat4 getViewMat()
	{
		auto view = glm::lookAt(_cameraPos,
			_cameraPos + _Front,
			_Up);

		return view;
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = _MovementSpeed * deltaTime;
		if (direction == FORWARD)
			_cameraPos += _Front * velocity;
		if (direction == BACKWARD)
			_cameraPos -= _Front * velocity;
		if (direction == LEFT)
			_cameraPos -= _Right * velocity;
		if (direction == RIGHT)
			_cameraPos += _Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= _MouseSensitivity;
		yoffset *= _MouseSensitivity;

		_Yaw += xoffset;
		_Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (_Pitch > 89.0f)
				_Pitch = 89.0f;
			if (_Pitch < -89.0f)
				_Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset)
	{
		if (_Zoom >= 1.0f && _Zoom <= 160.0f)
			_Zoom -= yoffset;
		if (_Zoom <= 1.0f)
			_Zoom = 1.0f;
		if (_Zoom >= 160.0f)
			_Zoom = 160.0f;
	}


	void setCameraPos(glm::vec3 c)
	{
		_cameraPos = c;
	}

	void setCameraPosX(float x)
	{
		_cameraPos.x = x;
	}

	void setCameraPosZ(float z)
	{
		_cameraPos.z = z;
	}

	float getZoom()
	{
		return _Zoom;
	}
	
	glm::vec3& getCameraPos()
	{
		return _cameraPos;
	}

	glm::vec3& getCameraFront()
	{
		return _Front;
	}
private:

 
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new _Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
		front.y = sin(glm::radians(_Pitch));
		front.z = sin(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
		_Front = glm::normalize(front);
		// Also re-calculate the _Right and Up vector
		_Right = glm::normalize(glm::cross(_Front, _WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		_Up = glm::normalize(glm::cross(_Right, _Front));
	}


	glm::vec3 _cameraPos{0.0f, 0.0f, 3.0f};
	glm::vec3 _Up{ 0.f, 1.f, 0.f };
	glm::vec3 _Front{ 0.0f, 0.0f, -1.0f };
	glm::vec3 _Right;
	glm::vec3 _WorldUp{ _Up };

	float _MovementSpeed{ SPEED };
	float _MouseSensitivity{ SENSITIVITY };
	float _Zoom{ ZOOM };

	// Euler Angles
	float _Yaw{ YAW };
	float _Pitch{ PITCH };

	glm::vec3 _targetPos{ 0.f, 0.f, 0.f };
};

//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);