#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define __ARG_ERR_HANDLE__ {fprintf(stderr, "Error: not enough arguments."); exit(EXIT_FAILURE); }
#define __FILE_ERR_HANDLE__ {fprintf(stderr, "Error: can't open the file."); exit(EXIT_FAILURE); }
#define __MEMORY_ERR_HANDLE__ {fprintf(stderr, "Error: not enough memory."); exit(EXIT_FAILURE); }
#define __LINE_LENGTH_IN_BYTES__ 16
#define IS_WRITEABLE(c) (((int)c >= 32 && (int)c <= 255))

int main(int argc, char *argv[]) {
	if (argc == 1) __ARG_ERR_HANDLE__;

	FILE* fp;
	uint8_t offset = 0x0;
	size_t bytes, i;
	char* line = NULL, *aux;

	if (!(fp = fopen(*++argv, "rb"))) __FILE_ERR_HANDLE__;
	do {
		if(!(aux = (char*)realloc(line, __LINE_LENGTH_IN_BYTES__ + 1))) __MEMORY_ERR_HANDLE__;
		line = aux;

		bytes = fread(line, sizeof(char), __LINE_LENGTH_IN_BYTES__, fp);
		line[bytes] = 0;

		printf("%.7x0: ", offset++);
		for (i = 0; i < bytes; i++) {
			if (!IS_WRITEABLE(line[i])) line[i] = '.';
			printf("%x ", line[i]);
		}

		if (bytes != __LINE_LENGTH_IN_BYTES__) {
			bytes = __LINE_LENGTH_IN_BYTES__ - bytes;
			while (bytes--) printf("00 ");
		}
		printf("| %s\n", line);

	} while (!feof(fp));
	
	free(line);
	fclose(fp);

	return 0;
}
