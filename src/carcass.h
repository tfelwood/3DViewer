#ifndef CARCASS_H
#define CARCASS_H

typedef struct vertex {
  double *array;
  double *Start_arr;
  double *ptrArr;
  unsigned int amount;  // amount of vertex, multiply by 3 for calloc
  double MaxCoord;
  double *minmaxX[2];
  double *minmaxY[2];
  double *minmaxZ[2];
} vertex_st;

typedef struct facets {
  unsigned *array;
  unsigned *ptrArr;
  unsigned amount; // amount of polygons, multiply by 2 for calloc
} facets_st;

typedef struct carcass {
  vertex_st vertexes;
  facets_st facets;
} carcass_st;

//typedef struct coordinates_struct {
//  double rx;
//  double ry;
//  double rz;
//  double scale;
//  double tx;
//  double ty;
//  double tz;
//} state_st;

////typedef struct size_color {
////  double rL;
////  double gL;
////  double bL;
////  double rV;
////  double gV;
////  double bV;
////  double rW;
////  double gW;
////  double bW;
////  double aW;
////  double widthLine;
////  double widthVertex;
////  int typeLine;
////} look_st;

void freeCarcass(struct carcass *crc);
void scaleCarcass (vertex_st *carcass, double scale);
void translateCoord(vertex_st *vertexes, unsigned start, double shift, double scaleCf);
void rotateX(vertex_st *vertexes, double angle);
void rotateY(vertex_st *vertexes, double angle);
void rotateZ(vertex_st *vertexes, double angle);
void restoreOrigin(vertex_st *vertexes);


//void rotateXPoint(double* point, double angle);
//void rotateYPoint(double* point, double angle);
//void rotateZPoint(double* point, double angle);

#endif //CARCASS_H
