#include <stdio.h>
#include <math.h>
#include <string.h>
#include <winsock2.h>
#include <mysql.h>
#include <errno.h>

int main()
{
	FILE*	repo;
	repo = fopen("C:\\Users\\Juni\\Desktop\\libsvm-3.20\\libsvm-3.20\\windows\\repo.txt", "w");
	fprintf(repo, "");
	fclose(repo);

	repo = fopen("C:\\Users\\Juni\\Desktop\\libsvm-3.20\\libsvm-3.20\\windows\\repo.txt", "a");

	for (int i = 0; i < 82; i++) {
		char jongmok_code[10] = {};
		scanf("%s", jongmok_code);
		
		char dest[1000] = "C:\\Users\\Juni\\Desktop\\libsvm-3.20\\libsvm-3.20\\windows\\result\\";
		strcat(dest, jongmok_code);
		strcat(dest, "_result.txt");


		FILE* in_file = fopen(dest, "r");

		if(in_file != NULL){
			char o_result[50] = {};
			fscanf(in_file, "%s", o_result);
			fprintf(repo, "%s\t%s\n", jongmok_code, o_result);
		}
		printf("%d\n", i);
		fclose(in_file);
	}
	fclose(repo);
	return 0;
}
