#include "space.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iterator>



SDL_Window* displayWindow;


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("I really hope this platform thing works", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 858, 480, SDL_WINDOW_OPENGL);
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
	enum GameMode {
		STATE_MAIN_MENU, STATE_GAME_LEVEL
	};
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Game g;
	Setup(g);
	
	g.program = program;
	while (!g.done) {

		//time
		float ticks = (float)SDL_GetTicks() / 10000.0f;
		float elapsed = ticks - g.lastFrameTicks;
		g.lastFrameTicks = 0.0f;
		float accumulator = 0.0f;

		const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
	
		if (g.start == false) {
			DrawText(g.program, g.fontTexture, "Platformer:Press F", 0.1, 0.0);
		}
	
		
		else {
			
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//Render(g,elapsed);
			//Update(g);

		}
		SDL_GL_SwapWindow(displayWindow);




    }
    
    SDL_Quit();
    return 0;
}
