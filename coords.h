#ifndef COORDS_H_INCLUDED
#define COORDS_H_INCLUDED

float height = 3.0f;
float barHeight = 2.0f;
float pictureLowerBound = 0.5f;
float pictureUpperBound = 2.5f;

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


#endif // COORDS_H_INCLUDED
