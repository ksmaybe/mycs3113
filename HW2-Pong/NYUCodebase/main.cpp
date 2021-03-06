#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iterator>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
GLuint LoadTexture(const char *filepath)
{
	int w, h, comp;
	unsigned char* image = stbi_load(filepath, &w, &h, &comp, STBI_rgb_alpha);
	if(image==NULL)
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
class Paddle
{
public:
	void Draw(ShaderProgram &p);
	float x=0.0f;
	float y=0.0f;
	float rotation;

	int textureID;

	float width=0.05f;
	float height=0.5f;

	float velocity;
	float direction_x;
	float direction_y;
};
void Paddle::Draw(ShaderProgram &p)
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


class Ball
{
public:
	void Draw(ShaderProgram &p);
	void reset();
	float x=0.0f;
	float y=0.0f;
	float rotation;

	int textureID;

	float width=0.05f;
	float height=0.05f;

	float velocity=8.0f;
	float direction_x;
	float direction_y;
	void move(float elapsed);
};
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
	double angle = (rand() % 100)*45/100*3.14159265/180;
	if (rightt % 2 == 1) { direction_x = cos(angle); }
	else { direction_x = -1 * cos(angle); }
	if (upp % 2 == 1) {direction_y = sin(angle); }
	else { direction_y = -1 * sin(angle); }
}
void Ball::move(float elapsed)
{
	x += elapsed *direction_x*velocity;
	y += elapsed *direction_y*velocity;
}

void Setup()
{
	
}
void detect_collision(Ball &b,Paddle p)
{
	float dx = abs(b.x - p.x) - ((b.width + p.width) / 2);
	float dy= abs(b.y - p.y) - ((b.height + p.height) / 2);
	if(dy<=0 & dx <=0)
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
void check_win(Ball &b,int &score_p1,int &score_p2)
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
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Look at this awesome PONG game that doesn't work", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 858, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();

#endif
	glViewport(0, 0, 858, 480);

	ShaderProgram program1;
	program1.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix_leftpaddle = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);


	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

	program1.SetProjectionMatrix(projectionMatrix);
	program1.SetViewMatrix(viewMatrix);
	glUseProgram(program1.programID);

    SDL_Event event;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float lastFrameTicks = 0.0f;

	float speed = 100.0f;
	Paddle left,right;
	left.x = -1.7f;
	right.x = 1.7f;
	Ball ball;
	ball.reset();
	int score_p1=0, score_p2=0;
    bool done = false;
    while (!done) {

		//time
		float ticks = (float)SDL_GetTicks() / 10000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            } else if(event.type == SDL_KEYDOWN)
            {
	            if(event.key.keysym.scancode==SDL_SCANCODE_UP)
				{
					right.y += elapsed * speed;
	            	//right paddle up
	            }
				if(event.key.keysym.scancode==SDL_SCANCODE_DOWN)
				{
					right.y -= elapsed * speed;
					//right paddle down
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_W)
				{
					left.y += elapsed * speed;
					//left paddle up			
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_S)
				{
					left.y -= elapsed * speed;
					//left paddle down
				}
            }
        }





		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		left.Draw(program1);
		right.Draw(program1);
		ball.Draw(program1);
		ball.move(elapsed);
		detect_collision(ball, right);
		detect_collision(ball, left);
		bounce_wall(ball);
		check_win(ball, score_p1, score_p2);

		SDL_GL_SwapWindow(displayWindow);




    }
    
    SDL_Quit();
    return 0;
}
