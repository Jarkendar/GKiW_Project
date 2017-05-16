/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // matematyka
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"

using namespace glm;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

float speed=3.14;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_RIGHT) speed = 3.14/4;
        if (key == GLFW_KEY_LEFT) speed =  - 3.14/4;
        }
if (action == GLFW_RELEASE)
{
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT) speed = 0;
}
}
//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
   // glClearColor(0.12,0.4,0,1);
     glClearColor(0,1,1,1); // kolor tla
    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glfwSetKeyCallback(window, key_callback);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 M = mat4(1.0f);

	mat4 V = lookAt(
    vec3(0.0f,0.0f,-5.0f),
    vec3(0.0f,0.0f,0.0f),
    vec3(0.0f,1.0f,0.0f));

    mat4 P = perspective(50*PI/180,1.0f,1.0f, 50.0f);
    glColor3d(1, 0.5, 0.1); // kolor rysowania
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(P));
    glMatrixMode(GL_MODELVIEW);
    M = translate(M, vec3(1.0f,0.0f,0.0f));
    M = rotate(M,angle,vec3(0.0f,0.0f,1.0f) );
    glLoadMatrixf(glm::value_ptr(V*M));

    Models::torus.drawSolid();

    M = mat4(1.0f);
    M = translate(M, vec3(-1.0f,0.0f,0.0f));
    M = rotate(M,-angle,vec3(0.0f,0.0f,1.0f) );
    glLoadMatrixf(glm::value_ptr(V*M));
    Models::torus.drawSolid();




    M = mat4(1.0f);
    M = translate(M, vec3(0.0f,0.0f,0.0f));
    //M = rotate(M,-angle,vec3(0.0f,0.0f,1.0f) );
    glLoadMatrixf(glm::value_ptr(V*M));
    float geomVertices[]={
        0,0,0, 1,1,-1,
        0,0,0, 1,-1,-1,
        0,0,0, -1,-1,-1,
        1,-1,-1, -1,-1,-1,
        1,-1,-1, 1,-1,-1,
    //-2.5,0,-1.44, 2.5,0,-1.44
};
int geomVertexCount=10;

glColor3d(0, 0, 0); // kolor rysowania
glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVertices);
glDrawArrays(GL_LINES,0,geomVertexCount);
glDisableClientState(GL_VERTEX_ARRAY);



    glfwSwapBuffers(window);

}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(800, 800, "Muzeum", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	float angle=0;
    glfwSetTime(0);
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
	    angle+=speed*glfwGetTime();
        glfwSetTime(0);
		drawScene(window, angle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
