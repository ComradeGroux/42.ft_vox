#include "GLError.hpp"

#include <iostream>

#include "glad.h"
#include "gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

#include "World.hpp"
#include "Camera.hpp"
#include "Frustum.hpp"
#include "InputManager.hpp"
#include "Shader.hpp"
#include "ChunkMesh.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "WorldConstants.hpp"
#include "utils.hpp"

struct GameContext {
	World			world;
	Camera			camera;
	InputManager	inputManager;
	Shader			shader;
	Texture			texture;
	Skybox			skybox;
	Frustum			frustum;
	float			timeOfDay;

	GameContext(uint64_t seed) :
		world(seed),
		camera(glm::vec3(0.0f, 120.0f, 0.0f), -90.0f, 0.0f),
		inputManager(camera),
		shader("shaders/chunk.vert", "shaders/chunk.frag"),
		texture("textures/atlas.png"),
		skybox("shaders/skybox.vert", "shaders/skybox.frag"),
		frustum(),
		timeOfDay(0.0f)
	{}
};

static GLFWwindow*	initGLFW()
{
	if (!glfwInit())
	{
		std::cerr << "[GLFW] Failed to initialize" << std::endl;
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	GLFWmonitor*	 monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode*	mode = glfwGetVideoMode(monitor);

	GLFWwindow*	window = glfwCreateWindow(mode->width, mode->height, "FT_VOX", monitor, nullptr);
	if (!window)
	{
		std::cerr << "[GLFW] Failed to create window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}

static bool	initGLAD(GLFWwindow* window)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		std::cerr << "[GLAD] Failed to initialize" << std::endl;
		return false;
	}
	return true;
}

static void	renderSolid(GameContext& context)
{
	for (auto& [key, chunk] : context.world.getChunks())
	{
		const ChunkMesh*	mesh = context.world.getMesh(key);
		if (!mesh || mesh->isEmpty())
			continue;

		glm::vec3	min = glm::vec3(chunk->getChunkX() * 16.0f, 0.0f, chunk->getChunkZ() * 16.0f);
		glm::vec3	max = glm::vec3(min.x + 16.0f, 256.0f, min.z + 16.0f);
		if (!context.frustum.isBoxVisible(min, max))
			continue;

		glm::mat4	model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk->getChunkX() * 16.0f, 0.0f, chunk->getChunkZ() * 16.0f));
		context.shader.setMat4("uModel", model);
		mesh->draw();
	}
}

static void	renderWater(GameContext& context)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	for (auto& [key, chunk] : context.world.getChunks())
	{
		const ChunkMesh*	mesh = context.world.getMesh(key);
		if (!mesh || mesh->isWaterEmpty())
			continue;

		glm::vec3	min = glm::vec3(chunk->getChunkX() * 16.0f, 0.0f, chunk->getChunkZ() * 16.0f);
		glm::vec3	max = glm::vec3(min.x + 16.0f, 256.0f, min.z + 16.0f);
		if (!context.frustum.isBoxVisible(min, max))
			continue;

		glm::mat4	model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk->getChunkX() * 16.0f, 0.0f, chunk->getChunkZ() * 16.0f));
		context.shader.setMat4("uModel", model);
		mesh->drawWater();
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

static glm::vec3	updateLightDir(GameContext& context, float deltaTime)
{
	glm::vec3		baseDir(0.0, -1.0, 0.0);

	context.timeOfDay += deltaTime * DAY_SPEED;
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), context.timeOfDay, glm::vec3(0.0f, 0.0f, 1.0f));
    return glm::normalize(glm::vec3(rotation * glm::vec4(baseDir, 0.0f)));
}

static void	render(GLFWwindow* window, GameContext& context, float deltaTime)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = (float)width / (float)height;

	glm::mat4	view = context.camera.getViewMatrix();
	glm::mat4	projection = context.camera.getProjectionMatrix(aspectRatio);

	glm::vec3	lightDir = updateLightDir(context, deltaTime);
	float lightIntensity = glm::clamp(-lightDir.y, 0.0f, 1.0f);
	lightIntensity = glm::smoothstep(0.0f, 0.3f, lightIntensity);

	context.skybox.draw(view, projection, lightDir, lightIntensity);

	context.texture.bind(0);
	context.shader.use();
	context.shader.setMat4("uView", view);
	context.shader.setMat4("uProjection", projection);
	context.shader.setInt("uAtlas", 0);
	context.shader.setVec3("uLightDir", lightDir);
	context.shader.setFloat("uLightIntensity", lightIntensity);

	context.frustum.update(view, projection);

	renderSolid(context);
	renderWater(context);
}

static void	gameLoop(GLFWwindow *window, GameContext& context)
{
	float		lastTime  = 0.0f;
	float		deltaTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float	currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents();
		context.inputManager.processKeyboard(window, deltaTime);

		glm::vec3 pos = context.camera.getPosition();
		context.world.updateLoadedChunks(pos.x, pos.z);

		cgl(glClearColor(0.529f, 0.808f, 0.922f, 1.0f));
		cgl(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		render(window, context, deltaTime);

		glfwSwapBuffers(window);
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: Wrong number of argument." << std::endl;
		std::cout << "Usage: ./ft_vox <SEED>" << std::endl;
		return 1;
	}

	GLFWwindow* window = initGLFW();
	if (!window)
		return 2;

	if (!initGLAD(window))
		return 3;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	cgl(glViewport(0, 0, width, height));
	{
		GameContext context(hashSeed(argv[1]));

		glfwSetWindowUserPointer(window, (void*)&(context.inputManager));
		glfwSetCursorPosCallback(window, mouseCallback);

		cgl(glEnable(GL_DEPTH_TEST));
		cgl(glEnable(GL_CULL_FACE));
		cgl(glCullFace(GL_BACK));

		gameLoop(window, context);
	}

	glfwTerminate();
	return 0;
}
