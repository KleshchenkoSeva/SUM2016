#include <stdio.h> 
#include <conio.h> 
#define MAX 30 

double A[MAX][MAX], sum; 
int P[MAX], n; 
char Parity = 1, SaveParity; 

void Swap( int *A, int *B ) 
{ 
  int tmp; 

  tmp = *A; 
  *A = *B; 
  *B = tmp; 
} 

void Go( int Pos ) 
{ 
  int i; 
  double prod = 1; 

  if (Pos == n) 
  { 
    for (i = 0; i < n; i++) 
    prod *= A[i][P[i]]; 

    if (Parity) 
    sum += prod; 
    else 
    sum -= prod; 

    return; 
  } 
  else 
  { 
    for (i = Pos; i < N; i++) 
    {
      Swap(&P[Pos], &P[i]); 
      if (Pos != i) 
      Parity = !Parity; 
      Go(Pos + 1); 
      if (Pos != i) 
      Parity = !Parity; 
      Swap(&P[Pos], &P[i]); 
    } 
  } 
} 

void LoadMatrix( char *FileName ) 
{ 
  FILE *F;
  int i, j;

  F = fopen(FileName,"r");
  if (F == NULL)
  return;

  fscanf(F,"%d",&n);

  for (i = 0; i < n; i++)
    for (j = 0 ; j < n ; j++)
      fscanf(F,"%lf", &A[i][j]);

  fclose(F);
} 

double EvalDeterm( char *FileName ) 
{ 
  int i; 

  LoadMatrix(FileName); 

  sum = 0; 
  for (i = 0; i < n; i++) 
  P[i] = i; 
  Go(0); 

  return sum; 
} 

void main( void ) 
{ 
  int i; 
  char *M[] = 
  { 
  "mat1.txt"
  }; 

  for (i = 0; i < n; i++) 
    P[i] = i; 

  for (i = 0; i < sizeof(M) / sizeof(M[0]); i++) 
    printf("Det(%d)[%s] = %f\n", i, M[i], EvalDeterm(M[i])); 

  _getch(); 
}
