
#include <g3log/g3log.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "helper/TinyTexture.h"
#include "helper/TinyShader.h"
#include "helper/glHelper.h"

#include "RaytraceApp.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int RENDER_WIDTH = 80*16; // 1280
const int RENDER_HEIGHT = 45*16; // 720

RaytraceApp::RaytraceApp()
{
	initialize();
}

int RaytraceApp::run()
{
	TinyTexture tex;
	TinyTexture::Image image;

	image.channels = 3;
	image.bitDepth = 32;
	image.width = RENDER_WIDTH;
	image.height = RENDER_HEIGHT;

	tex.create(image);
	tex.bind();

	// Display Shader
	//
	TinyShader display("display");
	display.loadSource(GL_VERTEX_SHADER, "shader/display_vs.glsl");
	display.loadSource(GL_FRAGMENT_SHADER, "shader/display_fs.glsl");
	display.create();

	display.use();
	glBindFragDataLocation(display.getProgramId(), 0, "color");
	display.setUniform("srcTex", GL_TEXTURE0);
	LOG_IF(WARNING, GlHelper::hasError()) <<  GlHelper::createMessage("Display Shader");

	// Computer Shader
	//
	TinyShader compute("compute");
	compute.loadSource(GL_COMPUTE_SHADER, "shader/compute.glsl");
	compute.create();

	compute.use();
	compute.setUniform("destTex", GL_TEXTURE0);
	compute.setUniform("renderWidth", RENDER_WIDTH);
	compute.setUniform("renderHeight", RENDER_HEIGHT);
	LOG_IF(WARNING, GlHelper::hasError()) << GlHelper::createMessage("Compute Shader");

	// Vertex Array
	//
	GLuint vertArray;
	glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);

	GLuint posBuf;
	glGenBuffers(1, &posBuf);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	float data[] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, data, GL_STREAM_DRAW);
	const GLint posPtr = display.getAttributeLocation("pos");
	glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posPtr);

	LOG_IF(WARNING, GlHelper::hasError()) << GlHelper::createMessage("Array Buffer Data");


	float frame = 0.0f;

	while (!glfwWindowShouldClose(m_window))
	{
		processInput(m_window);

		// clearFrame();

		// compute
		//
		compute.use();
		compute.setUniform("frameTime", frame);
		glDispatchCompute(RENDER_WIDTH/16, RENDER_HEIGHT/16, 1);

		LOG_IF(WARNING, GlHelper::hasError()) << GlHelper::createMessage("Inner Loop");

		// draw
		//
		display.use();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Swap
		//
		glfwPollEvents();
		glfwSwapBuffers(m_window);

		frame += 0.01f;
		if (frame > 100.0f)
			frame = 0.0f;
	}

	glfwTerminate();
	return 0;
}

void RaytraceApp::initialize()
{
	// initialize GLFW
	//
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ray Tracing in a Weekend", nullptr, nullptr);
	if (!m_window)
	{
		LOG(WARNING) << "Could not create GL Window!";
		glfwTerminate();
		return;
	}
	
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	// initialize GLEW
	//
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum error = glGetError();

}

void RaytraceApp::clearFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RaytraceApp::processInput(GLFWwindow* window)
{
	if (!window)
		return;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void RaytraceApp::framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
}
