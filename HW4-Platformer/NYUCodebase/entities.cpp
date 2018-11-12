#include "space.h"







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

SheetSprite::SheetSprite()
{
	this->textureID = 0;
	this->u = 0;
	this->v = 0;
	this->width = 0;
	this->height = 0;
	this->size = 0;
}


SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size)
{
	this->textureID = textureID;
	this->u = u;
	this->v = v;
	this->width = width;
	this->height = height;
	this->size = size;

}
void SheetSprite::Draw(ShaderProgram &p)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix,glm::vec3(x,y,0.0));
	p.SetModelMatrix(modelMatrix);
	GLfloat texCoords[] = { u,v + height,
		u + width,v,
		u,v,
	u + width,v,
	u,v + height,
	u + width,v + height };
	float aspect = width / height;
	float vertices[] = { -0.5f*size*aspect,-0.5f*size,
	0.5f*size*aspect,0.5f*size,
	-0.5f*size*aspect,0.5f*size,
	0.5f*size*aspect,0.5f*size,
	-0.5f * size * aspect, -0.5f * size ,
	0.5f * size * aspect, -0.5f * size };
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(p.positionAttribute);
	glVertexAttribPointer(p.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(p.texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}
void Entity::Draw(ShaderProgram &p)
{
	this->sprite.Draw(p);
};

void Update(Game &g){
	glClear(GL_COLOR_BUFFER_BIT);
	g.ship.sprite.Draw(g.program);
	for(int i=0;i<g.entities.size();i++)
	{
		
		g.entities[i].sprite.Draw(g.program);
	}
	for (int i = 0; i < g.bullets.size(); i++)
	{
		g.bullets[i].sprite.Draw(g.program);
	}
}

void DrawText(ShaderProgram &p, int fontTexture, std::string text, float size, float spacing)
{
	float character_size = 1.0 / 16.0f;
	std::vector<float>vertexData;
	std::vector<float>texCoordData;
	for (int i = 0; i < text.size(); i++)
	{
		int SpriteIndex = (int)text[i];
		float texture_x = (float)(SpriteIndex % 16) / 16.0f;
		float texture_y = (float)(SpriteIndex / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
		((size + spacing) * i) + (-0.5f * size), 0.5f * size,
		((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		((size + spacing) * i) + (0.5f * size), 0.5f * size,
		((size + spacing) * i) + (0.5f * size), -0.5f * size,
		((size + spacing) * i) + (0.5f * size), 0.5f * size,
		((size + spacing) * i) + (-0.5f * size), -0.5f * size });
		texCoordData.insert(texCoordData.end(), {
		texture_x, texture_y,
		texture_x, texture_y + character_size,
		texture_x + character_size, texture_y,
		texture_x + character_size, texture_y + character_size,
		texture_x + character_size, texture_y,
		texture_x, texture_y + character_size });

	}
	float* x1 = &vertexData[1];
	float* x2 = &texCoordData[2];
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix=translate(modelMatrix, glm::vec3( -1.3, 0.0, 0.0));
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(p.positionAttribute);
	glVertexAttribPointer(p.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(p.texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);

	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);

	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);

}

void shootBullet(Game &g)
{
	Entity newBullet;
	float gg = 1024.0f;
	GLuint spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
	newBullet.sprite = SheetSprite(spriteSheetTexture, 856.0f / gg, 57.0f / gg, 9.0f / gg, 37.0f / gg, 0.1);
	newBullet.sprite.x = g.ship.sprite.x;
	newBullet.sprite.y = g.ship.sprite.y;
	newBullet.velocity = 4.0f;
	newBullet.timeAlive = 0.0f;
	g.bullets.push_back(newBullet);

};
bool shouldRemoveBullet(Entity bullet)
{
	if(bullet.timeAlive>=2.0f)
	{return(true);}
	return(false);
}
