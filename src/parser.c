//
// Created by Aleksey Tvorogov on 14/01/2023.
//

#include "parser.h"
#include "get_next_line_bonus.h"
#include <sys/fcntl.h>

///**
//* @brief create new matrix
//*
//* @param rows
//* @param columns
//* @return new matrix_t
//*/
//matrix_t *s21_create_matrix(int rows, int columns) {
//  matrix_t *result = malloc(1 * sizeof(matrix_t));
//  double **row = malloc(rows * sizeof(double *));
//  double **ptrRow = row;
//  for (int i = 0; i < rows; i++) {
//	*ptrRow = calloc(columns, sizeof(double));
//	ptrRow++;
//  }
//  result->matrix = row;
//  result->rows = rows;
//  result->columns = columns;
//  return result;
//}

///**
// * @brief remove matrix and free all dynamic memory
// *
// * @param  matrix pointer
// */
//void s21_remove_matrix(matrix_t *A) {
//  for (int i = 0; i < A->rows; i++) {
//	free(A->matrix[i]);
//  }
//  free(A->matrix);
//  free(A);
//}

///**
// * @brief  multiplication of two matrices
// *
// * @param pointer first matrix
// * @param pointer second matrix
// * @param pointer matrix for record result
// * @return pointer matrix result
// */
//void s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
//  for (int i = 0; i < A->rows; i++) {
//	for (int j = 0; j < B->columns; j++) {
//	  result->matrix[i][j] = 0;
//	  for (int k = 0; k < B->rows; k++) {
//		result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
//	  }
//	}
//  }
//}

///**
// * @brief creating a matrix for affine transformations along the x-axis
// *
// * @param degree of displacement
// * @return matrix for affine transformations
// */
//matrix_t *matrixOX(double align) {
//  matrix_t *OXMatrix = s21_create_matrix(4, 4);
//  OXMatrix->matrix[0][0] = 1;
//  OXMatrix->matrix[1][1] = cos(align);
//  OXMatrix->matrix[1][2] = -sin(align);
//  OXMatrix->matrix[2][1] = sin(align);
//  OXMatrix->matrix[2][2] = cos(align);
//  OXMatrix->matrix[3][3] = 1;
//  return OXMatrix;
//}
///**
// * @brief creating a matrix for affine transformations along the y-axis
// *
// * @param degree of displacement
// * @return matrix for affine transformations
// */
//matrix_t *matrixOY(double align) {
//  matrix_t *OYMatrix = s21_create_matrix(4, 4);
//  OYMatrix->matrix[0][0] = cos(align);
//  OYMatrix->matrix[0][2] = sin(align);
//  OYMatrix->matrix[1][1] = 1;
//  OYMatrix->matrix[2][0] = -sin(align);
//  OYMatrix->matrix[2][2] = cos(align);
//  OYMatrix->matrix[3][3] = 1;
//  return OYMatrix;
//}

///**
// * @brief creating a matrix for affine transformations along the z-axis
// *
// * @param degree of displacement
// * @return matrix for affine transformations
// */
//matrix_t *matrixOZ(double align) {
//  matrix_t *OZMatrix = s21_create_matrix(4, 4);
//  OZMatrix->matrix[0][0] = cos(align);
//  OZMatrix->matrix[0][1] = -sin(align);
//  OZMatrix->matrix[1][0] = sin(align);
//  OZMatrix->matrix[1][1] = cos(align);
//  OZMatrix->matrix[2][2] = 1;
//  OZMatrix->matrix[3][3] = 1;
//  return OZMatrix;
//}

///**
// * @brief creating a matrix for affine transformations. decrease/increase
// *
// * @param degree of displacement
// * @return matrix for affine transformations
// */
//matrix_t *scaling(double scale) {
//  matrix_t *scaleMatrix = s21_create_matrix(4, 4);
//  scaleMatrix->matrix[0][0] = scale;
//  scaleMatrix->matrix[1][1] = scale;
//  scaleMatrix->matrix[2][2] = scale;
//  scaleMatrix->matrix[3][3] = 1;
//  return scaleMatrix;
//}

///**
// * @brief creating a matrix to calculate new coordinates shifted by a given
// * offset
// *
// * @param structure with degrees to which the shift is made
// * @return matrix for counting new coordinates
// */
//matrix_t *countMatrix(state_st *state) {
//  matrix_t *OXMatrix = matrixOX(state->rx);
//  matrix_t *OYMatrix = matrixOY(state->ry);
//  matrix_t *OZMatrix = matrixOZ(state->rz);
//  matrix_t *scaleMaitrix = scaling(state->scale);
//  matrix_t *resultXY = s21_create_matrix(4, 4);
//  matrix_t *resultXYZ = s21_create_matrix(4, 4);
//  matrix_t *resultXYZScale = s21_create_matrix(4, 4);
//  matrix_t *resultMatrix = s21_create_matrix(4, 4);

