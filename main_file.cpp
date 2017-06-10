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
#include <process.h>


using namespace glm;

//zmienne globalne

GLuint tex[20]; // uchwyt do tekstur

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
float x_camera_position = -5; //startowa pozycja X
float z_camera_position = 6; //startowa pozycja Z
double ANGLE = 0;
float height = 3.0f;
float barHeight = 2.0f;
float pictureLowerBound = 0.5f;
float pictureUpperBound = 2.5f;

void drawMatrix(){//minimapa w konsoli
    for(int i = 19; i >= 0; i--){
        for(int j = 0; j < 20; j++){
            switch(macierzRuchu[i][j]){
                case 1:{
                    std::cout<<"X ";
                    break;
                }
                case 0:{
                    std::cout<<"  ";
                    break;
                }
                default:{
                    std::cout<<macierzRuchu[i][j]<<" ";
                    break;
                }
            }
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
    std::cout<<"cos"<<ANGLE<<" "<<cos(ANGLE*PI/180.0)<<"\n";
    std::cout<<"sin"<<ANGLE<<" "<<sin(ANGLE*PI/180.0)<<"\n";
}

int matrixPosition(float realPosition){
    return (int)realPosition+9;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS) {//pojedyńcze naciśnięcie klawisza
        if (key == GLFW_KEY_LEFT){
            if( ANGLE == 0.0){
                ANGLE = 350.0;
            }else{
                ANGLE -= 10.0;
            }
        }
        if (key == GLFW_KEY_RIGHT){
            if( ANGLE == 360.0){
                ANGLE = 10.0;
            }else{
                ANGLE += 10.0;
            }
        }
        if (key == GLFW_KEY_UP){
            macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 0;
            std::cout<<"ruch przod"<< macierzRuchu[matrixPosition((x_camera_position-(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]<< "\n";
            if(macierzRuchu[matrixPosition((x_camera_position-(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]==0){
                x_camera_position += -(float)cos(ANGLE*PI/180.0);
                std::cout<<"ruchx\n";
                if(x_camera_position > 9.0f){
                    x_camera_position = 9.0f;
                }else if(x_camera_position < -9.0f){
                    x_camera_position = -9.0f;
                }
            }
            if(macierzRuchu[matrixPosition((x_camera_position))][matrixPosition((z_camera_position-(float)sin(ANGLE*PI/180.0)))]==0){
                z_camera_position += -(float)sin(ANGLE*PI/180.0);
                std::cout<<"ruchz\n";
                if(z_camera_position > 9.0f){
                    z_camera_position = 9.0f;
                }else if(z_camera_position < -9.0f){
                    z_camera_position = -9.0f;
                }
            }
            macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;
        }
        if (key == GLFW_KEY_DOWN){
            macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 0;
            std::cout<<"ruch przod"<< macierzRuchu[matrixPosition((x_camera_position+(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]<< "\n";
            if(macierzRuchu[matrixPosition((x_camera_position+(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]==0){
                x_camera_position += (float)cos(ANGLE*PI/180.0);
                std::cout<<"ruchx\n";
                if(x_camera_position > 9.0f){
                    x_camera_position = 9.0f;
                }else if(x_camera_position < -9.0f){
                    x_camera_position = -9.0f;
                }
            }
            if(macierzRuchu[matrixPosition((x_camera_position))][matrixPosition((z_camera_position+(float)sin(ANGLE*PI/180.0)))]==0){
                z_camera_position += (float)sin(ANGLE*PI/180.0);
                std::cout<<"ruchz\n";
                if(z_camera_position > 9.0f){
                    z_camera_position = 9.0f;
                }else if(z_camera_position < -9.0f){
                    z_camera_position = -9.0f;
                }
            }
            macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;
        }
    }
    if(action == GLFW_REPEAT){
        if (key == GLFW_KEY_LEFT){
            if( ANGLE == 0.0){
                ANGLE = 350.0;
            }else{
                ANGLE -= 2.0;
            }
        }
        if (key == GLFW_KEY_RIGHT){
            if( ANGLE == 360.0){
                ANGLE = 10.0;
            }else{
                ANGLE += 2.0;
            }
        }
        if (key == GLFW_KEY_UP){
            macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 0;
            std::cout<<"ruch przod"<< macierzRuchu[matrixPosition((x_camera_position-(float)cos(ANGLE*PI/180.0)*0.2))][matrixPosition((z_camera_position))]<< "\n";
            if(macierzRuchu[matrixPosition((x_camera_position-(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]==0){
                x_camera_position += -(float)cos(ANGLE*PI/180.0)*0.2;
                std::cout<<"ruchx\n";
                if(x_camera_position > 9.0f){
                    x_camera_position = 9.0f;
                }else if(x_camera_position < -9.0f){
                    x_camera_position = -9.0f;
                }
            }
            if(macierzRuchu[matrixPosition((x_camera_position))][matrixPosition((z_camera_position-(float)sin(ANGLE*PI/180.0)*0.2))]==0){
                z_camera_position += -(float)sin(ANGLE*PI/180.0)*0.2;
                std::cout<<"ruchz\n";
                if(z_camera_position > 9.0f){
                    z_camera_position = 9.0f;
                }else if(z_camera_position < -9.0f){
                    z_camera_position = -9.0f;
                }
            }
            macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;
        }
        if (key == GLFW_KEY_DOWN){
            macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 0;
            std::cout<<"ruch przod"<< macierzRuchu[matrixPosition((x_camera_position+(float)cos(ANGLE*PI/180.0)*0.2))][matrixPosition((z_camera_position))]<< "\n";
            if(macierzRuchu[matrixPosition((x_camera_position+(float)cos(ANGLE*PI/180.0)*0.2))][matrixPosition((z_camera_position))]==0){
                x_camera_position += (float)cos(ANGLE*PI/180.0)*0.2;
                std::cout<<"ruchx\n";
                if(x_camera_position > 9.0f){
                    x_camera_position = 9.0f;
                }else if(x_camera_position < -9.0f){
                    x_camera_position = -9.0f;
                }
            }
            if(macierzRuchu[matrixPosition((x_camera_position))][matrixPosition((z_camera_position+(float)sin(ANGLE*PI/180.0)*0.2))]==0){
                z_camera_position += (float)sin(ANGLE*PI/180.0)*0.2;
                std::cout<<"ruchz\n";
                if(z_camera_position > 9.0f){
                    z_camera_position = 9.0f;
                }else if(z_camera_position < -9.0f){
                    z_camera_position = -9.0f;
                }
            }
            macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;
        }
    }
    //drawMatrix();
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

    float diff[] = {1,1,1,1};
    float spec[] = {1,1,1,1};
    float lightPos1[] = {5.0f, height, -5.0f, 1.0f}; // lewy dol na minimapie
    float lightPos2[] = {-5.0f, height, -5.0f, 1.0f}; // lewa gora
    float lightPos3[] = {5.0f, height, 5.0f, 1.0f}; // prawy dol
    float lightPos4[] = {-5.0f, height, 5.0f, 1.0f}; // prawa gora

    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);


    glLightfv(GL_LIGHT1,GL_AMBIENT,diff);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,spec);

    glLightfv(GL_LIGHT2,GL_AMBIENT,diff);
    glLightfv(GL_LIGHT2,GL_DIFFUSE,spec);

    glLightfv(GL_LIGHT3,GL_AMBIENT,diff);
    glLightfv(GL_LIGHT3,GL_DIFFUSE,spec);

    glLightfv(GL_LIGHT4,GL_AMBIENT,diff);
    glLightfv(GL_LIGHT4,GL_DIFFUSE,spec);


    glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);
    glLightfv(GL_LIGHT2,GL_POSITION,lightPos2);
    glLightfv(GL_LIGHT3,GL_POSITION,lightPos3);
    glLightfv(GL_LIGHT4,GL_POSITION,lightPos4);

    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, key_callback);

    std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error;
	// 0 - podloga
	error = lodepng::decode(image, width, height, "blue_marble.png");
	glGenTextures(20,tex); // inicjacja 20 w tablicy
	glBindTexture(GL_TEXTURE_2D, tex[0]); // wybranie uchwytu
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 1 - sciany
    glBindTexture(GL_TEXTURE_2D, tex[1]); // wybor drugiego uchwytu
    image.clear();
    error = lodepng::decode(image, width, height, "walls.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 2 - sufit
    glBindTexture(GL_TEXTURE_2D, tex[2]);
    image.clear();
    error = lodepng::decode(image, width, height, "bricks.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 3 - picture0
    glBindTexture(GL_TEXTURE_2D, tex[3]);
    image.clear();
    error = lodepng::decode(image, width, height, "pic0.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 4 - picture1
    glBindTexture(GL_TEXTURE_2D, tex[4]);
    image.clear();
    error = lodepng::decode(image, width, height, "pic1.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 5 - picture2
    glBindTexture(GL_TEXTURE_2D, tex[5]);
    image.clear();
    error = lodepng::decode(image, width, height, "pic2.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 6 - picture3
    glBindTexture(GL_TEXTURE_2D, tex[6]);
    image.clear();
    error = lodepng::decode(image, width, height, "pic3.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 7 - picture4
    glBindTexture(GL_TEXTURE_2D, tex[7]);
    image.clear();
    error = lodepng::decode(image, width, height, "pic4.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 8 - picture5
    glBindTexture(GL_TEXTURE_2D, tex[8]);
    image.clear();
    error = lodepng::decode(image, width, height, "pic5.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 9 - picture6
    glBindTexture(GL_TEXTURE_2D, tex[9]);
    image.clear();
    error = lodepng::decode(image, width, height, "pic6.png");
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // 10 - picture7
    glBindTexture(GL_TEXTURE_2D, tex[10]);
    image.clear();
    error = lodepng::decode(image, width, height, "pic7.png");
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
    vec3(0.5f, 0.0f, 0.0f),//na co patrzy
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
    M = translate(M, vec3(-0.5f,-3.0f,-0.5f));
    glLoadMatrixf(glm::value_ptr(V*M));

float geomVerticesPicture0[]={
    9.9f,pictureLowerBound,-6.0f,
    9.9f,pictureLowerBound,-4.0f,
    9.9f,pictureUpperBound,-4.0f,
    9.9f,pictureUpperBound,-6.0f
};
float geomVerticesPicture1[]={
    9.9f,pictureLowerBound,6.0f,
    9.9f,pictureLowerBound,4.0f,
    9.9f,pictureUpperBound,4.0f,
    9.9f,pictureUpperBound,6.0f
};
float geomVerticesPicture2[]={
    -9.9f,pictureLowerBound,-6.0f,
    -9.9f,pictureLowerBound,-4.0f,
    -9.9f,pictureUpperBound,-4.0f,
    -9.9f,pictureUpperBound,-6.0f
};
float geomVerticesPicture3[]={
    -9.9f,pictureLowerBound,6.0f,
    -9.9f,pictureLowerBound,4.0f,
    -9.9f,pictureUpperBound,4.0f,
    -9.9f,pictureUpperBound,6.0f
};
float geomVerticesPicture4[]={
    6.0f,pictureLowerBound,-9.9f,
    4.0f,pictureLowerBound,-9.9f,
    4.0f,pictureUpperBound,-9.9f,
    6.0f,pictureUpperBound,-9.9f
};
float geomVerticesPicture5[]={
    6.0f,pictureLowerBound,9.9f,
    4.0f,pictureLowerBound,9.9f,
    4.0f,pictureUpperBound,9.9f,
    6.0f,pictureUpperBound,9.9f
};
float geomVerticesPicture6[]={
    -6.0f,pictureLowerBound,-9.9f,
    -4.0f,pictureLowerBound,-9.9f,
    -4.0f,pictureUpperBound,-9.9f,
    -6.0f,pictureUpperBound,-9.9f
};
float geomVerticesPicture7[]={
    -6.0f,pictureLowerBound,9.9f,
    -4.0f,pictureLowerBound,9.9f,
    -4.0f,pictureUpperBound,9.9f,
    -6.0f,pictureUpperBound,9.9f
};

float geomVerticesDoorBarUp[]={
        -6.5f,barHeight,0.5f,
        -6.5f,height,0.5f,
        -3.5f,height,0.5f,
        -3.5f,barHeight,0.5f, // sciana przod

        -6.5f,barHeight,0.5f,
        -3.5f,barHeight,0.5f,
        -3.5f,barHeight,-0.5f,
        -6.5f,barHeight,-0.5f, // sciana dolna

        -6.5f,barHeight,-0.5f,
        -3.5f,barHeight,-0.5f,
        -3.5f,height,-0.5f,
        -6.5f,height,-0.5f, // sciana tyl

        -6.5f,height,-0.5f,
        -3.5f,height,-0.5f,
        -3.5f,height,0.5f,
        -6.5f,height,0.5f, // sciana lewa
};
float geomVerticesDoorBarDown[]={
        6.5f,barHeight,0.5f,
        6.5f,height,0.5f,
        3.5f,height,0.5f,
        3.5f,barHeight,0.5f, // sciana przod

        6.5f,barHeight,0.5f,
        3.5f,barHeight,0.5f,
        3.5f,barHeight,-0.5f,
        6.5f,barHeight,-0.5f, // sciana dolna

        6.5f,barHeight,-0.5f,
        3.5f,barHeight,-0.5f,
        3.5f,height,-0.5f,
        6.5f,height,-0.5f, // sciana tyl

        6.5f,height,-0.5f,
        3.5f,height,-0.5f,
        3.5f,height,0.5f,
        6.5f,height,0.5f, // sciana lewa
};
float geomVerticesDoorBarLeft[]={
        0.5f,barHeight,-3.5f,
        0.5f,height,-3.5f,
        0.5f,height,-6.5f,
        0.5f,barHeight,-6.5f, // sciana przod

        0.5f,barHeight,-6.5f,
        0.5f,barHeight,-3.5f,
        -0.5f,barHeight,-3.5f,
        -0.5f,barHeight,-6.5f, // sciana dolna

        -0.5f,barHeight,-6.5f,
        -0.5f,barHeight,-3.5f,
        -0.5f,height,-3.5f,
        -0.5f,height,-6.5f, // sciana tyl

        -0.5f,height,-6.5f,
        -0.5f,height,-3.5f,
        0.5f,height,-3.5f,
        0.5f,height,-6.5f, // sciana lewa
};
float geomVerticesDoorBarRight[]={
        0.5f,barHeight,3.5f,
        0.5f,height,3.5f,
        0.5f,height,6.5f,
        0.5f,barHeight,6.5f, // sciana przod

        0.5f,barHeight,6.5f,
        0.5f,barHeight,3.5f,
        -0.5f,barHeight,3.5f,
        -0.5f,barHeight,6.5f, // sciana dolna

        -0.5f,barHeight,6.5f,
        -0.5f,barHeight,3.5f,
        -0.5f,height,3.5f,
        -0.5f,height,6.5f, // sciana tyl

        -0.5f,height,6.5f,
        -0.5f,height,3.5f,
        0.5f,height,3.5f,
        0.5f,height,6.5f, // sciana górna
};
float geomVerticesWallsHorizontalInside[]={
        0.5f,0.0f,-3.5f,
        0.5f,height,-3.5f,
        0.5f,height,3.5f,
        0.5f,0.0f,3.5f, // sciana przod

        0.5f,0.0f,3.5f,
        0.5f,height,3.5f,
        -0.5f,height,3.5f,
        -0.5f,0.0f,3.5f, // sciana prawa

        -0.5f,0.0f,3.5f,
        -0.5f,height,3.5f,
        -0.5f,height,-3.5f,
        -0.5f,0.0f,-3.5f, // sciana tyl

        -0.5f,0.0f,-3.5f,
        -0.5f,height,-3.5f,
        0.5f,height,-3.5f,
        0.5f,0.0f,-3.5f // sciana lewa
};
float geomVerticesWallsVerticalInside[]={
        -3.5f,0.0f,-0.5f,
        -3.5f,height,-0.5f,
        -3.5f,height,0.5f,
        -3.5f,0.0f,0.5f, // sciana przod

        -3.5f,0.0f,0.5f,
        -3.5f,height,0.5f,
        3.5f,height,0.5f,
        3.5f,0.0f,0.5f, // sciana prawa

        3.5f,0.0f,0.5f,
        3.5f,height,0.5f,
        3.5f,height,-0.5f,
        3.5f,0.0f,-0.5f, // sciana tyl

        3.5f,0.0f,-0.5f,
        3.5f,height,-0.5f,
        -3.5f,height,-0.5f,
        -3.5f,0.0f,-0.5f // sciana lewa
};
float geomVerticesWallsLeftOutsite[]={
        0.5f,0.0f,-10.0f,
        0.5f,height,-10.0f,
        0.5f,height,-6.5f,
        0.5f,0.0f,-6.5f, // sciana przod

        0.5f,0.0f,-6.5f,
        0.5f,height,-6.5f,
        -0.5f,height,-6.5f,
        -0.5f,0.0f,-6.5f, // sciana prawa

        -0.5f,0.0f,-6.5f,
        -0.5f,height,-6.5f,
        -0.5f,height,-10.0f,
        -0.5f,0.0f,-10.0f, // sciana tyl

        -0.5f,0.0f,-10.0f,
        -0.5f,height,-10.0f,
        0.5f,height,-10.0f,
        0.5f,0.0f,-10.0f // sciana lewa
};
float geomVerticesWallsUpOutsite[]={
        10.0f,0.0f,-0.5f,
        10.0f,height,-0.5f,
        10.0f,height,0.5f,
        10.0f,0.0f,0.5f, // sciana przod

        10.0f,0.0f,0.5f,
        10.0f,height,0.5f,
        6.5f,height,0.5f,
        6.5f,0.0f,0.5f, // sciana prawa

        6.5f,0.0f,0.5f,
        6.5f,height,0.5f,
        6.5f,height,-0.5f,
        6.5f,0.0f,-0.5f, // sciana tyl

        6.5f,0.0f,-0.5f,
        6.5f,height,-0.5f,
        10.0f,height,-0.5f,
        10.0f,0.0f,-0.5f // sciana lewa
};
float geomVerticesWallsRightOutsite[]={
        -0.5f,0.0f,10.0f,
        -0.5f,height,10.0f,
        -0.5f,height,6.5f,
        -0.5f,0.0f,6.5f, // sciana przod

        -0.5f,0.0f,6.5f,
        -0.5f,height,6.5f,
        0.5f,height,6.5f,
        0.5f,0.0f,6.5f, // sciana prawa

        0.5f,0.0f,6.5f,
        0.5f,height,6.5f,
        0.5f,height,10.0f,
        0.5f,0.0f,10.0f, // sciana tyl

        0.5f,0.0f,10.0f,
        0.5f,height,10.0f,
        -0.5f,height,10.0f,
        -0.5f,0.0f,10.0f // sciana lewa
};
float geomVerticesWallsDownOutsite[]={
        -10.0f,0.0f,0.5f,
        -10.0f,height,0.5f,
        -10.0f,height,-0.5f,
        -10.0f,0.0f,-0.5f, // sciana przod

        -10.0f,0.0f,-0.5f,
        -10.0f,height,-0.5f,
        -6.5f,height,-0.5f,
        -6.5f,0.0f,-0.5f, // sciana prawa

        -6.5f,0.0f,-0.5f,
        -6.5f,height,-0.5f,
        -6.5f,height,0.5f,
        -6.5f,0.0f,0.5f, // sciana tyl

        -6.5f,0.0f,0.5f,
        -6.5f,height,0.5f,
        -10.0f,height,0.5f,
        -10.0f,0.0f,0.5f // sciana lewa
};
float geomVerticesWalls[]={
        -10.0f,0.0f,-10.0f,
        -10.0f,height,-10.0f,
        10.0f,height,-10.0f,
        10.0f,0.0f,-10.0f, // sciana przod

        10.0f,0.0f,-10.0f,
        10.0f,height,-10.0f,
        10.0f,height,10.0f,
        10.0f,0.0f,10.0f, // sciana prawa

        10.0f,0.0f,10.0f,
        10.0f,height,10.0f,
        -10.0f,height,10.0f,
        -10.0f,0.0f,10.0f, // sciana tyl

        -10.0f,0.0f,10.0f,
        -10.0f,height,10.0f,
        -10.0f,height,-10.0f,
        -10.0f,0.0f,-10.0f // sciana lewa
};
float geomVerticesFloor[]={
        -10.0f,0.0f,-10.0f,
        10.0f,0.0f,-10.0f,
        10.0f,0.0f,10.0f,
        -10.0f,0.0f,10.0f // podloga
};

float geomVerticesCeiling[]={
        -10.0f,height,-10.0f,
        10.0f,height,-10.0f,
        10.0f,height,10.0f,
        -10.0f,height,10.0f // sufit
};
float geomTexCoordsPictures[]={ // 1:1 dla obrazow
    0,1, 1,1, 1,0, 0,0
};

float geomTexCoordsRepeat[]={ // 1:20 dla podlogi i sufitu
    0,20, 20,20, 20,0, 0,0
};

float geomTexWallCoords[]={ // 1:10 dla scian
    0,10, 10,10, 10,0, 0,0,
    0,10, 10,10, 10,0, 0,0,
    0,10, 10,10, 10,0, 0,0,
    0,10, 10,10, 10,0, 0,0
};

int geomVertexWallsCount = 16;
int geomVertexFloorCount = 4;

//glColor3d(0, 0.5, 0.3); // kolor rysowania

//OBRAZ NUMER 0
glBindTexture(GL_TEXTURE_2D,tex[3]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesPicture0);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoordsPictures);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//OBRAZ NUMER 1
glBindTexture(GL_TEXTURE_2D,tex[4]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesPicture1);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoordsPictures);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//OBRAZ NUMER 2
glBindTexture(GL_TEXTURE_2D,tex[5]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesPicture2);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoordsPictures);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//OBRAZ NUMER 3
glBindTexture(GL_TEXTURE_2D,tex[6]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesPicture3);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoordsPictures);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//OBRAZ NUMER 4
glBindTexture(GL_TEXTURE_2D,tex[7]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesPicture4);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoordsPictures);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//OBRAZ NUMER 5
glBindTexture(GL_TEXTURE_2D,tex[8]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesPicture5);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoordsPictures);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//OBRAZ NUMER 6
glBindTexture(GL_TEXTURE_2D,tex[9]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesPicture6);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoordsPictures);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//OBRAZ NUMER 7
glBindTexture(GL_TEXTURE_2D,tex[10]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesPicture7);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoordsPictures);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);

//ŚCIANY GŁÓWNE
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesWalls);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//ŚCIANA ZEWNĘTRZNA MAŁA LEWA
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesWallsLeftOutsite);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//ŚCIANA ZEWNĘTRZNA MAŁA PRAWA
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesWallsRightOutsite);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//ŚCIANA ZEWNĘTRZNA MAŁA GÓRNA
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesWallsUpOutsite);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//ŚCIANA ZEWNĘTRZNA MAŁA DOLNA
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesWallsDownOutsite);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//ŚCIANA WEWNETRZNA MAŁA POZIOMA
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesWallsHorizontalInside);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//ŚCIANA WEWNETRZNA MAŁA PIONOWA
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesWallsVerticalInside);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//BELKA NAD LEWYM PRZEJSCIEM
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesDoorBarLeft);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//BELKA NAD PRAWYM PRZEJSCIEM
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesDoorBarRight);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//BELKA NAD GORNYM PRZEJSCIEM
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesDoorBarUp);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//BELKA NAD DOLNYM PRZEJSCIEM
glBindTexture(GL_TEXTURE_2D,tex[1]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0, geomVerticesDoorBarDown);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexWallCoords);
glDrawArrays(GL_QUADS,0,geomVertexWallsCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);

//SUFIT
glBindTexture(GL_TEXTURE_2D,tex[2]);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer( 3, GL_FLOAT, 0,  geomVerticesCeiling);
glTexCoordPointer( 2, GL_FLOAT, 0,  geomTexCoordsRepeat);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);

