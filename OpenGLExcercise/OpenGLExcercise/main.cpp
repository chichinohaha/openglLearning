#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionLight.h"
#include "Model.h"
#include "sstream"
#include  <GL/glu.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <map>
void error_callback(int error, const char* description) {
	std::cout << "Error:" << error + description << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Determine whether a ray intersect with a triangle
// Parameters
// orig: origin of the ray
// dir: direction of the ray
// v0, v1, v2: vertices of triangle
// t(out): weight of the intersection for the ray
// u(out), v(out): barycentric coordinate of intersection

bool IntersectTriangle(const glm::vec3& orig, const glm::vec3& dir,
	glm::vec3& v0, glm::vec3& v1, glm::vec3& v2,
	float* t, float* u, float* v)
{

	// E1
	glm::vec3 E1 = v1 - v0;

	// E2
	glm::vec3 E2 = v2 - v0;

	// P
	glm::vec3 P = glm::cross(dir,E2);

	// determinant
	float det = glm::dot(E1,P);

	// keep det > 0, modify T accordingly
	glm::vec3 T;
	if (det > 0)
	{
		T = orig - v0;
	}
	else
	{
		T = v0 - orig;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if (det < 0.0001f)
		return false;

	// Calculate u and make sure u <= 1
	*u = glm::dot(T,P);
	if (*u < 0.0f || *u > det)
		return false;

	// Q
	glm::vec3 Q = glm::cross(T,E1);

	// Calculate v and make sure u + v <= 1
	*v = glm::dot(dir,Q);
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = glm::dot(E2,Q);

	float fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return true;
}

const float WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
#pragma region Camera
glm::vec2 translations[100] = {};
int selectedID = 0;
float lastX = 400, lastY = 300;
bool firstMouse = true;
double winX;
double winY;
bool rightMouseButtonDown = false;
bool leftMouseButtonDown = false;
glm::mat4 projection = glm::perspective(glm::radians(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
Camera camera = Camera(glm::vec3(0, 0, 3.0f));
glm::vec3 unProject(glm::vec3 screen) {
	glm::mat4 view = camera.getViewMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport); // 得到的是最后一个设置视口的参数
	auto rayClip = glm::vec4(2.0f * (screen.x - viewport[0]) / viewport[2] - 1.0f, -2.0f * (screen.y - viewport[1]) / viewport[3] + 1, screen.z * 2.0f - 1.0f, 1.0f);
	auto rayEye = glm::inverse(projection) * rayClip;
	glm::vec4 rayPoint = glm::inverse(view) * rayEye;
	glm::vec3 posOut = glm::vec3(rayPoint);
	posOut /= rayPoint.w;
	return posOut;
}
#pragma endregion

#pragma region Model
float vertices[] = {
	// positions          // colors
	 0.05f,  0.05f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.05f, -0.05f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.05f, -0.05f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.05f,  0.05f, 0.0f, 1.0f, 0.0f, 0.0f
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

std::vector<std::string> faces
{
	"right.jpg",
	"left.jpg",
	"top.jpg",
	"bottom.jpg",
	"front.jpg",
	"back.jpg"
};
unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};                               
#pragma endregion

#pragma region Mouse and keyboard callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	winX = xpos;
	winY = ypos;
	if (rightMouseButtonDown) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.05;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera.yaw += xoffset;
		camera.pitch += yoffset;

		if (camera.pitch > 89.0f)
			camera.pitch = 89.0f;
		if (camera.pitch < -89.0f)
			camera.pitch = -89.0f;

	}
	else {
		firstMouse = true;
	}
}
void processInput(GLFWwindow* window)
{
	if (rightMouseButtonDown) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		float cameraSpeed = 0.005f; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			cameraSpeed = 0.1f;
		}
		else {
			cameraSpeed = 0.005f;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.position += cameraSpeed * camera.front;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.position -= cameraSpeed * camera.front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.position += glm::normalize(camera.up) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.position -= glm::normalize(camera.up) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}
};
// 键盘回调函数原型声明
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // 关闭窗口
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {

		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT: 
		
		{

			glm::vec3 nearPoint = unProject(glm::vec3(winX,winY,0));
			glm::vec3 farPoint = unProject(glm::vec3(winX, winY, 1));
			glm::vec3 rayOrigin = nearPoint;
			glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);

			auto minT = 10000000.0f;
			float t,u,v;
			std::cout << "  ray point x:" << rayOrigin[0] << "  y:" << rayOrigin[1] << "  z:" << rayOrigin[2] << std::endl;
			std::cout << "  ray dir x:" << rayDir[0] << "  y:" << rayDir[1] << "  z:" << rayDir[2] << std::endl;
			for (int k = 0; k < sizeof translations/sizeof * translations; k++)
			{

			for (int i = 0; i < sizeof(indices) / sizeof(*indices) / 3; i++)
			{
				glm::vec3 triangle[3];
				
					for (int j = 0; j < 3; j++)
					{
						auto pointIndex = indices[i * 3 + j];
						//auto offset = translations[k];
						auto model = glm::translate(glm::mat4(1.0f),glm::vec3(translations[k],0.0f));
						triangle[j] = model* glm::vec4( vertices[pointIndex * 6] ,vertices[pointIndex * 6 + 1],vertices[pointIndex * 6 + 2] ,1.0f);
	
							
					}
					auto isIntersect = IntersectTriangle(rayOrigin,rayDir,triangle[0], triangle[1], triangle[2],&t,&u,&v);
					if (isIntersect) {
						if (t < minT) {
							minT = t;
							selectedID = k;
						}
						std::cout << "hit" << t << std::endl;
					}
				
			}
			}
			std::cout << "left button press" << "  x:" << winX << "  y:" << winY  << std::endl;
			 //now we iterate through all meshes, and find the closest mesh that intersects the camera ray.

		}
			
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			rightMouseButtonDown = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			std::cout << "right button press" << "x:" << winX << "y:" << winY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:

			std::cout << "middle button press" << "x:" << winX << "y:" << winY << std::endl;
			break;
		default:
			break;
		}
	}
	if (action == GLFW_RELEASE) {
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			leftMouseButtonDown = false;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			rightMouseButtonDown = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;
		default:
			break;
		}
	}

}

