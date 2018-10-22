#include "space.h"
void Setup(Game &g)
{
	g.lastFrameTicks = 0.0f;

	g.speed = 100.0f;
	g.ship.x = 0.0f;
	g.ship.y = -0.85f;
	
	g.ball.reset();
	g.score_p1 = 0;
	g.done = false;
	float gg = 1024.0f;
	GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
	for(int i=0;i<30;i++)
	{
		Entity myEntity;
		myEntity.sprite = SheetSprite(spriteSheetTexture, 425.0f / gg, 468.0f / gg, 93.0f / gg, 84.0f / gg, 0.2);
		g.entities.push_back(myEntity);
	}


}

void Render(Game &g,ShaderProgram program, float elapsed)
{
	//g.ship.Draw(program);
	//g.ball.Draw(program);
	//g.ball.move(elapsed);
	//for (int i = 0; i < 30; i++)
	//{
		//g.entities[i].Draw();
	//}
}
void Runner(Game &g)
{
	detect_collision(g.ball, g.ship);
	bounce_wall(g.ball);
	check_win(g.ball, g.score_p1);
}