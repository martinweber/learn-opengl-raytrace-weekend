#include <cassert>
#include <g3log/g3log.hpp>
#include <GL/glew.h>

#include "glHelper.h"
#include "TinyTexture.h"


TinyTexture::~TinyTexture()
{
	destroy();
}

void TinyTexture::create(const Image& image)
{
	assert(image.channels >= 1 && image.channels <= 4);
	assert(image.bitDepth == 8 || image.bitDepth == 16 || image.bitDepth == 32);

	GLenum type = GL_UNSIGNED_BYTE;
	switch (image.bitDepth)
	{
	case 16: 
		type = GL_UNSIGNED_SHORT; 
		break;
	case 32:
		type = GL_FLOAT;
		break;
	default:
		type = GL_UNSIGNED_BYTE;
	}

	GLenum format = GL_RGB;
	switch (image.channels)
	{
	case 1:
		format = GL_LUMINANCE;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
	default:
		format = GL_RGBA;
		break;
	}


	m_width = image.width;
	m_height = image.height;

	create(image.data);
}

void TinyTexture::destroy()
{
	if (m_textureId > 0) 
		glDeleteTextures(1, &m_textureId);

	m_textureId = 0;
}

bool TinyTexture::create(const unsigned char* data)
{
	glGenTextures(1, &m_textureId);
	if (m_textureId <= 0)
	{
		LOG(WARNING) << "Could not create Texture!";
		return false;
	}

	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, //TODO: currently HARDCODED
		m_width, m_height,
		0, // border
		GL_RGB, //TODO: currently HARDCODED
		GL_FLOAT, //TODO: currently HARDCODED
		data);

	// Because we're also using this tex as an image (in order to write to it),
	// we bind it to an image unit as well
	glBindImageTexture(0, m_textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

	LOG_IF(WARNING, GlHelper::hasError()) << GlHelper::createMessage("Could not create texture!");

	unbind();

	return true;
}

void TinyTexture::bind(const unsigned int samplerSlot) const
{
	if (m_textureId > 0)
	{
		glActiveTexture(GL_TEXTURE0 + samplerSlot);
		LOG_IF(WARNING, (GlHelper::hasError())) << GlHelper::createMessage("Could not activate texture #") << samplerSlot;

		glBindTexture(GL_TEXTURE_2D, m_textureId);
		LOG_IF(WARNING, (GlHelper::hasError())) << GlHelper::createMessage("Could not bind texture with id ") << m_textureId;
	}
	else LOG(WARNING) << "Texture not on GPU.";

}

void TinyTexture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned TinyTexture::getId() const
{
	return m_textureId;
}
