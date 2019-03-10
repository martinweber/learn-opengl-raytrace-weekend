#pragma once

#include <string>
#include <GL/glew.h>

class TinyShader
{
public:
	TinyShader(const std::string& m_name);
	~TinyShader();
	void create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	void setVertexSource(const std::string &source);
	void setFragmentSource(const std::string &source);
	void setComputeSource(const std::string &source);

	void create();
	void loadSource(GLuint type, const std::string& filename);
	GLint getUniformLocation(const std::string& name) const;
	GLint getAttributeLocation(const std::string& name) const;

	void setUniform(const std::string& name, const float value) const;
	void setUniform(const std::string& name, const int value) const;
	void use();

	GLuint getProgramId() const;

private:
	void compile();
	GLuint compileShader(const GLuint target, const std::string& source) const;

private:
	GLuint m_program = 0;

	std::string m_name = "";

	std::string m_vertexShaderSource = "";
	std::string m_fragmentShaderSource = "";
	std::string m_computeShaderSource = "";
};