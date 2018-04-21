#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "sintatico.h"
#include "sintatico.c"

int main() {
	char *exp = (char *) malloc(sizeof(char) * TAMANHO_CODIGO);
	char *lex = (char *) malloc(sizeof(char) * TAMANHO_LEX);
	char *tk_rec = (char *) malloc(sizeof(char) * TAMANHO_LEX);
	int parse_res;

	FILE *fp;
	fp = fopen("program.c", "r");
	fread(exp, 1, 600, fp);
	fflush(fp);
	fclose(fp);

	parse_res = parse(exp, lex, tk_rec);

	if (parse_res == 1) {
		printf("OK\n");
	} else if (parse_res == 0) {
		printf("Erro sintatico: token %s não esperado na linha: %d, coluna: %d.\n", tk_rec, linha, obter_coluna(lex));
	} else if (parse_res == -1) {
		printf("Erro lexico: encontrou o caracter %s na linha: %d, coluna: %d\n", lex, linha, obter_coluna(lex));
	}

	free(exp);
	free(lex);

	#ifdef _WIN32
		system("pause");
	#endif
}