#include "Renderer.h"

#include <iostream>

void GLClearError()
{
	while(glGetError() != GL_NO_ERROR); //CLEAR ERRORS
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while(GLenum error = glGetError())
	{
		std::cout << "[OpenGL_Error] (" << error << ")" << function << " " << file << ":" << line  <<std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); //use for index buffer, 6 indices, using unsigned ints, buffer already bound so can use nullptr

}