//  s21_mult_matrix(OXMatrix, OYMatrix, resultXY);
//  s21_mult_matrix(resultXY, OZMatrix, resultXYZ);
//  s21_mult_matrix(resultXYZ, scaleMaitrix, resultMatrix);

//  s21_remove_matrix(OXMatrix);
//  s21_remove_matrix(OYMatrix);
//  s21_remove_matrix(OZMatrix);
//  s21_remove_matrix(scaleMaitrix);
//  s21_remove_matrix(resultXY);
//  s21_remove_matrix(resultXYZ);
//  s21_remove_matrix(resultXYZScale);

//  return resultMatrix;
//}

//void	ft_check_errors_type(enum e_errors err)
//{
//  if (!err)
//	return ;
//  ft_putstr_fd("Error\n", 2);
//  else if (err == BAD_FILE_FORMAT)
//	ft_putstr_fd("Bad file format, should be *.obj\n", 2);
//  else if (err == BAD_DESCRIPTOR)
//	ft_putstr_fd("Сould not open the file, bad fd\n", 2);
//  exit(err);
//}

//t_errors	ft_check_file_format(char *argv)
//{
//  if (!ft_strncmp(&argv[ft_strlen(argv) - 4], ".obj", 4))
//	return (NO_ERROR);
//  return (BAD_FILE_FORMAT);
//}

int ft_isObjNum(unsigned char value) {
  if ((value >= '0' && value <= '9') || value == ',' || value == '.' ||
	  value == '-' || value == '+')
	return (1);
  return (0);
}


void ft_facets_count (char *line, carcass_st *carcass) {
  while (*line != '\0') {
	if (ft_isObjNum(*line)) {
	  while (ft_isObjNum(*line)) {
		line++;
	  }
	  carcass->facets.amount++;
	  while (*line != ' ' && *line != '\n' && *line != '\0') {
		line++;
	  }
	}
    line++;
  }
}

int ft_read_file(FILE *fp, carcass_st *carcass)
{
  //char line[512];
    char* line;
  //FILE *fp;

  carcass->vertexes.amount = 0;
  carcass->facets.amount = 0;
  //fp = fopen(file, "r");
  if (fp != NULL)
  {
    //while (fgets(line, 512, fp) != NULL)
      while ((line = get_next_line(fp->_file)))
	{
	  if (line[0] == 'v' && line[1] == ' ')
		carcass->vertexes.amount++;
	  if (line[0] == 'f' && line[1] == ' ')
		ft_facets_count(line, carcass);
      free(line);
	}
    //fclose(fp);
    //rewind(fp); // instead of fclose to jump to begining of the file
    //fseek (fp, 0, SEEK_SET);
  }
  else
	return(1);
  if (carcass->vertexes.amount == 0)
	printf ("file have no object to draw\n");
  printf ("vertexes: %i\n facets: %i\n", carcass->vertexes.amount,
		  carcass->facets.amount);
  return (0);
}

/**
 * @brief reserve the memory for arrays
 *
 * @param structure
 * @return 0 if all OK, 1 if memory allocation fail
 */

int ft_reserve_space (carcass_st *carcass){
  carcass->vertexes.array = calloc(3 * carcass->vertexes.amount, sizeof
  (double)); //multiply by 3 as vertex has 3 coordinates X Y Z
  if (!carcass->vertexes.array)
	return (1);
  carcass->vertexes.ptrArr = carcass->vertexes.array;
  carcass->vertexes.Start_arr = calloc(3 * carcass->vertexes.amount, sizeof(double));
  if (!carcass->vertexes.Start_arr)
	return (1);
  carcass->vertexes.MaxCoord = 0;

  carcass->facets.array = calloc(2 * carcass->facets.amount, sizeof
  (unsigned)); // multiply by 2 as we double facets Numbers to be connected
  // for Drawing
  if (!carcass->facets.array)
	return (1);
  carcass->facets.ptrArr = carcass->facets.array;
  return (0);
}

void fill_Vertex(char *line, carcass_st *carcass) {
    char coord[64];
    char *ptrCoord = coord;

    while (*line != '\0') {
        if (ft_isObjNum(*line)) {
            while (ft_isObjNum(*line)) {
                *ptrCoord = *line;
                line++;
                ptrCoord++;
            }
            *ptrCoord = '\0';
            ptrCoord = coord;
            *carcass->vertexes.ptrArr = atof(coord);
            if (fabs(*carcass->vertexes.ptrArr) > carcass->vertexes.MaxCoord)
                carcass->vertexes.MaxCoord = *carcass->vertexes.ptrArr;
            carcass->vertexes.ptrArr++;
            while (*line != ' ' && *line != '\n' && *line != '\0') {
                line++;
            }
        }
        line++;
    }
}

