#ifndef RENDERHELPER_H
#define RENDERHELPER_H

#include <vector>
#include <simd/simd.h>

template<class T>
static void appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T> vectorToAppend) {
    for(T item : vectorToAppend) {
        vectorToAppendTo->push_back(item);
    }
}

static matrix_float4x4 calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar) {
    float n = zNear, r = 0, l = 0, t = 0, b = 0, f = zFar;
    
    float scale = tan(FOV * 0.5 * M_PI / 180) * n;
    r = aspectRatio * scale;
    l = -r; 
    t = scale;
    b = -t;
    
    matrix_float4x4 perspectiveMatrix = matrix_identity_float4x4;

    simd_float4 column1 = simd_float4();
    column1[0] = 2*n/(r-l);
    column1[1] = 0;
    column1[2] = 0;
    column1[3] = 0;

    simd_float4 column2 = simd_float4();
    column2[0] = 0;
    column2[1] = 2*n/(t-b);
    column2[2] = 0;
    column2[3] = 0;

    simd_float4 column3 = simd_float4();
    column3[0] = (r+l)/(r-l);
    column3[1] = (t+b)/(t-b);
    column3[2] = -1*(f+n)/(f-n);
    column3[3] = -1;

    simd_float4 column4 = simd_float4();
    column4[0] = 0;
    column4[1] = 0;
    column4[2] = -2*f*n/(f-n);
    column4[3] = 0;

    perspectiveMatrix.columns[0] = column1;
    perspectiveMatrix.columns[1] = column2;
    perspectiveMatrix.columns[2] = column3;
    perspectiveMatrix.columns[3] = column4;
    return perspectiveMatrix;
}

static matrix_float3x3 multiplyTwo3x3Matrices(matrix_float3x3 m1, matrix_float3x3 m2) {
    matrix_float3x3 result = matrix_float3x3();
    simd_float3 col1 = simd_float3();
    simd_float3 col2 = simd_float3();
    simd_float3 col3 = simd_float3();

    col1[0] = m1.columns[0][0] * m2.columns[0][0] + m1.columns[1][0] * m2.columns[0][1] + m1.columns[2][0] * m2.columns[0][2];
    col1[1] = m1.columns[0][1] * m2.columns[0][0] + m1.columns[1][1] * m2.columns[0][1] + m1.columns[2][1] * m2.columns[0][2];
    col1[2] = m1.columns[0][2] * m2.columns[0][0] + m1.columns[1][2] * m2.columns[0][1] + m1.columns[2][2] * m2.columns[0][2];
    
    col2[0] = m1.columns[0][0] * m2.columns[1][0] + m1.columns[1][0] * m2.columns[1][1] + m1.columns[2][0] * m2.columns[1][2];
    col2[1] = m1.columns[0][1] * m2.columns[1][0] + m1.columns[1][1] * m2.columns[1][1] + m1.columns[2][1] * m2.columns[1][2];
    col2[2] = m1.columns[0][2] * m2.columns[1][0] + m1.columns[1][2] * m2.columns[1][1] + m1.columns[2][2] * m2.columns[1][2];
    
    col3[0] = m1.columns[0][0] * m2.columns[2][0] + m1.columns[1][0] * m2.columns[2][1] + m1.columns[2][0] * m2.columns[2][2];
    col3[1] = m1.columns[0][1] * m2.columns[2][0] + m1.columns[1][1] * m2.columns[2][1] + m1.columns[2][1] * m2.columns[2][2];
    col3[2] = m1.columns[0][2] * m2.columns[2][0] + m1.columns[1][2] * m2.columns[2][1] + m1.columns[2][2] * m2.columns[2][2];
    
    result.columns[0] = col1;
    result.columns[1] = col2;
    result.columns[2] = col3;
    return result;
}

static matrix_float3x3 addTwo3x3Matrices(matrix_float3x3 m1, matrix_float3x3 m2) {
    matrix_float3x3 result = matrix_float3x3();
    simd_float3 col1 = simd_float3();
    simd_float3 col2 = simd_float3();
    simd_float3 col3 = simd_float3();

    col1[0] = m1.columns[0][0] + m2.columns[0][0];
    col1[1] = m1.columns[0][1] * m2.columns[0][1];
    col1[2] = m1.columns[0][2] * m2.columns[0][2];

    col2[0] = m1.columns[1][0] + m2.columns[1][0];
    col2[1] = m1.columns[1][1] * m2.columns[1][1];
    col2[2] = m1.columns[1][2] * m2.columns[1][2];    

    col3[0] = m1.columns[2][0] + m2.columns[2][0];
    col3[1] = m1.columns[2][1] * m2.columns[2][1];
    col3[2] = m1.columns[2][2] * m2.columns[2][2]; 
    
    result.columns[0] = col1;
    result.columns[1] = col2;
    result.columns[2] = col3;
    return result;
}

static matrix_float3x3 calculateXRotationMatrix(double xRotation) {
    matrix_float3x3 rotationMatrix = matrix_float3x3();

    double xRads = xRotation * M_PI / 180;

    simd_float3 column1 = simd_float3();
    column1[0] = cos(xRads);
    column1[1] = 0;
    column1[2] = -sin(xRads);

    simd_float3 column2 = simd_float3();
    column2[0] = 0;
    column2[1] = 1;
    column2[2] = 0;

    simd_float3 column3 = simd_float3();
    column3[0] = sin(xRads);
    column3[1] = 0;
    column3[2] = cos(xRads);

    rotationMatrix.columns[0] = column1;
    rotationMatrix.columns[1] = column2;
    rotationMatrix.columns[2] = column3;

    return rotationMatrix;
}

static matrix_float3x3 calculateYRotationMatrix(double yRotation) {

    matrix_float3x3 rotationMatrix = matrix_float3x3();

    double yRads = yRotation * M_PI / 180;

    simd_float3 column1 = simd_float3();
    column1[0] = 1;
    column1[1] = 0;
    column1[2] = 0;

    simd_float3 column2 = simd_float3();
    column2[0] = 0;
    column2[1] = cos(yRads);
    column2[2] = sin(yRads);

    simd_float3 column3 = simd_float3();
    column3[0] = 0;
    column3[1] = -sin(yRads);
    column3[2] = cos(yRads);

    rotationMatrix.columns[0] = column1;
    rotationMatrix.columns[1] = column2;
    rotationMatrix.columns[2] = column3;

    return rotationMatrix;
}

#endif