//
//  FMath.cpp
//  OpenGL
//
//  Created by Denis on 29.11.14.
//  Copyright (c) 2014 Denis. All rights reserved.
//

#include "FMath.h"

void mult(Mat4x4 &out, Mat4x4 A, Mat4x4 B) {
    for(int i=0; i<out.columns*out.lines; i++) {
        out.x[i] = 0;
    }
    for(int i=0; i < A.lines; i++) {
        for(int j=0; j < A.columns; j++) {
            for(int k=0; k < B.lines; k++) {
                out.x[i*4+j] += A.x[i*4+k] * B.x[A.lines*k+j];
            }
        }
    }
}