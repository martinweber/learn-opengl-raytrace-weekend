
#include <fstream>
#include <g3log/g3log.hpp>
#include "TinyShader.h"


TinyShader::TinyShader(const std::string &name)
: m_name(name)
{
}

TinyShader::~TinyShader()
{
	if (m_program != 0) 
		glDeleteProgram(m_program);
}


void TinyShader::create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	m_vertexShaderSource = vertexShaderSource;
	m_fragmentShaderSource = fragmentShaderSource;

	compile();
}

void TinyShader::setVertexSource(const std::string& source)
{
	m_vertexShaderSource = source;
}

void TinyShader::setFragmentSource(const std::string& source)
{
	m_fragmentShaderSource = source;
}

void TinyShader::setComputeSource(const std::string& source)
{
	m_computeShaderSource = source;
}


void TinyShader::create()
{
	if (m_vertexShaderSource.empty() && m_fragmentShaderSource.empty() && m_computeShaderSource.empty())
		return;
	
	compile();
}

GLuint TinyShader::compileShader(const GLuint target, const std::string& source) const
{
	GLuint shader = glCreateShader(target);
	const GLchar* sourceStr = source.c_str();
	glShaderSource(shader, 1, &sourceStr, nullptr);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		GLchar log[1024];
		glGetShaderInfoLog(shader, 1024, nullptr, log);
		LOG(WARNING) << "ERROR: Shader Compilation: " << m_name << std::endl << log << std::endl;
	}

	return shader;
}

void TinyShader::compile()
{
	GLuint vertex = 0;
	GLuint fragment = 0;
	GLuint compute = 0;

	if (!m_vertexShaderSource.empty())
		vertex = compileShader(GL_VERTEX_SHADER, m_vertexShaderSource);

	if (!m_fragmentShaderSource.empty())
		fragment = compileShader(GL_FRAGMENT_SHADER, m_fragmentShaderSource);

	if (!m_computeShaderSource.empty())
		compute = compileShader(GL_COMPUTE_SHADER, m_computeShaderSource);

	//
	// Shader Program
	//
	m_program = glCreateProgram();

	if (vertex)
		glAttachShader(m_program, vertex);

	if (fragment)
		glAttachShader(m_program, fragment);

	if (compute)
		glAttachShader(m_program, compute);

	glLinkProgram(m_program);

	GLint result;
	glGetProgramiv(m_program, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		GLchar log[1024];
		glGetProgramInfoLog(m_program, 1024, nullptr, log);
		LOG(WARNING) << "ERROR: Shader Program Linking: " << m_name << std::endl << log << std::endl;
	}

	// 
	// delete shaders, they are linked to program now
	//
	if (vertex)
		glDeleteShader(vertex);

	if (fragment)
		glDeleteShader(fragment);

	if (compute)
		glDeleteShader(compute);
}



/*!
	@param type is the shader type. Valid parameters are: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
*/

void TinyShader::loadSource(GLuint type, const std::string& filename)
{
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try
	{
		shaderFile.open(filename);
		std::stringstream shaderSource;
		shaderSource << shaderFile.rdbuf();
		shaderFile.close();

		switch(type)
		{
		case GL_VERTEX_SHADER: 
			m_vertexShaderSource = shaderSource.str();
			break;
		case GL_FRAGMENT_SHADER:
			m_fragmentShaderSource = shaderSource.str();
			break;
		case GL_COMPUTE_SHADER:
			m_computeShaderSource = shaderSource.str();
			break;
		default:
			LOG(WARNING) << "Unknown shader type for loaded source!";
		}
	}
	catch (std::ifstream::failure e)
	{
		LOG(WARNING) << "ERROR: Could not load Shader from File: " << filename << std::endl;
	}
}


GLint TinyShader::getUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(m_program, name.c_str());
}


GLint TinyShader::getAttributeLocation(const std::string& name) const
{
	return glGetAttribLocation(m_program, name.c_str());
}

void TinyShader::setUniform(const std::string& name, const float value) const
{
	glUniform1f(getUniformLocation(name), value);
}

void TinyShader::setUniform(const std::string& name, const int value) const
{
	glUniform1i(getUniformLocation(name), value);
}


void TinyShader::use()
{
	if (m_program != 0 ) glUseProgram(m_program);
}

GLuint TinyShader::getProgramId() const
{
	return m_program;
}

