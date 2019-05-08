#include "types.h"
#include "stat.h"
#include "user.h"

void
test1(void)
{
  printf(1, "OK\n");
}

int
main(int argc, char *argv[])
{
  test1();
  exit();
}
