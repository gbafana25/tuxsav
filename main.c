#include <stdio.h>
#include <string.h>

#include "swpread.h"


int start_dialog() {
    printf("-s : run setup dialog\n");
	printf("-i [document] [/path/to/file] : backup one file\n");
	printf("-a [document] [/path/to/file] : add file to backup list\n");
	
	printf("-c [document] : create document on server\n");
	printf("-d [document] : delete document object on server\n");
	printf("-f [document] [/path/to/file] : fetch document from server and put it into specified file\n");
	printf("-sync : automatically backup all files in config\n");
	printf("-restore : restore all files in config\n");
	printf("-h : display this help menu\n");

    return 0;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        start_dialog();
        return -1;
    }

    if(strncmp(argv[1], "-i", 2) == 0) {
        //printf("%s\n", argv[2]);
        read_raw(".config.json.swp");
    }

}