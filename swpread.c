#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_raw(char *file_path) {
    long fsize;
    FILE *f = fopen(file_path, "rb");
    fseek(f, 0L, SEEK_END);
    fsize = ftell(f);
    rewind(f);
    char *buf = (char*)malloc(fsize);
    fread(buf, 1, fsize, f);
    fclose(f);
    char *content = (char*)malloc(900*sizeof(char));
    strncpy(content, buf+(fsize-900), 899);
    for(int i = 0; i < fsize; i++) {
        fwrite(&buf[i], 1, sizeof(char), stdout);
    }
    printf("%s\n", content);
    free(buf);
    free(content);
    return 0;
}