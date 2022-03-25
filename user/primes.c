#include "kernel/types.h"
#include "user/user.h"

void pipeline(int rp){

    //create buffer for the 1st element and others
    int p, n;

    //read reaches EOF
    if(read(rp, &p, sizeof(int)) == 0){
        close(p[0]);
        exit(0);
    }

    //print "prime"
    printf("prime %d\n", p);

    //create pipes recursively
    int ppl[2];
    pipe(ppl);

    //for child
    if(fork() == 0){
        close(ppl[1]);
        pipeline(ppl[0]);
    }
    //for parent
    //loop until EOF
    //if "potential" prime then write to the pipe
    else {
        close(ppl[0]);
        for(;;){
            if(read(rp, &n, sizeof(int)) == 0){
                //close the read terminal
                close(rp);
                break;
            }

            if(n%p != 0){
                write(ppl[1], &n, sizeof(int));
            }
        }
        close(ppl[1]);
        wait((int *) 0);
        exit(0);
    }

    //parent close write terminal
    //wait for child to return
}

int main(int argc, char const *argv[]){
    //create a pipe
    int p[2];
    pipe(p);

    //parent process generates numbers 2-35
    //write to the p[1] terminal
    if(fork() != 0){
        close(p[0]);
        for(int i=2; i<36; i++){
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait((int *) 0);
        exit(0);
    }
    else {
        close(p[1]);
        pipeline(p[0]);
        exit(0);
    }

    //wait for child process returns


    //child process creates pipeline recursively





}