//PODLOGA
glBindTexture(GL_TEXTURE_2D,tex[0]);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, geomVerticesFloor);
glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoordsRepeat);
glDrawArrays(GL_QUADS,0,geomVertexFloorCount);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//glDisable(GL_TEXTURE_2D);

    glfwSwapBuffers(window); // zawsze ostatnie
}

//FUNKCJA WĄTKU
void __cdecl ThreadProc( void * Args )
{
    int g_Counter = 0;
    while( true){
         g_Counter++;
         std::cout<<g_Counter<<"\n";
    }
    _endthread();
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 1000, "Muzeum", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

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


//WCZYTYWANIE WIERZCHOŁKÓW I WEKTORÓW NORMALNYCH MODELU
    std::vector < float > temp_vertices;
    std::vector < float > temp_uvs;
    std::vector < float > temp_normals;

    int counter = 0;

    FILE * file = fopen("04.obj","r");
    if(file == NULL){
        std::cout<<"Nie można wczytać pliku\n";
    }
    while(true){
        char lineHeader[1000];
        int res = fscanf(file,"%s",lineHeader);
        if (res == EOF){
            break;
        }else if (strcmp(lineHeader,"v") == 0){//współrzędne wierzchołków
            float x, y, z;
            fscanf(file, "%f %f %f \n", &x, &y, &z);
            temp_vertices.push_back(x);
            temp_vertices.push_back(y);
            temp_vertices.push_back(z);
            counter++;
        }else if (strcmp(lineHeader,"vn") == 0){//wektory normalne
            float x, y, z;
            fscanf(file, "%f %f %f \n", &x, &y, &z);
            temp_normals.push_back(x);
            temp_normals.push_back(y);
            temp_normals.push_back(z);
        }
    }

    if(temp_normals.size() == 0){
        for(int i = 0; i<temp_vertices.size()-1; i++){
            temp_normals.push_back(0.0f);
        }
    }

//    for(int i =0; i<counter*3; i+=3){
//        std::cout<<temp_vertices[i]<<" "<<temp_vertices[i+1]<<" "<<temp_vertices[i+2]<<" "<<i<<"\n";
//    }
//*********************************************************

    //ODPALENIE WĄTKU (funkcja wątku, rozmiar stosu, argumenty przekazane do wątku)
 //   _beginthread( ThreadProc, 0, NULL );

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
