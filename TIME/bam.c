#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

void bam(int size, const int matA[size][size], const int matB[size][size], int matC[size][size]);

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

	clock_t start = clock();
	bam(size, matA, matB, matC);
	clock_t end = clock();
	double duration = (end - start) * 1000.0 / CLOCKS_PER_SEC;

	duration = duration / 1000.0;
	printf("That took %f s!\n", duration);
	fprintf(fs, "%d,%f\n", size, duration);
	
	return 0;
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
