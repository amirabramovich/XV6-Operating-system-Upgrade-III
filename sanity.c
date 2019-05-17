#include "types.h"
#include "stat.h"
#include "user.h"

void
test1(void)
{
  int *t1 = pmalloc();
  int *t2 = pmalloc();
  printf(1, "t1 is %d and modulu 4096 is %d\n",(int)t1-8,((int)t1-8)%4096);
  printf(1, "t2 is %d and modulu 4096 is %d\n",(int)t2-8,((int)t2-8)%4096);
  printf(1, "t2-t1 is %d\n",(int)t2-(int)t1);
  if(protect_page(t1)>0)
    printf(1, "t1 is protected\n");
  if(pfree(t1)>0&&pfree(t2)>0)
    printf(1, "t1 & t2 freed\n");
}

int
main(int argc, char *argv[])
{
  test1();
  exit();
}
