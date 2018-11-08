#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "matrix.h"
//TODO
//Create a matrix as columns of vectors?
//Check if okay to malloc(0) in copyArray
//Overhead from recalling error checking several times in for loops
//Consistency with accessing rows and columns versus i and j
//Checking for null matrix and null vector accessing garbage giving false positives?
//Create difference between row and column
//Make mulitplication identify which product to use
//Transpose function
//Create identity matrices
//Change swap function by copying array instead of entire vector
//Constructors with possible bool arguments for initialization
//Change Matrix struct to have a pointer of pointers of arrays
//Can I optimize the elementary operations (like addRows)
//Organize error checking
//Function previews when typing them in autocomplete for sublime text

/*******Getters*********/
bool isNullVector(Vector vector) {
	if(vector.m <= 0) {
		printf("Using a null vector.\n");
		return true;
	}

	return false;
}

bool isNullMatrix(Matrix matrix) {
	if(matrix.m <= 0 || matrix.n <= 0) {
		printf("Using a null matrix.\n");
		return true;
	}

	return false;
}

bool isVectorOutOfBounds(Vector vector, int row) {
	if(row <= 0 || row > vector.m) {
		printf("Vector out of bounds.\n");
		return true;
	}

	return false;
}

bool isMatrixOutOfBounds(Matrix matrix, int row, int col) {
	if(row <= 0 || row > matrix.m  || col <= 0 || col > matrix.n) {
		printf("Matrix out of bounds.\n");
		return true;
	}

	return false;
}

//Calling the argument for the entry index "row" because vectors are
//to be visualized as column vectors
float getVectorElem(Vector vector, int row) {
	//Note that either of these messages being true will print an error message
	if(isNullVector(vector) || isVectorOutOfBounds(vector, row))
		return 0;
	//I can treat entries as a 1-dimensional array
	return vector.entries[row - 1];
}

float getMatrixElem(Matrix matrix, int row, int col) {
	if(isNullMatrix(matrix) || isMatrixOutOfBounds(matrix, row, col))
		return 0;
	//Innermost term will access the correct row list
	//outer term then accesses the "col" or the entry number in that list
	//Minus 1 from each part to account for 0-indexing
	//of lists vs 1-indexing of matrices in theory
	return ( *(*(matrix.rows + (row - 1)) + (col - 1)) );
}

float *getRow(Matrix matrix, int row) {
	if(isNullMatrix(matrix) || isMatrixOutOfBounds(matrix, row, 1))
		return malloc(0);

	return copyArray(*((matrix.rows) + (row - 1)), matrix.n);
}

float *getCol(Matrix matrix, int col) {
	if(isNullMatrix(matrix) || isMatrixOutOfBounds(matrix, col, 1))
		return malloc(0);
	//Note that columns cannot be handled the same as rows
	//since I need to jump array to array versus grabbing a single one
	float *a = malloc(matrix.m * sizeof(float));

	for(int row = 1; row <= matrix.m; row++) 
		*(a + row - 1) = getMatrixElem(matrix, row, col);
	
	return a;
}

Vector getColVector(Matrix matrix, int col) {
	//Checks to ensure matrix is well-defined
	//Also ensures that matrix has as many columns as attempting to be accessed
	//by checking to see if the first row and col = col exists
	if(isNullMatrix(matrix) || isMatrixOutOfBounds(matrix, 1, col))
		return NULL_VECTOR;

	Vector colVector = {matrix.m, getCol(matrix, col)};
	return colVector;
}

Vector getRowVector(Matrix matrix, int row) {
	//Ensures that matrix has as many rows as attempting to be accessed
	//by checking to see if the first col and row = row exists
	if(isNullMatrix(matrix) || isMatrixOutOfBounds(matrix, row, 1))
		return NULL_VECTOR;

	Vector rowVector = {matrix.n, getRow(matrix, row)};

	return rowVector;
}

/**********CONSTRUCTORS**********/
Vector createVector(int m) {
	if(m <= 0) {
		printf("Attempting to create an undefined vector.\n");
		return NULL_VECTOR;
	}

	float size = m * sizeof(float);
	Vector vector = {m, malloc(size)};
	//Initialize the vector's entries to 0
	for(int row = 1; row <= vector.m; row++) 
		setVectorElem(vector, row, 0);
	
	return vector;
}

