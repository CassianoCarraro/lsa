#ifndef TABELA_SLR_DEF_H
#define TABELA_SLR_DEF_H

#define QTD_ELEM_GRAMATICA 6

#define TK_FIM_SENTENCA 0
#define TK_PROD_E -1
#define TK_PROD_F -2
#define TK_PROD_G -3

#define GRAMATICA TK_SOMA, TK_CONST, TK_FIM_SENTENCA, TK_PROD_E, TK_PROD_F, TK_PROD_G

#define TABELA_SLR \
{NULL, 		     cel('S', 3, 1), NULL, 		 	  NULL, cel(' ', 1, 1), cel(' ', 2, 1)}, \
{cel('S', 4, 1), NULL, 	         cel('A', -1, 1), NULL, NULL, 	   	  	NULL}, \
{cel('R', 2, 1), NULL, 	   		 cel('R', 2, 1),  NULL, NULL, 	   		NULL}, \
{cel('R', 3, 1), NULL, 	   		 cel('R', 3, 1),  NULL, NULL,       	NULL}, \
{NULL, 		     cel('S', 3, 1), NULL, 		 	  NULL, NULL, 	   		cel(' ', 5, 1)}, \
{cel('R', 1, 2), NULL, 	   		 cel('R', 1, 2),  NULL, NULL, 	   		NULL} \

struct slr_cel {
	char op;
	int estado;
	int qtd_desempilhar;
} typedef slr_cel;

struct slr_busca {
	int *tk;
	slr_cel *cel;
} typedef slr_busca;

struct slr_tabela {
	int gramatica[QTD_ELEM_GRAMATICA];
	slr_cel *slr_estados[6][QTD_ELEM_GRAMATICA];
} typedef slr_tabela;

#endif