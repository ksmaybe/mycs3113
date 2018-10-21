#include "space.h"
void Setup(Game &g)
{
	g.lastFrameTicks = 0.0f;

	g.speed = 100.0f;
	g.ship.x = 0.0f;
	
	g.ball.reset();
	g.score_p1 = 0;
	g.done = false;
}

void Render(Game &g,ShaderProgram program, float elapsed)
{
	g.ship.Draw(program);
	g.ball.Draw(program);
	g.ball.move(elapsed);
}
void Runner(Game &g)
{
	detect_collision(g.ball, g.ship);
	bounce_wall(g.ball);
	check_win(g.ball, g.score_p1);
}