//Implementar IA básica?

#include <stdio.h>
#include <stdlib.h>
int IS_PLAYING = 1;
int PLATAFORMA = 0;
int TIPO_DE_JOGADA = 1; //[1, 2, 3] [normal, roque, en_passant]

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

peca peca_null = {'-', '-', 0};

typedef struct tabuleiro
{
    int linhas, colunas;
    peca mat[8][8];
} tabuleiro;

void confirmacao (char string[100]);

void clear_tab (tabuleiro *tab);
void print_tab (tabuleiro tab);
void print_peca(peca p);
void colocar_peca (tabuleiro *tab, posicao pos, peca peca);
peca remover_peca (tabuleiro *tab, posicao pos);
peca info_peca (tabuleiro tab, posicao pos);
int peca_inimiga (peca p, char jogador_atual);
int null_or_enemy (tabuleiro tab, posicao pos, char jogador_atual);

void realiza_jogada (tabuleiro *tab, posicao origem, posicao destino, char *jogador_atual);
peca executa_movimento (tabuleiro *tab, posicao origem, posicao destino);
void desfaz_movimento (tabuleiro *tab, posicao origem, posicao destino, peca peca_capturada);

void montar_tabuleiro (tabuleiro *tab);
void movimentos_possiveis (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8], int limparMatriz);
int origem_valida (tabuleiro tab, posicao pos, char jogador_atual);
int destino_valido (posicao pos, int movimentos_possiveis[8][8]);
posicao to_position (char cpos[3]);
int matriz_vazia (int matriz[8][8]);
void print_movimentos_possiveis (tabuleiro tab, int movimentos_possiveis[8][8]);
void troca_jogador_atual (char *jogador_atual);
int is_king (peca peca);
int existe_rei (tabuleiro tab, char jogador_atual);
int esta_em_xeque (tabuleiro *tab, char jogador_atual);
int teste_xeque_mate (tabuleiro *tab, char *jogador_atual);
void limpar_matriz (int matriz[8][8]);
int teste_sair_xeque (tabuleiro *tab, int matriz_teste[8][8], posicao origem, char *jogador_atual);
int fora_dos_limites (posicao pos);

