// Programa Soma gerado automaticamente por PIFC

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char string[1024];
typedef int bool;
enum {false, true};

int main(int argc, char *argv[]){
	double a, b, soma;
	printf("%s\n", "Digite dois numeros:");
	scanf("%lf %lf", &a, &b);
	soma = a + b;
	printf("%s%lf\n", "A soma deles eh : ", soma);
	return EXIT_SUCCESS;
}
