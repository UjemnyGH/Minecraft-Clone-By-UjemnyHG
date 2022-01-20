#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <array>
#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders.h"
#include "perlinNoise.h"

const double pixelInAtlas512x512 = 1.0 / 512.0;
const double textureSizeInAtlas512x512 = 1.0 / 512.0 * 16.0;
const float grassUpX = 0.0f;
const float grassUpY = 0.0f;

const float grassRightX = 1.0f;
const float grassRightY = 0.0f;

const float dirtX = 2.0f;
const float dirtY = 0.0f;

class Chunk
{
private:
	int chunk_blocks_ID[32][64][32];

	unsigned int sh, vao, vbo[3], tex;

	std::vector<float> vertices;
	std::vector<float> textureCoords;
	std::vector<float> color;

	glm::mat4 transform = glm::mat4(1.0);

	Perlin1D perlin = Perlin1D(2.0f, 2);

public:
	void Init();
	void Generate();
	void Applay();
	void Pos(float x, float y, float z);
	void TextureAtlas(const char* atlatPath);
	void Draw(glm::mat4 proj, glm::mat4 view);
	void Terminate();
};

void Chunk::Init()
{
	sh = glCreateProgram();
	glAttachShader(sh, ShaderFromCode(vsAtlas, GL_VERTEX_SHADER));
	glAttachShader(sh, ShaderFromCode(fsAtlas, GL_FRAGMENT_SHADER));
	glLinkProgram(sh);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(3, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float), textureCoords.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), color.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	glGenTextures(1, &tex);
	
	glUseProgram(sh);

	glUniform1i(glGetUniformLocation(sh, "textures"), 0);

	glUseProgram(0);

	glBindVertexArray(0);
}

