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
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

struct FlareMapEntity {
	std::string type;
	float x;
	float y;

};

class FlareMap {
public:
	FlareMap();
	~FlareMap();

	void Load(const std::string fileName);

	int mapWidth;
	int mapHeight;
	unsigned int **mapData;
	std::vector<FlareMapEntity> entities;

private:

	bool ReadHeader(std::ifstream &stream);
	bool ReadLayerData(std::ifstream &stream);
	bool ReadEntityData(std::ifstream &stream);

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
	float x;
	float y;
	float width=1;
	float height = 1;
};

enum EntityType{ENTITY_PLAYER,ENTITY_ENEMY};

class Entity
{
public:
	void Update(float elapsed);
	void Draw(ShaderProgram &p);
	bool CollidesWith(Entity &entity);
	glm::vec3 position=glm::vec3(-1.0, 0.0, 0.0);
	float velocity=0.0;
	float speed_x = 0;
	float speed_y = 0;
	float friction = 0.5f;
	float acceleration_x = 0.0;
	float acceleration_y = 0.0;
	float gravity = -1.0f;
	glm::vec3 size=glm::vec3(-1.0, 0.0, 0.0);

	float rotation=0.0;

	SheetSprite sprite;

	float health=1.0;
	float timeAlive=0.0f;

	bool isStatic;
	EntityType entityType;

};

class Game
{
public:
	Entity ship;
	float lastFrameTicks;
	float speed;
	bool done;
	std::vector<Entity> entities;
	std::vector<float> vertices;
	std::vector<float> texCoords;
	std::vector<Entity> solids;
	ShaderProgram program;
	GLuint fontTexture;
	bool start;
	std::vector<Entity> bullets;
	FlareMap map;
	GLuint roadTexture;
};



GLuint LoadTexture(const char *filepath);
void Setup(Game &g);
void Render(Game &g, float elapsed);
void Runner(Game &g);
void Update(Game &g);
void DrawText(ShaderProgram &p, int fontTexture, std::string text, float size, float spacing);
void shootBullet(Game &g);
void placeEntity(std::string type, float x, float y);
void drawMap(Game &g);
extern float TILE_SIZE;