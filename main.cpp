#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VAO.h"
#include"EBO.h"


#define M_PI 3.1415926535897932384626433832795
#define H 1.53884f
#define S sin(18*(M_PI/180))
#define C cos(18*(M_PI/180))
#define STEX 2.0f
GLfloat wierzcholki[] =
{
	  0.0f,      0.0f,      -0.5f,   1.0f, 0.0f, 0.0f,	 0.0f,		0.0f,			//S - 0
	 -0.5f,     -H / 2,     -0.5f,	 0.0f, 1.0f, 0.0f,	-0.5f,     -H / 2,			//A - 1
	  0.5f,     -H / 2,     -0.5f,	 0.0f, 0.0f, 1.0f,	 0.5f,     -H / 2, 			//B - 2
	  0.5f + S, -H / 2 + C, -0.5f,   1.0f, 1.0f, 1.0f,	 0.5f + S, -H / 2 + C,		//C - 3
	  0.0f,      H / 2,		-0.5f,	 0.0f, 0.0f, 1.0f,	 0.0f,      H / 2,			//D - 4
	 -0.5f - S, -H / 2 + C, -0.5f,	 1.0f, 1.0f, 1.0f,	-0.5f - S, -H / 2 + C		//E - 5
};

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};

GLuint indices5[] =			//kolejnosc wierzcholkow
{
	1,2,0,
	2,3,0,
	3,4,0,
	4,5,0,
	5,1,0,
};



int main() {
	glfwInit();	//INICJALIZACJA GLFW
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	//UZYWANIE GLFW 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //UZYWAMY CORE PROFLIU CZYLI TYLKO NOWE 
																	//FUNKCJE, jest jescze GLFW_OPENGL_COMPAT -stare funkcje

	GLFWwindow* window = glfwCreateWindow(800, 800, "MENTAL BREAKDOWN COUSE BY RW", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Blad prrzy tworzeniu okna" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);		//chcemy uzyc okna
	gladLoadGL();
	
	//kordynaty gdzie w oknie chcemy renderowaæ w t m przypakdu x1,y1 = (0,0) - lewy dolny rog x2,y2 =(800,800) prawy gorny rog
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(wierzcholki, sizeof(wierzcholki));
	EBO EBO1(indices5, sizeof(indices5));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	int imgWidth, imgHeight, imgChannels;
	stbi_set_flip_vertically_on_load(true);				//przestawia obraz zeby nie byl dogry nogami
	unsigned char* bytes = stbi_load("pepe.png", &imgWidth, &imgHeight, &imgChannels, 0);	//musi byc jpeg
	
	GLuint texture;					//Whrywanie tektur
	glGenTextures(1, &texture);	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//GL_Linear rozmyta bardziej
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		//s axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		//t axis

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);	
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();			//mowimy opengl ktorego shader programu uzyc - mozna roznycch?
	glUniform1i(tex0Uni, 0);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);	//Ustawienie koloru czwarta to przezroczystosc
		glClear(GL_COLOR_BUFFER_BIT);				//ustawienie back buffera
		
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);

		VAO1.Bind();	//bindujemy VAO
		
		glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);	//15 - ile elemetow tablicy indices, 0-indeks indices
		//glDrawArrays(GL_TRIANGLES, 0, 3);		//rusujemy uzywajac prymitywow

		glfwSwapBuffers(window);					//zamiana buffera - tak
		glfwPollEvents();	// Bez tego kno nie odpowiada
	}
	
	
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window); //USUNIECIE OKNA
	glfwTerminate();			//ZAKONCZENIE GLFW
	return 0;
}