void Chunk::Generate()
{
	std::random_device rd;

	for (int x = 0; x < 32; x++)
	{
		for (int z = 0; z < 32; z++)
		{
			int y = rd() % 64;

			//chunk_blocks_ID[x][y + 1][z] = 1;

			for (int yl = y; yl > 0; yl--)
			{
				chunk_blocks_ID[x][yl][z] = 1;
			}
		}
	}

	for (int x = 0; x < 32; x++)
	{
		for (int y = 1; y < 64; y++)
		{
			for (int z = 0; z < 32; z++)
			{
				int yr = rd() % 64;

				int perlinY = (int)perlin.get_noise(yr);

				if (chunk_blocks_ID[x][y][z] == 1 && chunk_blocks_ID[x][y - 1][z] == 0 || y == 0)
				{
					//TOP


					vertices.push_back(up_face[0] + x);
					vertices.push_back(up_face[1] + perlinY);
					vertices.push_back(up_face[2] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * (grassUpX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512 * grassUpY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
												  
					vertices.push_back(up_face[3] + x);
					vertices.push_back(up_face[4] + perlinY);
					vertices.push_back(up_face[5] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * grassUpX);
					textureCoords.push_back(textureSizeInAtlas512x512 * grassUpY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
												  
					vertices.push_back(up_face[6] + x);
					vertices.push_back(up_face[7] + perlinY);
					vertices.push_back(up_face[8] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * (grassUpX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512 * (grassUpY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
												  
					vertices.push_back(up_face[9] + x);
					vertices.push_back(up_face[10] + perlinY);
					vertices.push_back(up_face[11] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * grassUpX);
					textureCoords.push_back(textureSizeInAtlas512x512 * grassUpY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
												  
					vertices.push_back(up_face[12] + x);
					vertices.push_back(up_face[13] + perlinY);
					vertices.push_back(up_face[14] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * (grassUpX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512 * (grassUpY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
												  
					vertices.push_back(up_face[15] + x);
					vertices.push_back(up_face[16] + perlinY);
					vertices.push_back(up_face[17] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * grassUpX);
					textureCoords.push_back(textureSizeInAtlas512x512 * (grassUpY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);


					//FRONT


					vertices.push_back(front_face[0] + x);
					vertices.push_back(front_face[1] + perlinY);
					vertices.push_back(front_face[2] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512 * grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[3] + x);
					vertices.push_back(front_face[4] + perlinY);
					vertices.push_back(front_face[5] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512 * grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[6] + x);
					vertices.push_back(front_face[7] + perlinY);
					vertices.push_back(front_face[8] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512 * (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[9] + x);
					vertices.push_back(front_face[10] + perlinY);
					vertices.push_back(front_face[11] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512 * grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[12] + x);
					vertices.push_back(front_face[13] + perlinY);
					vertices.push_back(front_face[14] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512 * (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[15] + x);
					vertices.push_back(front_face[16] + perlinY);
					vertices.push_back(front_face[17] + z);

					textureCoords.push_back(textureSizeInAtlas512x512 * grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512 * (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);


					//BACK


					vertices.push_back(front_face[0] + x);
					vertices.push_back(front_face[1] + perlinY);
					vertices.push_back(front_face[2] + z - 1);

					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512* grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[3] + x);
					vertices.push_back(front_face[4] + perlinY);
					vertices.push_back(front_face[5] + z - 1);

					textureCoords.push_back(textureSizeInAtlas512x512* grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512* grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[6] + x);
					vertices.push_back(front_face[7] + perlinY);
					vertices.push_back(front_face[8] + z - 1);

					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[9] + x);
					vertices.push_back(front_face[10] + perlinY);
					vertices.push_back(front_face[11] + z - 1);

					textureCoords.push_back(textureSizeInAtlas512x512* grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512* grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[12] + x);
					vertices.push_back(front_face[13] + perlinY);
					vertices.push_back(front_face[14] + z - 1);

					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(front_face[15] + x);
					vertices.push_back(front_face[16] + perlinY);
					vertices.push_back(front_face[17] + z - 1);

					textureCoords.push_back(textureSizeInAtlas512x512* grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);


					//RIGHT


					vertices.push_back(right_face[0] + x);
					vertices.push_back(right_face[1] + perlinY);
					vertices.push_back(right_face[2] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512* grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[3] + x);
					vertices.push_back(right_face[4] + perlinY);
					vertices.push_back(right_face[5] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512* grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[6] + x);
					vertices.push_back(right_face[7] + perlinY);
					vertices.push_back(right_face[8] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[9] + x);
					vertices.push_back(right_face[10] + perlinY);
					vertices.push_back(right_face[11] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512* grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[12] + x);
					vertices.push_back(right_face[13] + perlinY);
					vertices.push_back(right_face[14] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[15] + x);
					vertices.push_back(right_face[16] + perlinY);
					vertices.push_back(right_face[17] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);


					//LEFT


					vertices.push_back(right_face[0] + x - 1);
					vertices.push_back(right_face[1] + perlinY);
					vertices.push_back(right_face[2] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512* grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[3] + x - 1);
					vertices.push_back(right_face[4] + perlinY);
					vertices.push_back(right_face[5] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512* grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[6] + x - 1);
					vertices.push_back(right_face[7] + perlinY);
					vertices.push_back(right_face[8] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[9] + x - 1);
					vertices.push_back(right_face[10] + perlinY);
					vertices.push_back(right_face[11] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512* grassRightY);

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[12] + x - 1);
					vertices.push_back(right_face[13] + perlinY);
					vertices.push_back(right_face[14] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightX + 1.0f));
					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);

					vertices.push_back(right_face[15] + x - 1);
					vertices.push_back(right_face[16] + perlinY);
					vertices.push_back(right_face[17] + z);

					textureCoords.push_back(textureSizeInAtlas512x512* grassRightX);
					textureCoords.push_back(textureSizeInAtlas512x512* (grassRightY - 1.0f));

					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
					color.push_back(1.0f);
				}
			}
		}
	}
}

void Chunk::Applay()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float), textureCoords.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), color.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	glUseProgram(sh);

	glUniform1i(glGetUniformLocation(sh, "textures"), 0);

	glUseProgram(0);

	glBindVertexArray(0);
}

void Chunk::Pos(float x, float y, float z)
{
	transform = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
}

void Chunk::TextureAtlas(const char* atlasPath)
{
	glBindVertexArray(vao);

	int w, h, bits;

	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(atlasPath, &w, &h, &bits, STBI_rgb_alpha);

	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(pixels);

	glBindVertexArray(0);
}

void Chunk::Draw(glm::mat4 proj, glm::mat4 view)
{
	glBindVertexArray(vao);
	glUseProgram(sh);

	glBindTexture(GL_TEXTURE_2D, tex);

	glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(sh, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glUseProgram(0);
	glBindVertexArray(0);
}

void Chunk::Terminate()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(3, vbo);
	glDeleteTextures(1, &tex);
	glDeleteProgram(sh);
}

/*class Chunk
{
private:
	unsigned int vs, fs, vao, vbo[4], sh;

	std::vector<float> chunk_vertices;
	std::vector<float> chunk_textures;
	std::vector<float> chunk_color;
	std::vector<float> chunk_color_ID;
	std::vector<float> chunk_texture_ID;

	glm::mat4 transform;

	float p[3] = { 0.0f, 0.0f, 0.0f };

public:
	void Init();
	void Generate();
	void Pos(float x, float y, float z);
	void Draw(glm::mat4 proj, glm::mat4 view);
};*/