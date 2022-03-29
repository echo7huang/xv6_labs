#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    // save the arguments
    char *args[MAXARG];
    int i;
    for (i = 0; i < argc; ++i)
        args[i] = argv[i];

    char buf[256];

    for (;;) {
        // read lines from stdin
        int j = 0;
        while ((0 != read(0, buf+j, sizeof(char))) && buf[j] != '\n')
            ++j;
        if (j == 0) break; // read all lines
        buf[j] = 0; // end_of_string
        // put the arguments in stdin into args[]
        args[i] = buf;
        args[i+1] = 0;

//        if (fork() == 0) {
            // printf("command: %s\n", args[1]);
            // args[0] = xargs, args[1] = the command，the remaining are arguments
            exec(args[1], args+1);
            printf("exec error\n");
//        } else {
//            wait((void*)0);
//        }
    }

    exit(0);
}