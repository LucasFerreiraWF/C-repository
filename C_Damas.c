#include<stdio.h>

typedef struct peca
{
	char cor;
	int type;
}peca;

peca peca_null = {'-', -1};

typedef struct tabuleiro
{
	int linhas, colunas;
	peca mat[8][8];
}tabuleiro;

void clear_tab(tabuleiro *tab);
void print_tab(tabuleiro tab);
void montar_tabuleiro(tabuleiro *tab;);

int main()
{
	tabuleiro tab = {8, 8};
	clear_tab(&tab);
	montar_tabuleiro(&tab);
	print_tab(tab);
	
	return 0;
}

void clear_tab(tabuleiro *tab)
{
	int i, j;
	
	for (i = 0; i < tab->linhas; i++)
	{
		for(j = 0; j < tab->colunas; j++)
		{
			tab->mat[i][j] = peca_null;
		}
	}
}

void print_tab(tabuleiro tab)
{
	int i, j;
	
	for (i = 0; i < tab.linhas; i++)
	{
		printf("%d ", i);
		for(j = 0; j < tab.colunas; j++)
		{
			peca p = tab.mat[i][j];
			printf("%c ", p.cor);
		}
		printf("\n");
	}
	printf("  a b c d e f g h");
}

void montar_tabuleiro(tabuleiro *tab)
{
	int i, j;
	
	for (i = 0; i < 3; i++)
	{
		for(j = 0; j < tab->colunas; j++)
		{
			if ((i+j)%2 != 0)
					tab->mat[i][j] = (peca){'p', 0};
		}
	}

	for (i = 5; i < tab->linhas; i++)
	{
		for(j = 0; j < tab->colunas; j++)
		{
			if ((i+j)%2 != 0)
					tab->mat[i][j] = (peca){'b', 0};
		}
	}
}

void movimentos_possiveis(tabuleiro tab, int movimentos_possiveis[8][8])
{
	
}
