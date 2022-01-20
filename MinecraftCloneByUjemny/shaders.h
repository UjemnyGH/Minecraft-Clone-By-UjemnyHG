#pragma once
#include <array>
#include <stb_image/stb_image.h>

const char* vsTexture =
"#version 450 core\n"
"uniform mat4 transform;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"layout(location = 0)in vec4 iPos;\n"
"layout(location = 1)in vec2 iTex;\n"
"layout(location = 2)in vec4 iCol;\n"
"layout(location = 3)in float iTexIndex;\n"
"out vec4 ioCol;\n"
"out float ioTexIndex;\n"
"out vec2 ioTex;\n"
"void main()\n"
"{\n"
"gl_Position = (projection * view * transform[tIndex]) * iPos;\n"
"ioCol = iCol;\n"
"ioTexIndex = iTexIndex;\n"
"ioTex = iTex;\n"
"}\n";

const char* fsTexture = 
"#version 450 core\n"
"uniform sampler2D textures[32];\n"
"out vec4 oCol;\n"
"in vec3 ioCol;\n"
"in float ioTexIndex;\n"
"in vec2 ioTex;\n"
"void main()\n"
"{\n"
"int texID = int(ioTexIndex);\n"
"vec4 fragment;\n"
"if(texID >= 0) fragment = texture(textures[texID], ioTex) * ioCol;\n"
"if(texID == -1) fragment = iCol[colID];\n"
"if(fragment.w < 0.01) discard;\n"
"oCol = fragment;\n"
"}\n";

const char* vsAtlas =
"#version 450 core\n"
"uniform mat4 transform;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"layout(location = 0)in vec4 iPos;\n"
"layout(location = 1)in vec2 iTex;\n"
"layout(location = 2)in vec4 iCol;\n"
"out vec4 ioCol;\n"
"out vec2 ioTex;\n"
"void main()\n"
"{\n"
"gl_Position = (projection * view * transform) * iPos;\n"
"ioCol = iCol;\n"
"ioTex = iTex;\n"
"}\n";

const char* fsAtlas =
"#version 450 core\n"
"uniform sampler2D textures;\n"
"out vec4 oCol;\n"
"in vec4 ioCol;\n"
"in vec2 ioTex;\n"
"void main()\n"
"{\n"
"vec4 fragment;\n"
"fragment = texture(textures, ioTex) * ioCol;\n"
"if(fragment.w < 0.1) discard;\n"
"oCol = fragment;\n"
"}\n";

const char* fsAtlasWhite =
"#version 450 core\n"
"uniform sampler2D textures;\n"
"out vec4 oCol;\n"
"in vec4 ioCol;\n"
"in vec2 ioTex;\n"
"void main()\n"
"{\n"
"vec4 fragment;\n"
"vec4 texCol = texture(textures, ioTex);\n"
"if(texCol > vec4(0.8, 0.8, 0.8, 0.8)) fragment = texCol * ioCol;\n"
"else fragment = texCol;\n"
"if(fragment.w < 0.01) discard;\n"
"oCol = fragment;\n"
"}\n";

const std::array<float, 18 * 6>block_data = {
    //front
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    //front 2
    -0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    //back
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    //back 2
    -0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    //up
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    //up 2
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,

    //down
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    //down 2
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    //right
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    //right 2
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,

    //left
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    //left 2
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f
};

const std::array<float, 12 * 6>block_texture_data = {
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

int samplers[32] = {
    0,      1,      2,      3,      4,      5,      6,      7,      8,      9,      10,     11,     12,     13,     14,     15,
    16,     17,     18,     19,     20,     21,     22,     23,     24,     25,     26,     27,     28,     29,     30,     31
};

const std::array<float, 6 * 3> up_face = {
    //up
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    //up 2
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f
};

const std::array<float, 6 * 3> front_face = {
    //front
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    //front 2
    -0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f
};

const std::array<float, 6 * 3> right_face = {
    //right
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    //right 2
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f
};

unsigned int ShaderFromCode(const char* shader, int type)
{
    unsigned int shad = glCreateShader(type);
    glShaderSource(shad, 1, &shader, nullptr);
    glCompileShader(shad);

    return shad;
}