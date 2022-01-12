#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
	glfwInit();	//INICJALIZACJA GLFW
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	//UZYWANIE GLFW 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //UZYWAMY CORE PROFLIU CZYLI TYLKO NOWE 
																	//FUNKCJE, jest jescze GLFW_OPENGL_COMPAT -stare funkcje

	GLFWwindow* window = glfwCreateWindow(800, 600, "MENTAL BREAKDOWN COUSE BY RW", NULL, NULL);
	
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

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);	//Ustawienie koloru czwarta to przezroczystosc
	glClear(GL_COLOR_BUFFER_BIT);				//ustawienie back buffera
	glfwSwapBuffers(window);					//zamiana buffera - tak

	while (!glfwWindowShouldClose(window))
	{
		
		glfwPollEvents();	// Bez tego kno nie odpowiada
	}

	glfwDestroyWindow(window); //USUNIECIE OKNA
	glfwTerminate();			//ZAKONCZENIE GLFW
	return 0;
}