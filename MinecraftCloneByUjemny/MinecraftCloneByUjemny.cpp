#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <array>
#include <vector>
#include <random>
#include "chunks.h"
#include "text.h"

#define KEY(window, key) glfwGetKey(window, key)

std::random_device rd;
float grid = 1.0f;

float delta, last, current;
int lastX, lastY;
float xa, ya;
float speed = 10.0f;
glm::vec3 playerPos;

glm::mat4 proj;
glm::mat4 view;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camRight = glm::vec3(0.0f, 0.0f, 0.0f);

void framebuffer(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	proj = glm::perspectiveFov(70.0f, (float)w, (float)h, 0.01f, 1000.0f);
}

void mouse(GLFWwindow* window, double xp, double yp)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		float x = xp - lastX;
		float y = lastY - yp;

		lastX = xp;
		lastY = yp;

		float sens = 0.5f;
		xa += x * sens;
		ya += y * sens;

		if (ya > 89.0f)
			ya = 89.0f;

		if (ya < -89.0f)
			ya = -89.0f;

		camDirection.x = cos(glm::radians(xa)) * cos(glm::radians(ya));
		camDirection.y = sin(glm::radians(ya));
		camDirection.z = sin(glm::radians(xa)) * cos(glm::radians(ya));
		cameraFront = glm::normalize(camDirection);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void input(GLFWwindow* window)
{
	if (KEY(window, GLFW_KEY_W) == GLFW_PRESS)
		playerPos += cameraFront * speed * delta;

	if (KEY(window, GLFW_KEY_S) == GLFW_PRESS)
		playerPos -= cameraFront * speed * delta;

	if (KEY(window, GLFW_KEY_A) == GLFW_PRESS)
		playerPos -= camRight * speed * delta;

	if (KEY(window, GLFW_KEY_D) == GLFW_PRESS)
		playerPos += camRight * speed * delta;

	if (KEY(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		playerPos.y += speed * delta;

	if (KEY(window, GLFW_KEY_C) == GLFW_PRESS)
		playerPos.y -= speed * delta;

	camRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Ujemnocraft", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();

		return -2;
	}

	if (!GLEW_VERSION_4_5)
	{
		glfwTerminate();

		return -3;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer);
	glfwSetCursorPosCallback(window, mouse);
	framebuffer(window, 800, 600);

	/*Block block;
	block.Init();

	block.Add();
	block.Col((rd() % 100) / 100.0f, (rd() % 100) / 100.0f, (rd() % 100) / 100.0f, 1.0f);

	block.Set(0);
	block.SetID(-1.0f, (float)0, (float)0);
	
	for (int i = 0; i < 256; i++)
	{
		block.Add();
		block.Col((rd() % 100) / 100.0f, (rd() % 100) / 100.0f, (rd() % 100) / 100.0f, 1.0f);
	}

	for (int i = 0; i < 256 * 18; i++)
	{
		block.Set(i / 18);
		block.SetID(-1.0f, (float)i / 18, (float)i / 18);
	}

	block.Accept();*/

	Chunk chunk;

	chunk.Init();
	chunk.Generate();
	chunk.TextureAtlas("data/textures/textureAtlas.png");
	chunk.Applay();

	Text text[2];
	text[0].Init();
	text[1].Init();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		delta = current - last;
		last = current;
		current = glfwGetTime();

		input(window);

		view = glm::lookAt(playerPos, playerPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

		/*for (int i = 0; i < 256; i++)
		{
			block.Set(i);
			block.Pos(grid + i, grid + i, grid + i);
		}*/

		/*block.Set(0);
		block.Pos(grid, grid, grid);

		block.Draw(proj, view);*/

		chunk.Draw(proj, view);

		text[0].Scl(0.25f, 0.5f, 0.5f);
		text[0].Pos(-0.95f, 0.95f, 0.5f);
		text[1].Scl(0.25f, 0.5f, 0.5f);
		text[1].Pos(-0.95f, 0.9f, 0.5f);

		std::string dpf = "DPF: " + std::to_string(delta * 1000) + ' ' + 'm' + 's';
		std::string cameraPosition = "Position: X: " + std::to_string(playerPos.x) + " Y: " + std::to_string(playerPos.y) + " Z: " + std::to_string(playerPos.z);

		text[0].SetText(dpf, 1.0f, 1.0f, 1.0f, 1.0f);
		text[1].SetText(cameraPosition, 1.0f, 1.0f, 1.0f, 1.0f);

		text[0].Draw();
		text[1].Draw();

		glfwPollEvents();

		glfwSwapBuffers(window);

		glfwSwapInterval(0);
	}

	glfwTerminate();

	return 0;
}