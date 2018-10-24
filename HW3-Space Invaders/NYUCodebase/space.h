#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"
#include <iterator>
#include <vector>
#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


class SheetSprite
{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);
	void Draw(ShaderProgram &p);
	float size;
	unsigned int textureID;
	float u;
	float v;
	float x;
	float y;
	float width;
	float height = 0;
};


class Entity
{
public:
	void Draw(ShaderProgram &p);
	glm::vec3 position=glm::vec3(-1.0, 0.0, 0.0);
	float velocity=3.0;
	glm::vec3 size=glm::vec3(-1.0, 0.0, 0.0);

	float rotation=0.0;

	SheetSprite sprite;

	float health=1.0;
	float timeAlive=0.0f;
	void erase();
};

class Game
{
public:
	Entity ship;
	float lastFrameTicks;
	float speed;
	bool done;
	std::vector<Entity> entities;
	ShaderProgram program;
	GLuint fontTexture;
	bool start;
	std::vector<Entity> bullets;
};

GLuint LoadTexture(const char *filepath);
void Setup(Game &g);
void Render(Game &g, float elapsed);
void Runner(Game &g);
void Update(Game &g);
void DrawText(ShaderProgram &p, int fontTexture, std::string text, float size, float spacing);
void shootBullet(Game &g);