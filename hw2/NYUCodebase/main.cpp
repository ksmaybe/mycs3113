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

	float velocity=5.0f;
	float direction_x;
	float direction_y;
	void move();
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
	direction_x;
	direction_y;
}

void Setup()
{
	
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

	ShaderProgram program;
	ShaderProgram program1;
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	program1.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	GLuint emojiTexture = LoadTexture(RESOURCE_FOLDER"smile.gif");
	GLuint hahaTexture = LoadTexture(RESOURCE_FOLDER"mario.png");
	GLuint pikaTexture = LoadTexture(RESOURCE_FOLDER"pikachu-transparent-3.png");
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix_leftpaddle = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);


	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program1.SetProjectionMatrix(projectionMatrix);
	program1.SetViewMatrix(viewMatrix);
	glUseProgram(program.programID);

    SDL_Event event;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float lastFrameTicks = 0.0f;
	float distance = 0.0f;

	float speed = 30.0f;
	Paddle left,right;
	left.x = -1.7f;
	right.x = 1.7f;
	Ball ball;
	int score_p1, score_p2;

    bool done = false;
    while (!done) {
		//time
		float ticks = (float)SDL_GetTicks() / 10000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

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
					modelMatrix_leftpaddle = glm::translate(modelMatrix_leftpaddle, glm::vec3(distance, 0.0f, 1.0f));
				}
            }
        }





		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		left.Draw(program1);
		right.Draw(program1);
		ball.Draw(program1);

		




		//left paddle

		/*modelMatrix_leftpaddle = glm::mat4(1.0f);
		

		program.SetModelMatrix(modelMatrix_leftpaddle);



		//right paddle







		//untextured
		glUseProgram(program1.programID);

		glm::mat4 modelMatrix1 = glm::mat4(1.0f);
		modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(distance, 0.0f, 0.0f));
		program1.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		program1.SetModelMatrix(modelMatrix1);

		
		float vertices1[] = { 0.5f, -1.5f, 0.0f, 0.5f, -0.5f, -0.5f };
		glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
		glEnableVertexAttribArray(program1.positionAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(program1.positionAttribute);
		glDisableVertexAttribArray(program1.texCoordAttribute);

		//first textured animated
		glUseProgram(program.programID);

		glBindTexture(GL_TEXTURE_2D, emojiTexture);
		float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);


		glDrawArrays(GL_TRIANGLES, 0, 6);
		modelMatrix_leftpaddle = glm::mat4(1.0f);
		distance += elapsed;
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);


		//second 
		glBindTexture(GL_TEXTURE_2D, hahaTexture);

		modelMatrix_leftpaddle = glm::mat4(1.0f);
		modelMatrix_leftpaddle = glm::translate(modelMatrix1, glm::vec3(-1.0f, -0.5f, 0.0f));
		program.SetModelMatrix(modelMatrix_leftpaddle);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);


		glDrawArrays(GL_TRIANGLES, 0, 7);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);


		glUseProgram(program.programID);


		//third
		glBindTexture(GL_TEXTURE_2D, pikaTexture);

		modelMatrix_leftpaddle = glm::mat4(1.0f);
		modelMatrix_leftpaddle = glm::translate(modelMatrix1, glm::vec3(1.0f,0.5f, 0.0f));
		program.SetModelMatrix(modelMatrix_leftpaddle);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);


		glDrawArrays(GL_TRIANGLES, 0, 7);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);*/

		SDL_GL_SwapWindow(displayWindow);




    }
    
    SDL_Quit();
    return 0;
}
