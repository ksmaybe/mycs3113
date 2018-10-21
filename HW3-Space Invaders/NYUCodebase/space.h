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
	void Draw(ShaderProgram &program);
	float size;
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
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
void Render(Game &g, ShaderProgram program, float elapsed);
void Runner(Game &g);