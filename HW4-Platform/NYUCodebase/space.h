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
	float width=0;
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
};

class Game
{
public:
	float tileSize = 0.1;
	int sprite_count_x = 16;
	int sprite_count_y = 8; 
	std::vector<float> vertices;
	std::vector<float> texCoords;
	FlareMap map;
	Entity ship;
	float lastFrameTicks;
	float speed;
	bool done;
	std::vector<Entity> entities;
	ShaderProgram program;
	GLuint fontTexture;
	bool start;
	std::vector<Entity> bullets;
	unsigned int platformSheetTexture;
};

GLuint LoadTexture(const char *filepath);
void Setup(Game &g);
void Render(Game &g, float elapsed);
void Runner(Game &g);
void Update(Game &g);
void DrawText(ShaderProgram &p, int fontTexture, std::string text, float size, float spacing);
void shootBullet(Game &g);
void drawMap(Game &g);
void worldToTileCoordinates(Game &g, float worldX, float worldY, int *gridX, int *gridY);