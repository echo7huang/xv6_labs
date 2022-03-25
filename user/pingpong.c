#include "kernel/types.h"
#include "user/user.h"

//define the reading and writing terminals
#define R 0
#define W 1

//define the exit code for success
#define EXIT_SUCCESS 0

int main(int argc, char const *argv[]){
    //create two pipes
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);

    //create the byte to be written, DO NOT USE ""
    char buff= 'x';

    /*If the process is parent:
     *
     * close the unused terminals
     * perform the write & read
     * print ping/pong message
     * close the used terminals
     * exit with proper exit code
     *
     * */
    if(fork() != 0){
        close(p2c[R]);
        close(c2p[W]);

        write(p2c[W], &buff, sizeof(char));
        read(c2p[R], &buff, sizeof(char));
        printf("%d: received pong\n", getpid());

        close(p2c[W]);
        close(c2p[R]);

        exit(EXIT_SUCCESS);

    }
    else {
        close(p2c[W]);
        close(c2p[R]);

        read(p2c[R], &buff, sizeof(char));
        printf("%d: received ping\n", getpid());
        write(c2p[W], &buff, sizeof(char));

        close(p2c[R]);
        close(c2p[W]);

        exit(EXIT_SUCCESS);
    }

    //similar thing works for the child process

    //note that if fork() returns 0 then the child proc is created successfully
    //getpid() returns the process id

}
