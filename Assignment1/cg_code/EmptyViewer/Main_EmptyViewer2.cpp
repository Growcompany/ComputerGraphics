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
int Height = 51;
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
				color = glm::vec3(1.0f, 1.0f, 1.0f); // red color [0,1] in RGB channel
			}
			
			// set the color
			OutputImage.push_back(color.x); // R
			OutputImage.push_back(color.y); // G
			OutputImage.push_back(color.z); // B
		}
	}
}

void render2()
{
    // 렌더링할 이미지 데이터를 초기화
    OutputImage.clear();

    // 카메라 및 시야 영역 설정
    vec3 eye(0.0f, 0.0f, 0.0f);      // 카메라 위치
    float l = -0.1f, r = 0.1f;        // 좌우 경계
    float b = -0.1f, t = 0.1f;        // 상하 경계
    float d = 0.1f;                 // 이미지 평면까지의 거리 (카메라의 -z 방향)

    int nx = Width;   // 이미지 가로 해상도 (512)
    int ny = Height;  // 이미지 세로 해상도 (512)

    // 모든 픽셀에 대해 광선 생성 및 교차 검사
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            // 픽셀 중심 좌표 계산 (이미지 평면 상)
            float u = l + (r - l) * (i + 0.5f) / nx;
            float v = b + (t - b) * (j + 0.5f) / ny;
            vec3 pixelPos(u, v, -d);

            // 눈에서 픽셀 중심으로 향하는 단위 방향 벡터
            vec3 rayDir = normalize(pixelPos - eye);

            // t_min은 가장 가까운 교차점을 저장하기 위한 변수
            float t_min = 1e30f;
            bool hit = false;

            // ==================================================
            // 평면 P (y = -2)와의 교차 검사
            // 광선: R(t) = eye + t * rayDir, 평면: y = -2
            // => eye.y + t * rayDir.y = -2  =>  t = (-2 - eye.y) / rayDir.y (단, rayDir.y != 0)
            if (rayDir.y != 0) {
                float t_plane = (-2.0f - eye.y) / rayDir.y;
                if (t_plane > 0 && t_plane < t_min) {
                    t_min = t_plane;
                    hit = true;
                }
            }

            // ==================================================
            // 구 S1: 중심 (-4, 0, -7), 반지름 1
            {
                vec3 center(-4.0f, 0.0f, -7.0f);
                float radius = 1.0f;
                vec3 oc = eye - center;
                float A = dot(rayDir, rayDir);  // rayDir는 정규화되어 있으므로 A=1
                float B = 2.0f * dot(rayDir, oc);
                float C = dot(oc, oc) - radius * radius;
                float disc = B * B - 4 * A * C;
                if (disc >= 0) {
                    float t1 = (-B - glm::sqrt(disc)) / (2 * A);
                    if (t1 > 0 && t1 < t_min) { t_min = t1; hit = true; }
                    float t2 = (-B + glm::sqrt(disc)) / (2 * A);
                    if (t2 > 0 && t2 < t_min) { t_min = t2; hit = true; }
                }
            }

            // ==================================================
            // 구 S2: 중심 (0, 0, -7), 반지름 2
            {
                vec3 center(0.0f, 0.0f, -7.0f);
                float radius = 2.0f;
                vec3 oc = eye - center;
                float A = dot(rayDir, rayDir);
                float B = 2.0f * dot(rayDir, oc);
                float C = dot(oc, oc) - radius * radius;
                float disc = B * B - 4 * A * C;
                if (disc >= 0) {
                    float t1 = (-B - glm::sqrt(disc)) / (2 * A);
                    if (t1 > 0 && t1 < t_min) { t_min = t1; hit = true; }
                    float t2 = (-B + glm::sqrt(disc)) / (2 * A);
                    if (t2 > 0 && t2 < t_min) { t_min = t2; hit = true; }
                }
            }

            // ==================================================
            // 구 S3: 중심 (4, 0, -7), 반지름 1
            {
                vec3 center(4.0f, 0.0f, -7.0f);
                float radius = 1.0f;
                vec3 oc = eye - center;
                float A = dot(rayDir, rayDir);
                float B = 2.0f * dot(rayDir, oc);
                float C = dot(oc, oc) - radius * radius;
                float disc = B * B - 4 * A * C;
                if (disc >= 0) {
                    float t1 = (-B - glm::sqrt(disc)) / (2 * A);
                    if (t1 > 0 && t1 < t_min) { t_min = t1; hit = true; }
                    float t2 = (-B + glm::sqrt(disc)) / (2 * A);
                    if (t2 > 0 && t2 < t_min) { t_min = t2; hit = true; }
                }
            }

            // ==================================================
            // 픽셀 색상 결정: 하나라도 객체와 교차하면 흰색, 아니면 검정색
            if (hit) {
                OutputImage.push_back(1.0f); // R
                OutputImage.push_back(1.0f); // G
                OutputImage.push_back(1.0f); // B
            }
            else {
                OutputImage.push_back(0.0f); // R
                OutputImage.push_back(0.0f); // G
                OutputImage.push_back(0.0f); // B
            }
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
	render2();
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
