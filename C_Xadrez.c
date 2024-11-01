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
    int qtd_movimentos;
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
int origem_valida (tabuleiro tab, posicao pos, char jogador_atual);
int destino_valido (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
posicao to_position (char cpos[3]);
int matriz_vazia (int matriz[8][8]);
void print_movimentos_possiveis (tabuleiro tab, int movimentos_possiveis[8][8]);
void troca_jogador_atual (char *jogador_atual);
int is_king (peca peca);
int esta_em_xeque (peca p, tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
int fora_dos_limites (posicao pos);

void movimentos_torre (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_bispo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_cavalo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_peao (peca peao, tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_rei (tabuleiro *tab, posicao pos, int movimentos_possiveis[8][8]);

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
            //limpar console mobile
            //printf("\e[1;1H\e[2J\n");
            //limpar console windows
            system ("cls");

            print_tab (tab);
            printf ("\n\nJogador atual: %c", jogador_atual);
            printf ("\nOrigem (LxC): ");
            setbuf (stdin, NULL);
            scanf ("%s", origem);
        } while (!origem_valida(tab, to_position(origem), jogador_atual));

        int movep[tab.linhas][tab.colunas];

        movimentos_possiveis(tab, to_position(origem), movep);

        //Del later
        printf ("\n\n");
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
        printf ("    a  b  c  d  e  f  g  h\n\n");
        
        if (matriz_vazia(movep))
            confirmacao ("\n\nNao ha movimentos possiveis para a peca selecionada!\n\n");
        else
        {
            //print_movimentos_possiveis (tab, movep);
            printf ("\nDestino: ");
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
            tab->mat[i][j].cor = '-';
            tab->mat[i][j].qtd_movimentos = 0;
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
    peca peca_null = {'-', '-'};

    peca retirada = tab->mat[pos.linha][pos.coluna];
    tab->mat[pos.linha][pos.coluna] = peca_null;

    return retirada;
}

void realiza_jogada (tabuleiro *tab, posicao origem, posicao destino, char *jogador_atual)
{
    peca peca_origem = remover_peca (tab, origem);
    peca peca_destino = remover_peca (tab, destino);

    colocar_peca (tab, destino, peca_origem);
    tab->mat[destino.linha][destino.coluna].qtd_movimentos++;
    troca_jogador_atual (jogador_atual);
}

void montar_tabuleiro (tabuleiro *tab)
{
    colocar_peca (tab, to_position("a1"), (peca) {'T', 'b', 0});
    colocar_peca (tab, to_position("b1"), (peca) {'B', 'b', 0});
    colocar_peca (tab, to_position("g1"), (peca) {'B', 'b', 0});
    colocar_peca (tab, to_position("h1"), (peca) {'T', 'b', 0});
    colocar_peca (tab, to_position("a2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("b2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("g2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("h2"), (peca) {'P', 'b', 0});

    colocar_peca (tab, to_position("a8"), (peca) {'T', 'p', 0});
    colocar_peca (tab, to_position("b8"), (peca) {'B', 'p', 0});
    colocar_peca (tab, to_position("g8"), (peca) {'B', 'p', 0});
    colocar_peca (tab, to_position("h8"), (peca) {'T', 'p', 0});
    colocar_peca (tab, to_position("a7"), (peca) {'P', 'p', 0});
    colocar_peca (tab, to_position("b7"), (peca) {'P', 'p', 0});
    colocar_peca (tab, to_position("g7"), (peca) {'P', 'p', 0});
    colocar_peca (tab, to_position("h7"), (peca) {'P', 'p', 0});
}

int origem_valida (tabuleiro tab, posicao pos, char jogador_atual)
{
    if (tab.mat[pos.linha][pos.coluna].nome == '-')
    {
        confirmacao ("Nao ha peca na origem escolhida! \nPressione Enter:");
        return 0;
    }
    else if (tab.mat[pos.linha][pos.coluna].cor != jogador_atual)
    {
        confirmacao ("A peca escolhida nao pertence a voce! \nPressione Enter:");
        return 0;
    }
    else if (pos.linha < 0 && pos.linha >= 8 && pos.coluna < 0 && pos.coluna >= 8)
    {
        confirmacao ("Origem invalida! \nPressione Enter:");
        return 0;
    }
    
    return 1;
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

void movimentos_possiveis (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    int i, j;
    peca aux = tab.mat[pos.linha][pos.coluna];

    for (i = 0; i < tab.linhas; i++)
    {
        for (j = 0; j < tab.colunas; j++)
        {
            movimentos_possiveis[i][j] = 0;
        }
    }

    switch (aux.nome)
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
        
    case 'P':
        movimentos_peao (aux, tab, pos, movimentos_possiveis);
        break;
        
    case 'R':
        //movimentos_rei (tab, pos, movimentos_possiveis)
        break;    
    }
}

void movimentos_torre (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    int i;

    //direita
    for (i = 1; i < (8 - pos.coluna); i++)
    {
        posicao pos_teste = {pos.linha, pos.coluna + i};

        /*if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;*/

        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }

    //esquerda
    for (i = 1; i <= pos.coluna; i++)
    {
        posicao pos_teste = {pos.linha, pos.coluna - i};

        /*if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;*/

        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }

    //cima
    for (i = 1; i <= pos.linha; i++)
    {
        posicao pos_teste = {pos.linha - i, pos.coluna};

        /*if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;*/

        movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }

    //baixo
    for (i = 1; i < (8 - pos.linha); i++)
    {
        posicao pos_teste = {pos.linha + i, pos.coluna};

        /*if (tab.mat[pos_teste.linha][pos_teste.coluna].nome != '-')
            break;*/

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
        peca torre = tab.mat[pos.linha][pos.coluna];
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome != '-' && peca_teste.cor != torre.cor)     
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1; 
    }
    
    //baixo - direita
    for (i = 1, j = 1; i < (8 - pos.linha) && j < (8 - pos.coluna); i++, j++)
    {
        posicao pos_teste = {pos.linha + i, pos.coluna + j};
        peca torre = tab.mat[pos.linha][pos.coluna];
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome != '-' && peca_teste.cor != torre.cor)     
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
    }

    //baixo - esquerda
    for (i = 1, j = 1; i < (8 - pos.linha) && j <= pos.coluna; i++, j++)
    {
        posicao pos_teste = {pos.linha + i, pos.coluna - j};
        peca torre = tab.mat[pos.linha][pos.coluna];
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome != '-' && peca_teste.cor != torre.cor)     
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1; 
    }

    //cima - esquerda
    for (i = 1, j = 1; i <= (pos.linha) && j <= (pos.coluna); i++, j++)
    {
        posicao pos_teste = {pos.linha - i, pos.coluna - j};
        peca torre = tab.mat[pos.linha][pos.coluna];
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome != '-' && peca_teste.cor != torre.cor)     
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1; 
    }
}

void movimentos_cavalo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    //2cima 1direita
    posicao pos_teste = {pos.linha - 2, pos.coluna + 1};
    if (!fora_dos_limites(pos_teste))
    {
        /*peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
        if (!is_king(peca_teste))*/
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    } 
     
    //1cima 2direita     
    pos_teste.linha = pos.linha - 1;
    pos_teste.coluna = pos.coluna + 2;
    if (!fora_dos_limites(pos_teste))
    {
        /*peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
        if (!is_king(peca_teste))*/
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }
    //1baixo 2direita   
    pos_teste.linha = pos.linha + 1;
    pos_teste.coluna = pos.coluna + 2;
    if (!fora_dos_limites(pos_teste))
    {    
        /*peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
        if (!is_king(peca_teste))*/
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }
    
    //2baixo 1direita   
    pos_teste.linha = pos.linha + 2;
    pos_teste.coluna = pos.coluna + 1;
    if (!fora_dos_limites(pos_teste))
    {
        /*peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
        if (!is_king(peca_teste))*/
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }

    //2baixo 1esquerda
    pos_teste.linha = pos.linha + 2;
    pos_teste.coluna = pos.coluna - 1;
    if (!fora_dos_limites(pos_teste))
    {
        /*peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
        if (!is_king(peca_teste))*/
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }
    
    //1baixo 2esquerda
    pos_teste.linha = pos.linha + 1;
    pos_teste.coluna = pos.coluna - 2;
    if (!fora_dos_limites(pos_teste))
    {
        /*peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
        if (!is_king(peca_teste))*/
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }        

    //1cima 2esquerda
    pos_teste.linha = pos.linha - 1;
    pos_teste.coluna = pos.coluna - 2;
    if (!fora_dos_limites(pos_teste))
    {
        /*peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
        if (!is_king(peca_teste))*/
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }    
    
    //2cima 1esquerda
    pos_teste.linha = pos.linha - 2;
    pos_teste.coluna = pos.coluna - 1;
    if (!fora_dos_limites(pos_teste))
    {
        /*peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
        if (!is_king(peca_teste))*/
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }    
}
   
void movimentos_peao (peca peao, tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    if (peao.cor == 'b')
    {
        //só anda se a casa a frente estiver vazia
        posicao pos_teste = {pos.linha - 1, pos.coluna};
        if (!fora_dos_limites(pos_teste))
            if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            {
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
                if (peao.qtd_movimentos == 0)
                {
                    posicao pos_teste = {pos.linha - 2, pos.coluna};
                    if (!fora_dos_limites(pos_teste))
                        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
                            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
                }
            }
        
        //ataca caso haja inimigo na diagonal
        pos_teste = (posicao) {pos.linha - 1, pos.coluna - 1};
        if (!fora_dos_limites(pos_teste))
        {
            peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
            if (peca_teste.nome != '-' && peca_teste.cor != 'b')    
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        } 

        pos_teste = (posicao) {pos.linha - 1, pos.coluna + 1};
        if (!fora_dos_limites(pos_teste))
        {
            peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
            if (peca_teste.nome != '-' && peca_teste.cor != 'b')    
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        }   
    }
    else
    {
        posicao pos_teste = {pos.linha + 1, pos.coluna};
        if (!fora_dos_limites(pos_teste))
            if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
            {
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
                if (peao.qtd_movimentos == 0)
                {
                    posicao pos_teste = {pos.linha + 2, pos.coluna};
                    if (!fora_dos_limites(pos_teste))
                        if (tab.mat[pos_teste.linha][pos_teste.coluna].nome == '-')
                            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
                }
            }

        pos_teste = (posicao) {pos.linha + 1, pos.coluna - 1};
        if (!fora_dos_limites(pos_teste))
        {
            peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
            if (peca_teste.nome != '-' && peca_teste.cor != 'p')    
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        }   
        
        pos_teste = (posicao) {pos.linha + 1, pos.coluna + 1};
        if (!fora_dos_limites(pos_teste))
        {
            peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];
            if (peca_teste.nome != '-' && peca_teste.cor != 'p')    
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        }    
    }
}   

void movimentos_rei (tabuleiro *tab, posicao pos, int movimentos_possiveis[8][8])
{
    posicao pos_teste = {pos.linha - 1, pos.coluna + 1};
    if (!fora_dos_limites(pos_teste))
    {
        peca peca_teste = tab->mat[pos_teste.linha][pos_teste.coluna];
        if (!is_king(peca_teste))
        {
            //realiza_jogada(tab, pos, pos_teste, );
            //if ()
        }
    }
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

void troca_jogador_atual (char *jogador_atual)
{
    if (*jogador_atual == 'b')
        *jogador_atual = 'p';
    else
        *jogador_atual = 'b';
}

int is_king (peca peca)
{
    if (peca.nome == 'R')
        return 1;

    return 0;
}

int esta_em_xeque (peca p, tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    /*if (is_king(tab.mat[pos.linha][pos.coluna]))
        if (matriz_vazia(movimentos_possiveis))
            return 1;*/
    
    return 0;
}

int fora_dos_limites (posicao pos)
{
    if (pos.linha < 0 || pos.linha > 7)
        return 1;
    if (pos.coluna < 0 || pos.coluna > 7) 
        return 1; 
        
    return 0;    
}