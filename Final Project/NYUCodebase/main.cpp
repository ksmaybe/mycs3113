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
	enum GameMode {
		STATE_MAIN_MENU, STATE_GAME_LEVEL
	};
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Game g;
	Setup(g,"zz1.txt");
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	g.program = program;

	g.shootSound = Mix_LoadWAV("Shoot.wav");
	g.hitSound = Mix_LoadWAV("Hit1.wav");
	Mix_Music *music;
	music = Mix_LoadMUS("Adventure.mp3");

	Mix_PlayMusic(music, -1);


	while (!g.done) {
		//time
		float ticks = (float)SDL_GetTicks() / 10000.0f;
		float elapsed = ticks - g.lastFrameTicks;
		g.lastFrameTicks = ticks;

		const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
		if(keyboard[SDL_SCANCODE_LEFT])
		{
			g.ship.sprite.x -= elapsed * g.speed;
			g.ship.faceLeft = true;
			//battleship move left
			
		}
		if(keyboard[SDL_SCANCODE_RIGHT])
		{
			g.ship.sprite.x += elapsed * g.speed;
			g.ship.faceLeft = false;
			//battleship move right
		}
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				g.done = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_F)
				{
					g.start = true;
					//change game mode, start game level
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
				{
					shootBullet(g);
					Mix_PlayChannel(-1,g.shootSound,0);
					//battleship fire
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_A)
				{
					g.ship.sprite.y += elapsed * g.speed*4;
					//battleship jump
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_Q)
				{
					g.quit = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_E)
				{
					if (g.quit) { SDL_Quit(); }
				}
			}
		}
		if (g.start == false) {
			
			DrawText(g.program, g.fontTexture, "Final Fantasy:Press F", 0.1, 0.0);
		}

		
		else if (g.start & g.quit != true) {
			viewMatrix = glm::mat4(1.0f);
			viewMatrix = glm::translate(viewMatrix, glm::vec3(-g.ship.sprite.x, -g.ship.sprite.y, 0.0f));
			g.program.SetViewMatrix(viewMatrix);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			bool k = collisionBot(g, g.ship);
			Render(g, elapsed);
			g.enemyKilled = false;
			Update(g);
			drawMap(g);
			for (int i = 0; i < g.enemies.size(); i++)
			{
				g.enemies[i].sprite.Draw(g.program);
			}
		}
		else if(g.quit==true)
		{
			viewMatrix = glm::mat4(1.0f);
			g.program.SetViewMatrix(viewMatrix);
			DrawText(g.program, g.fontTexture, "         GAME OVER", 0.1, 0.0);
		}
		if (g.next == 2) { g.done = true; }

		SDL_GL_SwapWindow(displayWindow);
    }
	Mix_HaltMusic();
	Mix_HaltChannel(-1);
	SDL_Quit();
	g.done = false;
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("I really hope this space thing works", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 858, 480, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();

#endif
	glViewport(0, 0, 858, 480);
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	projectionMatrix = glm::mat4(1.0f);
	modelMatrix_leftpaddle = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);



	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	glUseProgram(program.programID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Game x;
	g = x;
	g.map.entities.clear();
	g.enemies.clear();
	g.bullets.clear();
	g.entities.clear();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	g.program = program;

	g.shootSound = Mix_LoadWAV("Shoot.wav");
	g.hitSound = Mix_LoadWAV("Hit1.wav");
	music = Mix_LoadMUS("Adventure.mp3");

	Mix_PlayMusic(music, -1);


	g.program = program;









	//map2
	Setup(g, "zz2.txt");
	while (!g.done) {
		//time
		float ticks = (float)SDL_GetTicks() / 10000.0f;
		float elapsed = ticks - g.lastFrameTicks;
		g.lastFrameTicks = ticks;

		const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
		if (keyboard[SDL_SCANCODE_LEFT])
		{
			g.ship.sprite.x -= elapsed * g.speed;
			g.ship.faceLeft = true;
			//battleship move left

		}
		if (keyboard[SDL_SCANCODE_RIGHT])
		{
			g.ship.sprite.x += elapsed * g.speed;
			g.ship.faceLeft = false;
			//battleship move right
		}
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				g.done = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_F)
				{
					g.start = true;
					//change game mode, start game level
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
				{
					shootBullet(g);
					Mix_PlayChannel(-1, g.shootSound, 0);
					//battleship fire
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_A)
				{
					g.ship.sprite.y += elapsed * g.speed * 4;
					//battleship jump
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_Q)
				{
					g.quit = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_E)
				{
					if (g.quit) { SDL_Quit(); }
				}
			}
		}
		if (g.start == false) {

			DrawText(g.program, g.fontTexture, "This is Part 2:Press F", 0.1, 0.0);
		}


		else if (g.start & g.quit != true) {
			viewMatrix = glm::mat4(1.0f);
			viewMatrix = glm::translate(viewMatrix, glm::vec3(-g.ship.sprite.x, -g.ship.sprite.y, 0.0f));
			g.program.SetViewMatrix(viewMatrix);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			bool k = collisionBot(g, g.ship);
			Render(g, elapsed);
			g.enemyKilled = false;
			Update(g);
			drawMap(g);
			for (int i = 0; i < g.enemies.size(); i++)
			{
				g.enemies[i].sprite.Draw(g.program);
			}
		}
		else if (g.quit == true)
		{
			viewMatrix = glm::mat4(1.0f);
			g.program.SetViewMatrix(viewMatrix);
			DrawText(g.program, g.fontTexture, "         GAME OVER", 0.1, 0.0);
		}


		SDL_GL_SwapWindow(displayWindow);
	}
    SDL_Quit();
    return 0;
}