Matrix createMatrix(int m, int n) {
	if(m <= 0 || n <= 0) {
		printf("Attempting to create an undefined matrix.\n");
		return NULL_MATRIX;
	}
	//Initializes the matrix's dimensions
	Matrix matrix = {m, n};
	//Needs to calculate the correct sizes, since the rows
	//lists of lists and the columns are lists, the memory size
	//is different
	float rowSize = m * sizeof(float *);
	float colSize = n * sizeof(float);

	matrix.rows = malloc(rowSize);

	for(int row = 1; row <= m; row++) {
		//Allocates memory for each row vector
		*(matrix.rows + (row - 1)) = malloc(colSize);

		for(int col = 1; col <= n; col++)
			setMatrixElem(matrix, row, col, 0);
		
	}

	return matrix;
}

/********Printers*******/
void printVector(Vector vector) {
	//Escapes if Vector is not defined
	if(isNullVector(vector))
		return;

	printf("Vector of dimension: %d\n", vector.m);
	for(int row = 1; row <= vector.m; row++) {
		//Padding for right justification
		printf("%6.3f,\n", getVectorElem(vector, row));
	}
	//Final line padding
	printf("\n");
}

void printMatrix(Matrix matrix) {
	//Escapes if matrix is not defined
	if(isNullMatrix(matrix))
		return;

	printf("Matrix of dimension: %dx%d\n", matrix.m, matrix.n);

	for(int row = 1; row <= matrix.m; row++) {
		for(int col = 1; col <= matrix.n; col++) {
			//Padding for right justification
			printf("%5.3f ", getMatrixElem(matrix, row, col));
		}
		//Prints a row ending comma and then enters a new line for the next row
		//backspace character is to delete last space from for loop
		printf("\b,\n");
	}
	//Final line padding
	printf("\n");
}



/******Setters***********/
void setVectorElem(Vector vector, int row, float value) {
	if(isNullVector(vector) || isVectorOutOfBounds(vector, row))
		return;

	vector.entries[row - 1] = value;
}

void setMatrixElem(Matrix matrix, int row, int col, float value) {
	if(isNullMatrix(matrix) || isMatrixOutOfBounds(matrix, row, col))
		return;
	//Uses the same pointer arithmetic/logic as getMatrixElem
	( *(*(matrix.rows + (row - 1)) + (col - 1)) ) = value;	
}

float * copyArray(float *a, int len) {
	if(len <= 0) {
		printf("Invalid array length.\n");	
		return malloc(0);
	}
	//Size of the array to be copied
	float size = len * sizeof(float);
	//Allocates memory corresponding to the length of the array
	float *copy = malloc(size);
	memcpy(copy, a, size);

	return copy;
}

void scaleArray(float *a, int len, float scale) {
	if(len <= 0) {
		printf("Invalid array length.\n");	
		return;
	}

	for(int i = 0; i < len; i++)
		*(a + i) *= scale;
}

Vector copyVector(Vector vector) {
	//Escapes if Vector is not defined
	if(isNullVector(vector))
		return NULL_VECTOR;

	//Initializes the copy
	Vector copy = createVector(vector.m);
	//Copies the entries
	copy.entries = copyArray(vector.entries, vector.m);

	return copy;
}

Matrix copyMatrix(Matrix matrix) {
	if(isNullMatrix(matrix))
		return NULL_MATRIX;

	Matrix copy = createMatrix(matrix.m, matrix.n);

	for(int row = 1; row <= matrix.m; row++) {
		*(copy.rows + (row - 1)) = copyArray(*(matrix.rows + (row - 1)), matrix.n);
	}

	return copy;
}

/*******Operations*******/
void scaleVector(Vector vector, float scale) {
	if(isNullVector(vector))
		return;

	scaleArray(vector.entries, vector.m, scale);
}

float dot_product(Vector v1, Vector v2) {
	//Vectors must be of same dimension
	//and they must be well defined vectors
	if(v1.m != v2.m) {
		printf("Dot product is not defined for vectors of different dimensions.\n");
		return 0;
	} else if(isNullVector(v1) || isNullVector(v2))
		return 0;

	//Initializes the product
	float dot_product = 0;
	//Loops through the vectors
	for(int row = 1; row <= v1.m; row++)
		dot_product += getVectorElem(v1, row) * getVectorElem(v2, row);

	return dot_product;
}