void movimentos_torre (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_bispo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_cavalo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_peao (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
void movimentos_rei (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);

int en_passant_possivel(tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);
int promocao_possivel(tabuleiro tab, posicao pos);
void promover(tabuleiro *tab, posicao pos);
void roque_possivel(tabuleiro tab, posicao pos, int movimentos_possiveis[8][8]);

int main ()
{
    tabuleiro tab = {8, 8};
    char jogador_atual = 'b';
    printf ("Mudar para mobile? [1]/[0]\n");
    scanf ("%d", &PLATAFORMA);

    clear_tab (&tab);
    montar_tabuleiro (&tab);

    if (!existe_rei(tab, jogador_atual))
    {
        IS_PLAYING = 0;
        printf("Rei da cor '%c' faltando no tabuleiro!", jogador_atual);
        confirmacao("");
    }

    while (IS_PLAYING)
    {
        char origem[3];
        do
        {
            print_tab (tab);
            printf ("\n\nJogador atual: %c", jogador_atual);
            printf ("\nOrigem: ");
            setbuf (stdin, NULL);
            scanf ("%s", origem);
        } while (!origem_valida(tab, to_position(origem), jogador_atual));

        int movep[tab.linhas][tab.colunas];

        movimentos_possiveis(tab, to_position(origem), movep, 1);
        
        if (matriz_vazia(movep))
            confirmacao ("\n\nNao ha movimentos possiveis para a peca selecionada!\n\n");
        else
        {
            print_movimentos_possiveis (tab, movep);
            printf ("\nDestino: ");
            char destino[3];
            setbuf (stdin, NULL);
            scanf ("%s", destino);

            if (destino_valido (to_position(destino), movep))
            {
                printf ("\n\n");
                realiza_jogada (&tab, to_position(origem), to_position(destino), &jogador_atual);

                if (teste_xeque_mate(&tab, &jogador_atual))
                {
                    system ("cls");
                    print_tab (tab);
                    printf ("\n\nXEQUE MATE!");
                    troca_jogador_atual (&jogador_atual);
                    printf ("\nVencedor: %c", jogador_atual);
                    IS_PLAYING = 0;
                }
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
        for (j = 0; j < tab->colunas; j++)
            tab->mat[i][j] = peca_null;
}

void limpar_matriz (int matriz[8][8])
{
    int i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            matriz[i][j] = 0;
}

void print_tab (tabuleiro tab)
{
    int i, j;
    
    if (PLATAFORMA == 1) 
        printf("\e[1;1H\e[2J\n");
    else system ("cls");

    for (i = 0; i < tab.linhas; i++)
    {
        printf ("%d ", 8 - i);
        for (j = 0; j < tab.colunas; j++)
        {
            peca p = tab.mat[i][j];
            if (p.nome != '-')
            {
                if (p.cor == 'b')
                    printf ("%c ", tab.mat[i][j].nome);
                else
                {
                    printf("\e[33m%c ", p.nome);//Sequencia ANSI "33" amarelo;
                    printf("\e[0m");// "\e[" inicia sequencia; "0m" reinicia cor padrao;
                }
            }
            else
                printf ("%c ", peca_null.nome);
        }
        printf ("\n");
    }

    printf ("  a b c d e f g h");
}

void colocar_peca (tabuleiro *tab, posicao pos, peca peca)
{
    tab->mat[pos.linha][pos.coluna] = peca;
}

peca remover_peca (tabuleiro *tab, posicao pos)
{
    peca retirada = tab->mat[pos.linha][pos.coluna];
    tab->mat[pos.linha][pos.coluna] = peca_null;

    return retirada;
}

peca info_peca (tabuleiro tab, posicao pos)
{
    return tab.mat[pos.linha][pos.coluna];
}

peca executa_movimento (tabuleiro *tab, posicao origem, posicao destino)
{
    peca peca_origem = remover_peca (tab, origem);
    peca peca_destino = remover_peca (tab, destino);

    colocar_peca (tab, destino, peca_origem);
    tab->mat[destino.linha][destino.coluna].qtd_movimentos++;
    
    return peca_destino;
}

void desfaz_movimento (tabuleiro *tab, posicao origem, posicao destino, peca peca_capturada)
{
    peca peca_origem = remover_peca (tab, destino);
    colocar_peca (tab, origem, peca_origem);
    colocar_peca (tab, destino, peca_capturada);
    tab->mat[destino.linha][destino.coluna].qtd_movimentos--;
}

void realiza_jogada (tabuleiro *tab, posicao origem, posicao destino, char *jogador_atual)
{
    peca peca_capturada = executa_movimento (tab, origem, destino);
    
    if (esta_em_xeque(tab, *jogador_atual))
    {
        desfaz_movimento(tab, origem, destino, peca_capturada);
        confirmacao("Voce nao pode se colocar em xeque!");
        return;
    }

    if (is_king(peca_capturada))
    {
        desfaz_movimento (tab, origem, destino, peca_capturada);
        return;
    }

    if (TIPO_DE_JOGADA == 2)
    {
        if (destino.coluna - origem.coluna > 0)
        {
            colocar_peca(tab, (posicao){origem.linha, origem.coluna + 1}, peca_capturada);//move torre
            peca temp = executa_movimento(tab, destino, (posicao){destino.linha, destino.coluna - 1});//move rei
        }
        else
        {
            colocar_peca(tab, (posicao){origem.linha, origem.coluna - 1}, peca_capturada);
            peca temp = executa_movimento (tab, destino, (posicao){destino.linha, destino.coluna + 2});
        }
        TIPO_DE_JOGADA = 1;
    }

    if(promocao_possivel(*tab, destino))
        promover(tab, destino);

    troca_jogador_atual (jogador_atual);
}

void montar_tabuleiro (tabuleiro *tab)
{
    colocar_peca (tab, to_position("a1"), (peca) {'T', 'b', 0});
    colocar_peca (tab, to_position("b1"), (peca) {'C', 'b', 0});
    colocar_peca (tab, to_position("c1"), (peca) {'B', 'b', 0});
    colocar_peca (tab, to_position("d1"), (peca) {'D', 'b', 0});
    colocar_peca (tab, to_position("e1"), (peca) {'R', 'b', 0});
    colocar_peca (tab, to_position("f1"), (peca) {'B', 'b', 0});
    colocar_peca (tab, to_position("g1"), (peca) {'C', 'b', 0});
    colocar_peca (tab, to_position("h1"), (peca) {'T', 'b', 0});
    colocar_peca (tab, to_position("a2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("b2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("c2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("d2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("e2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("f2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("g2"), (peca) {'P', 'b', 0});
    colocar_peca (tab, to_position("h2"), (peca) {'P', 'b', 0});

    colocar_peca (tab, to_position("a8"), (peca) {'T', 'p', 0});
    colocar_peca (tab, to_position("b8"), (peca) {'C', 'p', 0});
    colocar_peca (tab, to_position("c8"), (peca) {'B', 'p', 0});
    colocar_peca (tab, to_position("d8"), (peca) {'D', 'p', 0});
    colocar_peca (tab, to_position("e8"), (peca) {'R', 'p', 0});
    colocar_peca (tab, to_position("f8"), (peca) {'B', 'p', 0});
    colocar_peca (tab, to_position("g8"), (peca) {'C', 'p', 0});
    colocar_peca (tab, to_position("h8"), (peca) {'T', 'p', 0});
    colocar_peca (tab, to_position("a7"), (peca) {'P', 'p', 0});
    colocar_peca (tab, to_position("b7"), (peca) {'P', 'p', 0});
    colocar_peca (tab, to_position("c7"), (peca) {'P', 'p', 0});
    colocar_peca (tab, to_position("d7"), (peca) {'P', 'p', 0});
    colocar_peca (tab, to_position("e7"), (peca) {'P', 'p', 0});
    colocar_peca (tab, to_position("f7"), (peca) {'P', 'p', 0});
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

int destino_valido (posicao pos, int movimentos_possiveis[8][8])
{
    if (!fora_dos_limites(pos))
    {
        if (movimentos_possiveis[pos.linha][pos.coluna] != 0)
        {
            TIPO_DE_JOGADA = movimentos_possiveis[pos.linha][pos.coluna];
            return 1;
        }
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

void movimentos_possiveis (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8], int limparMatriz)
{
    int i, j;
    peca aux = tab.mat[pos.linha][pos.coluna];

    if (limparMatriz)
    {
        for (i = 0; i < tab.linhas; i++)
        {
            for (j = 0; j < tab.colunas; j++)
            {
                movimentos_possiveis[i][j] = 0;
            }
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
        movimentos_peao (tab, pos, movimentos_possiveis);
        en_passant_possivel(tab, pos, movimentos_possiveis);
        break;
        
    case 'R':
        movimentos_rei (tab, pos, movimentos_possiveis);
        roque_possivel (tab, pos, movimentos_possiveis);
        break;   

    default:
        break; 
    }
}

void movimentos_torre (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    int i;
    peca torre = tab.mat[pos.linha][pos.coluna];

    //direita
    for (i = 1; i < (8 - pos.coluna); i++)
    {
        posicao pos_teste = {pos.linha, pos.coluna + i};
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        else if(peca_inimiga(peca_teste, torre.cor))  
        {   
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
            break;
        }
        else break;
    }

    //esquerda
    for (i = 1; i <= pos.coluna; i++)
    {
        posicao pos_teste = {pos.linha, pos.coluna - i};
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        else if(peca_inimiga(peca_teste, torre.cor))  
        {   
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
            break;
        }
        else break;
    }

    //cima
    for (i = 1; i <= pos.linha; i++)
    {
        posicao pos_teste = {pos.linha - i, pos.coluna};
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        else if(peca_inimiga(peca_teste, torre.cor))  
        {   
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
            break;
        }
        else break;
    }

    //baixo
    for (i = 1; i < (8 - pos.linha); i++)
    {
        posicao pos_teste = {pos.linha + i, pos.coluna};
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        else if(peca_inimiga(peca_teste, torre.cor))  
        {   
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
            break;
        }
        else break;
    }
}

void movimentos_cavalo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    peca cavalo = tab.mat[pos.linha][pos.coluna];

    //2cima 1direita
    posicao pos_teste = {pos.linha - 2, pos.coluna + 1};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, cavalo.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
     
    //1cima 2direita  
    pos_teste = (posicao) {pos.linha - 1, pos.coluna + 2};   
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, cavalo.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    //1baixo 2direita   
    pos_teste = (posicao) {pos.linha + 1, pos.coluna + 2};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, cavalo.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    
    //2baixo 1direita  
    pos_teste = (posicao) {pos.linha + 2, pos.coluna + 1}; 
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, cavalo.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    //2baixo 1esquerda
    pos_teste = (posicao) {pos.linha + 2, pos.coluna - 1}; 
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, cavalo.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    
    //1baixo 2esquerda
    pos_teste = (posicao) {pos.linha + 1, pos.coluna - 2}; 
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, cavalo.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    //1cima 2esquerda
    pos_teste = (posicao) {pos.linha - 1, pos.coluna - 2}; 
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, cavalo.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    
    //2cima 1esquerda
    pos_teste = (posicao) {pos.linha - 2, pos.coluna - 1};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, cavalo.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
}

void movimentos_bispo (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    int i, j;
    peca bispo = tab.mat[pos.linha][pos.coluna];
    
    //cima - direita
    for (i = 1, j = 1; i <= pos.linha && j < (8 - pos.coluna); i++, j++)
    {
        posicao pos_teste = {pos.linha - i, pos.coluna + j};
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        else if(peca_inimiga(peca_teste, bispo.cor))  
        {   
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
            break;
        }
        else break;
    }
    
    //baixo - direita
    for (i = 1, j = 1; i < (8 - pos.linha) && j < (8 - pos.coluna); i++, j++)
    {
        posicao pos_teste = {pos.linha + i, pos.coluna + j};
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        else if(peca_inimiga(peca_teste, bispo.cor))  
        {   
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
            break;
        }
        else break;
    }

    //baixo - esquerda
    for (i = 1, j = 1; i < (8 - pos.linha) && j <= pos.coluna; i++, j++)
    {
        posicao pos_teste = {pos.linha + i, pos.coluna - j};
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        else if(peca_inimiga(peca_teste, bispo.cor))
        {   
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
            break;
        }
        else break;
    }

    //cima - esquerda
    for (i = 1, j = 1; i <= (pos.linha) && j <= (pos.coluna); i++, j++)
    {
        posicao pos_teste = {pos.linha - i, pos.coluna - j};
        peca peca_teste = tab.mat[pos_teste.linha][pos_teste.coluna];

        if (peca_teste.nome == '-')
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
        else if(peca_inimiga(peca_teste, bispo.cor))  
        {   
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;   
            break;
        }
        else break;
    }
}

   
void movimentos_peao (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    peca peao = tab.mat[pos.linha][pos.coluna];
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
        peca peca_teste = info_peca(tab, pos_teste);
        if (!fora_dos_limites(pos_teste))
            if (peca_inimiga(peca_teste, peao.cor))
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

        pos_teste = (posicao) {pos.linha - 1, pos.coluna + 1};
        peca_teste = info_peca(tab, pos_teste);
        if (!fora_dos_limites(pos_teste))
            if (peca_inimiga(peca_teste, peao.cor))
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
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
        peca peca_teste = info_peca(tab, pos_teste);
        if (!fora_dos_limites(pos_teste))
            if (peca_inimiga(peca_teste, peao.cor))
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
   
        pos_teste = (posicao) {pos.linha + 1, pos.coluna + 1};
        peca_teste = info_peca(tab, pos_teste);
        if (!fora_dos_limites(pos_teste))
            if (peca_inimiga(peca_teste, peao.cor))
                movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
    }
}   

void movimentos_rei (tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    peca rei = info_peca(tab, pos);
    posicao pos_teste = {pos.linha - 1, pos.coluna};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, rei.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    pos_teste = (posicao){pos.linha - 1, pos.coluna + 1};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, rei.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    pos_teste = (posicao){pos.linha, pos.coluna + 1};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, rei.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    pos_teste = (posicao){pos.linha + 1, pos.coluna + 1};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, rei.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    pos_teste = (posicao){pos.linha + 1, pos.coluna};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, rei.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    pos_teste = (posicao){pos.linha + 1, pos.coluna - 1};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, rei.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    pos_teste = (posicao){pos.linha, pos.coluna - 1};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, rei.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;

    pos_teste = (posicao){pos.linha - 1, pos.coluna - 1};
    if (!fora_dos_limites(pos_teste))
        if (null_or_enemy(tab, pos_teste, rei.cor))
            movimentos_possiveis[pos_teste.linha][pos_teste.coluna] = 1;
}
    
void print_movimentos_possiveis (tabuleiro tab, int movimentos_possiveis[8][8])
{
    int i, j;
     
    if (PLATAFORMA == 1) 
        printf("\e[1;1H\e[2J\n");
    else system ("cls");

    for (i = 0; i < tab.linhas; i++)
    {
        printf ("%d ", 8 - i);
        for (j = 0; j < tab.colunas; j++)
        {
            peca p = tab.mat[i][j];
            if (movimentos_possiveis[i][j])
            {
                if (p.cor == 'b' || p.nome == peca_null.nome)
                {   //char padrao - background cinza
                    printf("\e[0;100m%c ", p.nome);
                    printf("\e[0m");
                }
                else
                {   //char amarelo - background cinza
                    printf("\e[33;100m%c ", p.nome);
                    printf("\e[0m");
                }
            }    
            else
            {
                if (p.cor == 'b' || p.nome == peca_null.nome)
                {
                    printf("\e[0m%c ", p.nome);
                }
                else
                {
                    printf("\e[33m%c ", p.nome);
                    printf("\e[0m");
                }
            }    
        }
        printf ("\n");
    }

    printf ("  a b c d e f g h\n");
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

int peca_inimiga (peca p, char jogador_atual)
{
    return (p.nome != '-' && p.cor != jogador_atual); 
}

int null_or_enemy (tabuleiro tab, posicao pos, char jogador_atual)
{
    peca peca_teste = tab.mat[pos.linha][pos.coluna];
    return (peca_teste.nome == '-' || peca_inimiga(peca_teste, jogador_atual));
}

int is_king (peca peca)
{
    if (peca.nome == 'R')
        return 1;

    return 0;
}

int existe_rei (tabuleiro tab, char jogador_atual)
{
    int i, j;
    int existe_rei = 0;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            peca p = tab.mat[i][j];
            if (is_king(p) && p.cor == jogador_atual)
                existe_rei = 1;
        }
    }
    
    return existe_rei;
}

int esta_em_xeque (tabuleiro *tab, char jogador_atual)
{
    int matriz_teste[8][8];
    limpar_matriz(matriz_teste);
    int i, j;

        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                if (peca_inimiga(tab->mat[i][j], jogador_atual))
                    movimentos_possiveis (*tab, (posicao){i, j}, matriz_teste, 0);
            }
        }

        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                peca teste_rei = tab->mat[i][j];
                if (is_king(teste_rei) && teste_rei.cor == jogador_atual)
                    if (matriz_teste[i][j] == 1)
                        return 1;
            }
        }
    
    return 0;
}

int teste_xeque_mate (tabuleiro *tab, char *jogador_atual)
{
    int i, j;
    int matriz_teste[tab->linhas][tab->colunas];
    limpar_matriz(matriz_teste);
    
    if (!esta_em_xeque(tab, *jogador_atual))
        return 0;
        
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (!peca_inimiga(tab->mat[i][j], *jogador_atual))
            {
                movimentos_possiveis (*tab, (posicao){i,j}, matriz_teste, 1);
                if (teste_sair_xeque (tab, matriz_teste, (posicao){i,j}, jogador_atual))
                    return 0;
            }
        }
    }
    
    return 1;
}    

int teste_sair_xeque (tabuleiro *tab, int matriz_teste[8][8], posicao origem, char *jogador_atual)
{
    int i, j;
    int saiu_xeque = 0;
    
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (matriz_teste[i][j])
            {
                peca peca_capturada = executa_movimento (tab, origem, (posicao){i,j});
                if (!esta_em_xeque(tab, *jogador_atual))
                    saiu_xeque = 1;
                    
                desfaz_movimento (tab, origem, (posicao){i,j}, peca_capturada);
            }
        }
    }    
    
    return saiu_xeque;
}

