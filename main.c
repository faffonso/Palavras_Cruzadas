#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

#define MAX 10
#define clear() system("cls")

//Estrutura de dados de cada palavra
struct gameData {
    char question[5*MAX];   //Dica para palabra
    char position;          //Orientação da palabra
    char word[MAX];         //Palabra
    int positionInitial;    //Linha ou coluna da palavra
    int status;             //Status se a palavra vou inserida
};
typedef struct gameData sdata_t;

//Funcao para imprimir a matrix
void printMatrix (char matrix[MAX][MAX], int vertical, int horizontal) {
    for (int i = 0; i < horizontal; i++) {
        for (int j = 0; j < vertical; j++) {
            printf(" %c |", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//Funcao que imprimi as informacoes de cada palavra
void printGameWords (sdata_t gameData[], int n) {
    int aux = 0;
    for (int i = 0; i < n; i++) {
        if (gameData[i].status == 1) {
            printf("%d: %s (%ld letras) - Posicionamento %c\n", aux, gameData[i].question, strlen(gameData[i].word), gameData[i].position);
            aux++;
        }
    }
    printf("\n");
}

//Funcao que limpa o terminal e imprimi o padrao
void newScreen () {
    clear();
    printf("--------------------------------------------------------\n");
    printf("\n");
}

//Funcao que confirma os dados de input
void printGameInfo(sdata_t data) {
    printf("--------------------------------------------------------\n");
    printf("\nPalavra: %s\
            \nFrase dica: %s\
            \nPosicionamento escolhido: %c\n", data.word, data.question, data.position);
}

//Funcao que verifica a insercao de uma nova palavra
int newWord () {
    char word;
    printf("--------------------------------------------------------\n");
    printf("\nDeseja adicionar uma nova palavra ? (S ou N)");

    scanf(" %c", &word);
    while (word != 'S' && word != 'N') {
        printf("Insercao errada, digite novamente (S ou N): \n");
        scanf(" %c", &word);
    }

    if (word == 'S')
        return 0;
    return 1;
}

/* Funcoes auxiliares para matrix principal do jogo */

//Inseri palavras horizontais na matrix
void insertHorizontal(char matrix[MAX][MAX], char word[MAX], int size, int p, int initialPosition) {
    int aux = 0;
    printf("colocando %s na coluna %i\n", word, p);
    for (int i = initialPosition; i < size+initialPosition; i++) { 
        matrix[p][i] = word[i-initialPosition];
    }
}

//Inseri palavras verticais na matrix
void insertVertical(char matrix[MAX][MAX], char word[MAX], int size, int q, int initialPosition) {
    int aux = 0;
    for (int i = initialPosition; i < size+initialPosition; i++){
        matrix[i][q] = word[aux];
        aux++;
    }
}

//Verifica se a palavra pode ser inserida no espaço
int empty(char text[], char compar[], int x, int size) {
    if (x > 0 && text[x - 1] != ' ')
        return 0;
    else if (strlen(compar) < size && text[x + strlen(compar)] != ' ' && strlen(compar) + x < size)
        return 0;
    for (int i = 0; i < strlen(compar); i++) {
        if (text[i+x] != ' ' && text[i+x] != compar[i]){
            return 0;
        }
    }
    return 1;
}

int main () {

    setlocale(LC_ALL,"Portuguese");

    //Declarando variávceis locais
    unsigned int vertical, horizontal, 
    i = 0, j= 0, countWord = 0,
    k = 0, verify, multi,
    positionInitial; 

    char aux[5*MAX];

    sdata_t *gameData;
    gameData = malloc(20 * sizeof(sdata_t));

    newScreen();
    
    //vertical = MAX, horizontal = MAX;
    
    char matrix[MAX][MAX];
    char auxVertical[MAX];
    char auxHorizontal[MAX];

    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            matrix[i][j] = ' ';
        }
    }

    printf("Insira o numero de linhas:  ");
    scanf("%d", &vertical);
    printf("Insira o numero de colunas: ");
    scanf("%d", &horizontal);

    printMatrix(matrix, horizontal, vertical);
    sleep(1);
        
    clear();
    
    /* Receber as palavras */

    i = 0;
    while (countWord < 20) {

        newScreen();

        printf("i) Insira a pergunta: ");
        scanf (" %[^\t\n]s", aux);
        memcpy(gameData[i].question, aux, 10*MAX);


        printf("ii) Insira o posicionamento da palavra (H ou V): ");
        scanf(" %c", &gameData[i].position);
        while (gameData[i].position != 'V' && gameData[i].position != 'H') {
            printf("ii) Incorreto, insira o posicionamento da palavra denovo (H ou V): ");
            scanf(" %c", &gameData[i].position);            
        }

        printf("iii) Insira a palavra: ");
        scanf("%s", aux);
        while (strlen(aux) > (gameData[i].position == 'H' ? horizontal : vertical)) {
            printf("iii) Incorreto, insira a palavra (MAX %d): ", gameData[i].position == 'H' ? horizontal : vertical);
            scanf("%s", aux);
        }
        memcpy(gameData[i].word, aux, MAX);

        countWord++;
        if(newWord()) break;

        i++;
    }
    
    char sAux[MAX];

    /* Montar */ 

    //Inseri as palavras

    while(k < countWord) {      
        
        if (gameData[k].position == 'H') {
            for (i = 1; i < vertical; i+=2) {
                for (j = 0; j < horizontal; j++) {
                        multi = MAX*(i);
                        auxHorizontal[j] = matrix[0][j+multi];
                }
                for (positionInitial = 0; positionInitial <= horizontal - strlen(gameData[k].word); positionInitial++) {
                    if (empty(auxHorizontal, gameData[k].word, positionInitial, horizontal)) {
                        insertHorizontal(matrix, gameData[k].word, strlen(gameData[k].word), i, positionInitial);
                        gameData[k].positionInitial = i;
                        gameData[k].status = 1;
                        k++;
                        verify = 1;
                        break;
                    }
                    else {
                        verify = 0;
                    }
                
                }
                if (verify == 1) break;
            }
        } else if (gameData[k].position == 'V') {
            for (i = 1; i < horizontal; i+=2) {
                verify = 1;
                for (j = 0; j < vertical; j++) {
                    multi = 10*(j);
                    auxVertical[j] = matrix[0][i+(multi)];
                }
                for (positionInitial = 0; positionInitial <= vertical - strlen(gameData[k].word); positionInitial++) {
                    if (empty(auxVertical, gameData[k].word, positionInitial, vertical)) {
                        insertVertical(matrix, gameData[k].word, strlen(gameData[k].word), i, positionInitial);
                        gameData[k].positionInitial = i;
                        gameData[k].status = 1;
                        k++;
                        verify = 1;
                        break;
                    }
                    else {
                        verify = 0;
                    }
                }
                if (verify == 1) break;
            }
        }
        if (verify == 0){
            k++;
            gameData[k].status = 0;
        }
    }
    
    printMatrix(matrix, horizontal, vertical);

    /* Jogar */

    char play[MAX][MAX];
    char auxPlay[MAX];
    for (i = 0; i < MAX; i++) {
        multi = MAX*i;
        for (j = 0; j < MAX; j++) {
            if (matrix[0][j+multi] != ' ')
                play[i][j] = '+';
            else
                play[i][j] = ' ';
        }
    }
    newScreen();
    printGameWords(gameData, countWord);
    printMatrix(play, horizontal, vertical);

    int endGame = 0;
    for (i = 0; i < countWord; i++) {
        if (gameData[i].status == 1)
            endGame++;
    }
    while(endGame != 0){
        printf("Palavras restantes: %d\n", endGame);
        printf("Insira a palavra: ");
        scanf("%s", aux);
        for (i = 0; i < countWord; i++) {
            if (strcmp(gameData[i].word, aux) == 0 && gameData[i].status == 1) {
                gameData[i].status = 0;
                if (gameData[i].position == 'H') {
                    for (j = 0; j < horizontal; j++){
                        if (play[gameData[i].positionInitial][j] == '+'){
                            positionInitial = j;
                            break;
                        }

                    }
                    for (j = positionInitial; j < strlen(gameData[i].word) + positionInitial; j++) {
                        play[gameData[i].positionInitial][j] = gameData[i].word[j - positionInitial];
                    }
                    endGame--;
                } else {                
                    for (j = 0; j < vertical; j++){
                        if (play[j][gameData[i].positionInitial] == '+'){
                            positionInitial = j;
                            break;
                        }
                    }    
                    for (j = positionInitial; j < strlen(gameData[i].word) + positionInitial; j++) {
                        play[j][gameData[i].positionInitial] = gameData[i].word[j - positionInitial];
                    }
                    endGame--;
                }
            }
        }


        newScreen();
        printGameWords(gameData, countWord);
        printMatrix(play, horizontal, vertical);


    }
    
    newScreen();
    printf("               VOCE GANHOU!!!!\n\n");
    printMatrix(play, horizontal, vertical);
    free(gameData);
    return 0;
}
