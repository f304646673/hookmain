#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char * argv[]) {
	std::cout << "parent main start" << std::endl;

    pid_t child = 0;
    int status = 0;
    child = fork();
    if (child == -1) {
		std::cerr << "fork error" << std::endl;
        return -1;
    }
    if (child == 0) {
        //in child process
	    char* newargv[] = { "./child", "hello", "world", nullptr };
        char* newenviron[] = { "LD_PRELOAD=./inject.so", nullptr };
		execve("./child", newargv, newenviron);
    } 
	else {
        //in parent process
        wait(&status);
        std::cout << "child status=" << WEXITSTATUS(status) << std::endl;;
    }
    return 0;
}
