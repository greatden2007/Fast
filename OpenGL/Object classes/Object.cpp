//
//  Object.cpp
//  OpenGL
//
//  Created by Denis on 29.11.14.
//  Copyright (c) 2014 Denis. All rights reserved.
//

#include "Object.h"
#include <math.h>


void Object::readFromFile(const char *filename) {
    
    ModelLoader loader = *new ModelLoader();
    loader.read_from_file(filename);
    
    vertices_counter = loader.vertexs_counter;
    indices_counter = loader.indexes_counter;
    
    vertices_array = loader.vertexes_out;
    indices_array = loader.indexes;
    
}

/**
 Projection matrix
 @param fovy 
 @param aspect 
 @param znear nearest drawing pixel
 @para, zfar
 
 view matrix
 @param x
 @param y
 @param z
 */
void Object::init(float fovy, float aspect, float znear, float zfar, float x, float y, float z) {
    //set viewMatrix & ProjectionMatrix
    //...
    this->setProjectionMatrix(fovy, aspect, znear, zfar);
    this->setViewMatrix(x, y, z);
    modelMatrix.x[ 0] = 1; modelMatrix.x[ 1] = 0; modelMatrix.x[ 2] = 0; modelMatrix.x[ 3] += 0;
    modelMatrix.x[ 4] = 0; modelMatrix.x[ 5] = 1; modelMatrix.x[ 6] = 0; modelMatrix.x[ 7] += 0;
    modelMatrix.x[ 8] = 0; modelMatrix.x[ 9] = 0; modelMatrix.x[10] = 1; modelMatrix.x[11] += 0;
    modelMatrix.x[12] = 0; modelMatrix.x[13] = 0; modelMatrix.x[14] = 0; modelMatrix.x[15] = 1;
    mult(viewProjectionMatrix, projectionMatrix, viewMatrix);
    mult(MVPmatrix, viewProjectionMatrix, modelMatrix);
}

void Object::move(float x, float y, float z) {
    modelMatrix.x[11] += z;
    modelMatrix.x[ 7] += y;
    modelMatrix.x[ 3] += x;
    mult(MVPmatrix, viewProjectionMatrix, modelMatrix);
}

void Object::rotate(float x, float y, float z) {
    const float A = cosf(x), B = sinf(x), C = cosf(y),
    D = sinf(y), E = cosf(z), F = sinf(z);
    const float AD = A * D, BD = B * D;
        
    modelMatrix.x[ 0] = C * E;           modelMatrix.x[ 1] = -C * F;          modelMatrix.x[ 2] = D;
    modelMatrix.x[ 4] = BD * E + A * F;  modelMatrix.x[ 5] = -BD * F + A * E; modelMatrix.x[ 6] = -B * C;
    modelMatrix.x[ 8] = -AD * E + B * F; modelMatrix.x[ 9] = AD * F + B * E;  modelMatrix.x[10] = A * C;
    modelMatrix.x[12] = 0;               modelMatrix.x[13] = 0;               modelMatrix.x[14] = 0;      modelMatrix.x[15] = 1;
    mult(MVPmatrix, viewProjectionMatrix, modelMatrix);
}



void Object::setProjectionMatrix(float fovy, float aspect, float znear, float zfar) {
    // fovy передается в градусах - сконвертируем его в радианы
    float f = 1 / tanf(fovy * M_PI / 360),
    A = (zfar + znear) / (znear - zfar),
    B = (2 * zfar * znear) / (znear - zfar);
    
    projectionMatrix.x[ 0] = f / aspect; projectionMatrix.x[ 1] =  0; projectionMatrix.x[ 2] =  0; projectionMatrix.x[ 3] =  0;
    projectionMatrix.x[ 4] = 0;          projectionMatrix.x[ 5] =  f; projectionMatrix.x[ 6] =  0; projectionMatrix.x[ 7] =  0;
    projectionMatrix.x[ 8] = 0;          projectionMatrix.x[ 9] =  0; projectionMatrix.x[10] =  A; projectionMatrix.x[11] =  B;
    projectionMatrix.x[12] = 0;          projectionMatrix.x[13] =  0; projectionMatrix.x[14] = -1; projectionMatrix.x[15] =  0;
}

void Object::setViewMatrix(float x, float y, float z) {
    viewMatrix.x[ 0] = 1; viewMatrix.x[ 1] = 0; viewMatrix.x[ 2] = 0; viewMatrix.x[ 3] = x;
    viewMatrix.x[ 4] = 0; viewMatrix.x[ 5] = 1; viewMatrix.x[ 6] = 0; viewMatrix.x[ 7] = y;
    viewMatrix.x[ 8] = 0; viewMatrix.x[ 9] = 0; viewMatrix.x[10] = 1; viewMatrix.x[11] = z;
    viewMatrix.x[12] = 0; viewMatrix.x[13] = 0; viewMatrix.x[14] = 0; viewMatrix.x[15] = 1;
    mult(viewProjectionMatrix, projectionMatrix, viewMatrix);
}