int fora_dos_limites (posicao pos)
{
    if (pos.linha < 0 || pos.linha > 7)
        return 1;
    if (pos.coluna < 0 || pos.coluna > 7) 
        return 1; 
        
    return 0;    
}

int en_passant_possivel(tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    peca peao = tab.mat[pos.linha][pos.coluna];
    if (peao.cor == 'b')
    {
        if (pos.linha != 3) return 0;

        posicao direita = {pos.linha, pos.coluna + 1};
        if (!fora_dos_limites(direita))
        {
            peca inimigo_dir = tab.mat[direita.linha][direita.coluna];
            if (inimigo_dir.nome == 'P' && inimigo_dir.qtd_movimentos == 1)
            movimentos_possiveis[direita.linha][direita.coluna] = 1;
        }

        posicao esquerda = {pos.linha, pos.coluna - 1};
        if (!fora_dos_limites(esquerda))
        {
            peca inimigo_esq = tab.mat[esquerda.linha][esquerda.coluna];
            if (inimigo_esq.nome == 'P' && inimigo_esq.qtd_movimentos == 1)
            movimentos_possiveis[esquerda.linha][esquerda.coluna] = 1;
        }
    }
    else
    {
        if (pos.linha != 4) return 0;

        posicao direita = {pos.linha, pos.coluna + 1};
        if (!fora_dos_limites(direita))
        {
            peca inimigo_dir = tab.mat[direita.linha][direita.coluna];
            if (inimigo_dir.nome == 'P' && inimigo_dir.qtd_movimentos == 1)
            movimentos_possiveis[direita.linha][direita.coluna] = 1;
        }

        posicao esquerda = {pos.linha, pos.coluna - 1};
        if (!fora_dos_limites(esquerda))
        {
            peca inimigo_esq = tab.mat[esquerda.linha][esquerda.coluna];
            if (inimigo_esq.nome == 'P' && inimigo_esq.qtd_movimentos == 1)
            movimentos_possiveis[esquerda.linha][esquerda.coluna] = 1;
        }
    }

    return 0;
}

