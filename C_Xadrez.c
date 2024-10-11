#include <stdio.h>

typedef struct posicao
{
    int linha, coluna;
}posicao;

typedef struct peca
{
    char nome;
    //posicao pos;
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
peca remover_peca (tabuleiro *tab, posicao pos);
void realiza_jogada (tabuleiro *tab, posicao origem, posicao destino);
void montar_tabuleiro (tabuleiro *tab);
int origem_valida (tabuleiro tab, posicao pos);

int main ()
{
    int is_playing = 1;
    tabuleiro tab = {8, 8};
    
    clear_tab (&tab);
    montar_tabuleiro (&tab);
    print_tab (tab);
    
    while (is_playing == 1)
    {
        /*char origem[3];
        scanf ("%s", &origem);*/
        posicao origem;
        do
        {
            printf ("\n\nOrigem (LxC): ");
            scanf("%d", &origem.linha);
            scanf("%d", &origem.coluna);
        } while (!origem_valida(tab, origem));
        
        printf ("\n\nDestino: ");
        posicao destino;
        scanf("%d", &destino.linha);
        scanf("%d", &destino.coluna);
        
        realiza_jogada (&tab, origem, destino);
        
        print_tab (tab);
    }
    
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
    //peca.pos = pos;
}

peca remover_peca (tabuleiro *tab, posicao pos)
{
    peca peca_null;
    peca_null.nome = '-';
    
    peca retirada = tab->mat[pos.linha][pos.coluna];
    tab->mat[pos.linha][pos.coluna] = peca_null;
    
    return retirada;
}

void realiza_jogada (tabuleiro *tab, posicao origem, posicao destino)
{
    peca peca_origem = remover_peca (tab, origem);
    peca peca_destino = remover_peca (tab, destino);
    
    colocar_peca (tab, destino, peca_origem);
}

void montar_tabuleiro (tabuleiro *tab)
{
    colocar_peca (tab, (posicao){0, 0}, (peca){'t', 'b'});
    colocar_peca (tab, (posicao){2, 4}, (peca){'p', 'b'});
}

int origem_valida (tabuleiro tab, posicao pos)
{
    int ver = 0;

    if (pos.linha >= 0 && pos.linha < 8 && pos.coluna >= 0 && pos.coluna < 8)
        if (tab.mat[pos.linha][pos.coluna].nome != '-')
            ver = 1;

    return ver;
}