#include "space.h"
void Setup(Game &g)
{
	g.lastFrameTicks = 0.0f;

	g.speed = 100.0f;
	float x = 0.0f;
	float y = -0.85f;
	
	g.start = false;
	g.done = false;
	float gg = 1024.0f;
	GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
	g.ship.sprite = SheetSprite(spriteSheetTexture, 0.0f / gg, 941.0f / gg, 112.0f / gg, 75.0f / gg, 0.15);
	g.ship.sprite.x = x;
	g.ship.sprite.y = y;
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
	for (int i = 0; i < 30; i++)
	{
		g.entities[i].sprite.x+=elapsed*g.entities[i].velocity;
	}
	if(g.entities[5].sprite.x+g.entities[5].sprite.width/2 >1.777f || g.entities[0].sprite.x - g.entities[0].sprite.width / 2<-1.777f)
	{
		for(int i=0;i<30;i++)
		{
			g.entities[i].velocity = -g.entities[i].velocity;
		}
	}

}
