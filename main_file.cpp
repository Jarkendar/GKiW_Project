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
#include <iostream>
#include "lodepng.h"
#include "constants.h"
#include "allmodels.h"

using namespace glm;

//zmienne globalne

GLuint tex[10]; // podloga, sciany



//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}
// --->x
/*
    |
    |   z
    |
   \|/
    |

*/
//8 ściana, 0 można się poruszać, 1 gracz, 2 bot
int macierzRuchu[20][20] = {{8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8},//0
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//1
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//2
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//3
                            {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},//4
                            {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},//5
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//6
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//7
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//8
                            {8,8,8,8,0,0,8,8,8,8,8,8,8,8,0,0,8,8,8,8},//9 połowa
                            {8,8,8,8,0,0,8,8,8,8,8,8,8,8,0,0,8,8,8,8},//10
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//11
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//12
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//13
                            {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},//14
                            {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},//15
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//16
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//17
                            {8,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,8},//18
                            {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8}};//19

float speed=3.14;
int x_camera_position = -5; //startowa pozycja X
int z_camera_position = 6; //startowa pozycja Z
float ANGLE = 0;
float height = 3.0f;

void drawMatrix(){
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            std::cout<<macierzRuchu[i][j]<<" ";
        }
        std::cout<<"\n";
    }

    std::cout<<"Pozycja gracza : x="<<x_camera_position<<" z="<<z_camera_position<<"\n"<<"\n";

}

int mySinus(){
    switch ((int)ANGLE){
        case 0: return 0;
        case 90: return 1;
        case 180: return 0;
        case 270: return -1;
    }
}

int myCosinus(){
    switch ((int)ANGLE){
        case 0: return 1;
        case 90: return 0;
        case 180: return -1;
        case 270: return 0;
    }
}

void displayTrigonometrics(){
    std::cout<<"cos"<<ANGLE<<" "<<cos(ANGLE)<<"\n";
    std::cout<<"sin"<<ANGLE<<" "<<mySinus()<<"\n";
    for(int i =0; i<361; i++){
        std::cout<<"cos"<<i<<" "<<cos((float)i*PI/180)<<"\n";
    }
}

int matrixPosition(int realPosition){
    return realPosition+10;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS) {//pojedyńcze naciśnięcie klawisza
        if (key == GLFW_KEY_RIGHT){
            if( ANGLE == 0.0){
                ANGLE = 270.0;
            }else{
                ANGLE -= 90.0;
            }
        }
        if (key == GLFW_KEY_LEFT){
            if( ANGLE == 270.0){
                ANGLE = 0.0;
            }else{
                ANGLE += 90.0;
            }
        }
        if (key == GLFW_KEY_UP){
            if (ANGLE == 0 || ANGLE == 180){
                if (macierzRuchu[matrixPosition(x_camera_position+myCosinus())][matrixPosition(z_camera_position)] == 0){
                    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 0;//zwolnienie starej pozycji
                    x_camera_position += myCosinus();
                    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;//zajęcie nowej pozycji
                }
            }else{
                if (macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position-mySinus())] == 0){
                    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 0;
                    z_camera_position += -mySinus();
                    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;
                }
            }
        }
        if (key == GLFW_KEY_DOWN){
            if (ANGLE == 0 || ANGLE == 180){
                if (macierzRuchu[matrixPosition(x_camera_position-myCosinus())][matrixPosition(z_camera_position)] == 0){
                    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 0;
                    x_camera_position -= myCosinus();
                    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;
                }
            }else{
                if (macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position+mySinus())] == 0){
                    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 0;
                    z_camera_position -= -mySinus();
                    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;
                }

            }
        }
        }
        drawMatrix();
        displayTrigonometrics();


/*if (action == GLFW_RELEASE)
{
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT) speed = 0;
}*/
}
//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {

    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;
    drawMatrix();
    displayTrigonometrics();

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
//RUCH KAMERY (GRACZA)
//pozycja startowa
    float tmp = ANGLE*PI/180.0;
	mat4 V = lookAt(
    vec3(0.0f, 0.0f, 0.0f),//pozycja oka
    vec3(2.0f, 0.0f, 0.0f),//na co patrzy
    vec3(0.0f, 1.0f, 0.0f)); //wektor w górę
    V = rotate(V, tmp, vec3(0.0f, 1.0f, 0.0f));//obrót wokół osi Y -- oś obrotu prosta przechodząca przez (0,0,0); (0,1,0);
    V = translate(V, vec3(0.0f, 1.9f, 0.0f));//przesunięcie kamery do pozycji wyjściowej
    V = translate(V, vec3(x_camera_position, 0, z_camera_position));// przesunięcie kamery do pozycji aktualnej
//*************************


    mat4 P = perspective(50*PI/180, 1.0f, 1.0f, 50.0f);
    glColor3d(1, 0.5, 0.1); // kolor rysowania
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(P));
    glMatrixMode(GL_MODELVIEW);
//    M = translate(M, vec3(1.0f,0.0f,1.0f));
//    M = rotate(M,angle,vec3(0.0f,0.0f,1.0f) );
//    glLoadMatrixf(glm::value_ptr(V*M));

//    Models::torus.drawSolid();
//    glRectf(0,0,4,4);
//    glColor3d(0, 0.5, 0.1);
//    M = mat4(1.0f);
//    M = translate(M, vec3(1.0f,0.0f,-1.0f));
//    M = rotate(M,-angle,vec3(0.0f,0.0f,1.0f) );
//    glLoadMatrixf(glm::value_ptr(V*M));
//    Models::torus.drawSolid();

    M = mat4(1.0f);
    M = translate(M, vec3(0.0f,-3.0f,0.0f));
//    M = rotate(M,-angle,vec3(0.0f,0.0f,1.0f) );
    glLoadMatrixf(glm::value_ptr(V*M));
float geomVerticesWalls[]={
        -9.5f,0.0f,-9.5f,
        -9.5f,height,-9.5f,
        9.5f,height,-9.5f,
        9.5f,0.0f,-9.5f, // sciana przod

        9.5f,0.0f,-9.5f,
        9.5f,height,-9.5f,
        9.5f,height,9.5f,
        9.5f,0.0f,9.5f, // sciana prawa

        9.5f,0.0f,9.5f,
        9.5f,height,9.5f,
        -9.5f,height,9.5f,
        -9.5f,0.0f,9.5f, // sciana tyl

        -9.5f,0.0f,9.5f,
        -9.5f,height,9.5f,
        -9.5f,height,-9.5f,
        -9.5f,0.0f,-9.5f // sciana lewa
};

float geomVerticesFloor[]={
        -9.5f,0.0f,-9.5f,
        9.5f,0.0f,-9.5f,
        9.5f,0.0f,9.5f,
        -9.5f,0.0f,9.5f // podloga
};

/*
float geomVerticesFloor[]={
        0.5f,-3.0f,0.5f,
        19.5f,-3.0f,0.5f,
        19.5f,-3.0f,19.5f,
        0.5f,-3.0f,19.5f // podloga
};
*/
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
