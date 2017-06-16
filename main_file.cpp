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
#include "coords.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <string.h>

using namespace glm;

//zmienne globalne

const int tex_size = 50; // liczba textur
GLuint tex[tex_size]; // uchwyt do tekstur

std::vector<float> walkX;
std::vector<float> walkZ;

//Przetrzymywanie wierzcholkow modelu
struct model{
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals; // nieuzywane
    float posX;
    float posY;
    float posZ;
    float ray; // tylko dla tych po okregu
    float angleX; // wszystkie katy w stopniach
    float angleY;
    float angleZ;
};

std::vector<model> myModels;

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

mat4 V, M;

void loadTEX(std::string name, std::string description)
{
        static int amount = 0;
        if (amount == 0)
        {
            glGenTextures(tex_size,tex);
        }
        std::vector<unsigned char> image;
        unsigned width, height;
        unsigned error = lodepng::decode(image, width, height, "textures/" + name);
        glBindTexture(GL_TEXTURE_2D, tex[amount]);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glEnable(GL_TEXTURE_2D);
        if (error == 0)
        {
            std::cout << amount<< "\t"  << name << "\t\t--" << description << std::endl;
        }
        else
        {
            std::cout << amount<< "\t"  << name << "\t\t--" << description << "\tERROR" << std::endl;
        }
        amount++;
}

