#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Matrix Matrix;
struct Matrix
{
    int line;       // Number of line of the matrix
    int col;           // Number of column of the matrix
    double **values;  // Contains the values
};

Matrix* read_line();  // Read input matrix. Columns are seperate by space and Lines by ; eg: (1 2 3; 0 2 4; -1 -1 0; 1 1 5) => matrix 4x3 

Matrix* create_matrix(int line, int col); // Allocate memory

void free_matrix(Matrix *mat);            // Free allocated memory

void display_matrix(Matrix *mat);          // Display matrix

void operation_checker(int line, int col); // Check whether an operation is possible

Matrix* sum_matrix(Matrix *mat1, Matrix *mat2); // Sum two matrix

Matrix* product_matrix(Matrix *mat1, Matrix *mat2); // Product of two matrix

Matrix* reduce_matrix(Matrix *mat,int line, int col); // delete line = line and column  =col from matrix

Matrix* transpose_matrix(Matrix *mat); // Transpose of matrix

void fill_matrix(Matrix *mat, char *clean_data);   // Fill out the matrix

double det_matrix(Matrix *mat);  // Calculate the determinant of matrix

Matrix* com_matrix(Matrix *mat); // Calculate the co-matrix

Matrix* inverse_matrix(Matrix *mat); // Calculate the inverse

void copy_matrix(Matrix *mat1, Matrix *mat2);  // Copy mat2 in mat1

void pow_matrix(Matrix *mat, int n);  // Calculate Matrix^n, n> 1

#endif // MATRIX_H_INCLUDED
