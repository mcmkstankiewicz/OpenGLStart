#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"VAO.h"
#include"VAO.h"
#include"EBO.h"
#include"Camera.h"

const unsigned int WINDOW_W = 800;
const unsigned int WINDOW_H = 800;

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
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

GLuint indices5[] =			//kolejnosc wierzcholkow
{
	1,2,0,
	2,3,0,
	3,4,0,
	4,5,0,
	5,1,0
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

	//VBO VBO1(wierzcholki, sizeof(wierzcholki));
	//EBO EBO1(indices5, sizeof(indices5));

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

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

	float rotation = 0.0f;		//do rotacji
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);		//glebia - potrzebna do 3d


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);	//Ustawienie koloru czwarta to przezroczystosc
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//ustawienie back buffera
		
		shaderProgram.Activate();		//shader musi byc aktywowany przed matprzypisyawniem do matryc
		
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.1f;		//wolniej/szybciej
			prevTime = crntTime;
		}

		glm::mat4 model = glm::mat4(1.0f);		//inicjalizacja model matrix
		glm::mat4 view = glm::mat4(1.0f);		//inicjalizacja view matrix
		glm::mat4 proj = glm::mat4(1.0f);		//inicjalizacja projection matrix

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));		//ustawienie widou przesuwamyu 0.5 w dol i 2 od siebie 
		proj = glm::perspective(glm::radians(45.0f), (float)(800/800), 0.1f, 100.0f);	//ustawienie perspektywy 

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);	//bindujemy teksture

		VAO1.Bind();	//bindujemy VAO
		
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);	//15 - ile elemetow tablicy indices, 0-indeks indices
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