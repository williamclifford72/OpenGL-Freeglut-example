// OpenGL_Texturedcube.cpp : main project file.
// Barebones 3D: Textured cube rotating
// More work required, last visited 14/02/2021
//#include "stdafx.h"

// Mathematics GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
#include <gtc/type_ptr.hpp>

#include <glew.h>

#include<stdio.h>
#include<stdlib.h>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <fstream>
#include <sstream>

// OpenGL utility toolkit
#include <freeglut.h>

// using vao/vbo/ibo setup
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

// also using own shader and renderer objects
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"

// own texture object using stb library
#include "Texture.h"


// Just used for Sin and Cos (M_PI should be there but I have had to add it)
#include <math.h>
#include <vector>
#define M_PI 3.14159265358979323846




#define FRAMETIMEMS 5

// Camera position and field of view (Arc Ball)
double longitude;
double latitude;
double radius;

// Object Rotation on/off (true=>rotating)
bool rotation_flag;
bool more_cubes; 
bool phong_flag; 


/*
previously had to keep track of rendering id's
these are now kept in the separate object instances
*/


glm::mat4 MVP;


std::unique_ptr<VertexBuffer> VBO;
std::unique_ptr<IndexBuffer> IBO;
std::unique_ptr<VertexArray> VAO; 
std::unique_ptr<Shader> cube_shader;
std::unique_ptr<Shader> phong_shader;
std::unique_ptr<Texture> image; 
std::unique_ptr<Camera> cam;
Renderer renderer; 

glm::vec3 lightPos; 



// Function prototypes (allows functions to be placed in any order within program)
// Tells compiler about each function prior to first pass of the compile...
//void make_face(int, int, int, int, int);
//void textured_cube(void);
void loadCube(void);
void camera(void);
void Init(void);
//void LoadTextureBMP(LPCWSTR*);
//GLuint LoadTextureBMP(const char*);
//GLuint LoadShaders(const char*, const char*);

// Define vertices of the cube in (X,Y,Z) space
/*GLfloat cube_vertices[][3] = {	
								{-1.0,	-1.0,	-1.0},
								{1.0,	-1.0,	-1.0},
								{1.0,	1.0,	-1.0},
								{-1.0,	1.0,	-1.0},
								{-1.0,	-1.0,	1.0},
								{1.0,	-1.0,	1.0},
								{1.0,	1.0,	1.0},
								{-1.0,	1.0,	1.0}
};*/


void loadCube()
{
	//creating the data for the buffers entered into the shaders
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  1.0f/6.0f,	1.0f,	0.0f, 0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,			1.0f,	0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,			0.0f,	0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,			0.0f,	0.0f, 0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  1.0f / 6.0f,	0.0f,	0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  1.0f / 6.0f,	1.0f,	0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  2.0f / 6.0f, 1.0f,	0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f / 6.0f, 1.0f,	0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f / 6.0f, 0.0f,	0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f / 6.0f, 0.0f,	0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  2.0f / 6.0f, 0.0f,	0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  2.0f / 6.0f, 1.0f,	0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  3.0f / 6.0f, 1.0f,	-1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  2.0f / 6.0f, 1.0f,	-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  2.0f / 6.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  2.0f / 6.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  3.0f / 6.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  3.0f / 6.0f, 1.0f,	-1.0f, 0.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  4.0f / 6.0f, 1.0f,	1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  3.0f / 6.0f, 1.0f,	1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  3.0f / 6.0f, 0.0f,	1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  3.0f / 6.0f, 0.0f,	1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  4.0f / 6.0f, 0.0f,	1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  4.0f / 6.0f, 1.0f,	1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  5.0f / 6.0f, 1.0f,	0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  4.0f / 6.0f, 1.0f,	0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  4.0f / 6.0f, 0.0f,	0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  4.0f / 6.0f, 0.0f,	0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  5.0f / 6.0f, 0.0f,	0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  5.0f / 6.0f, 1.0f,	0.0f, -1.0f, 0.0f,

	-0.5f,  0.5f, -0.5f,  6.0f / 6.0f, 1.0f,	0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  5.0f / 6.0f, 1.0f,	0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  5.0f / 6.0f, 0.0f,	0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  5.0f / 6.0f, 0.0f,	0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  6.0f / 6.0f, 0.0f,	0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  6.0f / 6.0f, 1.0f,	0.0f, 1.0f, 0.0f
	};

	unsigned int indices[36];
	for (unsigned int i = 0; i < 36; i++)
		indices[i] = i;
	//making the vertex array, buffer, and index buffer
	VAO = std::make_unique<VertexArray>();
	VBO = std::make_unique<VertexBuffer>(vertices, 36 * 8 * sizeof(float));
	//this specificies the layout of the buffer for each vertex
	VertexBufferLayout layout;
	layout.Push<float>(3); //position
	layout.Push<float>(2); //colour
	layout.Push<float>(3); //normal
	VAO->AddBuffer(*VBO, layout);
	IBO = std::make_unique<IndexBuffer>(indices, 36);
}

// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices each with x,y,z 12*3*3=108
//GLfloat g_vertex_buffer_data[108] = { 0 };
// Two UV coordinatesfor each vertex. They were created with Blender.
//GLfloat g_uv_buffer_data[72] = { 0 };


// Camera
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// cube color
glm::vec4 light_cube_color;


/*
void textured_cube()
{
	// Use vertices 0,3,2,1 to define square facet of cube, 0 = picture [0,5]
	uv_count = 0;
	vertex_count = 0;
	make_face(1, 0, 3, 2, 0);
	make_face(2, 3, 7, 6, 1);
	make_face(0, 4, 7, 3, 2);
	make_face(1, 2, 6, 5, 3);
	make_face(4, 5, 6, 7, 4);
	make_face(0, 1, 5, 4, 5);
}
*/
// Draw and texture a single facet of the cube
/*
void make_face(int a, int b, int c, int d, int pic)
{
	// Texture assumned to be in a single texture map (h x 6h)
	float texture_xl = ((float)pic) / 6.0f;
	float texture_xr = ((float)(pic + 1)) / 6.0f;

	// Triangle abc
	g_vertex_buffer_data[vertex_count] = cube_vertices[a][0]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[a][1]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[a][2]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[b][0]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[b][1]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[b][2]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[c][0]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[c][1]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[c][2]; vertex_count++;

	g_uv_buffer_data[uv_count] = texture_xl; uv_count++;
	g_uv_buffer_data[uv_count] = 0.0f; uv_count++;
	g_uv_buffer_data[uv_count] = texture_xr; uv_count++;
	g_uv_buffer_data[uv_count] = 0.0f; uv_count++;
	g_uv_buffer_data[uv_count] = texture_xr; uv_count++;
	g_uv_buffer_data[uv_count] = 1.0f; uv_count++;

	// Triangle acd
	g_vertex_buffer_data[vertex_count] = cube_vertices[a][0]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[a][1]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[a][2]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[c][0]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[c][1]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[c][2]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[d][0]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[d][1]; vertex_count++;
	g_vertex_buffer_data[vertex_count] = cube_vertices[d][2]; vertex_count++;

	g_uv_buffer_data[uv_count] = texture_xl; uv_count++;
	g_uv_buffer_data[uv_count] = 0.0f; uv_count++;
	g_uv_buffer_data[uv_count] = texture_xr; uv_count++;
	g_uv_buffer_data[uv_count] = 1.0f; uv_count++;
	g_uv_buffer_data[uv_count] = texture_xl; uv_count++;
	g_uv_buffer_data[uv_count] = 1.0f; uv_count++;
}
*/

