#ifndef CARCASS_H
#define CARCASS_H

typedef struct VertexSt {
  double *array;
  double *start_arr;
  double *ptr_arr;
  unsigned int amount;  // amount of vertex, multiply by 3 for calloc
  double max_coord;
  double min_max_x[2];
  double min_max_y[2];
  double min_max_z[2];
} VertexTdef;

typedef struct FacetsSt {
  unsigned *array;
  unsigned *ptr_arr;
  unsigned amount;  // amount of polygons, multiply by 2 for calloc
} FacetsTdef;

typedef struct CarcassSt {
  VertexTdef vertexes;
  FacetsTdef facets;
} CarcassTdef;

void freeCarcass(CarcassTdef **crc);
void scaleCarcass(VertexTdef *carcass, double scale);
void translateCoord(VertexTdef *vertexes, unsigned start, double shift);
void rotateX(double *vertexes, unsigned size, double angle);
void rotateY(double *vertexes, unsigned size, double angle);
void rotateZ(double *vertexes, unsigned size, double angle);
void restoreOrigin(VertexTdef *vertexes);

#endif