void fill_Facets(char *line, carcass_st *carcass) {
    char index[64];
    char *ptrIndex = index;
    int indexNum = 0;
    int firstLast = 0;
    int firstFlag = 1;

    while (*line != '\0') {
        if (ft_isObjNum(*line)) {
            while (ft_isObjNum(*line)) {
                *ptrIndex = *line;
                line++;
                ptrIndex++;
            }
            *ptrIndex = '\0';
            ptrIndex = index;
            indexNum = atof(index) - 1;
            if (firstFlag == 1) {
                firstFlag = 0;
                *carcass->facets.ptrArr = indexNum;
                firstLast = indexNum;
                //printf("IndexNum first %i\n", *carcass->facets.ptrArr);
                carcass->facets.ptrArr++;
            } else {
                *carcass->facets.ptrArr = indexNum;
                carcass->facets.ptrArr++;
                *carcass->facets.ptrArr = indexNum;
                //printf("IndexNum %i\n", *carcass->facets.ptrArr);
                carcass->facets.ptrArr++;
            }
            while (*line != ' ' && *line != '\n' && *line != '\0') {
                line++;
            }
        }
        line++;
    }
    *carcass->facets.ptrArr = firstLast;
    carcass->facets.ptrArr++;
}

//void fill_carcass(char *file, carcass_st *carcass)
void fill_carcass(FILE *fp, carcass_st *carcass)
{
  //char line[512];
  char *line;
  //FILE *fp;

  //fp = fopen(file, "r");
  if (fp != NULL)
  {
    //while (fgets(line, 512, fp) != NULL)
      while ((line = get_next_line(fp->_file)))
	{
	  if (line[0] == 'v' && line[1] == ' ')
		fill_Vertex(line, carcass);
	  if (line[0] == 'f' && line[1] == ' ')
		fill_Facets(line, carcass);
      free(line);
	}
    //fclose(fp);
  }
  else
      printf("File corrupted or doesnt exist\n");
}

void findMinMax(carcass_st *carcass)
{
    unsigned i = 0;
    unsigned j = 0;
    carcass->vertexes.ptrArr = carcass->vertexes.array;
    carcass->vertexes.minmaxX[0] = &carcass->vertexes.ptrArr[0]; // начальные значения
    carcass->vertexes.minmaxX[1] = &carcass->vertexes.ptrArr[0];
    carcass->vertexes.minmaxY[0] = &carcass->vertexes.ptrArr[1];
    carcass->vertexes.minmaxY[1] = &carcass->vertexes.ptrArr[1];
    carcass->vertexes.minmaxZ[0] = &carcass->vertexes.ptrArr[2];
    carcass->vertexes.minmaxZ[1] = &carcass->vertexes.ptrArr[2];

    while(i < (3 * carcass->vertexes.amount)){
        if (j == 0){
            if (carcass->vertexes.ptrArr[i] < *carcass->vertexes.minmaxX[0])
                carcass->vertexes.minmaxX[0] = &carcass->vertexes.ptrArr[i];
            if (carcass->vertexes.ptrArr[i] > *carcass->vertexes.minmaxX[1])
                carcass->vertexes.minmaxX[1] = &carcass->vertexes.ptrArr[i];
            j++;
            i++;
        }
        if (j == 1){
            if (carcass->vertexes.ptrArr[i] < *carcass->vertexes.minmaxY[0])
                carcass->vertexes.minmaxY[0] = &carcass->vertexes.ptrArr[i];
            if (carcass->vertexes.ptrArr[i] > *carcass->vertexes.minmaxY[1])
                carcass->vertexes.minmaxY[1] = &carcass->vertexes.ptrArr[i];
            j++;
            i++;
        }
        if (j == 2){
            if (carcass->vertexes.ptrArr[i] < *carcass->vertexes.minmaxZ[0])
                carcass->vertexes.minmaxZ[0] = &carcass->vertexes.ptrArr[i];
            if (carcass->vertexes.ptrArr[i] > *carcass->vertexes.minmaxZ[1])
                carcass->vertexes.minmaxZ[1] = &carcass->vertexes.ptrArr[i];
            j=0;
            i++;
        }
    }
}

double coefficient(carcass_st *carcass){
  double lengthX = fabs((*carcass->vertexes.minmaxX[1] - *carcass->vertexes
	  .minmaxX[0])/2);
  double lengthY = fabs((*carcass->vertexes.minmaxY[1] - *carcass->vertexes
	  .minmaxY[0])/2);
  double lengthZ = fabs((*carcass->vertexes.minmaxZ[1] - *carcass->vertexes
	  .minmaxZ[0])/2);

  if (lengthX > lengthY && lengthX > lengthZ)
	return(lengthX);
  if (lengthY > lengthX && lengthY > lengthZ)
	return(lengthY);
  else
	return(lengthZ);
//  if (lengthZ > lengthX && lengthZ > lengthY)
//	return(lengthZ);
}

