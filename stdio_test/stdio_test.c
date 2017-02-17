#include <stdio.h>
#include <wchar.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage:stdio_test file\n");
		return -1;
	}
	FILE *fp = fopen(argv[1], "r+");
	char buf[500];
	char buf2[500];
	setbuf(fp, buf);
	if(fp == NULL){
		printf("fopen failed.errno:%s\n", strerror(errno));
		return -2;
	}
	int n = fread(buf2, 1, 4, fp);
	printf("n = %d %s\n", n, buf2);
	fprintf(fp, "hello world in file xxx");

	fclose(fp);
	printf("\nEOF=%d\n", EOF);	
	FILE *tmp = tmpfile();
	fprintf(tmp, "hello, this is a tmp file.\n");
	sleep(20);
	fclose(tmp);
	return 0;
}
