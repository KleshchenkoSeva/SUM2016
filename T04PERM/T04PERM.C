#include <stdio.h>
#define N 5

int P[N], Parity = 0;

void SavePerm( void )
{
  int i;

  FILE *F;
  F = fopen("perm.log", "a");
  if (F == NULL)
    return;
  for (i = 0; i < N; i++)
    fprintf(F, "%2i", P[i]);
  if (Parity)
    fprintf(F, " - odd\n");
  else
    fprintf(F, " - even\n");
  fclose(F);
}

void Swap( int *A, int *B)
{
  int tmp = *A;
  *A = *B;
  *B = tmp;
}

void Go( int Pos )
{
  int i, x, SaveParity;

  if (Pos == N)
  {
    SavePerm();
    return;
  }
  else 
  {

    Go(Pos + 1);
    SaveParity = Parity;
    for (i = Pos + 1; i < N; i++)
    {
 
      Swap(&P[Pos], &P[i]);
      Parity = !Parity;
      Go(Pos + 1);
    }
   
    x = P[Pos];
    for (i = Pos + 1; i < N; i++)
      P[i - 1] = P[i];
    P[N - 1] = x;
    Parity = SaveParity;
  }
}

void main( void )
{
  int i;

  for (i = 0; i < N; i++)
    P[i] = i  + 1;
  Go(0);
}

