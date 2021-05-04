#include "Camera.h"

Camera::Camera(std::string& proj_type)
	: view_mat(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -20.0f)))
{
	std::string ortho("ortho");
	if (proj_type.compare(ortho) == 0) {
		projection_mat = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	}
	else {
		projection_mat = glm::perspective(45.0, 512.0 / 512.0, 0.1, 100.0);
	}
}

/*
@param cols:		witdth of the image being received into the viewport
@param rows:		height of the image being received into the viewport
@param fx:			x-focal length of the camera
@param fy:			y-focal length of the camera
@param cx:			x center or projection 
@param cy:			y center or projection
@param negative_z:	whether the camera's forward vector is a negative z-axis or not
*/

Camera::Camera(float cols, float rows, float fx, float fy, float cx, float cy, bool negative_z)
	: view_mat(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)))
{
	glm::mat4 K;
	glm::mat4 NDC;
	if (negative_z) 
	{
		//looking down negative z
		float near = 0.1f;
		float far = 2000.0f;
		float A = (near + far);
		float B = near * far;
		
		K[0] = glm::vec4(fx, 0.0f, 0.0f, 0.0f);
		K[1] = glm::vec4(0.0f, fy, 0.0f, 0.0f);
		K[2] = glm::vec4(-cx, -cy, A, -1.0f);
		K[3] = glm::vec4(0.0f, 0.0f, B, 0.0f);

		float right = cols;
		float left = 0.0f;
		float top = rows;
		float bottom = 0.0f;
		float tx = -((right + left) / (right - left));
		float ty = -((top + bottom) / (top - bottom));
		float tz = -(far + near) / (far - near);

		// http://ksimek.github.io/2013/06/03/calibrated_cameras_in_opengl/
		//the comparison of glortho to actual matrix is very useful
		//NDC = glm::ortho(left, right,bottom, top, near, far);
		NDC[0] = glm::vec4(2.0f / (right - left), 0.0f, 0.0f, 0.0f);
		NDC[1] = glm::vec4(0.0f, 2.0f / (top - bottom), 0.0f, 0.0f);
		NDC[2] = glm::vec4(0.0f, 0.0f, -2.0f / (far - near), 0.0f);
		NDC[3] = glm::vec4(tx, ty, tz, 1.0f);
	}
	else 
	{
		//looking down negative z
		float near = 0.1f;
		float far = 1000.0f;
		float A = -(near + far);
		float B = near * far;
		K[0] = glm::vec4(fx, 0.0f, 0.0f, 0.0f);
		K[1] = glm::vec4(0.0f, -fy, 0.0f, 0.0f);
		K[2] = glm::vec4(-(cols-cx), -(rows-cy), A, 1.0f);
		K[3] = glm::vec4(0.0f, 0.0f, B, 0.0f);

		float right = cols;
		float left = 0.0f;
		float top = rows;
		float bottom = 0.0f;
		float tx = ((right + left) / (right - left));
		float ty = ((top + bottom) / (top - bottom));
		float tz = -(far + near) / (far - near);

		// http://ksimek.github.io/2013/06/03/calibrated_cameras_in_opengl/
		//the comparison of glortho to actual matrix is very useful
		//NDC = glm::ortho(left, right,bottom, top, near, far);
		NDC[0] = glm::vec4(2.0f / right, 0.0f, 0.0f, 0.0f);
		NDC[1] = glm::vec4(0.0f, 2.0f / top, 0.0f, 0.0f);
		NDC[2] = glm::vec4(0.0f, 0.0f, -2.0f / (far - near), 0.0f);
		NDC[3] = glm::vec4(tx, ty, tz, 1.0f);
	}
	projection_mat = NDC * K;
}

Camera::~Camera()
{
}

void Camera::setView(glm::quat orientation, glm::vec3 translation)
{
	glm::mat4 rotation = glm::toMat4(orientation);
	glm::mat4 inv_rotation = glm::transpose(rotation);
	glm::mat4 translated_mat = glm::translate(glm::mat4(1.0), translation);
	glm::mat4 inv_translated_mat = glm::translate(glm::mat4(1.0), -glm::transpose(glm::toMat3(orientation)) *  translation);
	view_mat = translated_mat * rotation * glm::mat4(1.0);
	inv_view_mat = inv_translated_mat * inv_rotation * glm::mat4(1.0);
}

void Camera::setView(glm::mat3 orientation, glm::vec3 translation)
{
	glm::mat4 rotation = glm::mat4(orientation);
	glm::mat4 inv_rotation = glm::transpose(rotation);
	glm::mat4 translated_mat = glm::translate(glm::mat4(1.0), translation);
	glm::mat4 inv_translated_mat = glm::translate(glm::mat4(1.0), - glm::transpose(orientation) * translation);
	view_mat = translated_mat * rotation * glm::mat4(1.0);
	inv_view_mat = inv_translated_mat * inv_rotation * glm::mat4(1.0);
}

glm::mat3 Camera::getRotation()
{
	return glm::mat3(view_mat);
}

glm::vec3 Camera::getTranslation()
{
	return glm::vec3(view_mat[3][0], view_mat[3][1], view_mat[3][2]);
}


