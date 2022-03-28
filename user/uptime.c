#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){

    int total_ticks;

    total_ticks = uptime();

    printf("%d\n", total_ticks);
    exit(0);
}