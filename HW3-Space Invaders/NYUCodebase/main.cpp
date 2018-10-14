#include "space.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iterator>
#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


SDL_Window* displayWindow;


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("I really hope this space thing works", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 858, 480, SDL_WINDOW_OPENGL);
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
	Player left,right;
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
