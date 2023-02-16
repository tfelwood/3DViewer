//
// Created by Aleksey Tvorogov on 20/01/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "carcass.h"

void freeCarcass(carcass_st *crc)
{
  if (crc){
      free(crc->vertexes.array);
      free(crc->vertexes.Start_arr);
      free(crc->facets.array);
      free(crc);
      crc = NULL;
  }
}

void scaleCarcass (vertex_st *vertexes, double scale){
    if (scale != 0 && vertexes && vertexes->array){
        for(unsigned i = 0; i < vertexes->amount * 3; i++)
          vertexes->array[i] = vertexes->array[i] * scale;
    }
}

void translateCoord(vertex_st *vertexes, unsigned start, double shift, double scaleCf){
    if (vertexes && vertexes->array && scaleCf != 0){
        for (unsigned i = start; i < vertexes->amount * 3; i += 3)
            vertexes->array[i] += scaleCf * shift;
    }
}

void rotateX(vertex_st *vertexes, double angle){
    if (vertexes && vertexes->array){
        for (unsigned i = 0; i < vertexes->amount * 3; i+=3){
            double tempY = vertexes->array[i + 1];
            vertexes->array[i + 1] = tempY * cos(angle) + vertexes->array[i + 2] * -sin(angle);
            vertexes->array[i + 2] = tempY * sin(angle) + vertexes->array[i + 2] * cos(angle);
        }
    }
}

void rotateY(vertex_st *vertexes, double angle){
    if (vertexes && vertexes->array){
        for (unsigned i = 1; i < vertexes->amount * 3; i+=3){
            double tempX = vertexes->array[i - 1];
            vertexes->array[i - 1] = tempX * cos(angle) + vertexes->array[i + 1] * sin(angle);
            vertexes->array[i + 1] = tempX * -sin(angle) + vertexes->array[i + 1] * cos(angle);
        }
    }
}

void rotateZ(vertex_st *vertexes, double angle){
    if (vertexes && vertexes->array){
        for (unsigned i = 2; i < vertexes->amount * 3; i+=3){
            double tempX = vertexes->array[i - 2];
            vertexes->array[i - 2] = tempX * cos(angle) + vertexes->array[i - 1] * -sin(angle);
            vertexes->array[i - 1] = tempX * sin(angle) + vertexes->array[i - 1] * cos(angle);
        }
    }
}

void restoreOrigin(vertex_st *vertexes){
    if (vertexes && vertexes->Start_arr && vertexes->array){
        for (unsigned i = 0; i < vertexes->amount * 3; i++)
            vertexes->array[i] = vertexes->Start_arr[i];
    }
}

//void rotateXPoint(double* point, double angle){
//    unsigned i = 0;
//    double tempY = point[i + 1];
//    point[i + 1] = tempY * cos(angle) + point[i + 2] * sin(angle);
//    point[i + 2] = tempY * -sin(angle) + point[i + 2] * cos(angle);
//}

//void rotateYPoint(double *point, double angle){
//    unsigned i = 1;
//    double tempX = point[i - 1];
//    point[i - 1] = tempX * cos(angle) + point[i + 1] * -sin(angle);
//    point[i + 1] = tempX * sin(angle) + point[i + 1] * cos(angle);
//}

//void rotateZPoint(double *point, double angle){
//    unsigned i = 2;
//    double tempX = point[i - 2];
//    point[i - 2] = tempX * cos(angle) + point[i - 1] * sin(angle);
//    point[i - 1] = tempX * -sin(angle) + point[i - 1] * cos(angle);
//}



//void rotateX(vertex_st *vertexes, double angle){
//    for (unsigned i = 0; i < vertexes->amount * 3; i+=3){
//        double tempY = vertexes->Start_arr[i + 1];
//        //vertexes->array[i] = vertexes->array[i];
//        vertexes->array[i + 1] = vertexes->Start_arr[i + 1] * cos(angle) - vertexes->Start_arr[i + 2] * sin(angle);
//        vertexes->array[i + 2] = tempY * sin(angle) + vertexes->Start_arr[i + 2] * cos(angle);
//    }
//}



//void rotateY(vertex_st *vertexes, double angle){
//    for (unsigned i = 1; i < vertexes->amount * 3; i+=3){
//        double tempX = vertexes->Start_arr[i - 1];
//        vertexes->array[i - 1] = vertexes->Start_arr[i - 1] * cos(angle) + vertexes->Start_arr[i + 1] * sin(angle);
//        vertexes->array[i + 1] = - tempX * sin(angle) + vertexes->Start_arr[i + 1] * cos(angle);
//    }
//}



//void rotateZ(vertex_st *vertexes, double angle){
//    for (unsigned i = 2; i < vertexes->amount * 3; i+=3){
//        double tempX = vertexes->Start_arr[i - 2];
//        vertexes->array[i - 2] = vertexes->Start_arr[i - 2] * cos(angle) - vertexes->Start_arr[i - 1] * sin(angle);
//        vertexes->array[i - 1] = tempX * sin(angle) + vertexes->Start_arr[i - 1] * cos(angle);
//    }
//}



