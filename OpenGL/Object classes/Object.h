//
//  Object.h
//  OpenGL
//
//  Created by Denis on 29.11.14.
//  Copyright (c) 2014 Denis. All rights reserved.
//
#include "ModelLoader.h"
#include "FMath.h" 

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
    
    void setProjectionMatrix(float fovy, float aspect, float znear, float zfar);
    void setViewMatrix(float x, float y, float z);
    
    //
    // camera
    Mat4x4 viewMatrix;
    Mat4x4 modelMatrix;
    Mat4x4 projectionMatrix;
    
    Mat4x4 viewProjectionMatrix;
    Mat4x4 MVPmatrix;
};