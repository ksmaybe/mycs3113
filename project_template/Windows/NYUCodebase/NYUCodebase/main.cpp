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



int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Look the huge smiley moves", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 858, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();

#endif
	glViewport(0, 0, 858, 360);

	ShaderProgram program;
	ShaderProgram program1;
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	program1.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	GLuint emojiTexture = LoadTexture(RESOURCE_FOLDER"smile.gif");
	GLuint hahaTexture = LoadTexture(RESOURCE_FOLDER"mario.png");
	GLuint pikaTexture = LoadTexture(RESOURCE_FOLDER"pikachu-transparent-3.png");
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);


	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
	glUseProgram(program.programID);

    SDL_Event event;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float lastFrameTicks = 0.0f;
	float distance = 0.0f;




    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
		glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


		float ticks = (float)SDL_GetTicks() / 10000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;


		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(distance, 0.0f, 1.0f));

		program.SetModelMatrix(modelMatrix);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);
		program1.SetProjectionMatrix(projectionMatrix);
		program1.SetViewMatrix(viewMatrix);

		glUseProgram(program1.programID);

		glm::mat4 modelMatrix1 = glm::mat4(1.0f);
		modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(0.0f, 0.0f, 0.0f));
		program1.SetColor(0.4f, 0.9f, 0.4f, 1.0f);
		program1.SetModelMatrix(modelMatrix1);

		float vertices1[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
		glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
		glEnableVertexAttribArray(program1.positionAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(program1.positionAttribute);
		glDisableVertexAttribArray(program1.texCoordAttribute);

		//first
		glUseProgram(program.programID);

		glBindTexture(GL_TEXTURE_2D, emojiTexture);
		float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);


		glDrawArrays(GL_TRIANGLES, 0, 6);
		modelMatrix = glm::mat4(1.0f);
		distance += elapsed;
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);


		//second 
		glBindTexture(GL_TEXTURE_2D, hahaTexture);

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix1, glm::vec3(-1.0f, 0.0f, 0.0f));
		program.SetModelMatrix(modelMatrix);
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

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix1, glm::vec3(1.0f, 0.0f, 0.0f));
		program.SetModelMatrix(modelMatrix);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);


		glDrawArrays(GL_TRIANGLES, 0, 7);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		SDL_GL_SwapWindow(displayWindow);




    }
    
    SDL_Quit();
    return 0;
}
