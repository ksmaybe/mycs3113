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
#include "SatCollision.h"

float hw = 0.3f;

class Object
{
public:
	Object(float x, float y, glm::mat4 model, float angle);
	float x;
	float y;
	glm::mat4 model;
	float angle;
	float height = 0.3f;
	float width = 0.3f;
	void Draw(ShaderProgram &p);
};
Object::Object(float x, float y, glm::mat4 model, float angle)
{
	this->x = x;
	this->y = y;
	this->model = model;
	this->angle = angle;
}
void Object::Draw(ShaderProgram& p)
{
	float vertices[] = { -width / 2,  -height / 2,width / 2,  -height / 2,width / 2,  height / 2,-width / 2,  -height / 2,width / 2,  height / 2,-width / 2,  height / 2 };
	glm::vec3 dir = glm::vec3(this->x, this->y, 0.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, dir);
	model = glm::rotate(model, this->angle,glm::vec3(0.0, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
	p.SetModelMatrix(model);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(p.positionAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

std::vector<std::pair<float, float>> get_point(Object &o)
{
	//bot l
	glm::vec4 x1 = glm::vec4(-hw / 2, -hw / 2, 0.0f, 1.0f);
	glm::vec4 bl = o.model*x1;
	std::pair<float, float> obl = { bl.x,bl.y };

	//bot r
	glm::vec4 x2 = glm::vec4(hw / 2, -hw / 2, 0.0f, 1.0f);
	glm::vec4 br = o.model*x2;
	std::pair<float, float> obr = { br.x,br.y };
	
	//top r
	glm::vec4 x3 = glm::vec4(hw / 2, hw / 2, 0.0f, 1.0f);
	glm::vec4 tr = o.model*x3;
	std::pair<float, float> otr = { tr.x,tr.y };
	
	//top l
	glm::vec4 x4 = glm::vec4(-hw / 2, hw / 2, 0.0f, 1.0f);
	glm::vec4 tl = o.model*x4;
	std::pair<float, float> otl = { tl.x,tl.y };

	std::vector<std::pair<float, float>> pp = { obl,obr,otr,otl};

	return pp;
}


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
	ShaderProgram program2;

	program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	program1.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	program2.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	/*GLuint emojiTexture = LoadTexture(RESOURCE_FOLDER"smile.gif");
	GLuint hahaTexture = LoadTexture(RESOURCE_FOLDER"mario.png");
	GLuint pikaTexture = LoadTexture(RESOURCE_FOLDER"pikachu-transparent-3.png");*/
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);


	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
	glUseProgram(program.programID);

	glm::mat4 modelMatrix1 = glm::mat4(1.0f);
	glm::mat4 modelMatrix2 = glm::mat4(1.0f);
	
	program.SetColor(0.4f, 0.9f, 0.4f, 1.0f);
	program1.SetColor(0.3f, 0.3f, 0.3f, 1.0f);
	program2.SetColor(0.6f, 0.6f, 0.6f, 1.0f);

	program.SetModelMatrix(modelMatrix);
	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program1.SetProjectionMatrix(projectionMatrix);
	program1.SetViewMatrix(viewMatrix);
	program2.SetProjectionMatrix(projectionMatrix);
	program2.SetViewMatrix(viewMatrix);

    SDL_Event event;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float lastFrameTicks = 0.0f;
	float distance = 0.0f;

	Object oo = Object(0.0f, 0.0f, modelMatrix, 32.0f*3.13159/180);
	Object o1 = Object(1.1f, 0.4f, modelMatrix, 0);
	Object o2 = Object(-1.1f, -0.4f, modelMatrix, 64* 3.13159 / 180);




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


		o1.angle += elapsed;
		o2. angle -= elapsed;
		o1.x -= elapsed;
		o2.x += elapsed;



		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		//modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(1.1f, 0.4f, 0.0f));
		//modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(-1.1f, -0.4f, 0.0f));
		oo.Draw(program);
		o1.Draw(program1);
		o2.Draw(program2);

		std::vector<std::pair<float, float>> p_oo = get_point(oo);
		std::vector<std::pair<float, float>> p_o1 = get_point(o1);
		std::vector<std::pair<float, float>> p_o2 = get_point(o2);

		std::pair<float, float> pen;
		
		bool collision = CheckSATCollision(p_oo, p_o1, pen);


		if(collision)
		{
			oo.x += pen.first*0.3f;
			oo.y += pen.second*0.3f;

			o1.x -= pen.first*0.3f;
			o1.y -= pen.second*0.3f;
		}

		std::pair<float, float> pen1;
		bool collision1 = CheckSATCollision(p_o1, p_o2, pen1);


		if(collision1)
		{
			o1.x += pen1.first*0.3f;
			o1.y += pen1.second*0.3f;

			o2.x -= pen1.first*0.3f;
			o2.y -= pen1.second*0.3f;
		}

		std::pair<float, float> pen2;
		bool collision2 = CheckSATCollision(p_oo, p_o2, pen2);
		if (collision2)
		{
			oo.x += pen2.first*0.3f;
			oo.y += pen2.second*0.3f;

			o2.x -= pen2.first*0.3f;
			o2.y -= pen2.second*0.3f;
		}

		/*
		//0
		glUseProgram(program.programID);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		program1.SetColor(0.4f, 0.9f, 0.4f, 1.0f);
		program1.SetModelMatrix(modelMatrix);

		float vertices[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//1
		glUseProgram(program1.programID);

		modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(0.0f, 0.0f, 0.0f));
		program1.SetColor(0.3f, 0.3f, 0.3f, 1.0f);
		program1.SetModelMatrix(modelMatrix1);

		float vertices1[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
		glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
		glEnableVertexAttribArray(program1.positionAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(program1.positionAttribute);
		glDisableVertexAttribArray(program1.texCoordAttribute);

		//3
		glUseProgram(program2.programID);

		modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0.0f, 0.0f, 0.0f));
		program1.SetColor(0.6f, 0.6f, 0.6f, 1.0f);
		program1.SetModelMatrix(modelMatrix2);

		float vertices2[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
		glVertexAttribPointer(program2.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
		glEnableVertexAttribArray(program2.positionAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(program2.positionAttribute);
		glDisableVertexAttribArray(program2.texCoordAttribute);
		*/
		SDL_GL_SwapWindow(displayWindow);




    }
    
    SDL_Quit();
    return 0;
}