// Code to re-draw the screen invoked by glutDisplayFunc() call back function. 
static void Draw(void)
{
	

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	//glGenBuffers(1, &vertexbuffer);
	//// The following commands will talk about our 'vertexbuffer' buffer
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	//textured_cube();

	//glGenBuffers(1, &vertexbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//glGenBuffers(1, &uvbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // | GLUT_ACCUM

	//glUseProgram(programID);
	//MatrixID = glGetUniformLocation(programID, "MVP");
	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture);
	//glUniform1i(TextureID, 0);

	//// 1rst attribute buffer : vertices
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glVertexAttribPointer(
	//	0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
	//	3,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride
	//	(void*)0            // array buffer offset
	//);

	//// 2nd attribute buffer : UVs
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	//glVertexAttribPointer(
	//	1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
	//	2,                                // size : U+V => 2
	//	GL_FLOAT,                         // type
	//	GL_FALSE,                         // normalized?
	//	0,                                // stride
	//	(void*)0                          // array buffer offset
	//);

	//// Draw the triangles
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	renderer.Clear();



	glm::vec3 cubePositions[] = {
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	image->Bind(); //sets image to position 0 in the texture list
	
	
	if (!phong_flag)
	{
		light_cube_color.w = 0.0;
		cube_shader->Bind();
		cube_shader->SetUniform1i("u_Texture", 0);
		if (more_cubes)
		{
			for (unsigned int i = 0; i < 9; i++)
			{
				// calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 t_model = glm::mat4(1.0f);
				t_model = glm::translate(t_model, cubePositions[i]);
				float angle = 20.0f * i;
				t_model = glm::rotate(t_model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				glm::mat4 mvp = cam->projection_mat * cam->view_mat * t_model;
				cube_shader->SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(*VAO, *IBO, *cube_shader);
			}
		}
		
		cube_shader->SetUniformMat4f("u_MVP", MVP);
		renderer.Draw(*VAO, *IBO, *cube_shader);
	}
	else 
	{
		phong_shader->Bind();
		phong_shader->SetUniform1i("u_Texture", 0);
		phong_shader->SetUniformVec3f("light_pos", lightPos);

		if (more_cubes)
		{
			for (unsigned int i = 0; i < 9; i++)
			{
				// calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 t_model = glm::mat4(1.0f);
				t_model = glm::translate(t_model, cubePositions[i]);
				float angle = 20.0f * i;
				t_model = glm::rotate(t_model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				glm::mat4 mvp = cam->projection_mat * cam->view_mat * t_model;
				phong_shader->SetUniformMat4f("u_MVP", mvp);
				phong_shader->SetUniformMat4f("model", t_model);
				renderer.Draw(*VAO, *IBO, *phong_shader);
			}
		}
		phong_shader->SetUniformMat4f("u_MVP", MVP);
		phong_shader->SetUniformMat4f("model", model);
		renderer.Draw(*VAO, *IBO, *phong_shader);

		// draw the light
		
		light_cube_color.w = 1.0;
		glm::mat4 t_model = glm::mat4(1.0f);
		t_model = glm::translate(t_model,lightPos);
		glm::mat4 mvp = cam->projection_mat * cam->view_mat * t_model;

		cube_shader->Bind();
		cube_shader->SetUniformVec4f("cube_color", light_cube_color);
		cube_shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*VAO, *IBO, *cube_shader);
	}
	

	glutSwapBuffers();
}


// Used to update parameters of system, game play (not to re-draw the scene).
// Called at monitor frame rate or there abouts, typically 60Hz.
// Can be called more frequently in some (non-default) modes (Vsync off).
// Probably good to use system time to drive the physics of the system 
// rather than assuming each visit to update() is separated by 1/60 second.

//static void Update(void)     // Reads this way for Option 1
static void Update(int dummy)  // Reads this way for Option 2 (dummy int required)
{

	// If user has set rotation_flag=true (using spacebar) then start cube tumbling 
	if (rotation_flag)
	{
		model = glm::rotate(model, 0.006f, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	camera();

	glutPostRedisplay();
	glutTimerFunc(FRAMETIMEMS, Update, 0); // Restart timer: Remove for option 1 (freerun using idle)
}


// Creates an Arc-Ball view camera
void camera()
{
	// Horrid work around (I think there is something like gimbal lock at latitude=(+/-)90 degrees)
	// When camera pointing directly down, the 'up' vector has little meaning, it could be NSEW but it is neither up or down
	// Fault visble at latitude=90 or -90 degrees and cube not at starting orientation
	// Comment out the second line if you would like to investigate further
	//float latitude_temp = (float)latitude;
	//if (fabs(fabs(latitude) - 90.0f) < 0.01f) latitude_temp = (float)latitude + 0.01f;
	//// Will look at this again ...

	//// Control the camera/view (Arc Ball), map spherical-coordinates(longitude, latitude, radius) to (X,Y,Z)
	//// Theta and phi are just a radian conversion (with an offset to set a starting position) of longitude and latitude.
	//double theta = (M_PI / 180.0) * (longitude)+(3.0f * M_PI) / 2.0f;
	//double phi = (M_PI / 180.0) * (latitude_temp)+(M_PI / 2.0);

	//// Calculate the spherical co-ordinates of the camera
	//double Cx, Cy, Cz;
	//Cx = radius * cos(theta) * sin(phi);
	//Cy = radius * cos(phi);
	//Cz = radius * sin(theta) * sin(phi);

	//glm::mat4 projection = glm::perspective(45.0f * (float)M_PI / 180.0f, 1.0f, 0.1f, 100.0f);  // FOV, Aspect ratio, Near, Far

	//if ((latitude_temp <= -90.0) || (latitude_temp > 90.0))
	//{
	//	view = glm::lookAt(glm::vec3(Cx, Cy, Cz), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	//}
	//else
	//{
	//	view = glm::lookAt(glm::vec3(Cx, Cy, Cz), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	//}

	glm::mat3 rot = cam->getRotation();
	glm::vec3 trans = glm::vec3(longitude, latitude, -5.0f);
	cam->setView(rot, trans);

	MVP = cam->projection_mat * cam->view_mat * model;
}


// The Keyboard Interface requires two call back functions
// glutKeyboardFunc(Keyboard) works on the standard keys and
// glutSpecialFunc(KeyboardSpecial)works on the cursor and function keys
void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);   // Quit when <escape> pressed

	switch (key)
	{
	case '+':
		radius += 0.25; // Move away from cube
		break;
	case '-':
		radius -= 0.25; // Move nearer to cube
		break;
	case ' ':
		rotation_flag = !rotation_flag; // <space_bar> to turn free rotation on/off
		break;
	case 'm':
		more_cubes = !more_cubes;
		break;
	case 'l':
		phong_flag = !phong_flag;
		break;
	case 'd':
		lightPos.x += 0.5f;
		break;
	case 'w':
		lightPos.z -= 0.5f;
		break;
	case 'a':
		lightPos.x -= 0.5f;
		break;
	case 's':
		lightPos.z += 0.5f;
		break;

	}
}
// Special keys, see Keyboard() above
void KeyboardSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		longitude += 0.5;
		break;
	case GLUT_KEY_RIGHT:
		longitude -= 0.5;
		break;
	case GLUT_KEY_UP:
		latitude += 0.5;
		break;
	case GLUT_KEY_DOWN:
		latitude -= 0.5;
		break;
	
	}
}


