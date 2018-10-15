#include "space.h"


void Player::Draw(ShaderProgram &p)
{
	glUseProgram(p.programID);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	float vertices1[] = { x + width / 2,y - height / 2,
		x + width / 2,y + height / 2,
		x - width / 2,y + height / 2 };
	p.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
	glEnableVertexAttribArray(p.positionAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 4);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}



void Ball::Draw(ShaderProgram &p)
{
	glUseProgram(p.programID);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	float vertices1[] = { x + width / 2,y - height / 2,
		x + width / 2,y + height / 2,
		x - width / 2,y + height / 2 };
	float vertices2[] = {
	x - width / 2,y + height / 2,
	x - width / 2,y - height / 2,
	x + width / 2,y - height / 2 };
	p.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
	glEnableVertexAttribArray(p.positionAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 4);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
	glDrawArrays(GL_TRIANGLES, 0, 4);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}
void Ball::reset()
{
	x = 0.0f;
	y = 0.0f;
	int rightt = (rand() % 100) % 2;
	int upp = (rand() % 100) % 2;
	float angle = (rand() % 100) * 45 / 100 * 3.14159265 / 180;
	if (rightt % 2 == 1) { direction_x = cos(angle); }
	else { direction_x = -1 * cos(angle); }
	if (upp % 2 == 1) { direction_y = sin(angle); }
	else { direction_y = -1 * sin(angle); }
}
void Ball::move(float elapsed)
{
	x += elapsed * direction_x*velocity;
	y += elapsed * direction_y*velocity;
}

void detect_collision(Ball &b, Player p)
{
	float dx = abs(b.x - p.x) - ((b.width + p.width) / 2);
	float dy = abs(b.y - p.y) - ((b.height + p.height) / 2);
	if ((dy <= 0) & (dx <= 0))
	{
		b.direction_x = -1 * b.direction_x;
	}
}
void bounce_wall(Ball &b)
{
	if (b.y >= 1.0f)
	{
		b.direction_y = -1 * b.direction_y;
	}
	if (b.y <= -1.0f)
	{
		b.direction_y = -1 * b.direction_y;
	}
}
void check_win(Ball &b, int &score_p1, int &score_p2)
{
	if (b.x >= 1.777f)
	{
		score_p1 += 1;
		b.reset();
	}
	if (b.x <= -1.777f)
	{
		score_p2 += 1;
		b.reset();
	}

}

GLuint LoadTexture(const char *filepath)
{
	int w, h, comp;
	unsigned char* image = stbi_load(filepath, &w, &h, &comp, STBI_rgb_alpha);
	if (image == NULL)
	{
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
	return retTexture;
}
