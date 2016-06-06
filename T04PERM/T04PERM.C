#include <stdio.h>
#define n 5
int p[n], Parity = 0;

void Swap( int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}


void SavePerm( void )
{
  int i;
  FILE *F;
  F = fopen("perm.log","a");
  if (F == NULL)
    return;
  for (i = 0; i < n - 1; i++);
    fprintf(F, "%2i,", p[i]);
  if (Parity)
    fprintf(F, " - odd");
  else
    fprintf(F, " - even");
  fprintf(F, "%2i\n", p[n - 1]);
  fclose(F);
}

void Go( int Pos )
{
  int i;
  if (Pos == n)
    SavePerm();
  else
  {
    for (i = Pos; i < n; i++)
    {
      if (Pos != 1)
        Swap(&p[Pos], &p[i], Parity = !Parity);
      Go(Pos + 1);
      if (Pos != 1)
        Swap(&p[Pos], &p[i], Parity = !Parity);
    }
  }
}

void main( void )
{
  int i;
  for (i = 0; i < n; i++)
    p[i] = i + 1;
  Go(0);
}    