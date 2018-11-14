#include "space.h"

void Setup(Game &g)
{
	/*g.map.Load("haha.txt");
	for(int y = 0; y < g.map.mapHeight; ++y)
	{
		for(int x = 0; x < g.map.mapWidth; x++)
		{
			if(g.map.mapData[y][x]!=0)
			{
				int z = g.map.mapData[y][x];
				if(z==2){
				}
			}
		}
	}*/


	//g.roadTexture = LoadTexture(RESOURCE_FOLDER"dirt-tiles.png");
	//g.lastFrameTicks = 0.0f;

	//g.speed = 100.0f;
	float x = 0.0f;
	float y = -0.85f;
	
	g.start = false;
	g.done = false;
	float gg = 1024.0f;
	//GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
	g.fontTexture = LoadTexture("font1.png");

}

void Render(Game &g, float elapsed)
{


}
