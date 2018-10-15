#include "space.h"
void Setup(Game &g)
{
	g.lastFrameTicks = 0.0f;

	g.speed = 100.0f;
	Player left, right;
	g.left.x = -1.7f;
	g.right.x = 1.7f;
	
	g.ball.reset();
	g.score_p1=0,g.score_p2 = 0;
	g.done = false;
}

void Render(Game &g,ShaderProgram program, float elapsed)
{
	g.left.Draw(program);
	g.right.Draw(program);
	g.ball.Draw(program);
	g.ball.move(elapsed);
}
void Runner(Game &g)
{
	detect_collision(g.ball, g.right);
	detect_collision(g.ball, g.left);
	bounce_wall(g.ball);
	check_win(g.ball, g.score_p1, g.score_p2);
}