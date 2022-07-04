#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char* read_from_file(const char* filename)
{
	static char c[1000];
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL) {
		printf("Error! File cannot be opened.!!!!!\n");
		exit(1);
	}
	fscanf(fptr, "%[^\n]", c);
	fclose(fptr);
	return &(c[0]);
}

double get_x(const char* arg_expr, unsigned int sz)
{
	double ab[] = {0, 0};
	char tmp[50] = "";
	int ascii = 0;
	for(int i = 0; i < sz; ++i) {
		if('=' == arg_expr[i]){
			break;
		}
		ascii = arg_expr[i];
		if(' ' == arg_expr[i]) {continue;}
		if((ascii >= 48 && ascii <= 57) || arg_expr[i] == '.') {
			strncat(tmp, &arg_expr[i], 1);
			continue;
		} else if(ascii >= 97 && ascii <= 122) {
			if(0 == i || 0 == strcmp(tmp,"-")){
				char num = '1';
				strncat(tmp, &num, 1);
			}
			ab[0] += strtod(tmp, NULL);
			strcpy(tmp, "");
		} else if('-' == arg_expr[i] || '+' == arg_expr[i]) {
			if(0 == i){
				strncat(tmp, &arg_expr[i], 1);
			} else if(0 == strlen(tmp)) {
				strncat(tmp, &arg_expr[i], 1);
			} else if(0 < strlen(tmp)){
				ab[1] += strtod(tmp, NULL);
				strcpy(tmp, "");
				strncat(tmp, &arg_expr[i], 1);
			}
		}
	}
	if(0 == ab[1]) { ab[1] = strtod(tmp, NULL); }
	if(0 == ab[1]) {
		assert(0 != ab[0]);
	}
	return (-ab[1]/ab[0]);
}

void test(double* x)
{
	FILE *fptr;
	fptr = fopen("golden.txt", "r");
	if(fptr == NULL){
		printf("file can not be opened\n");
		exit(0);
	}
	char x_str[50];
	fscanf(fptr, " %50s", x_str);
	double x_gold = strtod(x_str, NULL);
	if(*x == x_gold) {
		printf("Right solution: x = %f\n", *x);
	} else {
		printf("The solution is wrong: x = %f\n", *x);
		printf("Right solution is: x = %f\n", x_gold);
	}
	fclose(fptr);
	return;
}

void create_output_file(double *x){
	char x_output[50];
	snprintf(x_output, 50, "%f", *x);
	FILE * fptr;
	fptr = fopen("output.txt", "w");
	fwrite(x_output, sizeof(x_output[0]), sizeof(x_output), fptr);
	fclose(fptr);
}

int main(int argc, char** argv)
{
	const char* equation = read_from_file(argv[1]);
	double x = get_x(equation, strlen(equation));
	printf("x is : %f\n", x);
	test(&x);
	create_output_file(&x);
	return 0;
}
