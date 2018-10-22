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

class Player
{
public:
	void Draw(ShaderProgram &p);
	float x = 0.0f;
	float y = 0.0f;
	float rotation;

	int textureID;

	float width = 0.05f;
	float height = 0.5f;

	float velocity;
	float direction_x;
	float direction_y;
};


class Ball
{
public:
	void Draw(ShaderProgram &p);
	void reset();
	float x = 0.0f;
	float y = 0.0f;
	float rotation;

	int textureID;

	float width = 0.05f;
	float height = 0.05f;

	float velocity = 8.0f;
	float direction_x;
	float direction_y;
	void move(float elapsed);
};

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
	float width;
	float height = 0;
};

class Entity
{
public:
	void Draw(ShaderProgram &p);
	glm::vec3 position=glm::vec3(-1.0, 0.0, 0.0);
	glm::vec3 velocity=glm::vec3(-1.0, 0.0, 0.0);
	glm::vec3 size=glm::vec3(-1.0, 0.0, 0.0);

	float rotation=0.0;

	SheetSprite sprite;

	float health=0.0;
};

class Game
{
public:
	Player ship;
	Ball ball;
	float lastFrameTicks;
	float speed;
	int score_p1;
	bool done;
	std::vector<Entity> entities;
	ShaderProgram program;


};
class Bullet
{
public:
	void Draw(ShaderProgram &p);
	float x;
	float y;
	float width;
	float height;
	int textureID;
}
;
void detect_collision(Ball &b, Player p);
void bounce_wall(Ball &b);
void check_win(Ball &b, int &score_p1);
GLuint LoadTexture(const char *filepath);
void Setup(Game &g);
void Render(Game &g, float elapsed);
void Runner(Game &g);
void Update(Game &g);