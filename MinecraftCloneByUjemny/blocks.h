#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <vector>
#include "shaders.h"

class Block
{
private:
	unsigned int vs, fs, vao, vbo[5], sh;

	int objects;
	int textureAmount;
	int currentObject;

	std::vector<float> batch_block_data;
	std::vector<float> batch_block_texture_data;
	std::vector<float> batch_color;
	std::vector<float> batch_texture_ID;

	std::array<unsigned int, 32> batch_texture;

	glm::mat4 batch_transform;

	float p[3] = { 0.0f, 0.0f, 0.0f };

public:
	void Init();
	void Add();
	void Accept();
	void Set(short ID);
	void SetID(float texID);
	void Draw(glm::mat4 proj, glm::mat4 view);
	void Pos(float x, float y, float z);
	void Col(float r, float g, float b, float a);
	void Texture(const char* path, unsigned short ID = 0);
	void Terminate();
};

void Block::Init()
{
	for (int i = 0; i < 1000; i++)
	{
		batch_block_data.push_back(0.0f);
		batch_block_data.push_back(0.0f);
		batch_block_data.push_back(0.0f);

		batch_block_texture_data.push_back(0.0f);
		batch_block_texture_data.push_back(0.0f);

		batch_color.push_back(1.0f);
		batch_color.push_back(1.0f);
		batch_color.push_back(1.0f);
		batch_color.push_back(1.0f);

		batch_texture_ID.push_back(-1.0f);
	}

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsTexture, nullptr);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsTexture, nullptr);
	glCompileShader(fs);

	sh = glCreateProgram();
	glAttachShader(sh, vs);
	glAttachShader(sh, fs);
	glLinkProgram(sh);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(5, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, batch_block_data.size() * sizeof(float), batch_block_data.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, batch_block_texture_data.size() * sizeof(float), batch_block_texture_data.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, batch_color.size() * sizeof(float), batch_color.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, batch_texture_ID.size() * sizeof(float), batch_texture_ID.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(3);

	glUseProgram(sh);

	glUniform1iv(glGetUniformLocation(sh, "textures"), 32, samplers);

	glUseProgram(0);
	glBindVertexArray(0);
}

void Block::Add()
{
	for (int i = 0; i < 18 * 6; i++)
	{
		batch_block_data.push_back(block_data[i]);
		batch_texture_ID[objects + i / 6] = objects / 16;
	}
	
	for (int i = 0; i < 12 * 6; i++)
	{
		batch_block_texture_data[objects + i] = block_texture_data[i];
	}

	objects++;
	currentObject = objects;
}

void Block::Accept()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, batch_block_data.size() * sizeof(float), batch_block_data.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, batch_block_texture_data.size() * sizeof(float), batch_block_texture_data.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, batch_color.size() * sizeof(float), batch_color.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, batch_texture_ID.size() * sizeof(float), batch_texture_ID.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void Block::Set(short ID)
{
	currentObject = ID;
}

void Block::SetID(float texID)
{
	batch_texture_ID[currentObject] = texID;
}

void Block::Draw(glm::mat4 proj, glm::mat4 view)
{
	glBindVertexArray(vao);
	glUseProgram(sh);

	for (int i = 0; i < textureAmount && textureAmount < 32; i++)
		glBindTextureUnit(i, batch_texture[i]);

	glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(sh, "transform"), 1, GL_FALSE,glm::value_ptr(batch_transform));

	glDrawArrays(GL_TRIANGLES, 0, batch_block_data.size() / 2);

	glUseProgram(0);
	glBindVertexArray(0);
}

void Block::Pos(float x, float y, float z)
{
	for()
}

void Block::Texture(const char* path, unsigned short ID)
{
	int w, h, bits;

	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path, &w, &h, &bits, STBI_rgb_alpha);

	unsigned int textureID;

	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(pixels);

	batch_texture[textureAmount] = textureID;

	textureAmount++;
}

void Block::Terminate()
{
	glDeleteProgram(sh);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(5, vbo);
}