#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int x = 1;

    pid = fork(); //return twice

    //child process
    if(pid == 0) {
	printf("child: x=%d\n",++x);
	return 0;
    }

    //parent process
    printf("parent: x=%d\n",--x);
    return 0;
}
