
#include <g3log/g3log.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RaytraceApp.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

RaytraceApp::RaytraceApp()
{
	initialize();
}

int RaytraceApp::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		processInput(m_window);
		drawFrame();
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	glfwTerminate();
	return 0;
}

void RaytraceApp::initialize()
{
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

}

void RaytraceApp::drawFrame()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
