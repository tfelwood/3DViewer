#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <ctype.h>
#include <locale.h>

#include "get_next_line_bonus.h"

#include "parser.h"

int ft_isObjNum(char *str, unsigned pos) {
  return (((str[pos] >= '0' && str[pos] <= '9') ||
           ((str[pos] == '-' || str[pos] == '+') && str[pos - 1] == ' ' &&
            (isdigit((unsigned char)str[pos + 1]))))
              ? 1
              : 0);
}

ParserErrorsEnum ft_facets_count(char *line, CarcassTdef *carcass) {
  ParserErrorsEnum err = kSuccess;
  unsigned pos = 2;
  while (line[pos] != '\0' && line[pos] != '\n' && err == kSuccess) {
    while (ft_isObjNum(line, pos) ||
           (line[pos] == '/' && line[pos - 1] != ' ' && line[pos + 1] != ' ') ||
           ((line[pos] == '+' || line[pos] == '-') && line[pos - 1] == '/'))
      ++pos;
    if (line[pos] != ' ' && line[pos] != '\n' && line[pos] != '\0')
      err = kFileContentError;
    while (line[pos] == ' ') ++pos;
    if (err == kSuccess) ++carcass->facets.amount;
  }
  return err;
}

ParserErrorsEnum ft_read_file(FILE *fp, CarcassTdef *carcass) {
  char *line;
  ParserErrorsEnum error = kSuccess;

  carcass->vertexes.amount = 0;
  carcass->facets.amount = 0;
  if (fp != NULL) {
    while (error == kSuccess && (line = get_next_line(fileno(fp)))) {
      if (line[0] == 'v' && line[1] == ' ') carcass->vertexes.amount++;
      if (line[0] == 'f' && line[1] == ' ')
        error = ft_facets_count(line, carcass);
      free(line);
    }
  } else
    error = kFileOpenError;
  if (error == kSuccess && carcass->vertexes.amount == 0)
    error = kFileContentError;
  return (error);
}

ParserErrorsEnum ft_reserve_space(CarcassTdef *carcass) {
  int error = kSuccess;
  if (!(carcass->vertexes.array =
            calloc(3 * carcass->vertexes.amount, sizeof(double))))
    error = kMallocError;
  carcass->vertexes.ptr_arr = carcass->vertexes.array;
  if (!(carcass->vertexes.start_arr =
            calloc(3 * carcass->vertexes.amount, sizeof(double))))
    error = kMallocError;
  carcass->vertexes.max_coord = 0;
  if (!(carcass->facets.array =
            calloc(2 * carcass->facets.amount, sizeof(unsigned int))))
    error = kMallocError;
  carcass->facets.ptr_arr = carcass->facets.array;
  return error;
}

ParserErrorsEnum fill_Vertex(char *line, CarcassTdef *carcass) {
  char coord[64];
  char *ptrCoord = coord;
  unsigned pos = 2;
  ParserErrorsEnum err = kSuccess;
  int count = 0;

  while (line[pos] != '\0' && line[pos] != '\n' && err == kSuccess) {
    int flag = 1;
    while (ft_isObjNum(line, pos) ||
           (line[pos] == *(localeconv()->decimal_point) &&
            (isdigit((unsigned char)line[pos - 1]) ||
             isdigit((unsigned char)line[pos + 1])) &&
            flag)) {
      if (line[pos] == *(localeconv()->decimal_point)) flag = 0;
      *ptrCoord = line[pos];
      ++pos;
      ++ptrCoord;
    }
    *ptrCoord = '\0';
    ptrCoord = coord;
    if (line[pos] != ' ' && line[pos] != '\n' && line[pos] != '\0')
      err = kFileContentError;
    if (err == kSuccess) {
      *carcass->vertexes.ptr_arr = atof(coord);
      count++;
      if (fabs(*carcass->vertexes.ptr_arr) > carcass->vertexes.max_coord)
        carcass->vertexes.max_coord = *carcass->vertexes.ptr_arr;
      carcass->vertexes.ptr_arr++;
      while (line[pos] == ' ') ++pos;
    }
  }
  if (count != 3 && err == kSuccess) err = kFileContentError;
  return err;
}

