#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(const char *path, const char *filename, const char *etname)
{
    char buf[512], *p;  // file path
    int fd;             // file descriptor
    struct dirent de;   // directory
    struct stat st;     // file information

    // open the file
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot %s\n", path);
        return;
    }
    // get the file information
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    // determine file type
    switch (st.type) {
        // if file, then check name
        case T_FILE:
            if (etname && 0 == strcmp(etname, filename))
                printf("%s\n", path);
            break;
            // if directory, then find recursively
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            // scan every file in the directory
            while (read(fd, &de, sizeof(de)) == sizeof(de)){
                if (de.inum == 0)
                    continue;
                // scip "." and ".."
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);    // concatenate the strings
                p[DIRSIZ] = 0;                  // add end_of_string
                // printf("%s\n", buf);
                find(buf, filename, p);        // find recursively
            }
            break;
    }
    close(fd);
}

void
findfile(const char *path, const char *filename)
{
    find(path, filename, (void*)0);
}

int
main(int argc, char const *argv[])
{
    if (argc != 3) {
        printf("Usage: find <dir> <filename>");
        exit(1);
    }
    findfile(argv[1], argv[2]);
    exit(0);
}