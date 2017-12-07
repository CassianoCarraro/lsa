#include <stdio.h>

#include "lexico.h"
#include "lexico.c"

int main() {
	char *exp = (char *) malloc(sizeof(char) * 600);
	char *lex = (char *) malloc(sizeof(char) * 20);
	char token_desc[53][17] = {TOKENS};
	char buffer[50];
	int file_pos = 0;

	FILE *fp, *saida;

	fp = fopen("program.c", "r");
	fread(exp, 1, 600, fp);
	printf("Programa\n=======================================================\n%s \n=======================================================\n", exp);

	saida = fopen("Saida.lex", "w");

	printf("%16s \t %10s \t Linha \t Coluna", "Token", "Lexema");
	printf("\n=======================================================\n");

	while((tk = le_token(exp, lex)) > 0) {
		sprintf(buffer, "%16s \t %10s \t %2d \t %2d\n", token_desc[tk-1], lex, linha, obter_coluna(lex));
		printf("%s", buffer);
		fwrite(buffer, strlen(buffer), 1, fp);
		file_pos += strlen(buffer);
	}

	fflush(saida);
	fclose(saida);
	fclose(fp);

	printf("\n=======================================================\n");

	if (tk == -1) {
		printf("Erro lexico: encontrou o caracter %s na linha: %d, coluna: %d\n", lex, linha, obter_coluna(lex));
	}

	#ifdef _WIN32
		system("pause");
	#endif
}