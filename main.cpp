#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define M_PI 3.1415926535897932384626433832795
#define H 1.53884f
#define S sin(18*(M_PI/180))
#define C cos(18*(M_PI/180))
GLfloat wierzcholki[] =
{
	  0.0f,      0.0f,      -0.5f,		//S - 0
	 -0.5f,     -H / 2,     -0.5f,		//A - 1
	  0.5f,     -H / 2,     -0.5f,		//B - 2
	  0.5f + S, -H / 2 + C, -0.5f,		//C - 3
	  0.0f,      H / 2,		-0.5f,		//D - 4
	 -0.5f - S, -H / 2 + C, -0.5f,		//E - 5
};

GLfloat kwadrat[] =
{						//textury wierzcholki
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

GLuint indices[] =			//kolejnosc wierzcholkow
{
	1,2,0,
	2,3,0,
	3,4,0,
	4,5,0,
	5,1,0,
};

GLuint indicies_sq[] =
{
	0,1,3,
	1,2,3
};

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() {
	glfwInit();	//INICJALIZACJA GLFW
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	//UZYWANIE GLFW 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //UZYWAMY CORE PROFLIU CZYLI TYLKO NOWE 
																	//FUNKCJE, jest jescze GLFW_OPENGL_COMPAT -stare funkcje

	GLFWwindow* window = glfwCreateWindow(1440, 960, "MENTAL BREAKDOWN COUSE BY RW", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Blad prrzy tworzeniu okna" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);		//chcemy uzyc okna
	gladLoadGL();
	
	//kordynaty gdzie w oknie chcemy renderowaæ w t m przypakdu x1,y1 = (0,0) - lewy dolny rog x2,y2 =(800,800) prawy gorny rog
	glViewport(0, 0, 1440, 960);

	GLuint wierzcholkowyShader = glCreateShader(GL_VERTEX_SHADER);		//tworzymy zmienna 
	glShaderSource(wierzcholkowyShader, 1, &vertexShaderSource, NULL);	//przypisujemy zmiennej kod shadera (wyzej na pocz) 1 - oznacza ze uzywamy 1 string na shader
	glCompileShader(wierzcholkowyShader);								//analogicznie

	GLuint fragmentowyShader = glCreateShader(GL_FRAGMENT_SHADER);		//a teraz dla fragmenyujacego
	glShaderSource(fragmentowyShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentowyShader);

	GLuint shaderProgram = glCreateProgram();							//zeby uzyc shaderow trzeba je zwrapowac do shaderProgram
	glAttachShader(shaderProgram, wierzcholkowyShader);					//dod. wierzcholkowy i fragmentowy
	glAttachShader(shaderProgram, fragmentowyShader);
	glLinkProgram(shaderProgram);										//linkujemy shadery do programu

	glDeleteShader(wierzcholkowyShader);
	glDeleteShader(fragmentowyShader);
	
	//buffers -big batches do przesylanie miedzy CPU a GPU
	GLuint VAO, VBO, EBO;							//Vertex Array Object, Vertex Buffer Object VAO przed VBO, EBO - do indicies
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);				//1 - bo mamy 1 object 3d i referencja do obiektu
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);			
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	//bindujemy - pewien object staje sie zbindowany(current object)  kiedy odp funkcje modyfikujaca ten typ obiektu to modyfikujemy nasz obiekt
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bindujemy EBO

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies_sq), indicies_sq, GL_STATIC_DRAW);	//wsadzamy tab indices do naszego EBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(kwadrat), kwadrat, GL_STATIC_DRAW);	//wsadzamy nasza tablice do buffera - tablica wiec mielysmy array buffer
	/*
	
	glVertexAttribPointer(0, 5, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);		//konf vertex atribute zeby opengl wiedzial jak czytac VBO
																						//0 -poczakowy atrybut, 3- mamy 3 floaty w verteksie, typ wartosci; mamy 3 floaty wiec 3 *float
																						//(void*)0 - offset - gdzie nasze wierzcholki sie zaczynaja w array?
	glEnableVertexAttribArray(0);	//pozwolenie dla open gl 0 -pozycja vertex atrrib 
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);	//bindowanie VAO i VBO do 0 aby ich przypadkiem nie zmodyfikowaæ
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//musi byc po VAO!!!

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);	//Ustawienie koloru czwarta to przezroczystosc
		glClear(GL_COLOR_BUFFER_BIT);				//ustawienie back buffera
		
		glUseProgram(shaderProgram);			//mowimy opengl ktorego shader programu uzyc - mozna roznycch?
		glBindVertexArray(VAO);					//bindujemy VAO
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//15 - ile elemetow tablicy indices, 0-indeks indices
		//glDrawArrays(GL_TRIANGLES, 0, 3);		//rusujemy uzywajac prymitywow

		glfwSwapBuffers(window);					//zamiana buffera - tak
		glfwPollEvents();	// Bez tego kno nie odpowiada
	}
	
	
	glDeleteVertexArrays(1, &VAO);	//usuwanie VAO,VBO i shaderProgramu
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window); //USUNIECIE OKNA
	glfwTerminate();			//ZAKONCZENIE GLFW
	return 0;
}