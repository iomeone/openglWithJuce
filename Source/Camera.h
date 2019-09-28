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
class Camera2D
{
public:
	Camera2D() {};
	~Camera2D() {};
	
	glm::mat4 getViewMat()
	{
		auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		return view;
	}


private:
	

	glm::vec3 _cameraPos{ 0.f, 0.f, 3.f };
	glm::vec3 _targetPos{ 0.f, 0.f, 0.f };
	glm::vec3 _upDirection{ 0.f, 1.f, 0.f };
};