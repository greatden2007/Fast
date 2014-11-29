//
//  FMath.h
//  OpenGL
//
//  Created by Denis on 29.11.14.
//  Copyright (c) 2014 Denis. All rights reserved.
//
typedef struct Mat4x4{
    float x[16];
    int size = 16;
    int lines = 4;
    int columns = 4;
} Mat4x4;

void mult(Mat4x4 &out, Mat4x4 A, Mat4x4 B);

