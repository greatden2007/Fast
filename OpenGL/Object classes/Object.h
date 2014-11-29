//
//  Object.h
//  OpenGL
//
//  Created by Denis on 29.11.14.
//  Copyright (c) 2014 Denis. All rights reserved.
//
#include "ModelLoader.h"
typedef struct Mat4x4{
    float x[16];
} Mat4x4;
class Object {
    
    
public:
    
    int vertices_counter = 0;
    int indices_counter = 0;
    
    float *vertices_array;
    int *indices_array;
    
    void readFromFile(const char* filename);
    void init();
    /**
     *  Add to MVP matrix translate to 5 px
     *
     *  @param direction can be l LEFT,
                                r RIGHT,
                                u UP,
                                d DOWN
     */
    void move(float x, float y, float z);
    void rotate(float x, float y, float z);
    
    Mat4x4 MVPmatrix;
};