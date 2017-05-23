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
#include "lodepng.h"
#include "constants.h"
#include "allmodels.h"

using namespace glm;

//zmienne globalne
//test
GLuint tex[10]; // podloga, sciany


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

float speed=3.14;
float z_camera_position = -3;
float x_camera_position = 5;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_REPEAT) {
        if (key == GLFW_KEY_RIGHT)x_camera_position -= 0.3;
        if (key == GLFW_KEY_LEFT) x_camera_position += 0.3;
        if (key == GLFW_KEY_UP) z_camera_position += 0.3;
        if (key == GLFW_KEY_DOWN) z_camera_position -= 0.3;
        }
/*if (action == GLFW_RELEASE)
{
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT) speed = 0;
}*/
}
//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
   // glClearColor(0.12,0.4,0,1);
    glClearColor(0,1,1,1); // kolor tla
    float lightPos[]={5,1.5,-5.0};
    //glLightfv(GL_LIGHT0 + 1,GL_POSITION,lightPos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_COLOR_MATERIAL);
    glfwSetKeyCallback(window, key_callback);

    std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "blue_marble.png");;
	glGenTextures(2,tex); // inicjacja 2 w tablicy
	glBindTexture(GL_TEXTURE_2D, tex[0]); // wybranie uchwytu
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, tex[1]); // wybor drugiego uchwytu
    error = lodepng::decode(image, width, height, "light_tiles.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 M = mat4(1.0f);

	mat4 V = lookAt(
    vec3(x_camera_position, 1.9f, z_camera_position),
    vec3(10, 1.4f, -5),
    vec3(0.0f, 1.0f, 0.0f)); // polozenie kamery

    mat4 P = perspective(50*PI/180, 1.0f, 1.0f, 50.0f);
    glColor3d(1, 0.5, 0.1); // kolor rysowania
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(P));
    glMatrixMode(GL_MODELVIEW);
    //M = translate(M, vec3(1.0f,0.0f,1.0f));
    //M = rotate(M,angle,vec3(0.0f,0.0f,1.0f) );
    //glLoadMatrixf(glm::value_ptr(V*M));

    //Models::torus.drawSolid();
    //glRectf(0,0,4,4);
    //glColor3d(0, 0.5, 0.1);
    //M = mat4(1.0f);
    //M = translate(M, vec3(1.0f,0.0f,-1.0f));
    //M = rotate(M,-angle,vec3(0.0f,0.0f,1.0f) );
    //glLoadMatrixf(glm::value_ptr(V*M));
    //Models::torus.drawSolid();

    M = mat4(1.0f);
    M = translate(M, vec3(0.0f,0.0f,0.0f));
    M = rotate(M,-angle,vec3(0.0f,0.0f,1.0f) );
    glLoadMatrixf(glm::value_ptr(V*M));
    float geomVerticesWalls[]={
        0,0,0,
        0,3,0,
        16,3,0,
        16,0,0, // sciana przod

        16,0,0,
        16,3,0,
        16,3,-10,
        16,0,-10, // sciana prawa

        16,0,-10,
        16,3,-10,
        0,3,-10,
        0,0,-10, // sciana tyl

        0,0,-10,
        0,3,-10,
        0,3,0,
        0,0,0, // sciana lewa
};

float geomVerticesFloor[]={
        0,0,0,
        16,0,0,
        16,0,-10,
        0,0,-10 // podloga
};
int geomVertexWallsCount = 16;
int geomVertexFloorCount = 4;

//glColor3d(0, 0.5, 0.3); // kolor rysowania


//glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesWalls);
glTexCoordPointer( 2, GL_FLOAT, 0, geomVerticesWalls);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);

//glColor3d(0.5, 0.1, 0.3);

glBindTexture(GL_TEXTURE_2D,tex[0]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesFloor);
glTexCoordPointer( 2, GL_FLOAT, 0, geomVerticesFloor);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//glDisable(GL_TEXTURE_2D);
    glfwSwapBuffers(window); // zawsze ostatnie

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
	    //angle+=speed*glfwGetTime();
        glfwSetTime(0);
		drawScene(window, angle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
