#include "/usr/include/minix/callnr.h"
#include "/usr/include/lib.h"
#include <stdio.h>
#include <stdlib.h>

int getprocnr(int pos){
  message m;
  m.m2_l2 = pos;
  return _syscall(MM, GETPROCNR, &m);
}

int main(int argc, char *argv[]){
  if(argc <= 1)
    printf("Not enough arguments! Aborting.\n");
  else{
    int pos = atoi(argv[1]);
    int max = pos+10;
    while(pos <= max){
      int proc = getprocnr(pos);
      if(proc >= 0)
        printf("GETPROCNR: Process nr %d has index %d\n", pos, proc);
      else
        printf("GETPROCNR: Error nr %d for process nr %d\n", proc, pos);
      pos++;
    }
  }
  return 0;
}