ParserErrorsEnum fill_Facets(char *line, CarcassTdef *carcass) {
  char index[64];
  char *ptrIndex = index;
  long long indexNum = 0;
  unsigned firstLast = 0;
  int firstFlag = 1;
  unsigned pos = 2;
  ParserErrorsEnum err = kSuccess;

  while (carcass->facets.amount && line[pos] != '\0' && line[pos] != '\n' &&
         err == kSuccess) {
    while (ft_isObjNum(line, pos)) {
      *ptrIndex = line[pos];
      ++pos;
      ++ptrIndex;
    }
    if (line[pos] == ' ' || line[pos] == '\n' || line[pos] == '\0' ||
        line[pos] == '/') {
      *ptrIndex = '\0';
      ptrIndex = index;
      indexNum = atoll(index) - 1;
      if (indexNum < 0 || indexNum >= carcass->vertexes.amount)
        err = kFileContentError;
      if (firstFlag == 1) {
        firstFlag = 0;
        *carcass->facets.ptr_arr = indexNum;
        firstLast = indexNum;
        carcass->facets.ptr_arr++;
      } else {
        *carcass->facets.ptr_arr++ = indexNum;
        *carcass->facets.ptr_arr++ = indexNum;
      }
      while (ft_isObjNum(line, pos) ||
             (line[pos] == '/' && line[pos - 1] != ' ' && line[pos + 1] != ' '))
        ++pos;
    }
    if (err == kSuccess && line[pos] != ' ' && line[pos] != '\n' &&
        line[pos] != '\0')
      err = kFileContentError;
    while (line[pos] == ' ') ++pos;
  }
  *carcass->facets.ptr_arr = firstLast;
  carcass->facets.ptr_arr++;
  return err;
}

ParserErrorsEnum fill_carcass(FILE *fp, CarcassTdef *carcass) {
  char *line;
  ParserErrorsEnum err = kSuccess;
  if (fp != NULL) {
    while (err == kSuccess && (line = get_next_line(fileno(fp)))) {
      if (line[0] == 'v' && line[1] == ' ') err = fill_Vertex(line, carcass);
      if (err == kSuccess && line[0] == 'f' && line[1] == ' ')
        err = fill_Facets(line, carcass);
      free(line);
    }
  }
  return err;
}

void findMinMax(CarcassTdef *carcass) {
  unsigned i = 0;
  unsigned j = 0;
  carcass->vertexes.ptr_arr = carcass->vertexes.array;
  carcass->vertexes.min_max_x[0] = carcass->vertexes.ptr_arr[0];
  carcass->vertexes.min_max_x[1] = carcass->vertexes.ptr_arr[0];
  carcass->vertexes.min_max_y[0] = carcass->vertexes.ptr_arr[1];
  carcass->vertexes.min_max_y[1] = carcass->vertexes.ptr_arr[1];
  carcass->vertexes.min_max_z[0] = carcass->vertexes.ptr_arr[2];
  carcass->vertexes.min_max_z[1] = carcass->vertexes.ptr_arr[2];

  while (i < (3 * carcass->vertexes.amount)) {
    if (j == 0) {
      if (carcass->vertexes.ptr_arr[i] < carcass->vertexes.min_max_x[0])
        carcass->vertexes.min_max_x[0] = carcass->vertexes.ptr_arr[i];
      if (carcass->vertexes.ptr_arr[i] > carcass->vertexes.min_max_x[1])
        carcass->vertexes.min_max_x[1] = carcass->vertexes.ptr_arr[i];
      j++;
      i++;
    }
    if (j == 1) {
      if (carcass->vertexes.ptr_arr[i] < carcass->vertexes.min_max_y[0])
        carcass->vertexes.min_max_y[0] = carcass->vertexes.ptr_arr[i];
      if (carcass->vertexes.ptr_arr[i] > carcass->vertexes.min_max_y[1])
        carcass->vertexes.min_max_y[1] = carcass->vertexes.ptr_arr[i];
      j++;
      i++;
    }
    if (j == 2) {
      if (carcass->vertexes.ptr_arr[i] < carcass->vertexes.min_max_z[0])
        carcass->vertexes.min_max_z[0] = carcass->vertexes.ptr_arr[i];
      if (carcass->vertexes.ptr_arr[i] > carcass->vertexes.min_max_z[1])
        carcass->vertexes.min_max_z[1] = carcass->vertexes.ptr_arr[i];
      j = 0;
      i++;
    }
  }
}

