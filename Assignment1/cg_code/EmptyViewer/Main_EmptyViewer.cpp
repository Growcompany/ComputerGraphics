#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cmath>

using namespace glm;

// -------------------------------------------------
// Global Variables
// -------------------------------------------------
int Width = 512;
int Height = 512;
std::vector<float> OutputImage;
// -------------------------------------------------



void render()
{
	//Create our image. We don't want to do this in 
	//the main loop since this may be too slow and we 
	//want a responsive display of our beautiful image.
	//Instead we draw to another buffer and copy this to the 
	//framebuffer using glDrawPixels(...) every refresh
	OutputImage.clear();
	for (int j = 0; j < Height; ++j) 
	{
		for (int i = 0; i < Width; ++i) 
		{
			// ---------------------------------------------------
			// --- Implement your code here to generate the image
			// ---------------------------------------------------

			// draw a red rectangle in the center of the image
			vec3 color = glm::vec3(0.5f, 0.5f, 0.5f); // grey color [0,1] in RGB channel
			
			if (i > Width / 4 && i < 3 * Width / 4 
				&& j > Height / 4 && j < 3 * Height / 4)
			{
				color = glm::vec3(1.0f, 0.0f, 0.0f); // red color [0,1] in RGB channel
			}
			
			// set the color
			OutputImage.push_back(color.x); // R
			OutputImage.push_back(color.y); // G
			OutputImage.push_back(color.z); // B
		}
	}
}

// Ray ����ü: ������ ������ ����
struct Ray {
	vec3 origin;
	vec3 direction;
};

// ������ ���� �˻� �Լ�
// ray�� �߽� center, ������ radius�� ���� ���� ���θ� �˻��մϴ�.
// tMin, tMax ���̿� �������� �����ϸ� true�� ��ȯ�մϴ�.
bool intersectSphere(const Ray& ray, const vec3& center, float radius, float tMin, float tMax, float& tHit) {
	vec3 oc = ray.origin - center;
	float A = dot(ray.direction, ray.direction); // ���� ����ȭ�Ǿ����Ƿ� A=1
	float B = 2.0f * dot(ray.direction, oc);
	float C = dot(oc, oc) - radius * radius;
	float discriminant = B * B - 4 * A * C;
	if (discriminant < 0) return false;
	float sqrtDisc = glm::sqrt(discriminant);
	// �� ���� ���� �켱 ����
	float t1 = (-B - sqrtDisc) / (2 * A);
	if (t1 > tMin && t1 < tMax) { tHit = t1; return true; }
	float t2 = (-B + sqrtDisc) / (2 * A);
	if (t2 > tMin && t2 < tMax) { tHit = t2; return true; }
	return false;
}

// Scene.trace �Լ�: �־��� ������ ����� ��ü�� ���� ������ �˻�
// tMin���� tMax ���̿� �����ϴ� ��ü�� ������ white, �ƴϸ� black�� ��ȯ�մϴ�.
vec3 trace(const Ray& ray, float tMin, float tMax) {
	float tHit;
	// ��� P: y = -2���� ���� �˻�
	if (ray.direction.y != 0.0f) {
		float tPlane = (-2.0f - ray.origin.y) / ray.direction.y;
		if (tPlane > tMin && tPlane < tMax)
			return vec3(1.0f); // white
	}
	// �� S1: �߽� (-4, 0, -7), ������ 1
	if (intersectSphere(ray, vec3(-4.0f, 0.0f, -7.0f), 1.0f, tMin, tMax, tHit))
		return vec3(1.0f); // white
	// �� S2: �߽� (0, 0, -7), ������ 2
	if (intersectSphere(ray, vec3(0.0f, 0.0f, -7.0f), 2.0f, tMin, tMax, tHit))
		return vec3(1.0f); // white
	// �� S3: �߽� (4, 0, -7), ������ 1
	if (intersectSphere(ray, vec3(4.0f, 0.0f, -7.0f), 1.0f, tMin, tMax, tHit))
		return vec3(1.0f); // white

	return vec3(0.0f); // black
}

// render3 �Լ�: ��� �ȼ��� ���� ī�޶��� getRay�� ȣ���ϰ�, scene.trace�� ���� ������ �����մϴ�.
void render3() {
	OutputImage.clear();

	// ī�޶� ���� (eye point)
	vec3 eye(0.0f, 0.0f, 0.0f);
	// �̹��� ����� ��� ���� (view window)
	float l = -0.1f, r = 0.1f, b = -0.1f, t = 0.1f;
	float d = 0.1f; // �̹��� �������� �Ÿ� (ī�޶��� -z ����)

	int nx = Width;   // �̹��� ���� �ػ�
	int ny = Height;  // �̹��� ���� �ػ�

	// ��� �ȼ��� ���� �ݺ��մϴ�.
	for (int iy = 0; iy < ny; ++iy) {
		for (int ix = 0; ix < nx; ++ix) {
			// �ȼ� �߽� ��ǥ ��� (�̹��� ��� ��)
			float u = l + (r - l) * (ix + 0.5f) / nx;
			float v = b + (t - b) * (iy + 0.5f) / ny;
			vec3 pixelPos(u, v, -d);

			// ī�޶��� getRay ����: ������ �ȼ� �߽��� ���ϴ� ���� ����
			Ray ray;
			ray.origin = eye;
			ray.direction = normalize(pixelPos - eye);

			// scene.trace�� ȣ���Ͽ� ������ ���� (�����ϸ� white, �ƴϸ� black)
			vec3 color = trace(ray, 0.0f, 1e30f);

			// ���� ������ �̹��� ���ۿ� �����մϴ�.
			OutputImage.push_back(color.r);
			OutputImage.push_back(color.g);
			OutputImage.push_back(color.b);
		}
	}
}


void resize_callback(GLFWwindow*, int nw, int nh) 
{
	//This is called in response to the window resizing.
	//The new width and height are passed in so we make 
	//any necessary changes:
	Width = nw;
	Height = nh;
	//Tell the viewport to use all of our screen estate
	glViewport(0, 0, nw, nh);

	//This is not necessary, we're just working in 2d so
	//why not let our spaces reflect it?
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, static_cast<double>(Width)
		, 0.0, static_cast<double>(Height)
		, 1.0, -1.0);

	//Reserve memory for our render so that we don't do 
	//excessive allocations and render the image
	OutputImage.reserve(Width * Height * 3);
	render3();
}


int main(int argc, char* argv[])
{
	// -------------------------------------------------
	// Initialize Window
	// -------------------------------------------------

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(Width, Height, "OpenGL Viewer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//We have an opengl context now. Everything from here on out 
	//is just managing our window or opengl directly.

	//Tell the opengl state machine we don't want it to make 
	//any assumptions about how pixels are aligned in memory 
	//during transfers between host and device (like glDrawPixels(...) )
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	//We call our resize function once to set everything up initially
	//after registering it as a callback with glfw
	glfwSetFramebufferSizeCallback(window, resize_callback);
	resize_callback(NULL, Width, Height);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// -------------------------------------------------------------
		//Rendering begins!
		glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, &OutputImage[0]);
		//and ends.
		// -------------------------------------------------------------

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		//Close when the user hits 'q' or escape
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
