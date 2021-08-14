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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
void error_callback(int error, const char* description) {
	std::cout << "Error:" << error + description << std::endl;
}
const float WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
#pragma region Camera

float lastX = 400, lastY = 300;
bool firstMouse = true;
Camera camera = Camera(glm::vec3(0, 0, 3.0f));

#pragma endregion

#pragma region Model

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

#pragma endregion

#pragma region Mouse and keyboard callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
void processInput(GLFWwindow* window)
{
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

};
// 键盘回调函数原型声明
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // 关闭窗口
	}
}
#pragma endregion

unsigned int loadImageToGPU(const char* file, GLint internalFormat, GLenum format, int textureSlot) {
	unsigned int texBuffer;
	int width, height, nrChannel;
	glGenTextures(1, &texBuffer);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, texBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data = stbi_load(file, &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("load texture failled.");
	}
	stbi_image_free(data);
	return texBuffer;
}

int main(int argc, char* argv[]) {
#pragma region GLFWInitWindow
	std::string exePath = argv[0];
	std::cout << exePath.substr(0, exePath.find_last_of('\\'))+ "\\model\\nanosuit.obj" << std::endl;
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
	auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "nimasile", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 创建的窗口的context指定为当前context
	glfwMakeContextCurrent(window);
	// 注册窗口键盘事件回调函数
	glfwSetKeyCallback(window, key_callback);

	// 让glew获取所有拓展函数
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Init GLEW failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
#pragma endregion
#pragma region Init and load Model to VAO VBO

#pragma endregion
#pragma region Init and load Textures
	
#pragma endregion

#pragma region Init shader program
	Shader shader = Shader("vertex.vert", "fragment.frag");
	Shader singleColorShader = Shader("singleColorV.vert", "singleColorF.frag");

#pragma endregion

#pragma region Prepare MVP matrices
	//// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

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

#pragma region Model
	Model manModel(exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\nanosuit.obj");
#pragma endregion

#pragma endregion

	while (!glfwWindowShouldClose(window)) {
		// Process Input
		processInput(window);

		// Clear Screen
		// 清除颜色缓冲区 重置为指定颜色
		glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		// 1st. render pass, draw objects as normal, writing to the stencil buffer
// --------------------------------------------------------------------
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
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
		manModel.Draw(&shader);
		// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
		// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
		// the objects' size differences, making it look like borders.
		// -----------------------------------------------------------------------------------------------------------------------------
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
#pragma region borderShader setting
		singleColorShader.use();
		singleColorShader.setMat4("view", view);
		singleColorShader.setMat4("projection", projection);
		singleColorShader.setMat4("model", model);
		singleColorShader.setFloat("scale", 1.1f);
		singleColorShader.setMat4("normalTransform", normalTrasform);
#pragma endregion
		manModel.Draw(&shader);
		glBindVertexArray(0);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	for (int i = 0; i < 4; i++)
	{
		alloc.destroy(pointLights + i);
	}
	alloc.deallocate(pointLights, 4);
	glfwTerminate();
	return 0;
}

