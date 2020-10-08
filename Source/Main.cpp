#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <cstdio>
#include <vector>
#include <glm/glm.hpp>
#include <Gl/GLU.h>
#include "glm/gtc/matrix_transform.hpp"

// Delta time
float dt;
float curTime;
float lastTime;

// Globals
glm::vec3 position(0.f);
glm::vec3 rotation(0.f);
glm::vec3 scale(1.f);

int resX;
int resY;

// Camera (Not implemented yet)
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


void updateDt()
{
	curTime = static_cast<float>(glfwGetTime());
	dt = curTime - lastTime;
	lastTime = curTime;
}

void getInput(GLFWwindow* window)
{
	position.x = 0.f;
	position.y = 0.f;
	position.z = 0.f;
	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z = 0.f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position.y += 1.f * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position.y -= 1.f * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position.x -= 1.f * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position.x += 1.f * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotation.x = -1.f;
		rotation.y = -1.f;
		glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotation.y = 1.f;
		rotation.z = 1.f;
		glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

void renderTriangle()
{
    glRotatef(1.f, 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();
}

void renderCube()
{
	glTranslatef(position.x, position.y, position.z);
	
	glScalef(scale.x, scale.y, scale.z);

	glBegin(GL_QUADS);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glColor3f(0.0f, 1.0f, 0.0f);

	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glColor3f(0.0f, 0.0f, 1.0f);

	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(-0.5f, -0.5f, 0.5f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glEnd();
}


int main()
{
	resX = 960;
	resY = 840;

    GLFWwindow* window;

    if (!glfwInit())
    {
        return -1;
    }

    window = glfwCreateWindow(resX, resY, "CW3D", NULL, NULL);

    if (!window)
    {
        printf("There has been an error opening the window.");
        glfwTerminate();
        return -1;
    }

	dt = 0.f;
	curTime = 0.f;
	lastTime = 0.f;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        renderCube();

		getInput(window);

		updateDt();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}