int promocao_possivel(tabuleiro tab, posicao pos)
{
    peca p = tab.mat[pos.linha][pos.coluna];

    if(p.nome == 'P')
        if(p.qtd_movimentos > 0)
            if (pos.linha == 0 || pos.linha == 7)
            return 1;
    return 0;
}

void promover(tabuleiro *tab, posicao pos)
{
    printf("\nPromover peao\nPara qual peca deseja promover? [P, T, C, B, D]\n");
    char c;
    setbuf(stdin, NULL);
    scanf("%c", &c);

    if (c == 'P' || c == 'T' || c == 'C' || c == 'B' || c == 'D')
        tab->mat[pos.linha][pos.coluna].nome = c;
    else if (c == 'p' || c == 't' || c == 'c' || c == 'b' || c == 'd')
        tab->mat[pos.linha][pos.coluna].nome = (c - 32);
    else
    {
        tab->mat[pos.linha][pos.coluna].nome = 'D';
        confirmacao("Promocao Invalida! Promovendo para Dama...");  
    }
}

void roque_possivel(tabuleiro tab, posicao pos, int movimentos_possiveis[8][8])
{
    if (fora_dos_limites(pos) || !is_king(tab.mat[pos.linha][pos.coluna])) 
        return;

    peca rei = tab.mat[pos.linha][pos.coluna];
    if (rei.qtd_movimentos > 0 || esta_em_xeque(&tab, rei.cor))
        return;

    if (!fora_dos_limites((posicao){pos.linha, pos.coluna + 3}))
    {
        peca t1 = tab.mat[pos.linha][pos.coluna + 3];
        if (t1.nome == 'T' && t1.qtd_movimentos == 0 && t1.cor == rei.cor)
        {
            //nao ha peças entre a dama e a torre => roque pequeno disponivel
            if (tab.mat[pos.linha][pos.coluna + 1].nome == '-' && tab.mat[pos.linha][pos.coluna + 2].nome == '-')
            movimentos_possiveis[pos.linha][pos.coluna + 3] = 2;
        }
    }

    if (!fora_dos_limites((posicao){pos.linha, pos.coluna - 4}))
    {
        peca t2 = tab.mat[pos.linha][pos.coluna - 4];
        if (t2.nome == 'T' && t2.qtd_movimentos == 0 && t2.cor == rei.cor)
        {
            if (tab.mat[pos.linha][pos.coluna - 1].nome == '-' && tab.mat[pos.linha][pos.coluna - 2].nome == '-' && tab.mat[pos.linha][pos.coluna - 3].nome == '-')
            movimentos_possiveis[pos.linha][pos.coluna - 4] = 2;
        } 
    }
}