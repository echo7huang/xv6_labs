#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{

    // save para from argv[] to args[]
    char *args[MAXARG];
    int i;
    for (i = 0; i < argc; ++i)
        args[i] = argv[i];

    //buffer for stdin
    char buf[256];

    for (;;) {

        // read a line from stdin

        int j = 0;
        while ((read(0, buf+j, sizeof(char))) != 0 && buf[j] != '\n')
            ++j;
        if (j == 0) break; // all lines are read already
        buf[j] = 0; // put 0 at the end of buffer
        // insert buffer(containing stdin) as arguments to xargs
        args[i] = buf;
        args[i+1] = 0; // put 0 at the end of args[]
        // run the command
        if (fork() == 0) {
            // printf("command: %s\n", args[1]);
            // args[0] is xargs (command's name)
            // args[1] is the command (echo, for example)
            // the remaining are the parameters
            exec(args[1], args+1);
            printf("exec error\n");
        } else {
            wait((void*)0); //parent waits for child
        }
    }

    exit(0);
}