//!
//! \file
//! \date		2016
//! \authors	Martin Weber
//! \brief		OpenGL GlHelper Class
//!


#pragma once

#ifndef GL_HELPER_H__
#define GL_HELPER_H__

#include <string>
#include <GL/glew.h>

class GlHelper
{
public:
	//! return GL error and output Log in case of error. 
	//! @param message is optional for loggin
	//!
	static GLenum getError();

	//! @returns last GL error that was queried with getError()
	static GLenum lastError();

	//! checks OpenGL for an error, return true OpenGL status is not GL_NO_ERROR
	static bool hasError();

	//! @returns true, if the last error was other than GL_NO_ERROR
	static bool hadError();

	//! creates a message string from the last OpenGL error (call hasError() or getError() before)
	static std::string createMessage(const std::string& message = "");

	//! writes the OpenGL error message to g3log, writes nothing if error is GL_NO_ERROR
	static void logError(const std::string& message = "");

private:
	static GLenum	m_lastError;
};

#endif // GL_HELPER_H__

