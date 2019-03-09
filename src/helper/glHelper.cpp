

#include <sstream>
#include <iomanip>

#include <g3log/g3log.hpp>

#include "glHelper.h"

// static members
//
GLenum GlHelper::m_lastError = GL_NO_ERROR;


GLenum GlHelper::getError()
{
	m_lastError = glGetError();
	return m_lastError;
}


bool GlHelper::hasError()
{
	return(getError() != GL_NO_ERROR);
}


bool GlHelper::hadError()
{
	return(m_lastError != GL_NO_ERROR);
}


//! @returns the last error that was queried with getError()
//!
GLenum GlHelper::lastError()
{
	return m_lastError;
}



std::string GlHelper::createMessage(const std::string& message)
{
	if (m_lastError != GL_NO_ERROR)
	{
		std::string errorStr;
		switch (m_lastError)
		{
		case GL_INVALID_ENUM: errorStr = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE: errorStr = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: errorStr = "GL_INVALID_OPERATION"; break;
		case GL_INVALID_INDEX: errorStr = "GL_INVALID_INDEX"; break;
		case GL_STACK_OVERFLOW: errorStr = "GL_STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: errorStr = "GL_STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: errorStr = "GL_OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
		case GL_CONTEXT_LOST: errorStr = "GL_CONTEXT_LOST"; break;
		case GL_TABLE_TOO_LARGE: errorStr = "GL_TABLE_TOO_LARGE"; break;
		default:
			errorStr = (char*)glewGetErrorString(m_lastError);
			break;
		}
		std::stringstream out;
		out << "GL Error: 0x" << std::setbase(16) << m_lastError << ": " << errorStr << " - " << message;
		return out.str();

	}
	return "";
}


void GlHelper::logError(const std::string& message)
{
	LOG_IF(WARNING, (getError() != GL_NO_ERROR)) << createMessage(message);
}
