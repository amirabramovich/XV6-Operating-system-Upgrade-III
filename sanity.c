#include "types.h"
#include "stat.h"
#include "user.h"

void
test1(void)
{
  int *t1 = pmalloc();
  int *t2 = pmalloc();
  printf(1, "address of t1 is %d and t2 %d\n",t1,t2);
  printf(1, "t2-t1 is %d\n",(int)t2-(int)t1);
  free(t1);
  free(t2);
  printf(1, "t1 & t2 freed\n");
}

int
main(int argc, char *argv[])
{
  test1();
  exit();
}
