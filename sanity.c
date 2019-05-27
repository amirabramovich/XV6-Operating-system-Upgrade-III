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

void
test2(void)
{
  int *t1 = pmalloc();
  int *t2 = malloc(300);
  printf(1, "t1 is %d and modulu 4096 is %d\n",(int)t1-8,((int)t1-8)%4096);
  printf(1, "t2 is %d\n",(int)t2-8);
  if(protect_page(t1)>0)
    printf(1, "t1 is protected\n");
  if(pfree(t1)>0){
    free(t2);
    printf(1, "t1 & t2 freed\n");
  }
}

void
test3(void)
{
  int *t1 = malloc(300);
  int *t2 = pmalloc();
  printf(1, "t1 is %d\n",(int)t1-8);
  printf(1, "t2 is %d and modulu 4096 is %d\n",(int)t2-8,((int)t2-8)%4096);
  if(protect_page(t2)>0)
    printf(1, "t2 is protected\n");
  if(pfree(t2)>0){
    free(t1);
    printf(1, "t1 & t2 freed\n");
  }
}

void
test4(void)
{
  int *t1 = malloc(300);
  int *t2 = pmalloc();
  printf(1, "t1 is %d\n",(int)t1-8);
  printf(1, "t2 is %d and modulu 4096 is %d\n",(int)t2-8,((int)t2-8)%4096);
  if(protect_page(t2)>0)
    printf(1, "t2 is protected\n");
  free(t1);
  printf(1, "t1 freed\n");
  printf(1, "try to write t2[5]=10\n");
  t2[5]=10;
}

int
main(int argc, char *argv[])
{
  if(argc<2)
    printf(1, "usage: sanity <1-3>\n");
  else{
    switch (atoi(argv[1]))
    {
      case 1:
        test1();
        break;
      case 2:
        test2();
        break;
      case 3:
        test3();
        break;
      case 4:
        test4();
        break;
      default:
        printf(1, "usage: sanity <1-4>\n");
    }
  }
  exit();
}