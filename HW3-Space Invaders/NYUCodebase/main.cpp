#include "space.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iterator>



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

	ShaderProgram program{};
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix_leftpaddle = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);



	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	glUseProgram(program.programID);

    SDL_Event event;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Game g;
	Setup(g);
	g.program = program;
    while (!g.done) {

		//time
		float ticks = (float)SDL_GetTicks() / 10000.0f;
		float elapsed = ticks - g.lastFrameTicks;
		g.lastFrameTicks = ticks;

		const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                g.done = true;
            } else if(event.type == SDL_KEYDOWN)
            {
	            if(event.key.keysym.scancode==SDL_SCANCODE_LEFT)
				{
					g.ship.x-= elapsed * g.speed;
	            	//battleship move ship
	            }
				if(event.key.keysym.scancode==SDL_SCANCODE_RIGHT)
				{
					g.ship.x+= elapsed * g.speed;
					//battleship move right
				}
            }
        }


		


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		g.ship.Draw(program);
		//Render(g,elapsed);
		//Runner(g);
		//Update(g);
		DrawText(program, g.fontTexture, "H", 0.5f, 0.1f);
		SDL_GL_SwapWindow(displayWindow);




    }
    
    SDL_Quit();
    return 0;
}
