#ifndef PARSER_H
#define PARSER_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "carcass.h"

typedef enum ParserErrors {
  kSuccess,
  kFileOpenError,
  kMallocError,
  kFileContentError,
} ParserErrorsEnum;

int ft_isObjNum(char *str, unsigned pos);
ParserErrorsEnum parseCarcass(char *file, CarcassTdef **dst_carcass);
ParserErrorsEnum ft_read_file(FILE *fp, CarcassTdef *carcass);
ParserErrorsEnum ft_facets_count(char *line, CarcassTdef *carcass);
ParserErrorsEnum ft_reserve_space(CarcassTdef *carcass);
ParserErrorsEnum fill_carcass(FILE *fp, CarcassTdef *carcass);
ParserErrorsEnum fill_Vertex(char *line, CarcassTdef *carcass);
ParserErrorsEnum fill_Facets(char *line, CarcassTdef *carcass);
void findMinMax(CarcassTdef *carcass);
void center_scale(CarcassTdef *carcass);
double coefficient(CarcassTdef *carcass);

#endif