double coefficient(CarcassTdef *carcass) {
  double coeff = 1;

  double lengthX = fabs(
      (carcass->vertexes.min_max_x[1] - carcass->vertexes.min_max_x[0]) / 2);
  double lengthY = fabs(
      (carcass->vertexes.min_max_y[1] - carcass->vertexes.min_max_y[0]) / 2);
  double lengthZ = fabs(
      (carcass->vertexes.min_max_z[1] - carcass->vertexes.min_max_z[0]) / 2);

  if (lengthX > lengthY && lengthX > lengthZ)
    coeff = lengthX;
  else if (lengthY > lengthX && lengthY > lengthZ)
    coeff = lengthY;
  else
    coeff = lengthZ;
  return coeff;
}

void center_scale(CarcassTdef *carcass) {
  unsigned i = 0;
  unsigned j = 0;
  double centerX =
      carcass->vertexes.min_max_x[0] +
      ((carcass->vertexes.min_max_x[1] - carcass->vertexes.min_max_x[0])) / 2;
  double centerY =
      carcass->vertexes.min_max_y[0] +
      ((carcass->vertexes.min_max_y[1] - carcass->vertexes.min_max_y[0])) / 2;
  double centerZ =
      carcass->vertexes.min_max_z[0] +
      ((carcass->vertexes.min_max_z[1] - carcass->vertexes.min_max_z[0])) / 2;
  double coeff = coefficient(carcass);
  while (i < (3 * carcass->vertexes.amount)) {
    if (j == 0) {
      carcass->vertexes.array[i] =
          (carcass->vertexes.array[i] - centerX) / coeff;
      carcass->vertexes.start_arr[i] = carcass->vertexes.array[i];
      j++;
      i++;
    }
    if (j == 1) {
      carcass->vertexes.array[i] =
          (carcass->vertexes.array[i] - centerY) / coeff;
      carcass->vertexes.start_arr[i] = carcass->vertexes.array[i];
      j++;
      i++;
    }
    if (j == 2) {
      carcass->vertexes.array[i] =
          (carcass->vertexes.array[i] - centerZ) / coeff;
      carcass->vertexes.start_arr[i] = carcass->vertexes.array[i];
      j = 0;
      i++;
    }
  }
}

ParserErrorsEnum parseCarcass(char *file, CarcassTdef **dst_carcass) {
  ParserErrorsEnum error = kSuccess;
  *dst_carcass = NULL;
  FILE *fp = fopen(file, "r");
  if (fp == NULL) error = kFileOpenError;

  if ((*dst_carcass = calloc(1, sizeof(CarcassTdef))) == NULL)
    error = kMallocError;
  if (error == kSuccess &&
      ((error = ft_read_file(fp, *dst_carcass)) == kSuccess)) {
    if (fseek(fp, 0, SEEK_SET) < 0) error = kFileOpenError;
    if (error == kSuccess) error = ft_reserve_space(*dst_carcass);
    if (error == kSuccess &&
        (error = fill_carcass(fp, *dst_carcass)) == kSuccess) {
      findMinMax(*dst_carcass);
      center_scale(*dst_carcass);
    }
  }
  if (fp) ft_gnl_free_fd(fileno(fp));
  fclose(fp);
  if (error != kSuccess) freeCarcass(dst_carcass);
  return error;
}
