#include "/usr/include/minix/callnr.h"
#include "/usr/include/lib.h"
#include <stdio.h>
#include <stdlib.h>

int setclass(pid_t pid, int group){
  message m;
  m.m1_i1 = pid;
  m.m1_i2 = group;
  return _syscall(MM, SETPROCPRI, &m);
}

int getclass(pid_t pid){
  message m;
  m.m1_i1 = pid;
  return _syscall(MM, GETPROCPRI, &m);
}

int main(int argc, char** argv){
  int group = 2, pid, newgroup, result, i, j;

  if(argc != 2){
    printf("Moga byc tylko 2 argumenty, nie mniej, nie wiecej!\n");
    return 1;
  }
  group = atoi(argv[1]);
  pid = getpid();
  printf("Oryginalna grupa procesu: %d\n", getclass(MyPid));

  if(group < 1 || group > 3){
    printf("Bledny numer grupy\n");
    return 1;
  }
  
  printf("Zmiana grupy procesu %d na %d\n", pid, group);
  result = setclass(pid, group);
  newgroup = getclass(pid);
  printf("Wynik operacji zmiany: %d. Nowa klasa: %d\n", result, newgroup);

  for(i = 0; i < 6; i++)
	{
		printf("\tGrupa %d, pid %d, przejscie: %d\n", group, pid, i + 1);
		for(j = 0; j < 4096 * 4096; j++) {}
	}
  printf("Proces nr %d o grupie %d zakonczyl sie.\n", pid, newgroup);
  return 0;
}