#pragma endregion

unsigned int loadImageToGPU(const char* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
int main(int argc, char* argv[]) {
#pragma region GLFWInitWindow
	std::string exePath = argv[0];
	std::cout << exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\nanosuit.obj" << std::endl;
	
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		std::cout << "Failed to init GLFW " << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open GLFW Window
	auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "graph", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 创建的窗口的context指定为当前context
	glfwMakeContextCurrent(window);
	// 注册窗口键盘事件回调函数
	glfwSetKeyCallback(window, key_callback);
	// 注册窗口大小回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// 让glew获取所有拓展函数
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Init GLEW failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

#pragma endregion
#pragma region Init and load Model to VAO VBO
	unsigned int VBO, VAO, EBO,CubeVAO,CubeVBO;
	unsigned int INSTANCEDVBO;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &CubeVBO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}
	////// 将实例的数据单独存在这个 INSTANCEDVBO
	glGenBuffers(1,&INSTANCEDVBO);
	glBindBuffer(GL_ARRAY_BUFFER, INSTANCEDVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*100, &translations[0], GL_STATIC_DRAW);

	// glVertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, INSTANCEDVBO); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(CubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), nullptr, GL_STATIC_DRAW);
	void * ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	// 复制数据到内存
	memcpy(ptr, skyboxVertices, sizeof(skyboxVertices));
	// 记得告诉OpenGL我们不再需要这个指针了
	glUnmapBuffer(GL_ARRAY_BUFFER);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	
	glBindVertexArray(0);
#pragma endregion
#pragma region Init and load Textures
	auto cubeTexture = loadCubemap(faces);

#pragma endregion




#pragma region Init shader program
	Shader base = Shader("base.vert", "base.frag");
	Shader cube = Shader("cube.vert", "cube.frag");
	Shader shader = Shader("model.vert", "model.frag");
#pragma endregion

#pragma region light
	glm::vec3 lightRotation(45.0f, 45.0f, 2.0f);
	glm::vec3 lightDiffuseColor(1.0f, 1.0f, 1.0f);
	glm::vec3 lightAmbient(0.1f, 0.1f, 0.1f);
	glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
	DirectionLight light(&shader, lightAmbient, lightDiffuseColor, lightSpecular, lightRotation);
	std::allocator<PointLight>alloc;
	PointLight* pointLights = alloc.allocate(4);
	for (int i = 0; i < 4; i++)
	{
		alloc.construct(pointLights + i, &shader, lightAmbient, lightDiffuseColor, lightSpecular, pointLightPositions[i]);
	}
	SpotLight spotLight(&shader, lightAmbient, lightDiffuseColor, lightSpecular, camera.position, camera.front, glm::vec3(camera.pitch, camera.yaw, camera.roll), 1.0f, 0.09f, 0.032f, 12.5f, 15.0f);
#pragma endregion
#pragma region material
	float materialShininess = 64.0f;
	Material material(&shader, materialShininess, Shader::DIFFUSE, Shader::SPECULAR);
#pragma endregion
#pragma region loadModel
	Model star(exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\planet.obj");
#pragma endregion
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#pragma region shader setting
		glm::mat4 view = camera.getViewMatrix();
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		for (int i = 0; i < 4; i++)
		{
			std::stringstream stream;
			stream << "pointLights[" << i << "]";
			pointLights[i].applyToShader(stream.str());
		}
		shader.setVec3("cameraPos", camera.position);
		light.applyToShader("directionLight");
		material.applyToShader("material");

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));	// it's a bit too big for our scene, so scale it down
		shader.setMat4("model", model);
		glm::mat4 normalTrasform = glm::inverse(model);
		normalTrasform = glm::transpose(normalTrasform);
		shader.setMat4("normalTransform", normalTrasform);
		spotLight.position = camera.position;
		spotLight.direction = -1.0f * camera.front;
		spotLight.applyToShader("spotLight");
#pragma endregion
		star.Draw(&shader);

		shader.setMat4("model", glm::translate(model, glm::vec3(8, 0, 0)));
		star.Draw(&shader);

		base.use();
		base.setMat4("projection", projection);
		base.setMat4("view", view);
		base.setMat4("model", glm::mat4(1.0f));
		base.setInt("selectedID", selectedID);
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElementsInstanced(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0,100);
		cube.use();
		cube.setMat4("projection", projection);
		cube.setMat4("view", glm::mat4(glm::mat3(view)));
		cube.setInt("skybox", 0);
		glBindVertexArray(CubeVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//starShader.setMat4("projection", projection);
		//starShader.setMat4("view", view);
		//starShader.setMat4("model", glm::mat4(1.0f));
		//star.Draw(&starShader);
		glBindVertexArray(0);
		glDepthFunc(GL_LEQUAL);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
