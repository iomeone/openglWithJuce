/*
  ==============================================================================

	Camera2D.h
	Created: 23 Sep 2019 4:36:59pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



//Camera class for 2D games
class Camera
{
public:
	Camera() {};
	~Camera() {};
	void init()
	{
		_cameraPos = { 0.0f, 0.0f, 3.0f };
	}

	glm::mat4 getViewMat_()
	{
		auto view = glm::lookAt(_cameraPos,
			_targetPos,
			_upDirection);

		return view;
	}


	glm::mat4 getViewMat()
	{
		auto view = glm::lookAt(_cameraPos,
			_cameraPos + _cameraFront,
			_upDirection);

		return view;
	}

	void setCameraPos(float x, float y, float z)
	{
		_cameraPos.x = x;
		_cameraPos.y = y;
		_cameraPos.z = z;
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

	glm::vec3 getCameraPos()
	{
		return _cameraPos;
	}

	glm::vec3 getFrontPos()
	{
		return _cameraFront;
	}

	glm::vec3 getUpPos()
	{
		return _upDirection;
	}
private:

	glm::vec3 _cameraPos{0.0f, 0.0f, 3.0f};
	glm::vec3 _upDirection{ 0.f, 1.f, 0.f };
	glm::vec3 _cameraFront{ 0.0f, 0.0f, -1.0f };

	glm::vec3 _targetPos{ 0.f, 0.f, 0.f };
};

//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);