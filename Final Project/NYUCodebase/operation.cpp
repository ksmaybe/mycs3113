#include "space.h"
void Setup(Game &g,std::string stage_name)
{
	g.map.Load(stage_name);
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
		g.ground.push_back(4);
	}
	g.platformSheetTexture=LoadTexture(RESOURCE_FOLDER"arne_sprites.png");
	g.lastFrameTicks = 0.0f;

	g.speed = 10.0f;
	float x = 0.0f;
	float y = -0.85f;
	float xx = 256.0f;
	float yy = 128.0f;
	g.start = false;
	g.done = false;
	float gg = 1024.0f;
	GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
	g.ship.sprite = SheetSprite(spriteSheetTexture, 0.0f / gg, 941.0f / gg, 112.0f / gg, 75.0f / gg, 0.15);
	g.ship.sprite.x = x;
	g.ship.sprite.y = y;
	g.ship.sprite.x += 2.0f;
	g.ship.sprite.y -= 1.0f;
	g.ship.health = 1.0;
	g.fontTexture = LoadTexture("font1.png");
	for(int i=0;i< g.map.entities.size();i++)
	{
		FlareMapEntity haha = g.map.entities[i];
		Entity myEntity;
		myEntity.type = haha.type;
		myEntity.health = 1.0;		
		if(myEntity.type=="GATE")
		{
			myEntity.sprite = SheetSprite(g.platformSheetTexture, 0.0f/xx, 48.0f / yy, 16.0f/xx, 16.0f/yy, 0.15);
		}
		else { myEntity.sprite = SheetSprite(spriteSheetTexture, 425.0f / gg, 468.0f / gg, 93.0f / gg, 84.0f / gg, 0.1); }
		myEntity.sprite.x = haha.x*g.tileSize;
		myEntity.sprite.y = haha.y*-g.tileSize+0.3f;
		

		g.enemies.push_back(myEntity);
	}

}

void Render(Game &g, float elapsed)
{
	bool k=collisionBot(g,g.ship);
	if(!g.ship.isStatic)
	{
		g.ship.sprite.y -= elapsed * g.gravity;
	}
	
	/*for (int i = 0; i < 30; i++)
	{
		g.entities[i].sprite.x+=elapsed*g.entities[i].velocity;
		if(g.entities[i].health==0.0)
		{
			Entity NewEntity;
			g.entities[i] = NewEntity;
		}
	}*/
	for(Entity &e:g.enemies)
	{
		bool er = collisionBot(g, e);
		if (!e.isStatic)
		{
			e.sprite.y -= elapsed * g.gravity;
		}

	}
	for(int i=0;i<g.bullets.size();i++)
	{
		if (g.bullets[i].faceLeft == true) { g.bullets[i].sprite.x -= g.bullets[i].velocity*elapsed; }
		else{ g.bullets[i].sprite.x += g.bullets[i].velocity*elapsed; }
		for (int j = 0; j < g.enemies.size(); j++)
		{
			if(g.enemies[j].type!="GATE"){
			SheetSprite b = g.bullets[i].sprite;
			SheetSprite e = g.enemies[j].sprite;
			float dx = abs(b.x - e.x) - ((b.width + e.width) / 2);
			float dy = abs(b.y - e.y) - ((b.height + e.height) / 2);
			if (dy <= 0 & dx <= 0)
			{
				if (g.enemies[j].health == 1.0) { Mix_PlayChannel(-1, g.hitSound, 0); }
				g.enemies[j].health = 0.0;
				Entity bullet;
				g.bullets[i] = bullet;
			}
		}
		}
	}
	for (int i = 0; i < g.enemies.size(); i++)
	{
		if (g.enemies[i].health == 0.0)
		{
			Entity NewEntity;
			g.enemies[i] = NewEntity;
		}
		SheetSprite b = g.ship.sprite;
		SheetSprite e = g.enemies[i].sprite;
		float dx = abs(b.x - e.x) - ((b.width + e.width) / 2);
		float dy = abs(b.y - e.y) - ((b.height + e.height) / 2);
		if (dy <= 0 & dx <= 0)
		{
			if (g.enemies[i].type != "GATE") { g.quit = true; }
			else
			{
				g.done = true;

			}
		}
	}

}
