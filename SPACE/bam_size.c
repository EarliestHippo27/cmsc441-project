#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

int matrix_multiply(int size, int seed);
void bam(int size, const int matA[size][size], const int matB[size][size], int matC[size][size]);
void bam_print(int base, const int mat[base][base]);

int base;
int end;

int main(int numCommands, char* args[])
{	
	if(numCommands <= 2)
	{
		printf("ERROR: Insufficient Arguments\n");
		return -1;
	}

	int size = atoi(args[1]);
	int seed = time(NULL);
	char* fileName = args[2];
	FILE* fs;
	char bu[100];
	
	fs = fopen(fileName, "a+");
	
	struct rusage stack;
	getrusage(RUSAGE_SELF, &stack);
	base = stack.ru_maxrss;
	matrix_multiply(size, seed);

	unsigned int depth = end-base;
	printf("That was %d kilobytes!\n", depth);
	fprintf(fs, "%d,%d\n", size, depth);
	
	return 0;
}

void bam_print(int base, const int mat[base][base])
{
  for (int i = 0; i < base; i++) {
    for (int j = 0; j < base; j++) {
      printf("%d ", mat[i][j]);
    }
    printf("\n");
  }
}

void bam(int size, const int matA[size][size], const int matB[size][size], int matC[size][size])
{
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      int sum = 0;
      for (int k = 0; k < size; k++)
      {
        sum += matA[i][k] * matB[k][j];
      }
      matC[i][j] = sum;
    }
  }
  
  return;
}

int matrix_multiply(int size, int seed)
{
	srand(seed);
	int matA[size][size];
	int matB[size][size];
	int matC[size][size];	//Result matrix
	
	// Populate Matrices A and B
	for(int i = 0; i < size; i++)//row
	{
		for(int j = 0; j < size; j++)//column
		{
			matA[i][j] = (int) rand() % 999;
			matB[i][j] = (int) rand() % 999;
		}
	}
	
	bam(size, matA, matB, matC);
	
	struct rusage stack;
	getrusage(RUSAGE_SELF, &stack);
	end = stack.ru_maxrss;
	
	return matC[0][0];
}

