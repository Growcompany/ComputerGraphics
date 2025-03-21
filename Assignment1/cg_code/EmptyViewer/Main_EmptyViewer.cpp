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

// Ray 구조체: 광선의 원점과 방향
struct Ray {
	vec3 origin;
	vec3 direction;
};

// 구와의 교차 검사 함수
// ray와 중심 center, 반지름 radius인 구의 교차 여부를 검사합니다.
// tMin, tMax 사이에 교차점이 존재하면 true를 반환합니다.
bool intersectSphere(const Ray& ray, const vec3& center, float radius, float tMin, float tMax, float& tHit) {
	vec3 oc = ray.origin - center;
	float A = dot(ray.direction, ray.direction); // 보통 정규화되었으므로 A=1
	float B = 2.0f * dot(ray.direction, oc);
	float C = dot(oc, oc) - radius * radius;
	float discriminant = B * B - 4 * A * C;
	if (discriminant < 0) return false;
	float sqrtDisc = glm::sqrt(discriminant);
	// 더 작은 근을 우선 선택
	float t1 = (-B - sqrtDisc) / (2 * A);
	if (t1 > tMin && t1 < tMax) { tHit = t1; return true; }
	float t2 = (-B + sqrtDisc) / (2 * A);
	if (t2 > tMin && t2 < tMax) { tHit = t2; return true; }
	return false;
}

// Scene.trace 함수: 주어진 광선과 장면의 객체들 간의 교차를 검사
// tMin부터 tMax 사이에 교차하는 객체가 있으면 white, 아니면 black을 반환합니다.
vec3 trace(const Ray& ray, float tMin, float tMax) {
	float tHit;
	// 평면 P: y = -2와의 교차 검사
	if (ray.direction.y != 0.0f) {
		float tPlane = (-2.0f - ray.origin.y) / ray.direction.y;
		if (tPlane > tMin && tPlane < tMax)
			return vec3(1.0f); // white
	}
	// 구 S1: 중심 (-4, 0, -7), 반지름 1
	if (intersectSphere(ray, vec3(-4.0f, 0.0f, -7.0f), 1.0f, tMin, tMax, tHit))
		return vec3(1.0f); // white
	// 구 S2: 중심 (0, 0, -7), 반지름 2
	if (intersectSphere(ray, vec3(0.0f, 0.0f, -7.0f), 2.0f, tMin, tMax, tHit))
		return vec3(1.0f); // white
	// 구 S3: 중심 (4, 0, -7), 반지름 1
	if (intersectSphere(ray, vec3(4.0f, 0.0f, -7.0f), 1.0f, tMin, tMax, tHit))
		return vec3(1.0f); // white

	return vec3(0.0f); // black
}

// render3 함수: 모든 픽셀에 대해 카메라의 getRay를 호출하고, scene.trace를 통해 색상을 결정합니다.
void render3() {
	OutputImage.clear();

	// 카메라 설정 (eye point)
	vec3 eye(0.0f, 0.0f, 0.0f);
	// 이미지 평면의 경계 설정 (view window)
	float l = -0.1f, r = 0.1f, b = -0.1f, t = 0.1f;
	float d = 0.1f; // 이미지 평면까지의 거리 (카메라의 -z 방향)

	int nx = Width;   // 이미지 가로 해상도
	int ny = Height;  // 이미지 세로 해상도

	// 모든 픽셀에 대해 반복합니다.
	for (int iy = 0; iy < ny; ++iy) {
		for (int ix = 0; ix < nx; ++ix) {
			// 픽셀 중심 좌표 계산 (이미지 평면 상)
			float u = l + (r - l) * (ix + 0.5f) / nx;
			float v = b + (t - b) * (iy + 0.5f) / ny;
			vec3 pixelPos(u, v, -d);

			// 카메라의 getRay 역할: 눈에서 픽셀 중심을 향하는 광선 생성
			Ray ray;
			ray.origin = eye;
			ray.direction = normalize(pixelPos - eye);

			// scene.trace를 호출하여 색상을 결정 (교차하면 white, 아니면 black)
			vec3 color = trace(ray, 0.0f, 1e30f);

			// 계산된 색상을 이미지 버퍼에 저장합니다.
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
