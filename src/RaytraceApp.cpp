
#include <g3log/g3log.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "helper/TinyTexture.h"
#include "helper/TinyShader.h"
#include "helper/glHelper.h"

#include "RaytraceApp.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;


const char* display_vert = R"_(
#version 450
in vec2 pos;
out vec2 texCoord;

void main()
{
	texCoord = pos * 0.5 + 0.5;
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}
)_";

const char* display_frag = R"_(
#version 450
uniform sampler2D srcTex;
in vec2 texCoord;
out vec4 color;

void main()
{
	float c = texture(srcTex, texCoord).x;
	color = vec4(c, 1.0, 1.0, 1.0);
}
)_";

const char* compute_src = R"_(
#version 450
uniform float roll;
writeonly uniform image2D destTex;
layout (local_size_x = 16, local_size_y = 16) in;

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

void main()
{
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy)-8)/8.0);
	float globalCoef = sin(float(gl_WorkGroupID.x + gl_WorkGroupID.y) * 0.1 + roll) * 0.5;
	imageStore(destTex, storePos, vec4(1.0 - globalCoef*localCoef, 0.0, 0.0, 0.0));
	// imageStore(destTex, storePos, vec4(rand(vec2(globalCoef, localCoef)), 0.0, 0.0, 0.0));
}
)_";


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
	image.width = 512;
	image.height = 512;

	tex.create(image);
	tex.bind();

	// Display Shader
	//
	TinyShader display("display");
	display.setVertexSource(display_vert);
	display.setFragmentSource(display_frag);
	display.create();

	display.use();
	glBindFragDataLocation(display.getProgramId(), 0, "color");
	display.setUniform("srcTex", GL_TEXTURE0);
	GlHelper::logError("Display Shader");

	// Computer Shader
	//
	TinyShader compute("compute");
	compute.setComputeSource(compute_src);
	compute.create();

	compute.use();
	compute.setUniform("destTex", GL_TEXTURE0);
	GlHelper::logError("Compute Shader");

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

	GlHelper::logError("Array Buffer Data");


	float frame = 0.0f;

	while (!glfwWindowShouldClose(m_window))
	{
		processInput(m_window);

		// clearFrame();

		// compute
		//
		compute.use();
		compute.setUniform("roll", frame);
		glDispatchCompute(512/16, 512/16, 1);
		GlHelper::logError();

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