// Initalisation
void Init(void)
{
	
	glewExperimental = true;
	GLenum err = glewInit();

	std::string cam_type("proj");
	cam = std::make_unique<Camera>(cam_type);

	lightPos = glm::vec3(5.0f, 5.0f, 5.0f);

	// Set object rotation static (keys R=On, r=Off)
	rotation_flag = false;
	more_cubes = false;
	phong_flag = false;

	light_cube_color = glm::vec4(1.0, 1.0, 1.0, 0.0);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);

	loadCube();

	//programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	cube_shader = std::make_unique<Shader>(".\\src\\Shaders\\Basic.shader");
	phong_shader = std::make_unique<Shader>(".\\src\\Shaders\\Phong.shader");


	image = std::make_unique<Texture>(".\\London.bmp");
	//Texture = LoadTextureBMP(".\\London.bmp");
	//Texture = LoadTextureBMP("C:\\Users\\Alien\\source\\repos\\OpenGL_A1\\Debug\\Boxs.bmp");


	cube_shader->Bind();
	image->Bind();
	cube_shader->SetUniform1i("u_Texture", 0);
	cube_shader->SetUniformMat4f("u_MVP", MVP);
	cube_shader->SetUniformVec4f("cube_color", light_cube_color);
	//TextureID = glGetUniformLocation(programID, "myTextureSampler");

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glFrontFace(GL_CW); 
	//glDisable(GL_BLEND);

	//model = glm::translate(glm::mat4(), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
}

int main(int argc, char* argv[])
{
	glutInitWindowPosition(200, 50);
	glutInitWindowSize(512, 512);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("OpenGL Cube");
	Init();
	glutDisplayFunc(Draw);
	glutTimerFunc(FRAMETIMEMS, Update, 0);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(KeyboardSpecial);
	glutMainLoop();
	return 0;
}


/* Boiler plate functions - compile shader - load texture  - move to a .h file later */


/*
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
GLuint LoadTextureBMP(const char* fname)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char* lpBits;

	// Open the file
	FILE* file;
	fopen_s(&file, fname, "rb");
	if (!file) { printf("Image could not be opened\n"); return 0; }

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	lpBits = new unsigned char[imageSize];
	// Read the actual data from the file into the buffer
	fread(lpBits, 1, imageSize, file);
	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, lpBits);

	delete[]lpBits;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
	glGenerateMipmap(GL_TEXTURE_2D);
	return(textureID);
}
*/
/*
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
*/
