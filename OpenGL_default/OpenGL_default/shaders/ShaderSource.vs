#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;			// MODEL MATRIX
uniform mat4 view;			// VIEW MATRIX
uniform mat4 projection;	// PROJECTION MATRIX

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // matriz de transformações sendo aplicada as posições de coordenadas de vertice.
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}