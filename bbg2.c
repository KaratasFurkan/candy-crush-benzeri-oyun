#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void print_game_table(char **matrix, int row, int column);
void randomize_game_table(char **matrix, int row, int column);
void randomize_empty_slots(char **matrix, int row, int column, int *change);
void check_neighbors(char **matrix, int i, int j, int reference, int i_max, int j_max, int *change, int *score);
void gez(char **matrix, int row, int column, int *change, int *score);
void gravity(char **matrix, int row, int column, int *change);
void swap(char *char1, char *char2);
int will_continue(char **matrix, int row, int column);
void print_dashes(int column);



void main(){
    int i, j, row, column, change, *score = (int *) malloc(sizeof(int)); *score = 0;
    char **matrix;
    srand(time(NULL));
    printf("Oyun alaninin satir ve sutun sayisini veriniz:  "); scanf("%d %d", &row, &column); getchar();
    matrix = (char **) malloc(row * sizeof(char *));
    for(i = 0; i < row; i++){
        matrix[i] = (char *) malloc(column * sizeof(char));
    }

    randomize_game_table(matrix, row, column);
    print_game_table(matrix, row, column);
    do{
        do{
            do{
                gez(matrix, row, column, &change, score);
                if(change){
                    getchar();
                    print_game_table(matrix, row, column);
                    gravity(matrix, row, column, &change);
                    if(change){
                        getchar();                                      // change yerine fonksionlaru returnlu yaz
                        print_game_table(matrix, row, column);
                    }
                }
            } while(change);
            getchar();
            randomize_empty_slots(matrix, row, column, &change);
        } while(change);
    } while(will_continue(matrix, row, column));

    printf("Puanınız: %d\n", *score * *score);
}




void print_game_table(char **matrix, int row, int column){
    int i,j;

    print_dashes(column);
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

void randomize_game_table(char **matrix, int row, int column){
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            matrix[i][j] = rand()%10 + '0';
        }
    }
}

void randomize_empty_slots(char **matrix, int row, int column, int *change){
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            if(matrix[i][j] == ' '){
                *change = 1;
            }
        }
    }
    if(*change){
        print_dashes(column);
        for(i = 0; i < row; i++){
            for(j = 0; j < column; j++){
                if(matrix[i][j] == ' '){
                    matrix[i][j] = rand()%10 + '0';
                    *change = 1;
                    printf("\033[1;31m%c \033[0m", matrix[i][j]);
                }
                else{
                    printf("%c ", matrix[i][j]);
                }
            }
            printf("\n");
        }
    }
}

void check_neighbors(char **matrix, int i, int j, int reference, int i_max, int j_max, int *change, int *score){
    int has_neighbor = 0;

    if(j < j_max){
        if(matrix[i][j] == matrix[i][j + 1] && reference != 3){
            check_neighbors(matrix, i, j + 1, 1, i_max, j_max, change, score);
            has_neighbor = 1;
        }
    }
    if(j > 0){
        if(matrix[i][j] == matrix[i][j - 1] && reference != 1){
            check_neighbors(matrix, i, j - 1, 3, i_max, j_max, change, score);   
            has_neighbor = 1;
        }
    }
    if(i < i_max){
        if(matrix[i][j] == matrix[i + 1][j] && reference != 4){
            check_neighbors(matrix, i + 1, j, 2, i_max, j_max, change, score);
            has_neighbor = 1;
        }
    }
    if(i > 0){
        if(matrix[i][j] == matrix[i - 1][j] && reference != 2){
            check_neighbors(matrix, i - 1, j, 4, i_max, j_max, change, score);
            has_neighbor = 1;
        }
    }
    if(reference != 0 || has_neighbor){
        matrix[i][j] = ' ';
        (*score)++;
        *change = 1;
    }
}

void gez(char **matrix, int row, int column, int *change, int *score){
    int i, j;
    *change = 0;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){        //nadiren burası da patlıyor
            if(matrix[i][j] != ' '){
                check_neighbors(matrix, i, j, 0, row - 1, column - 1, change, score);
            }
        }
    }
}

void gravity(char **matrix, int row, int column, int *change){
    int i, j, k;
    *change = 0;
    for(j = 0; j < column; j++){
        for(i = row - 1; i >= 0; i--){
            if(matrix[i][j] == ' '){
                for(k = i - 1; k >= 0; k--){
                    if(matrix[k][j] != ' '){
                        swap(&matrix[i][j], &matrix[k][j]);
                        *change = 1;
                        break;
                    }
                }
            }
        }
    }
}

void swap(char *char1, char *char2){
    *char1 = *char1 ^ *char2;
    *char2 = *char1 ^ *char2;
    *char1 = *char1 ^ *char2;
}

int will_continue(char **matrix, int row, int column){
    char devam;
    int i, j, i1, j1, i2, j2;

    print_dashes(column);
    do{
        printf("Devam etmek istiyor musunuz? (E/H) --> "); scanf("%c", &devam);
        if(!((devam == 'E') || (devam == 'e') || (devam == 'H') || (devam == 'h'))){
            getchar();
            printf("Yanlış bir harf girdiniz.\n");  // burası sıkıntılı
            devam = 0;
        }
    } while(!devam);
    if(devam == 'E' || devam == 'e'){
        printf("Yer degistirilecek elemanlarin indislerini veriniz: "); scanf("%d,%d %d,%d", &i1, &j1, &i2, &j2);
        printf("\n");
        print_dashes(column);

        for(i = 0; i < row; i++){
            for(j = 0; j < column; j++){
                if((i == i1 && j == j1) || (i == i2 && j == j2)){
                    printf("\033[1;32m%c \033[0m", matrix[i][j]);
                }
                else{
                    printf("%c ", matrix[i][j]);
                }
            }
            printf("\n");
        }
        
        swap(&matrix[i1][j1], &matrix[i2][j2]);
        getchar(); getchar();
        print_game_table(matrix, row, column);
    }
    return (devam == 'E') || (devam == 'e') ? 1 : 0;
}


void print_dashes(int column){
    int i;
    for(i = 1; i < 2 * column; i++){
        printf("—");
    }
    printf("\n\n");
}
