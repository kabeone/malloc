#include <stdlib.h>

int main()
{
  char *ptr = malloc(100);

  free(ptr += 10);
  return 0;
}
