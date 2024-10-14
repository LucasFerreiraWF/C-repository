#include <stdio.h>

typedef struct posicao
{
    int linha, coluna;
}posicao;

typedef struct peca
{
    char nome;
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
void movimentos_possiveis (tabuleiro *tab, posicao pos, int movimentos_possiveis[8][8]);
int origem_valida (tabuleiro tab, posicao pos);
int destino_valido (tabuleiro tab, posicao pos);

int main ()
{
    int is_playing = 1;
    tabuleiro tab = {8, 8};
    
    clear_tab (&tab);
    montar_tabuleiro (&tab);
    print_tab (tab);
    
    while (is_playing)
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
        
        int movep[tab.linhas][tab.colunas];
        
                    //Del later
        movimentos_possiveis(&tab, origem, movep);
        printf ("\n\n");
        int i, j;
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                printf ("%d  ", movep[i][j]);
            }
            printf("\n");    
        }
        
        printf ("\n\nDestino: ");
        posicao destino;
        scanf("%d", &destino.linha);
        scanf("%d", &destino.coluna);
        
        if (destino_valido (tab, destino))
        {
            realiza_jogada (&tab, origem, destino);
            //print_tab (tab);
        }
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
                printf ("%c%c ", tab.mat[i][j].nome, tab.mat[i][j].cor);
            else 
                printf ("%c  ", vazio);
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
    colocar_peca (tab, (posicao){3, 3}, (peca){'T', 'b'});
    colocar_peca (tab, (posicao){2, 4}, (peca){'P', 'p'});
    colocar_peca (tab, (posicao){3, 5}, (peca){'P', 'p'});
}

int origem_valida (tabuleiro tab, posicao pos)
{
    int ver = 0;

    if (pos.linha >= 0 && pos.linha < 8 && pos.coluna >= 0 && pos.coluna < 8)
        if (tab.mat[pos.linha][pos.coluna].nome != '-')
            ver = 1;

    return ver;
}

int destino_valido (tabuleiro tab, posicao pos)
{
    int ver = 0;
    
    if (pos.linha >= 0 && pos.linha < 8 && pos.coluna >= 0 && pos.coluna < 8)
    {
       
    }
    
    return ver;
}
    
void movimentos_possiveis (tabuleiro *tab, posicao pos, int movimentos_possiveis[8][8])
{
    int i, j;
    posicao pos_original = pos;
    char nome = tab->mat[pos.linha][pos.coluna].nome;
    
    for (i = 0; i < tab->linhas; i++)
    {
        for (j = 0; j < tab->colunas; j++)
        {
            movimentos_possiveis[i][j] = 0;
        }
    }
    
    switch (nome)
    {
        case 'T':
        //direita
        for (i = 1; i < (8 - pos_original.coluna); i++)
        {
            posicao pos_teste = {pos.linha, pos.coluna + i};
            
            if (tab->mat[pos_teste.linha][pos_teste.coluna].nome != '-')
                break;
                
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        }

        //esquerda
        for (i = 1; i <= pos.coluna; i++)
        {
            posicao pos_teste = {pos.linha, pos.coluna - i};

            if (tab->mat[pos_teste.linha][pos_teste.coluna].nome != '-')
                break;

            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        }
        break;
    
        case 'C':
        break;
    }
}
    
