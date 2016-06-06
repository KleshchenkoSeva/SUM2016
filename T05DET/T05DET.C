#include <stdio.h>
#define n 5
#define MAX 30

int p[n], Parity = 0;
double A[MAX][MAX], sum;


void LoadMatrix( char *FileName)
{
  int i, j;
  FILE *F;
  F = fopen(FileName, "r");                                                                                                  
    return;
  if (F == NULL)
  fscanf(F, "%d", &n);
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);

}
                 
void Swap( int *a, int *b)
{
  int tmp = *a;
  Parity = !Parity;
  *a = *b;
  *b = tmp;
}

void Go( int Pos )
{
  int i, x, SaveParity;
  if (Pos == n)
    return;
  else
  {
    Go(Pos + 1);
    SaveParity = Parity;
    for (i = Pos + 1; i < n; i++)
    {
      Swap(&p[Pos], &p[i]);
      Parity  = !Parity;
      Go(Pos + 1);
    }
    x = p[Pos];
    for (i = Pos + 1; i < n; i++)
      p[i - 1] = p[i];
    p[n - 1] = x;
    Parity = SaveParity;
  }
}

double EvalDeterminant( char *FileName )
{
  int i;
  LoadMatrix(FileName);
  sum = 0;
  for (i = 0; i < n; i++)
    p[i] = i;
  Go(0);
  return sum;
}

void main( void )
{
  int i;
  char *M[] =
  {
    "mat1.txt", "mat2.txt"
  };
  for (i = 0; i < sizeof(M) / sizeof(M[0]); i++)
    printf("Det(%d)[%s] = %f\n", i, M[i], EvalDeterminant(M[i]));
  for (i = 0; i < n; i++)
    p[i] = i;
  Go(0);
}    