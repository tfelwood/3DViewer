//
// Created by Aleksey Tvorogov on 14/01/2023.
//

#ifndef PARSER_H
#define PARSER_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "carcass.h"

//typedef struct vertex {
//  double *array;
//  double *Start_arr;
//  double *ptrArr;
//  int amount;  // amount of vertex, multiply by 3 for calloc
//  double MaxCoord;
//  double minmaxX[2];
//  double minmaxY[2];
//  double minmaxZ[2];
//} vertex_st;
//
//typedef struct facets {
//  unsigned *array;
//  unsigned *ptrArr;
//  unsigned amount; // amount of polygons, multiply by 2 for calloc
//} facets_st;
//
//typedef struct carcass {
//  vertex_st vertexes;
//  facets_st facets;
//} carcass_st;

//void freeCarcass(struct carcass *crc);

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

typedef struct coordinates_struct {
  double rx;
  double ry;
  double rz;
  double scale;
  double tx;
  double ty;
  double tz;
} state_st;

//typedef	enum	e_errors
//{
//  NO_ERROR,
//  BAD_FILE_FORMAT,
//  BAD_DESCRIPTOR,
//  BAD_ARGS,
//}	t_errors;

//typedef struct size_color {
//  double rL;
//  double gL;
//  double bL;
//  double rV;
//  double gV;
//  double bV;
//  double rW;
//  double gW;
//  double bW;
//  double aW;
//  double widthLine;
//  double widthVertex;
//  int typeLine;
//} look_st;

// Парсер
carcass_st *parseCarcass(char *file);
//int ft_read_file(char *file, carcass_st *carcass);
int ft_read_file(FILE *fp, carcass_st *carcass);
void ft_facets_count (char *line, carcass_st *carcass);
int ft_isObjNum(unsigned char value);
int ft_reserve_space (carcass_st *carcass);
//void fill_carcass(char *file, carcass_st *carcass);
void fill_carcass(FILE *fp, carcass_st *carcass);
void fill_Vertex(char *line, carcass_st *carcass);
void fill_Facets(char *line, carcass_st *carcass);
void findMinMax(carcass_st *carcass);
void center_scale(carcass_st *carcass);
double centerCoordCount(double min, double max);
double coefficient(carcass_st *carcass);

// Функции меняющие состояния
void s21_remove_matrix(matrix_t *A);
void s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
void stateСhange(vertex_st *vertex, state_st *state);
matrix_t *countMatrix(state_st *state);
matrix_t *s21_create_matrix(int rows, int columns);
matrix_t *matrixOX (double align);
matrix_t *matrixOY (double align);
matrix_t *matrixOZ (double align);
matrix_t *scaling(double scale);

#endif //PARSER_H