double centerCoordCount(double min, double max){
    return (min + (max - min) / 2);
}

void center_scale(carcass_st *carcass){
  unsigned i = 0;
  unsigned j = 0;
  double centerX = *carcass->vertexes.minmaxX[0] + ((*carcass->vertexes
      .minmaxX[1] - *carcass->vertexes.minmaxX[0]))/2;
  //printf("CenterX %f\n", centerX);
  double centerY = *carcass->vertexes.minmaxY[0] + ((*carcass->vertexes
      .minmaxY[1] - *carcass->vertexes.minmaxY[0]))/2;
  //printf("CenterY %f\n", centerY);
  double centerZ = *carcass->vertexes.minmaxZ[0] + ((*carcass->vertexes
      .minmaxZ[1] - *carcass->vertexes.minmaxZ[0]))/2;
  //printf("CenterZ %f\n", centerZ);
  double coeff = coefficient(carcass);
  //printf("Coefficient %f\n", coeff);

//  for (int i = 0; i < ( 3 * carcass->vertexes.amount); i++){
//	carcass->vertexes.array[i] =
//  }
  while(i < (3 * carcass->vertexes.amount)){
	if (j == 0){
	  carcass->vertexes.array[i] = (carcass->vertexes.array[i] - centerX)/coeff;
	  carcass->vertexes.Start_arr[i] = carcass->vertexes.array[i];
	  j++;
	  i++;
	}
	if (j == 1){
	  carcass->vertexes.array[i] = (carcass->vertexes.array[i] - centerY)/coeff;
	  carcass->vertexes.Start_arr[i] = carcass->vertexes.array[i];
	  j++;
	  i++;
	}
	if (j == 2){
	  carcass->vertexes.array[i] = (carcass->vertexes.array[i] - centerZ)/coeff;
	  carcass->vertexes.Start_arr[i] = carcass->vertexes.array[i];
	  j=0;
	  i++;
	}
  }
}

/**
 * @brief parse carcass structure which is containing Vertexes and indexes,
 * resized and centered to fit the screen
 *
 * @param file
 * @return parsed carcass structure
 */

carcass_st *parseCarcass(char *file)
{
  //t_errors err;
//  int i = 0;

  //err = NO_ERROR;
 //err = ft_check_file_format(filename);
   FILE *fp = fopen(file, "r");

  carcass_st *carcass = malloc(1 * sizeof(carcass_st));
  //if (ft_read_file(file, carcass))
  if (ft_read_file(fp, carcass))
  {
    //free(file);
	free(carcass);

	printf("Bad file\n");
    return(NULL);
//	exit(1);
  }
  fseek (fp, 0, SEEK_SET);
  if (ft_reserve_space(carcass)) //memory allocation
    printf("No space to allocate the Memory, calloc fail");
  //fill_carcass(file, carcass); // fill the carcass
  fill_carcass(fp, carcass);
  fclose(fp);
//  printf("Array of Vertexes\n");
//  for(i = 0; i < (3 * carcass->vertexes.amount); ++i)
//        printf("%f ", carcass->vertexes.array[i]);
//    printf("\n ");
  findMinMax(carcass);
//  printf("MaxCoord: %f\n", carcass->vertexes.MaxCoord);
//  printf(" MinX: %f\n MaxX: %f\n MinY: %f\n MaxY: %f\n MinZ: %f\n MaxZ: %f\n", carcass->vertexes.minmaxX[0],
//           carcass->vertexes.minmaxX[1], carcass->vertexes.minmaxY[0], carcass->vertexes.minmaxY[1],carcass->vertexes.minmaxZ[0], carcass->vertexes.minmaxZ[1]);
//  printf("Array of Facets indexes\n");
//  for(i = 0; i < (2 * carcass->facets.amount); ++i)
//        printf("%i ", carcass->facets.array[i]);
  //  free(file);
//  ft_check_errors_type(err);
//  printf("Array of scaled and resized Vertexes\n");
  center_scale(carcass);
//    for(i = 0; i < (3 * carcass->vertexes.amount); ++i)
//        printf("%f ", carcass->vertexes.array[i]);
  return carcass;
//  return 0;
}

//int main(int argc,char **argv)
//{
//  parseCarcass(argv[1]);
//  return 0;
//}



//void freeCarcass(carcass_st *crc)
//{
//    if (!crc)
//        return ;
//    free(crc->vertexes.array);
//    free(crc->vertexes.Start_arr);
//    free(crc->facets.array);
//    free(crc);
//}
