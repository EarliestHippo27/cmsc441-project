#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <math.h> // Just for exponents, remove afterward
#include <sys/resource.h>

int base;
int end;

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
}

// size is a power of 2. Should be base/2^n. size <= base. size of result matrix
// base is the original size of the array. Power of 2
// (xr1,xc1) is where it will start changing/reading --Top-Right of the sub-matrix--, and the loop iterates size^2 times
// matA, matB are the original arrays from main()
// result is a sizexsize array, smaller that matA and matB
void mat_add(int base, int size, int ar1, int ac1, int br1, int bc1, const int matA[base][base], const int matB[base][base], int result[size][size]) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      result[i][j] = matA[i+ar1][j+ac1] + matB[i+br1][j+bc1];
    }
  }
}

void mat_sub(int base, int size, int ar1, int ac1, int br1, int bc1, const int matA[base][base], const int matB[base][base], int result[size][size]) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      result[i][j] = matA[i+ar1][j+ac1] - matB[i+br1][j+bc1];
    }
  }
}

// Copies a sizexsize chunk out of matA into result
void sam_copy(int base, int size, int ar1, int ac1, const int matA[base][base], int result[size][size])
{
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      result[i][j] = matA[i+ar1][j+ac1];
    }
  }
}

//Size is base/2
void populate(int base, int size, int result[base][base], int m1[size][size], int m2[size][size], int m3[size][size], int m4[size][size], int m5[size][size], int m6[size][size], int m7[size][size])
{
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      //C11
      result[i][j] = m1[i][j] + m4[i][j] - m5[i][j] + m7[i][j];
      //C12
      result[i][j+size] = m3[i][j] + m5[i][j];
      //C21
      result[i+size][j] = m2[i][j] + m4[i][j];
      //C22
      result[i+size][j+size] = m1[i][j] - m2[i][j] + m3[i][j] + m6[i][j];
    }
  }
  return;
}

void sam_mult_aux(int cutoff, int base, int size, int row, int col, const int matA[base][base], const int matB[base][base], int matC[base][base])
{
  //base case, 1x1 matrices
  //printf("Computing with %dx%d matrices!\n", base, base);
  if (size <= cutoff)
  {
    //printf("reached cutoff!\n");
    //matC[row][col] = matA[row][col] * matB[row][col];
    bam(size, matA, matB, matC);
    return;
  }

  //Temporary arrays
  int t1[size/2][size/2];
  int t2[size/2][size/2];

  //M1
  int m1[size/2][size/2];
  mat_add(base, size/2, row, col, row + size/2, col + size/2, matA, matA, t1);
  mat_add(base, size/2, row, col, row + size/2, col + size/2, matB, matB, t2);
  sam_mult_aux(cutoff, size/2, size/2, 0, 0, t1, t2, m1);
  //M2
  int m2[size/2][size/2];
  mat_add(base, size/2, row + size/2, col, row + size/2, col + size/2, matA, matA, t1);
  sam_copy(base, size/2, row, col, matB, t2);
  sam_mult_aux(cutoff, size/2, size/2, 0, 0, t1, t2, m2);
  //M3
  int m3[size/2][size/2];
  sam_copy(base, size/2, row, col, matA, t1);
  mat_sub(base, size/2, row, col + size/2, row + size/2, col + size/2, matB, matB, t2);
  sam_mult_aux(cutoff, size/2, size/2, 0, 0, t1, t2, m3);
  //M4
  int m4[size/2][size/2];
  sam_copy(base, size/2, row + size/2, col + size/2, matA, t1);
  mat_sub(base, size/2, row + size/2, col, row, col, matB, matB, t2);
  sam_mult_aux(cutoff, size/2, size/2, 0, 0, t1, t2, m4);
  //M5
  int m5[size/2][size/2];
  mat_add(base, size/2, row, col, row, col + size/2, matA, matA, t1);
  sam_copy(base, size/2, row + size/2, col + size/2, matB, t2);
  sam_mult_aux(cutoff, size/2, size/2, 0, 0, t1, t2, m5);
  //M6
  int m6[size/2][size/2];
  mat_sub(base, size/2, row + size/2, col, row, col, matA, matA, t1);
  mat_add(base, size/2, row, col, row, col + size/2, matB, matB, t2);
  sam_mult_aux(cutoff, size/2, size/2, 0, 0, t1, t2, m6);
  //M7
  int m7[size/2][size/2];
  mat_sub(base, size/2, row, col + size/2, row + size/2, col + size/2, matA, matA, t1);
  mat_add(base, size/2, row + size/2, col, row + size/2, col + size/2, matB, matB, t2);
  sam_mult_aux(cutoff, size/2, size/2, 0, 0, t1, t2, m7);

  populate(base, size/2, matC, m1, m2, m3, m4 ,m5, m6, m7);
  return;
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

int main(int numCommands, char* args[]) {
  if(numCommands <= 3)
	{
		printf("ERROR: Insufficient Arguments\n");
		return -1;
	}

  int size = atoi(args[1]);
  int k = atoi(args[3]);
  char* fileName = args[2];
  FILE* fs;
  char bu[100];
	
  fs = fopen(fileName, "a+");
  
  srand(time(NULL));
  
  struct rusage stack;
  getrusage(RUSAGE_SELF, &stack);
  base = stack.ru_maxrss;

  int matA[size][size];
  int matB[size][size];
  int matC[size][size];
  memset(matC, 0, sizeof(matC));

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      matA[i][j] = rand() % 9;
      matB[i][j] = rand() % 9;
    }
  }


  //This takes A12 and B12
  //mat_sub(size, size/2, 0, size/2, 0, size/2, matA, matB, matC);
  
  sam_mult_aux(k, size, size, 0, 0, matA, matB, matC);

  getrusage(RUSAGE_SELF, &stack);
  end = stack.ru_maxrss;

  unsigned int depth = end-base;
  printf("That was %d kilobytes!\n", depth);
  fprintf(fs, "%d,%d\n", size, depth);
  //bam_print(size, matA);
  //printf("\n");
  //bam_print(size, matB);
  //printf("\n");
  //bam_print(size, matC);
  //printf("\n");
}

