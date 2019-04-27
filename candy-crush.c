#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void print_game_table(char **matrix, int row, int column);
void randomize_game_table(char **matrix, int row, int column);
void randomize_empty_slots(char **matrix, int row, int column, int *change);
void check_neighbors(char **matrix, int i, int j, int reference, int i_max, int j_max, int *change, int *score);
void matrisi_dolas(char **matrix, int row, int column, int *change, int *score);
void gravity(char **matrix, int row, int column, int *change);
void swap(char *char1, char *char2);
void print_dashes(int column);
int will_continue(char **matrix, int row, int column);

// Genel bilgi:
// change değişkenini bazı işlemleri programın bazı işlemleri yapıp yapmayacağına karar
// vermek için koydum, bir fonksiyon matrisi değiştiren bir işlem yaptığında change 
// değişkenini 1 e çeviriyor.
// Koddaki getcharların çoğunu program çıktılarını entera bastıktan sonra versin diye koydum,
// bir kaç tanesini de scanf ile char alma konusundaki sıkıntıyı engellemek için koydum.


void main(){
    int i, j, row, column, change, *score = (int *) malloc(sizeof(int)); *score = 0;
    char **matrix;
    srand(time(NULL));  //system("COLOR FC"); 
    printf("Oyun alaninin satir ve sutun sayisini veriniz:  "); scanf("%d %d", &row, &column); getchar();
    matrix = (char **) malloc(row * sizeof(char *));         ///
    for(i = 0; i < row; i++){                                 // Matris oluşturma bloğu.
        matrix[i] = (char *) malloc(column * sizeof(char));   //
    }                                                        ///

    randomize_game_table(matrix, row, column);    // Tüm elemanlara 0-9 arası rastgele değer veriyor.
    print_game_table(matrix, row, column);
    do{
        do{
            do{
                matrisi_dolas(matrix, row, column, &change, score); // Matris elemanlarını dolaşıp aynı olanları patlatıyor.
                if(change){  // Eğer değişim(patlayan eleman) varsa;
                    getchar();
                    print_game_table(matrix, row, column);
                    gravity(matrix, row, column, &change);  // Havada kalanları aşağı indiriyor.
                    if(change){  // Aşağı inen eleman yoksa aynı matrisi tekrar yazdırmamak için if koydum.
                        getchar();                                     
                        print_game_table(matrix, row, column);
                    }
                }
            } while(change);  // Hiç bir eleman patlamayana kadar devam.
            getchar();
            randomize_empty_slots(matrix, row, column, &change);   // Boşluklara rastgele değer veriyor.
        } while(change);   // Hiç boşluk kalmayana kadar devam.
    } while(will_continue(matrix, row, column));  // Kullanıcı devam etmek isterse değişen elemanlar ile başa sarıyor.

    printf("Puanınız: %d\n\n", *score * *score);
}




void print_game_table(char **matrix, int row, int column){
    int i,j;

    print_dashes(column);   // Bu fonksiyon sadece görsellik için. Ekrana matrisleri ayıran çizgiyi basıyor.
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){            // Matrisi ekrana yazıyor.
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

void randomize_game_table(char **matrix, int row, int column){
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            matrix[i][j] = rand()%10 + '0';        // Matrisin ilklendirmesini yapıyor.
        }
    }
}

void randomize_empty_slots(char **matrix, int row, int column, int *change){
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){     
            if(matrix[i][j] == ' '){
                *change = 1;            // Boş yer yoksa gereksiz yere ekrana matrisi yazdırmamak için.
            }
        }
    }
    if(*change){
        print_dashes(column);
        for(i = 0; i < row; i++){
            for(j = 0; j < column; j++){
                if(matrix[i][j] == ' '){
                    matrix[i][j] = rand()%10  + '0';
                    *change = 1;
                    printf("\033[1;31m%c \033[0m", matrix[i][j]);       // Yeni eklenenleri renkli yaz,
                }
                else{
                    printf("%c ", matrix[i][j]);                        // eskileri normal.
                }
            }
            printf("\n");
        }
    }
}

void check_neighbors(char **matrix, int i, int j, int reference, int i_max, int j_max, int *change, int *score){
    int has_neighbor = 0;

    if(j < j_max){  // İlk ifler matrisin sınırlarını aşmamak için.
        if(matrix[i][j] == matrix[i][j + 1] && reference != 3){  // Geldiği elemanı tekrar kontrol etmesin diye reference koydum.
            check_neighbors(matrix, i, j + 1, 1, i_max, j_max, change, score); // Eğer eleman aynıysa o elemanı da kontrol ediyor.
            has_neighbor = 1; // Daha sonra patlatmak için elemanın komşusu var olarak işaretliyor.
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
        if(matrix[i][j] == matrix[i - 1][j] && reference != 2 && reference != 3){
            check_neighbors(matrix, i - 1, j, 4, i_max, j_max, change, score);
            has_neighbor = 1;
        }
    }
    if(reference != 0 || has_neighbor){
        matrix[i][j] = ' ';                 // Aynı olan komşusu varsa kendisini patlatıyor.
        (*score)++;                         // Reference kısmı ilk kontrol edilen elemanın komşusu olmamasına rağmen patlamaması için.
        *change = 1;
    }
}

void matrisi_dolas(char **matrix, int row, int column, int *change, int *score){
    int i, j;
    *change = 0;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){        // Matrisi dolaşıp check neighbors fonksiyonunu çağırıyor.
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
            if(matrix[i][j] == ' '){            // Eğer boşluğa denk gelirsen,
                for(k = i - 1; k >= 0; k--){    // yukarı doğru ilerke, 
                    if(matrix[k][j] != ' '){    // sayıya denk gelince, 
                        swap(&matrix[i][j], &matrix[k][j]);   // swap yap.
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
            printf("Yanlış bir harf girdiniz.\n");
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
    return (devam == 'E') || (devam == 'e') ? 1 : 0; // Mainde bu fonksiyonu while içine koymuştum. Devam edecekse true dönüyor.
}


void print_dashes(int column){      // Bu fonksiyon görsellik için, ekrana düz çizgi basıyor.
    int i;
    for(i = 1; i < 2 * column; i++){
        printf("—");
    }
    printf("\n\n");
}
