#include <math.h>
#include <stdlib.h>

#include "carcass.h"

void freeCarcass(CarcassTdef **crc) {
  if (crc && *crc) {
    free((*crc)->vertexes.array);
    free((*crc)->vertexes.start_arr);
    free((*crc)->facets.array);
    free(*crc);
    *crc = NULL;
  }
}

void scaleCarcass(VertexTdef *vertexes, double scale) {
  if (scale != 0 && vertexes && vertexes->array) {
    for (unsigned i = 0; i < vertexes->amount * 3; i++)
      vertexes->array[i] = vertexes->array[i] * scale;
  }
}

void translateCoord(VertexTdef *vertexes, unsigned start, double shift) {
  if (vertexes && vertexes->array) {
    for (unsigned i = start; i < vertexes->amount * 3; i += 3)
      vertexes->array[i] += shift;
  }
}

void rotateX(double *vertexes, unsigned size, double angle) {
  if (vertexes) {
    for (unsigned i = 0; i < size; i += 3) {
      double tempY = vertexes[i + 1];
      vertexes[i + 1] = tempY * cos(angle) + vertexes[i + 2] * -sin(angle);
      vertexes[i + 2] = tempY * sin(angle) + vertexes[i + 2] * cos(angle);
    }
  }
}

void rotateY(double *vertexes, unsigned size, double angle) {
  if (vertexes) {
    for (unsigned i = 1; i < size; i += 3) {
      double tempX = vertexes[i - 1];
      vertexes[i - 1] = tempX * cos(angle) + vertexes[i + 1] * sin(angle);
      vertexes[i + 1] = tempX * -sin(angle) + vertexes[i + 1] * cos(angle);
    }
  }
}

void rotateZ(double *vertexes, unsigned size, double angle) {
  if (vertexes) {
    for (unsigned i = 2; i < size; i += 3) {
      double tempX = vertexes[i - 2];
      vertexes[i - 2] = tempX * cos(angle) + vertexes[i - 1] * -sin(angle);
      vertexes[i - 1] = tempX * sin(angle) + vertexes[i - 1] * cos(angle);
    }
  }
}

void restoreOrigin(VertexTdef *vertexes) {
  if (vertexes && vertexes->start_arr && vertexes->array) {
    for (unsigned i = 0; i < vertexes->amount * 3; i++)
      vertexes->array[i] = vertexes->start_arr[i];
  }
}
