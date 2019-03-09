#pragma once

#ifndef RAYTRACE_APP_H__
#define RAYTRACE_APP_H__

struct GLFWwindow;

class RaytraceApp
{
public:
	RaytraceApp();
	RaytraceApp(const RaytraceApp& other) = delete;
	RaytraceApp(const RaytraceApp&& other) = delete;
	RaytraceApp& operator=(const RaytraceApp& other) = delete;
	RaytraceApp& operator=(const RaytraceApp&& other) = delete;
	~RaytraceApp() = default;

	int run();

private:
	void initialize();
	void clearFrame();
	void processInput(GLFWwindow* window);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	GLFWwindow *m_window = nullptr;
};

#endif // RAYTRACE_APP_H__
