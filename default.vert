#version 330 core


layout (location = 0) in vec3 aPos;	// Positions/Coordinates
layout (location = 1) in vec3 aColor;	// Colors
layout (location = 2) in vec2 aTex;	// Texture Coordinates	


out vec3 color;		// Outputs the color for the Fragment Shader
out vec2 texCoord;	// Outputs the texture coordinates to the fragment shader

uniform float scale;	// Controls the scale of the vertices

uniform mat4 model;		//matryce potrzebne do 3d do shadera
uniform mat4 view;
uniform mat4 proj;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = proj * view * model * vec4(aPos, 1.0);
	color = aColor;	// Assigns the colors from the Vertex Data to "color"
	texCoord = aTex;	// Assigns the texture coordinates from the Vertex Data to "texCoord"
}