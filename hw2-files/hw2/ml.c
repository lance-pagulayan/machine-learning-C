/* 
 * 
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the 
 * weight of different features.
 * 
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)  
 * 
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 * 
 */
 
#include<stdio.h>
#include<stdlib.h>

// all methods declarations
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double** transposeMatrix(double** mat, int row, int col);
double** inverseMatrix(double **matA, int dimension);
//double** makeMatrix(int rows, int cols);
void freeMem(double** matrix, int row);

// main method starts here
int main(int argc, char** argv){
  if(argc != 3){
    printf("Error: too few arguments!!\n");
  }

  FILE* training = fopen(argv[1], "r");
  if(training == NULL){
    printf("error\n");
  }
  int row, col;
  fscanf(training, "%d\n", &col);
  col = col + 1;
  fscanf(training, "%d\n", &row);
  
  //allocate space for X matrix
  double** X = malloc(sizeof(double*)*row);
  for(int i = 0; i < row; i++){
    X[i] = calloc(col, sizeof(double));
  }

  //allocate space for Y matrix
  double** Y = malloc(sizeof(double*)*row);
  for(int i = 0; i < row; i++){
    Y[i] = calloc(1,sizeof(double));
  }
  //double** X = makeMatrix(row, col);
  //double** Y = makeMatrix(row, 1);

  //we fill in matrix X and matrix Y
  double tmp;
  for(int i = 0; i < row; i++){
    X[i][0] = 1;
  }
  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      if(j == col-1){
	fscanf(training, "%lf\n", &tmp);
	Y[i][0] = tmp;
      }else{
        fscanf(training, "%lf,", &tmp);
        X[i][j+1] = tmp;
      }
    }
  }
  fclose(training);

  double** Xt = transposeMatrix(X, row, col);
  double**  XtX = multiplyMatrix(Xt, X, col, row, row, col);
  double** inverted = inverseMatrix(XtX, col);
  double** Z = multiplyMatrix(inverted, Xt, col, col, col, row);
  double** W = multiplyMatrix(Z, Y, col, row, row, 1);

  //Now we test the data
  FILE* testing = fopen(argv[2], "r");
  if(testing == NULL){
    printf("error\n");
  }
  int row_test;
  fscanf(testing, "%d\n", &row_test);
  
  double** X_test = malloc(sizeof(double*)*row_test);
  for(int i = 0; i < row_test; i++){
    X_test[i] = calloc(col, sizeof(double));
  }

  for(int i = 0; i < row_test; i++){
    for(int j = 0; j < col; j++){
      if(j == 0){
	X_test[i][j] = 1;
      }else{
        fscanf(testing, "%lf,", &tmp);
        X_test[i][j] = tmp;
      }
    }
  }
  fclose(testing);

  double** Y_final = multiplyMatrix(X_test, W, row_test, col, col, 1);
  for(int i = 0; i < row_test; i++){
    printf("%0.0lf\n", Y_final[i][0]);
  }

  freeMem(X, row);
  freeMem(Y, row);
  freeMem(Xt, col);
  freeMem(XtX, col);
  freeMem(inverted, col);
  freeMem(Z, col);
  freeMem(W, col);
  freeMem(X_test, row_test);
  freeMem(Y_final, row_test);
}

double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
    double** result=malloc(r1*sizeof(double*)); 
    for(int i = 0; i < r1; i++){
      result[i] = calloc(c2, sizeof(double));
    }
    for(int i = 0; i < r1; i++){
     for(int j = 0; j < c2; j++){
       for(int z = 0; z < c1; z++){
	result[i][j] = result[i][j] + matA[i][z]*matB[z][j];
       }
     }
   }

    return result;
}


double** transposeMatrix(double** mat, int row, int col)
{
  
     double** matTran=malloc(col*sizeof(double*));
     for(int i = 0; i < col; i++){
	 matTran[i] = calloc(row, sizeof(double));
     }
     for(int i = 0; i < row; i++){
       for(int j = 0; j < col; j++){
	 matTran[j][i] = mat[i][j];
       }
     }
    
    
    return matTran;        
}


double** inverseMatrix(double **matA, int dimension)
{

    double** matI=malloc(dimension*sizeof(double*));
    for(int i = 0; i < dimension; i++){
      matI[i] = calloc(dimension, sizeof(double));
    }
    for(int i = 0; i < dimension; i++){
      for(int j = 0; j < dimension; j++){
	if(j == i){
	  matI[i][j] = 1;
	}else{
	  matI[i][j] = 0;
	}
      }
    }
    
    for(int i = 0; i < dimension; i++){
     double f = matA[i][i];
     for(int j = 0; j < dimension; j++){
       matA[i][j] = matA[i][j] / f;
       matI[i][j] = matI[i][j] / f;
     }
     for(int k = i + 1; k < dimension; k++){
       double f = matA[k][i];
       for(int j = 0; j < dimension; j++){
	 matA[k][j] -= f*matA[i][j];
	 matI[k][j] -= f*matI[i][j];
       }
     }
      
    }
     for(int p = dimension - 1; p >= 0; p--){
       for(int i = p-1; i >= 0; i--){
	 double f = matA[i][p];
	 for(int j = 0; j < dimension; j++){
	   matA[i][j] -= f*matA[p][j];
	   matI[i][j] -= f*matI[p][j];
	 }
       }
     }
    
	return matI;
}

void freeMem(double** matrix, int dimension){
  for(int i = 0; i < dimension; i++){
    free(matrix[i]);
  }
  free(matrix);
}


