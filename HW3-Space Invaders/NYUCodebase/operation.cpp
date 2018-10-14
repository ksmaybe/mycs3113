#include "space.h"
void setup(Game &g)
{
	g.lastFrameTicks = 0.0f;

	g.speed = 100.0f;
	Player left, right;
	g.left.x = -1.7f;
	g.right.x = 1.7f;
	
	g.ball.reset();
	int score_p1 = 0, score_p2 = 0;
	bool done = false;
}

void render()
{
	left.Draw(program);
	right.Draw(program);
	ball.Draw(program);
	ball.move(elapsed);
}