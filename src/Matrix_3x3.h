#ifndef MATRIX3_3_H_INCLUDED
#define MATRIX3_3_H_INCLUDED

#include "Vector3D.h"

class Matrix_3x3
{
    public:
        float matrix_3x3[3][3];


    Matrix_3x3()
    {
        for(int i=0;i<4;i++)
        {
            for(int j =0;j<4;j++)
            {
                matrix_3x3[i][j] = 0.0;
            }
        }
    }

    Vector3D vectMatrixMult(Matrix_3x3 m, Vector3D v)
	{
        float newX = v.x*m.matrix_3x3[1][1] + v.y*m.matrix_3x3[2][1] + v.z*m.matrix_3x3[3][1];
        float newY = v.x*m.matrix_3x3[1][2] + v.y*m.matrix_3x3[2][2] + v.z*m.matrix_3x3[3][2];
        float newZ = v.x*m.matrix_3x3[1][3] + v.y*m.matrix_3x3[2][3] + v.z*m.matrix_3x3[3][3];
		return Vector3D(newX, newY, newZ);
	}







}

#endif // MATRIX3_3_H_INCLUDED