Matrix matrix_mult(Matrix m1, Matrix m2) {
	//Checks if the product is defined
	//As well if the matrices themselves are defined
	if(m1.n != m2.m || isNullMatrix(m1) || isNullMatrix(m2))
		return NULL_MATRIX;

	//Prepares the dimensions of the product
	//and initializes the matrix
	Matrix product = createMatrix(m1.m, m2.n);

	//Multiplication begins here
	for(int row = 1; row <= product.m; row++) {
		//Gets the row vector outside of the nested for loop
		//to prevent from reaccessing matrix more than necessary
		Vector m1RowVec = getRowVector(m1, row);

		for(int col = 1; col <= product.n; col++) {
			//Grabs the column vector of the second matrix to dot product
			//with row vector of first matrix
			setMatrixElem(product, row, col, dot_product(m1RowVec, getColVector(m2, col)));
		}	
	}

	return product;
}



/*******Elementary Operations*******/
void swapRow(Matrix matrix, int row1, int row2) {
	if(isNullMatrix(matrix) || isMatrixOutOfBounds(matrix, row1, 1) || isMatrixOutOfBounds(matrix, row2, 1))
		return;
	//Readjust for 1-indexing/0-indexing
	row1 -= 1;
	row2 -= 1;
	//Simply swaps the pointers directing to each row "vector"
	float* temp = matrix.rows[row1];
	matrix.rows[row1] = matrix.rows[row2];
	matrix.rows[row2] = temp;
}

void scaleRow(Matrix matrix, int row, float scale) {
	if(isNullMatrix(matrix) || isMatrixOutOfBounds(matrix, row, 1))
		return;

	scaleArray(*(matrix.rows + (row - 1)), matrix.n, scale);
}

void addRows(Matrix matrix, int row1, int row2) {
	if(isNullMatrix(matrix) || isMatrixOutOfBounds(matrix, row1, 1) || isMatrixOutOfBounds(matrix, row2, 1))
		return;

	//Goes through each column to add the entries
	for(int col = 1; col <= matrix.n; col++)
		setMatrixElem(matrix, row1, col, getMatrixElem(matrix, row1, col) + getMatrixElem(matrix, row2, col));
	
}

void addScaledRows(Matrix matrix, int row1, float scale1, int row2, float scale2) {
	//Adding a scaled row of 0 is just adding 0 to each term, waste of time
	if(scale1 == 0 || scale2 == 0)
		return;

	//Consider checking if either scale is 1 to skip steps
	scaleRow(matrix, row1, scale1);
	scaleRow(matrix, row2, scale2);
	addRows(matrix, row1, row2);
	//Undo the scaling on the second row
	scaleRow(matrix, row2, 1/scale2);
}



/***********Algorithms*********/
void GaussJordan(Matrix matrix) {
	//Start at the first row
	int row = 1;
	int col = 0;
	while(row < matrix.m) {
		col++;

		float elem = getMatrixElem(matrix, row, col);
		if(elem == 0)
			continue;

		scaleRow(matrix, row, 1/getMatrixElem(matrix, row, 1));
		addScaledRows(matrix, row + 1, row, 1, -1 * getMatrixElem(matrix, row + 1, col));


		row++;
	}
	//Reinverts last row TEMPORARY
	scaleRow(matrix, row, 1/getMatrixElem(matrix, matrix.m, matrix.m));
}

/***********TESTING FUNCTIONALITY*********/
Vector createRandomVector(int m) {
	static int counter = 0;

	Vector v = createVector(m);
	for(int row = 1; row <= m; row++) {
		//"Random" operations
		setVectorElem(v, row, row * row + (3/2 * counter * counter) - 24 / (counter + 1));
	}
	counter++;

	printVector(v);

	return v;
}

Matrix createRandomMatrix(int m, int n) {
	static int counter2 = 1;

	Matrix matrix = createMatrix(m, n);
	for(int row = 1; row <= m; row++) {
		for(int col = 1; col <= n; col++) {
			setMatrixElem(matrix, row, col, 2+counter2/4 - row * col + 5 * col);
		}
	}
	counter2++;

	printMatrix(matrix);

	return matrix;
}

//MAIN
int main() {
	//Some output padding
	printf("\n\n\n");

	Matrix matrix = createRandomMatrix(2, 2);
	GaussJordan(matrix);
	printMatrix(matrix);





	printf("\n");
	return 0;
}




