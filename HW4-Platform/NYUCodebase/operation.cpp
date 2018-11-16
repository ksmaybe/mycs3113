#include "space.h"
void Setup(Game &g)
{
	g.map.Load("haha.txt");
	for (int y = 0; y < g.map.mapHeight; y++) {
		for (int x = 0; x < g.map.mapWidth; x++) {
			if (g.map.mapData[y][x] != 0 && g.map.mapData[y][x] != 12) {
				float u = (float)(((int)g.map.mapData[y][x]) % g.sprite_count_x) / (float)g.sprite_count_x;
				float v = (float)(((int)g.map.mapData[y][x]) / g.sprite_count_x) / (float)g.sprite_count_y;
				float spriteWidth = 1.0f / (float)g.sprite_count_x;
				float spriteHeight = 1.0f / (float)g.sprite_count_y;
				g.vertices.insert(g.vertices.end(), {
					g.tileSize * x, -g.tileSize * y,
					g.tileSize * x, (-g.tileSize * y) - g.tileSize,
					(g.tileSize * x) + g.tileSize, (-g.tileSize * y) - g.tileSize,
					g.tileSize * x, -g.tileSize * y,
					(g.tileSize * x) + g.tileSize, (-g.tileSize * y) - g.tileSize,
					(g.tileSize * x) + g.tileSize, -g.tileSize * y
					});
				g.texCoords.insert(g.texCoords.end(), {
					u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u, v,
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
					});
				FlareMapEntity ground;
				ground.x = x*g.tileSize;
				ground.y = -y*g.tileSize;
				g.map.ground.push_back(ground);
			}
		}
		g.ground.push_back(532);
	}
	g.platformSheetTexture=LoadTexture(RESOURCE_FOLDER"arne_sprites.png");
	g.lastFrameTicks = 0.0f;

	g.speed = 30.0f;
	float x = 0.0f;
	float y = -0.85f;
	
	g.start = false;
	g.done = false;
	float gg = 1024.0f;
	GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
	g.ship.sprite = SheetSprite(spriteSheetTexture, 0.0f / gg, 941.0f / gg, 112.0f / gg, 75.0f / gg, 0.15);
	g.ship.sprite.x = x;
	g.ship.sprite.y = y;
	g.ship.sprite.x += 2.0f;
	g.ship.sprite.y -= 1.0f;
	g.fontTexture = LoadTexture("font1.png");
	for(int i=0;i<30;i++)
	{
		float x = 0.0;
		float y = 0.0;
		x = i % 6 * 0.2-0.3;
		y = -(i / 6*0.15)+0.8;
		Entity myEntity;
		myEntity.sprite = SheetSprite(spriteSheetTexture, 425.0f / gg, 468.0f / gg, 93.0f / gg, 84.0f / gg, 0.1);
		myEntity.sprite.x = x;
		myEntity.sprite.y = y;
		g.entities.push_back(myEntity);
	}
	

}

void Render(Game &g, float elapsed)
{
	bool k=collisionBot(g);
	if(!g.ship.isStatic)
	{
		g.ship.sprite.y -= elapsed * g.gravity;
	}
	
	for (int i = 0; i < 30; i++)
	{
		g.entities[i].sprite.x+=elapsed*g.entities[i].velocity;
		if(g.entities[i].health==0.0)
		{
			Entity NewEntity;
			g.entities[i] = NewEntity;
		}
	}

	for(int i=0;i<g.bullets.size();i++)
	{
		if (g.bullets[i].faceLeft == true) { g.bullets[i].sprite.x -= g.bullets[i].velocity*elapsed; }
		else{ g.bullets[i].sprite.x += g.bullets[i].velocity*elapsed; }
		for (int j=0;j<30;j++)
		{
			SheetSprite b = g.bullets[i].sprite;
			SheetSprite e = g.entities[j].sprite;
			float dx = abs(b.x - e.x) - ((b.width + e.width) / 2);
			float dy = abs(b.y - e.y) - ((b.height + e.height) / 2);
			if(dy<=0&dx<=0)
			{
				g.entities[j].health = 0.0;
				Entity bullet;
				g.bullets[i]=bullet;
			}
		}
	}

	if(g.entities[5].sprite.x+g.entities[5].sprite.width/2 >1.777f || g.entities[0].sprite.x - g.entities[0].sprite.width / 2<-1.777f)
	{
		for(int i=0;i<30;i++)
		{
			g.entities[i].velocity = -g.entities[i].velocity;
		}
	}

}