void useTEX(int textureNumber, GLenum mode, float *vertices, float *textureCoords, int number)
{
    glBindTexture(GL_TEXTURE_2D,tex[textureNumber]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer( 2, GL_FLOAT, 0, textureCoords);
    glDrawArrays(mode,0,number);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
};


//Wczytywanie OBJ
bool loadOBJ(
    std::string name, std::string description
)
{
    static int amount = 0;
    // wektory tymczasowe
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    // otwarcie pliku
    FILE * file = fopen(("models/" + name).c_str(), "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }
    // wczytywanie
    while( 1 ){
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        if ( strcmp( lineHeader, "v" ) == 0 ){
        glm::vec3 vertex;
        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
        temp_vertices.push_back(vertex);
        }
        else if ( strcmp( lineHeader, "vt" ) == 0 ){
        glm::vec2 uv;
        fscanf(file, "%f %f\n", &uv.x, &uv.y );
        temp_uvs.push_back(uv);
        }
        else if ( strcmp( lineHeader, "vn" ) == 0 ){
        glm::vec3 normal;
        fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
        temp_normals.push_back(normal);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 ){
        std::string vertex1, vertex2, vertex3;
        unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
        int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
        if (matches != 9){
            printf("File can't be read by our simple parser : ( Try exporting with other options\n");
            return false;
        }
        vertexIndices.push_back(vertexIndex[0]);
        vertexIndices.push_back(vertexIndex[1]);
        vertexIndices.push_back(vertexIndex[2]);
        uvIndices    .push_back(uvIndex[0]);
        uvIndices    .push_back(uvIndex[1]);
        uvIndices    .push_back(uvIndex[2]);
        normalIndices.push_back(normalIndex[0]);
        normalIndices.push_back(normalIndex[1]);
        normalIndices.push_back(normalIndex[2]);
        }
        model nowy;
        myModels.push_back(nowy);
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){
            unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
            myModels[amount].vertices.push_back(vertex);
        }
        for( unsigned int i=0; i<uvIndices.size(); i++ ){
            unsigned int uvIndex = uvIndices[i];
            glm::vec2 vertex = temp_uvs[ uvIndex-1 ];
            myModels[amount].uvs.push_back(vertex);
        }
        for( unsigned int i=0; i<normalIndices.size(); i++ ){
            unsigned int normalIndex = normalIndices[i];
            glm::vec3 vertex = temp_normals[ normalIndex-1 ];
            myModels[amount].normals.push_back(vertex);
        }
        vertexIndices.clear();
        uvIndices.clear();
        normalIndices.clear();
    }
    std::cout << amount << "\t" << name << "\t\t--" << description << std::endl;
    amount++;
}

void setModel(model & Model, float x, float y, float z, float ray = 0, float angleX = 0, float angleY = 0, float angleZ = 0)
{
    Model.posX = x;
    Model.posY = y;
    Model.posZ = z;
    Model.ray = ray;
    Model.angleX = angleX;
    Model.angleY = angleY;
    Model.angleZ = angleZ;
}

void useModel(int textureNumber,  GLenum mode,
        std::vector< glm::vec3 > &vertices, std::vector< glm::vec2 > &uvs, std::vector< glm::vec3 > &normals,
        float translateX = 5, float translateY = -2, float translateZ = -5,
        float angleX = 0, float angleY = 0, float angleZ = 0,
        float scaleX = 1, float scaleY = 1, float scaleZ = 1)
{
    float * wsk_vertices = glm::value_ptr(vertices[0]);
    float * wsk_uvs = glm::value_ptr(uvs[0]);
    float * wsk_normals = glm::value_ptr(normals[0]); // cieniowanie

    glBindTexture(GL_TEXTURE_2D,tex[textureNumber]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState( GL_NORMAL_ARRAY ); cieniowanie

    glVertexPointer(3, GL_FLOAT, 0, wsk_vertices);
    //glNormalPointer( GL_FLOAT, 0, wsk_normals); cieniowanie
    glTexCoordPointer( 2, GL_FLOAT, 0, wsk_uvs);

    M = mat4(1.0f);
    M = translate(M, vec3(translateX,translateY,translateZ));
    M = rotate(M,angleY * PI / 180,vec3(0, 1, 0));
    M = rotate(M,angleX * PI / 180,vec3(1, 0, 0));
    M = rotate(M,angleZ * PI / 180,vec3(0, 0, 1));
    M = scale(M, vec3(scaleX, scaleY, scaleZ));
    glLoadMatrixf(glm::value_ptr(V*M));
    glDrawArrays(mode, 0, vertices.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisableClientState( GL_NORMAL_ARRAY ); cieniowanie
}

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
            //std::cout<<"ruch przod"<< macierzRuchu[matrixPosition((x_camera_position-(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]<< "\n";
            if(macierzRuchu[matrixPosition((x_camera_position-(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]==0){
                x_camera_position += -(float)cos(ANGLE*PI/180.0);
                //std::cout<<"ruchx\n";
                if(x_camera_position > 9.0f){
                    x_camera_position = 9.0f;
                }else if(x_camera_position < -9.0f){
                    x_camera_position = -9.0f;
                }
            }
            if(macierzRuchu[matrixPosition((x_camera_position))][matrixPosition((z_camera_position-(float)sin(ANGLE*PI/180.0)))]==0){
                z_camera_position += -(float)sin(ANGLE*PI/180.0);
                //std::cout<<"ruchz\n";
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
            //std::cout<<"ruch przod"<< macierzRuchu[matrixPosition((x_camera_position+(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]<< "\n";
            if(macierzRuchu[matrixPosition((x_camera_position+(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]==0){
                x_camera_position += (float)cos(ANGLE*PI/180.0);
                //std::cout<<"ruchx\n";
                if(x_camera_position > 9.0f){
                    x_camera_position = 9.0f;
                }else if(x_camera_position < -9.0f){
                    x_camera_position = -9.0f;
                }
            }
            if(macierzRuchu[matrixPosition((x_camera_position))][matrixPosition((z_camera_position+(float)sin(ANGLE*PI/180.0)))]==0){
                z_camera_position += (float)sin(ANGLE*PI/180.0);
                //std::cout<<"ruchz\n";
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
            //std::cout<<"ruch przod"<< macierzRuchu[matrixPosition((x_camera_position-(float)cos(ANGLE*PI/180.0)*0.2))][matrixPosition((z_camera_position))]<< "\n";
            if(macierzRuchu[matrixPosition((x_camera_position-(float)cos(ANGLE*PI/180.0)))][matrixPosition((z_camera_position))]==0){
                x_camera_position += -(float)cos(ANGLE*PI/180.0)*0.2;
                //std::cout<<"ruchx\n";
                if(x_camera_position > 9.0f){
                    x_camera_position = 9.0f;
                }else if(x_camera_position < -9.0f){
                    x_camera_position = -9.0f;
                }
            }
            if(macierzRuchu[matrixPosition((x_camera_position))][matrixPosition((z_camera_position-(float)sin(ANGLE*PI/180.0)*0.2))]==0){
                z_camera_position += -(float)sin(ANGLE*PI/180.0)*0.2;
                //std::cout<<"ruchz\n";
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
           //std::cout<<"ruch przod"<< macierzRuchu[matrixPosition((x_camera_position+(float)cos(ANGLE*PI/180.0)*0.2))][matrixPosition((z_camera_position))]<< "\n";
            if(macierzRuchu[matrixPosition((x_camera_position+(float)cos(ANGLE*PI/180.0)*0.2))][matrixPosition((z_camera_position))]==0){
                x_camera_position += (float)cos(ANGLE*PI/180.0)*0.2;
                //std::cout<<"ruchx\n";
                if(x_camera_position > 9.0f){
                    x_camera_position = 9.0f;
                }else if(x_camera_position < -9.0f){
                    x_camera_position = -9.0f;
                }
            }
            if(macierzRuchu[matrixPosition((x_camera_position))][matrixPosition((z_camera_position+(float)sin(ANGLE*PI/180.0)*0.2))]==0){
                z_camera_position += (float)sin(ANGLE*PI/180.0)*0.2;
                //std::cout<<"ruchz\n";
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
    std::cout << "Models: " << std::endl;
    loadOBJ("cylindroman.obj", "latajacy dookola");
    loadOBJ("monument.obj", "popiersie w kacie");
    loadOBJ("klockoman.obj", "kwadratowy ludzik");
    loadOBJ("cylindroman.obj", "chodzacy po kwadracie");
    setModel(myModels[0], 5.0f, -2.0f, -5.0f, 5.3f, 45.0f);
    setModel(myModels[1], 1.0f, -3.0f, 1.0f);
    setModel(myModels[2], 5.0f, -3.7f, -5.0f, 7.0f, 8.0f);
    setModel(myModels[3], 2.0f, -2.1f, -2.0f, 0.0f, -5.5f, -90.0f);
    /*for(int i = 0; i < vertices.size();i++)
    {
        std::cout<<vertices[i].x<<"\t";
        std::cout<<vertices[i].y<<"\t";
        std::cout<<vertices[i].z<<"\n";
    }*/
    macierzRuchu[matrixPosition(x_camera_position)][matrixPosition(z_camera_position)] = 1;
    //drawMatrix();
    //displayTrigonometrics();

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

    std::cout << "Textures:" << std::endl;
    loadTEX("blue_marble.png","podloga");
    loadTEX("walls.png","sciany");
    loadTEX("bricks.png","sufit");
    loadTEX("pic0.png","picture0");
    loadTEX("pic1.png","picture1");
    loadTEX("pic2.png","picture2");
    loadTEX("pic3.png","picture3");
    loadTEX("pic4.png","picture4");
    loadTEX("pic5.png","picture5");
    loadTEX("pic6.png","picture6");
    loadTEX("pic7.png","picture7");
    loadTEX("marble.png","popiersie");


    for (int i = 0; i < 241; i++)
    {
        walkX.push_back(2 + 1.0*i/40);
        walkZ.push_back(-2);
    }
    for (int i = 0; i < 241; i++)
    {
        walkX.push_back(8);
        walkZ.push_back(-2 - 1.0*i/40);
    }
    for (int i = 0; i < 241; i++)
    {
        walkX.push_back(8 - 1.0*i/40);
        walkZ.push_back(-8);
    }
    for (int i = 0; i < 241; i++)
    {
        walkX.push_back(2);
        walkZ.push_back(-8 + 1.0*i/40);
    }




}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	M = mat4(1.0f);
//RUCH KAMERY (GRACZA)
//pozycja startowa
    float tmp = ANGLE*PI/180.0;
    V = lookAt(
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
//    mat4 M = mat4(1.0f);
//    M = translate(M, vec3(1.0f,0.0f,-1.0f));
//    M = rotate(M,-angle,vec3(0.0f,0.0f,1.0f) );
//    glLoadMatrixf(glm::value_ptr(V*M));
//    Models::torus.drawSolid();

    M = mat4(1.0f);
    M = translate(M, vec3(-0.5f,-3.0f,-0.75f));
    glLoadMatrixf(glm::value_ptr(V*M));


//glColor3d(0, 0.5, 0.3); // kolor rysowania

///OBRAZY
//OBRAZ NUMER 0
useTEX(3, GL_QUADS, geomVerticesPicture0, geomTexCoordsPictures, geomVertexFloorCount);
//OBRAZ NUMER 1
useTEX(4, GL_QUADS, geomVerticesPicture1, geomTexCoordsPictures, geomVertexFloorCount);
//OBRAZ NUMER 2
useTEX(5, GL_QUADS, geomVerticesPicture2, geomTexCoordsPictures, geomVertexFloorCount);
//OBRAZ NUMER 3
useTEX(6, GL_QUADS, geomVerticesPicture3, geomTexCoordsPictures, geomVertexFloorCount);
//OBRAZ NUMER 4
useTEX(7, GL_QUADS, geomVerticesPicture4, geomTexCoordsPictures, geomVertexFloorCount);
//OBRAZ NUMER 5
useTEX(8, GL_QUADS, geomVerticesPicture5, geomTexCoordsPictures, geomVertexFloorCount);
//OBRAZ NUMER 6
useTEX(9, GL_QUADS, geomVerticesPicture6, geomTexCoordsPictures, geomVertexFloorCount);
//OBRAZ NUMER 7
useTEX(10, GL_QUADS, geomVerticesPicture7, geomTexCoordsPictures, geomVertexFloorCount);


///SCIANY
//ŚCIANY GŁÓWNE
useTEX(1, GL_QUADS, geomVerticesWalls, geomTexWallCoords, geomVertexWallsCount);
//ŚCIANA ZEWNĘTRZNA MAŁA LEWA
useTEX(1, GL_QUADS, geomVerticesWallsLeftOutsite, geomTexWallCoords, geomVertexWallsCount);
//ŚCIANA ZEWNĘTRZNA MAŁA PRAWA
useTEX(1, GL_QUADS, geomVerticesWallsRightOutsite, geomTexWallCoords, geomVertexWallsCount);
//ŚCIANA ZEWNĘTRZNA MAŁA GÓRNA
useTEX(1, GL_QUADS, geomVerticesWallsUpOutsite, geomTexWallCoords, geomVertexWallsCount);
//ŚCIANA ZEWNĘTRZNA MAŁA DOLNA
useTEX(1, GL_QUADS, geomVerticesWallsDownOutsite, geomTexWallCoords, geomVertexWallsCount);
//ŚCIANA WEWNETRZNA MAŁA POZIOMA
useTEX(1, GL_QUADS, geomVerticesWallsHorizontalInside, geomTexWallCoords, geomVertexWallsCount);
//ŚCIANA WEWNETRZNA MAŁA PIONOWA
useTEX(1, GL_QUADS, geomVerticesWallsVerticalInside, geomTexWallCoords, geomVertexWallsCount);
//BELKA NAD LEWYM PRZEJSCIEM
useTEX(1, GL_QUADS, geomVerticesDoorBarLeft, geomTexWallCoords, geomVertexWallsCount);
//BELKA NAD PRAWYM PRZEJSCIEM
useTEX(1, GL_QUADS, geomVerticesDoorBarRight, geomTexWallCoords, geomVertexWallsCount);
//BELKA NAD GORNYM PRZEJSCIEM
useTEX(1, GL_QUADS, geomVerticesDoorBarUp, geomTexWallCoords, geomVertexWallsCount);
//BELKA NAD DOLNYM PRZEJSCIEM
useTEX(1, GL_QUADS, geomVerticesDoorBarDown, geomTexWallCoords, geomVertexWallsCount);

//SUFIT
useTEX(2, GL_QUADS, geomVerticesCeiling, geomTexCoordsRepeat, geomVertexFloorCount);

//PODLOGA
useTEX(0, GL_QUADS, geomVerticesFloor, geomTexCoordsRepeat, geomVertexFloorCount);


///MODELE
//MODEL OKRAGLEGO CZLOWIEKA
useModel(5, GL_TRIANGLES, myModels[0].vertices, myModels[0].uvs, myModels[0].normals, myModels[0].posX, myModels[0].posY, myModels[0].posZ, myModels[0].angleY, 1.0f, 4.0f, 2.0f);
//MODEL POSĄGU
useModel(11, GL_TRIANGLES, myModels[1].vertices, myModels[1].uvs, myModels[1].normals, myModels[1].posX, myModels[1].posY, myModels[1].posZ, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f);
//MODEL KWADRATOWEGO CZLOWIEKA
useModel(8, GL_TRIANGLES, myModels[2].vertices, myModels[2].uvs, myModels[2].normals, myModels[2].posX, myModels[2].posY, myModels[2].posZ, myModels[2].angleY, 0.0f, 0.05f);
//MODEL CHODZACY PO KWADRACIE
useModel(7, GL_TRIANGLES, myModels[3].vertices, myModels[3].uvs, myModels[3].normals, myModels[3].posX, myModels[3].posY, myModels[3].posZ, myModels[3].angleX, myModels[3].angleY, myModels[3].angleZ, 0.9f, 0.9f, 0.9f);
//std::cout <<  myModels[2].posX << "\t" <<  myModels[2].posY << "\t" << myModels[2].posZ << std::endl;

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

    glfwSetTime(0);
	//Główna pętla
	int k = 0;
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
	    k = k % 968;
	    myModels[0].angleY = speed*glfwGetTime();
	    myModels[0].posX =  myModels[0].ray*cos(glfwGetTime());
	    myModels[0].posZ =  myModels[0].ray*sin(glfwGetTime());
	    //std::cout<< myModels[0].posX << "\t" << myModels[0].posZ << "\n";
        //glfwSetTime(0);
        myModels[2].angleY = -speed/5*glfwGetTime();
	    myModels[2].posX =  -myModels[2].ray*cos(glfwGetTime());
	    myModels[2].posZ =  -myModels[2].ray*sin(glfwGetTime());

        if (k % 242 == 0)
        {
            myModels[3].angleY = int(90 + myModels[3].angleY) % 360;
        }

        if(k % 22 < 11)
        myModels[3].angleX += 1;
        else
        myModels[3].angleX -= 1;

        myModels[3].posX = walkX[k];
        myModels[3].posZ = walkZ[k];
        k++;
		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
