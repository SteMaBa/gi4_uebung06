#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "init_matrix.h"

#define MATRIX_SIZE (1024)

double **A;
double *b;
double *X;
double *X_old;
double *temp;

int main(int argc, char **argv)
{
	unsigned int i, j;
	unsigned int iterations = 0;
	double error, xi, norm, max = 0.0;

	//Neue Variablen
	double sum = 0.0;
	double epsilon = sqrt(0.00000001*MATRIX_SIZE);
	double sumindistance = 0.0;
	//Neue Variablen end

	struct timeval start, end;

	printf("\nInitialize system of linear equations...\n");
	/* allocate memory for the system of linear equations */
	init_matrix(&A, &b, MATRIX_SIZE);
	X = (double *)malloc(sizeof(double) * MATRIX_SIZE);
	X_old = (double *)malloc(sizeof(double) * MATRIX_SIZE);

	/* a "random" solution vector */
	for (i = 0; i < MATRIX_SIZE; i++) {
		X[i] = ((double)rand()) / ((double)RAND_MAX) * 10.0;
		X_old[i] = 0.0;
	}

	printf("Start Jacobi method...\n");

	gettimeofday(&start, NULL);


	/* TODO: Hier muss die Aufgabe geloest werden */
	

	norm = 1.0;
	

	//Loesung suchen, bis Abstand aufeinanderfolgender Loesungen sehr klein ist
	while (norm > epsilon)
	{
	
	//Alle X einmal durchgehen
	for (i = 0; i < MATRIX_SIZE; i++)
	{
	
	        //Summe berechnen
	        sum = 0.0;
	        for (j = 0; j < MATRIX_SIZE; j ++)
	        {
	                if (j == i)
	                {
	                        j++;
	                }
	        
	                sum = sum + A[i][j]*X_old[j];
	        
	        }//Summe end
	        
	        xi = X[i];
	        X[i] = 1 / A[i][i] * (b[i] - sum);
	        X_old[i] = xi;

	}//Alle X end
		
		
		
	//Abstand berechnen
		
	//Summe im Abstand
	sumindistance = 0.0;	
	for (i = 0; i < MATRIX_SIZE; i ++)
	{
	      sumindistance = sumindistance + (X_old[i]-X[i])*(X_old[i]-X[i]);
	        
	}//Abstandsumme end	
	
	norm = sqrt(sumindistance);
		
	
	iterations++;

	}//while end
	
	
	

	gettimeofday(&end, NULL);

	if (MATRIX_SIZE < 16) {
		printf("Print the solution...\n");
		/* print solution */
		for (i = 0; i < MATRIX_SIZE; i++) {
			for (j = 0; j < MATRIX_SIZE; j++)
				printf("%8.2f\t", A[i][j]);
			printf("*\t%8.2f\t=\t%8.2f\n", X[i], b[i]);
		}
	}

	printf("Check the result...\n");
	/* 
	 * check the result 
	 * X[i] have to be 1
	 */
	for (i = 0; i < MATRIX_SIZE; i++) {
		error = fabs(X[i] - 1.0f);

		if (max < error)
			max = error;
		if (error > 0.01f)
			printf("Result is on position %d wrong (%f != 1.0)\n",
			       i, X[i]);
	}
	printf("maximal error is %f\n", max);

	printf("\nmatrix size: %d x %d\n", MATRIX_SIZE, MATRIX_SIZE);
	printf("number of iterations: %d\n", iterations);
	printf("Time : %lf sec\n",
	       (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec -
							      start.tv_usec) /
	       1000000.0);

	/* frees the allocated memory */
	free(X_old);
	free(X);
	clean_matrix(&A);
	clean_vector(&b);

	return 0;
}
