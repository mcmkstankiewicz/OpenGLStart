#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLfloat wierzcholki[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	 0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
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
	GLuint VAO, VBO;							//Vertex Array Object, Vertex Buffer Object VAO przed VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);				//1 - bo mamy 1 object 3d i referencja do obiektu
	glBindVertexArray(VAO);			
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	//bindujemy - pewien object staje sie zbindowany(current object)
										// kiedy odp funkcje modyfikujaca ten typ obiektu to modyfikujemy nasz obiekt
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(wierzcholki), wierzcholki, GL_STATIC_DRAW);	//wsadzamy nasza tablice do buffera - tablica wiec mielysmy array buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);		//konf vertex atribute zeby opengl wiedzial jak czytac VBO
																						//0 -poczakowy atrybut, 3- mamy 3 floaty w verteksie, typ wartosci; mamy 3 floaty wiec 3 *float
																						//(void*)0 - offset - gdzie nasze wierzcholki sie zaczynaja w array?
	glEnableVertexAttribArray(0);	//pozwolenie dla open gl 0 -pozycja vertex atrrib 

	glBindBuffer(GL_ARRAY_BUFFER, 0);	//bindowanie VAO iVBO do 0 aby ich przypadkiem nie zmodyfikowaæ
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);	//Ustawienie koloru czwarta to przezroczystosc
		glClear(GL_COLOR_BUFFER_BIT);				//ustawienie back buffera
		
		glUseProgram(shaderProgram);			//mowimy opengl ktorego shader programu uzyc - mozna roznycch?
		glBindVertexArray(VAO);					//bindujemy VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);		//rusujemy uzywajac prymitywow

		glfwSwapBuffers(window);					//zamiana buffera - tak
		glfwPollEvents();	// Bez tego kno nie odpowiada
	}

	glfwDestroyWindow(window); //USUNIECIE OKNA
	glfwTerminate();			//ZAKONCZENIE GLFW
	return 0;
}