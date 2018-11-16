#include "space.h"

void Setup(Game &g)
{
	g.map.Load("haha.txt");
	for(int y = 0; y < g.map.mapHeight; ++y)
	{
		for(int x = 0; x < g.map.mapWidth; x++)
		{
			if(g.map.mapData[y][x]!=0)
			{
				int z = g.map.mapData[y][x];
				if(z==532){
					FlareMapEntity solid;
					solid.type = "solid";
					solid.x = (x/g.map.mapWidth)-(g.map.mapWidth/2)*1.777f;
					solid.y = (y / g.map.mapHeight) - (g.map.mapHeight / 2)*1.0f;
					g.map.entities.push_back(solid);
				}
			}
		}
	}
	g.roadTexture = LoadTexture(RESOURCE_FOLDER"arne_sprites.png");
	for (auto& entitie : g.map.entities)
	{
		Entity ent;
		ent.sprite.x = entitie.x;
		ent.sprite.y = entitie.y;
		ent.sprite.textureID = g.roadTexture;
		g.solids.push_back(ent);
	}

	
	g.lastFrameTicks = 0.0f;

	g.speed = 100.0f;
	float x = 0.0f;
	float y = -0.85f;
	
	g.start = false;
	g.done = false;
	float gg = 1024.0f;
	GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
	g.fontTexture = LoadTexture("font1.png");

}

void Render(Game &g, float elapsed)
{
	for(Entity i:g.solids)
	{
		i.sprite.Draw(g.program);
	}

}
