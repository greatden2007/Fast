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

void Object::init() {
    MVPmatrix.x[0] = 1; MVPmatrix.x[1] = 0; MVPmatrix.x[2] = 0; MVPmatrix.x[3] = 0;
    MVPmatrix.x[4] = 0; MVPmatrix.x[5] = 1; MVPmatrix.x[6] = 0; MVPmatrix.x[7] = 0;
    MVPmatrix.x[8] = 0; MVPmatrix.x[9] = 0; MVPmatrix.x[10] = 1; MVPmatrix.x[11] = 0;
    MVPmatrix.x[12] = 0; MVPmatrix.x[13] = 0; MVPmatrix.x[14] = 0; MVPmatrix.x[15] = 1;
}

void Object::move(float x, float y, float z) {
    MVPmatrix.x[3] += x;
    MVPmatrix.x[7] += y;
    MVPmatrix.x[11] += z;
}

void Object::rotate(float x, float y, float z) {
    const float A = cosf(x), B = sinf(x), C = cosf(y),
    D = sinf(y), E = cosf(z), F = sinf(z);
    const float AD = A * D, BD = B * D;
        
    MVPmatrix.x[ 0] = C * E;           MVPmatrix.x[ 1] = -C * F;          MVPmatrix.x[ 2] = D;      MVPmatrix.x[ 3] = 0;
    MVPmatrix.x[ 4] = BD * E + A * F;  MVPmatrix.x[ 5] = -BD * F + A * E; MVPmatrix.x[ 6] = -B * C; MVPmatrix.x[ 7] = 0;
    MVPmatrix.x[ 8] = -AD * E + B * F; MVPmatrix.x[ 9] = AD * F + B * E;  MVPmatrix.x[10] = A * C;  MVPmatrix.x[11] = 0;
    MVPmatrix.x[12] = 0;               MVPmatrix.x[13] = 0;               MVPmatrix.x[14] = 0;      MVPmatrix.x[15] = 1;
}
