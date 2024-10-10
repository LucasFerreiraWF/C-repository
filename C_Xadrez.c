#include <stdio.h>

typedef struct posicao
{
    int linha, coluna;
}posicao;

typedef struct peca
{
    char nome;
    posicao pos;
    char cor;
}peca;

typedef struct tabuleiro
{
    int linhas, colunas;
    peca mat[8][8];
}tabuleiro;

void clear_tab (tabuleiro *tab);
void print_tab (tabuleiro tab);
void colocar_peca (tabuleiro *tab, posicao pos, peca peca);

int main ()
{
    tabuleiro tab;
    tabuleiro *ptab;
    ptab = &tab;
    tab.linhas = 8;
    tab.colunas = 8;

    clear_tab (ptab);
    
    peca peao;
    peao.nome = 'p';
    peao.cor = 'b';
    posicao pos;
    pos.linha = 0;
    pos.coluna = 0;

    colocar_peca (ptab, pos, peao);
    print_tab (tab);
    
    return 0;
}

void clear_tab (tabuleiro *tab)
{
    int i, j;
    for (i = 0; i < tab->linhas; i++)
    {
        for (j = 0; j < tab->colunas; j++)
        {
            tab->mat[i][j].nome = '-';
        }
    }
}

void print_tab (tabuleiro tab)
{
    int i, j;
    char vazio = '-';

    for (i = 0; i < tab.linhas; i++)
    {
        for (j = 0; j < tab.colunas; j++)
        {
            if (tab.mat[i][j].nome != '-')
                printf ("%c ", tab.mat[i][j].nome);
            else 
                printf ("%c ", vazio);
        }
        printf ("\n");
    }
}

void colocar_peca (tabuleiro *tab, posicao pos, peca peca)
{
    tab->mat[pos.linha][pos.coluna] = peca;
    peca.pos = pos;
}