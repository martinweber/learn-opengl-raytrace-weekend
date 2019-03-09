#pragma once


class TinyTexture
{
public:
	struct Image
	{
		unsigned int bitDepth = 0;
		unsigned int channels = 0;
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned char* data = nullptr;
	};

	~TinyTexture();

	void create(const Image& image);
	void destroy();
//	static void generate2d();
	void bind(const unsigned int samplerSlot = 0) const;
	void unbind() const;
	unsigned int getId() const;

private:
	bool create(const unsigned char* data);

	unsigned int m_textureId = 0;
	unsigned int m_width = 0;
	unsigned int m_height = 0;

};