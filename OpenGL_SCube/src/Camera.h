#pragma once

#include <string>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

class Camera {
public:
	glm::mat4 projection_mat; 
	glm::mat4 view_mat;
	glm::mat4 inv_view_mat; 
	Camera(std::string& proj_type);
	Camera(float rows, float cols, float fx, float fy, float cx, float cy, bool negative_z=true);
	~Camera();
	void setView(glm::quat orientation, glm::vec3 translation);
	void setView(glm::mat3 orientation, glm::vec3 translation);
	glm::mat3 getRotation();
	glm::vec3 getTranslation();


	
};