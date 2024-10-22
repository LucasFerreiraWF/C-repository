#include <stdio.h>
#include <stdlib.h>

typedef struct posicao
{
    int linha, coluna;
} posicao;

typedef struct peca
{
    char nome;
    char cor;
} peca;

typedef struct tabuleiro
{
    int linhas, colunas;
    peca mat[8][8];
} tabuleiro;

void confirmacao (char string[100]);

void clear_tab (tabuleiro *tab);
void print_tab (tabuleiro tab);
void colocar_peca (tabuleiro *tab, posicao pos, peca peca);
peca remover_peca (tabuleiro *tab, posicao pos);
void realiza_jogada (tabuleiro *tab, posicao origem, posicao destino, char *jogador_atual);
void montar_tabuleiro (tabuleiro *tab);
void movimentos_possiveis (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
int origem_valida (tabuleiro tab, posicao pos);
int destino_valido (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
posicao to_position (char cpos[3]);
int matriz_vazia (int matriz[8][8]);
void print_movimentos_possiveis (tabuleiro tab, int movimentos_possiveis[8][8]);
char troca_jogador_atual (char *jogador_atual);

void movimentos_torre (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_bispo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_cavalo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_peao (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);

int main ()
{
    int is_playing = 1;
    tabuleiro tab = {8, 8};
    char jogador_atual = 'b';

    clear_tab (&tab);
    montar_tabuleiro (&tab);

    while (is_playing)
    {
        char origem[3];
        do
        {
            //não sei como, mas isso limpa o console
            printf("\e[1;1H\e[2J\n");
            print_tab (tab);
            printf ("\n\nOrigem (LxC): ");
            /*scanf("%d%d", &origem.linha, &origem.coluna);*/
            setbuf (stdin, NULL);
            scanf ("%s", origem);
        } while (!origem_valida(tab, to_position(origem)));

        int movep[tab.linhas][tab.colunas];

        movimentos_possiveis(tab, to_position(origem), movep);

        //Del later
        /*printf ("\n\n");
        int i, j;
        for (i = 0; i < 8; i++)
        {
            printf ("(%d) ", 8 - i);
            for (j = 0; j < 8; j++)
            {
                printf ("%d  ", movep[i][j]);
            }
            printf("\n");
        }
        printf ("    a  b  c  d  e  f  g  h\n\n");*/
        
        if (matriz_vazia(movep))
            printf ("\n\nNao ha movimentos possiveis para a peca selecionada!\n\n");
        else
        {
            printf ("\n\n");
            print_movimentos_possiveis (tab, movimentos_possiveis);
            printf ("\n\nDestino: ");
            char destino[3];
            setbuf (stdin, NULL);
            scanf ("%s", destino);

            if (destino_valido (tab, to_position(destino), movep))
            {
                printf ("\n\n");
                realiza_jogada (&tab, to_position(origem), to_position(destino), &jogador_atual);
            }
            else
                confirmacao ("Destino invalido! \nPressione Enter:");
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
        printf ("%d ", 8 - i);
        for (j = 0; j < tab.colunas; j++)
        {
            if (tab.mat[i][j].nome != '-')
                printf ("%c%c ", tab.mat[i][j].nome, tab.mat[i][j].cor);
            else
                printf ("%c  ", vazio);
        }
        printf ("\n");
    }

    printf ("  a  b  c  d  e  f  g  h");
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

void realiza_jogada (tabuleiro *tab, posicao origem, posicao destino, char *jogador_atual)
{
    peca peca_origem = remover_peca (tab, origem);
    peca peca_destino = remover_peca (tab, destino);

    colocar_peca (tab, destino, peca_origem);
    
    troca_jogador_atual (jogador_atual);
}

void montar_tabuleiro (tabuleiro *tab)
{
    colocar_peca (tab, (posicao) {3, 3}, (peca) {'T', 'b'});
    colocar_peca (tab, (posicao) {2, 4}, (peca) {'P', 'p'});
    colocar_peca (tab, (posicao) {3, 7}, (peca) {'P', 'p'});
    colocar_peca (tab, to_position("d4\0"), (peca) {'D', 'b'});
    colocar_peca (tab, to_position("e2\0"), (peca) {'C', 'b'});
}

int origem_valida (tabuleiro tab, posicao pos)
{
    if (pos.linha >= 0 && pos.linha < 8 && pos.coluna >= 0 && pos.coluna < 8)
        if (tab.mat[pos.linha][pos.coluna].nome != '-')
            return 1;

    return 0;
}

int destino_valido (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    if (pos.linha >= 0 && pos.linha < 8 && pos.coluna >= 0 && pos.coluna < 8)
    {
        if (movimentos_possiveis[pos.linha][pos.coluna] != 0)
            return 1;
    }

    return 0;
}

void movimentos_possiveis (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    int i, j;
    char nome = tab.mat[pos.linha][pos.coluna].nome;

    for (i = 0; i < tab.linhas; i++)
    {
        for (j = 0; j < tab.colunas; j++)
        {
            movimentos_possiveis[i][j] = 0;
        }
    }

    switch (nome)
    {
    case 'T':
        movimentos_torre (tab, pos, movimentos_possiveis);
        break;

    case 'C':
        movimentos_cavalo (tab, pos, movimentos_possiveis);
        break;
        
    case 'B':   
        movimentos_bispo (tab, pos, movimentos_possiveis);
        break;
        
    case 'D':
        movimentos_torre (tab, pos, movimentos_possiveis);
        movimentos_bispo (tab, pos, movimentos_possiveis);
        break;    
    }
}

posicao to_position (char cpos[3])
{
    posicao new_pos;
    char char_coluna = cpos[0];
    new_pos.coluna = (int)(char_coluna - 'a');

    char char_linha[2] = {cpos[1], '\0'};
    int x = atoi(char_linha); //ASCII to Int (stdlib)
    new_pos.linha = 8 - x;

    return new_pos;
}

int matriz_vazia (int matriz[8][8])
{
    int i, j;
    
    for (i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
           if (matriz[i][j] == 1)
               return 0;
        }       
    }
    
    return 1;
}

void movimentos_torre (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    int i, j;

    //direita
    for (i = 1; i < (8 - pos.coluna); i++)
    {
        posicao pos_teste = {pos.linha, pos.coluna + i};

        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;

        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }

    //esquerda
    for (i = 1; i <= pos.coluna; i++)
    {
        posicao pos_teste = {pos.linha, pos.coluna - i};

        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;

        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }

    //cima
    for (i = 1; i <= pos.linha; i++)
    {
        posicao pos_teste = {pos.linha - i, pos.coluna};

        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;

        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }

    //baixo
    for (i = 1; i < (8 - pos.linha); i++)
    {
        posicao pos_teste = {pos.linha + i, pos.coluna};

        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;

        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }
}

void movimentos_bispo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    int i, j;
    
    //cima - direita
    for (i = 1, j = 1; i <= pos.linha && j < (8 - pos.coluna); i++, j++)
    {
        posicao pos_teste = {pos.linha - i, pos.coluna + j};
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;
            
        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }
    
    //baixo - direita
    for (i = 1, j = 1; i < (8 - pos.linha) && j < (8 - pos.coluna); i++, j++)
    {
        posicao pos_teste = {pos.linha + i, pos.coluna + j};
        
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;
            
        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
    }

    //baixo - esquerda
    for (i = 1, j = 1; i < (8 - pos.linha) && j <= pos.coluna; i++, j++)
    {
        posicao pos_teste = {pos.linha + i, pos.coluna - j};

        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;

        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }

    //cima - esquerda
    for (i = 1, j = 1; i <= (pos.linha) && j <= (pos.coluna); i++, j++)
    {
        posicao pos_teste = {pos.linha - i, pos.coluna - j};

        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;

        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }
}

void movimentos_cavalo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    //2cima 1direita
    posicao pos_teste = {pos.linha - 2, pos.coluna + 1};
    if (pos_teste.linha >= 0 && pos_teste.coluna < 8)
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
     
    //1cima 2direita     
    pos_teste.linha = pos.linha - 1;
    pos_teste.coluna = pos.coluna + 2;
    if (pos_teste.linha >= 0 && pos_teste.coluna < 8)
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    //1baixo 2direita   
    pos_teste.linha = pos.linha + 1;
    pos_teste.coluna = pos.coluna + 2;
    if (pos_teste.linha < 8 && pos_teste.coluna < 8)
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    //2baixo 1direita   
    pos_teste.linha = pos.linha + 2;
    pos_teste.coluna = pos.coluna + 1;
    if (pos_teste.linha < 8 && pos_teste.coluna < 8)
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    //2baixo 1esquerda
    pos_teste.linha = pos.linha + 2;
    pos_teste.coluna = pos.coluna - 1;
    if (pos_teste.linha < 8 && pos_teste.coluna >= 0)
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    //1baixo 2esquerda
    pos_teste.linha = pos.linha + 1;
    pos_teste.coluna = pos.coluna - 2;
    if (pos_teste.linha < 8 && pos_teste.coluna >= 0)
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    //1cima 2esquerda
    pos_teste.linha = pos.linha - 1;
    pos_teste.coluna = pos.coluna - 2;
    if (pos_teste.linha >= 0 && pos_teste.coluna >= 0)
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    //2cima 1esquerda
    pos_teste.linha = pos.linha - 2;
    pos_teste.coluna = pos.coluna - 1;
    if (pos_teste.linha >= 0 && pos_teste.coluna >= 0)
        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
}
    
    
void print_movimentos_possiveis (tabuleiro tab, int movimentos_possiveis[8][8])
{
    int i, j;
    char vazio = '-';
    char vazio_movep = '_';

    for (i = 0; i < tab.linhas; i++)
    {
        printf ("%d ", 8 - i);
        for (j = 0; j < tab.colunas; j++)
        {
            if (tab.mat[i][j].nome != '-')
                printf ("%c%c ", tab.mat[i][j].nome, tab.mat[i][j].cor);
            else if (movimentos_possiveis[i][j] == 1)
                printf ("%c  ", vazio_movep);
            else    
                printf ("%c  ", vazio);
        }
        printf ("\n");
    }

    printf ("  a  b  c  d  e  f  g  h");
}

void confirmacao (char string[100])
{
    printf ("%s", string);
    char lixo[50];
    setbuf (stdin, NULL);
    fgets (lixo, 50, stdin);
    printf ("\n\n");
}

char troca_jogador_atual (char *jogador_atual)
{
    if (jogador_atual == 'b')
        *jogador_atual = 'p';
    else
        *jogador_atual = 'b';
}