#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SHOWS 10000
#define MAX_FIELD 512
#define MAX_LIST 50

typedef struct {
    char show_id[MAX_FIELD];
    char type[MAX_FIELD];
    char title[MAX_FIELD];
    char director[MAX_LIST][MAX_FIELD];
    int director_count;
    char cast[MAX_LIST][MAX_FIELD];
    int cast_count;
    char country[MAX_LIST][MAX_FIELD];
    int country_count;
    char date[MAX_FIELD];
    int release_year;
    char rating[MAX_FIELD];
    char duration[MAX_FIELD];
    char listen_in[MAX_LIST][MAX_FIELD];
    int listen_count;
} Show;

Show shows[MAX_SHOWS];
int show_count = 0;

void removeN_R(char* texto) {
    int tamanho = strlen(texto);
    if (tamanho > 0 && (texto[tamanho - 1] == '\n' || texto[tamanho - 1] == '\r'))
        texto[tamanho - 1] = '\0';
}

void split_list(char* entrada, char lista[MAX_LIST][MAX_FIELD], int* total_itens) {
    *total_itens = 0;
    char* parte = strtok(entrada, ",");
    while (parte != NULL && *total_itens < MAX_LIST) {
        while (*parte == ' ') parte++;
        strcpy(lista[*total_itens], parte);
        (*total_itens)++;
        parte = strtok(NULL, ",");
    }
}

void sort_list(char lista[MAX_LIST][MAX_FIELD], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < quantidade; j++) {
            if (strcmp(lista[j], lista[menor]) < 0) {
                menor = j;
            }
        }
        if (menor != i) {
            char temp[MAX_FIELD];
            strcpy(temp, lista[i]);
            strcpy(lista[i], lista[menor]);
            strcpy(lista[menor], temp);
        }
    }
}

void print_show(const Show* registro) {
    printf("=> %s ## %s ## %s ## ", registro->show_id, registro->title, registro->type);

    for (int i = 0; i < registro->director_count; i++) {
        printf("%s", registro->director[i]);
        if (i < registro->director_count - 1) printf(", ");
    }

    printf(" ## [");
    for (int i = 0; i < registro->cast_count; i++) {
        printf("%s", registro->cast[i]);
        if (i < registro->cast_count - 1) printf(", ");
    }
    printf("] ## ");

    for (int i = 0; i < registro->country_count; i++) {
        printf("%s", registro->country[i]);
        if (i < registro->country_count - 1) printf(", ");
    }

    printf(" ## %s ## %d ## %s ## %s ## [", 
        registro->date[0] ? registro->date : "March 1, 1900",
        registro->release_year != -1 ? registro->release_year : 0,
        registro->rating,
        registro->duration);

    for (int i = 0; i < registro->listen_count; i++) {
        printf("%s", registro->listen_in[i]);
        if (i < registro->listen_count - 1) printf(", ");
    }

    printf("] ##\n");
}

void read_csv(const char* caminho) {
    FILE* arquivo = fopen(caminho, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char linha[4000];
    fgets(linha, sizeof(linha), arquivo); 

    while (fgets(linha, sizeof(linha), arquivo) && show_count < MAX_SHOWS) {
        char* campos[13] = { NULL };
        int indice_campo = 0;
        int entre_aspas = 0;
        char* inicio_token = linha;

        for (char* ptr = linha; *ptr && indice_campo < 13; ptr++) {
            if (*ptr == '"') {
                entre_aspas = !entre_aspas;
                memmove(ptr, ptr + 1, strlen(ptr));
                ptr--;
            } else if (*ptr == ',' && !entre_aspas) {
                *ptr = '\0';
                campos[indice_campo++] = inicio_token;
                inicio_token = ptr + 1;
            }
        }

        if (indice_campo < 13) {
            campos[indice_campo++] = inicio_token;
        }

        while (indice_campo < 13) {
            campos[indice_campo++] = "";
        }

        Show* registro = &shows[show_count];
        removeN_R(campos[11]);

        strcpy(registro->show_id, strlen(campos[0]) > 0 ? campos[0] : "NaN");
        strcpy(registro->type, strlen(campos[1]) > 0 ? campos[1] : "NaN");
        strcpy(registro->title, strlen(campos[2]) > 0 ? campos[2] : "NaN");

        split_list(strlen(campos[3]) > 0 ? campos[3] : "NaN", registro->director, &registro->director_count);
        //sort_list(registro->director, registro->director_count);
        //Saída do verde não está ordenada

        split_list(strlen(campos[4]) > 0 ? campos[4] : "NaN", registro->cast, &registro->cast_count);
        sort_list(registro->cast, registro->cast_count);

        split_list(strlen(campos[5]) > 0 ? campos[5] : "NaN", registro->country, &registro->country_count);
        //sort_list(registro->country, registro->country_count);
        //Saída do verde não está ordenada
        strcpy(registro->date, strlen(campos[6]) > 0 ? campos[6] : "March 1, 1900");
        registro->release_year = strlen(campos[7]) > 0 ? atoi(campos[7]) : -1;

        strcpy(registro->rating, strlen(campos[8]) > 0 ? campos[8] : "NaN");
        strcpy(registro->duration, strlen(campos[9]) > 0 ? campos[9] : "NaN");

        split_list(strlen(campos[10]) > 0 ? campos[10] : "NaN", registro->listen_in, &registro->listen_count);
        sort_list(registro->listen_in, registro->listen_count);

        show_count++;
    }

    fclose(arquivo);
}

Show* buscar(const char* id) {
    for (int i = 0; i < show_count; i++) {
        if (strcmp(shows[i].show_id, id) == 0) {
            return &shows[i];
        }
    }
    return NULL;
}
/**
 * Lista estatica
 * @author Max do Val Machado
 * @version 2 01/2015
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXTAM    100
#define bool      short
#define true      1
#define false     0

Show array[MAXTAM];   // Elementos da pilha 
int n;               // Quantidade de array.


/**
 * Inicializacoes
 */
void start(){
   n = 0;
}


/**
 * Insere um elemento na primeira posicao da lista e move os demais
 * elementos para o fim da 
 * @param x int elemento a ser inserido.
 */
void inserirInicio(Show x) {
   int i;

   //validar insercao
   if(n >= MAXTAM){
      printf("Erro ao inserir!");
      exit(1);
   } 

   //levar elementos para o fim do array
   for(i = n; i > 0; i--){
      array[i] = array[i-1];
   }

   array[0] = x;
   n++;
}


/**
 * Insere um elemento na ultima posicao da 
 * @param x int elemento a ser inserido.
 */
void inserirFim(Show x) {

   //validar insercao
   if(n >= MAXTAM){
      printf("Erro ao inserir!");
      exit(1);
   }

   array[n] = x;
   n++;
}


/**
 * Insere um elemento em uma posicao especifica e move os demais
 * elementos para o fim da 
 * @param x int elemento a ser inserido.
 * @param pos Posicao de insercao.
 */
void inserir(Show x, int pos) {
   int i;

   //validar insercao
   if(n >= MAXTAM || pos < 0 || pos > n){
      printf("Erro ao inserir!");
      exit(1);
   }

   //levar elementos para o fim do array
   for(i = n; i > pos; i--){
      array[i] = array[i-1];
   }

   array[pos] = x;
   n++;
}


/**
 * Remove um elemento da primeira posicao da lista e movimenta 
 * os demais elementos para o inicio da mesma.
 * @return resp int elemento a ser removido.
 */
Show* removerInicio() {
    if (n == 0) {
        printf("Erro ao remover!\n");
        exit(1);
    }

    Show* removido = &array[0];

    for (int i = 0; i < n - 1; i++) {
        array[i] = array[i + 1];
    }
    n--;

    return removido;
}


/**
 * Remove um elemento da ultima posicao da 
 * @return resp int elemento a ser removido.
 */
Show* removerFim() {
    if (n == 0) {
        printf("Erro ao remover!\n");
        exit(1);
    }

    return &array[--n];  // retorna o endereço válido
}

/**
 * Remove um elemento de uma posicao especifica da lista e 
 * movimenta os demais elementos para o inicio da mesma.
 * @param pos Posicao de remocao.
 * @return resp int elemento a ser removido.
*/

Show* remover(int pos) {
    if (n == 0 || pos < 0 || pos >= n) {
        printf("Erro ao remover!\n");
        exit(1);
    }

    Show* removido = &array[pos];

    for (int i = pos; i < n - 1; i++) {
        array[i] = array[i + 1];
    }
    n--;

    return removido;
}


/**
 * Mostra os array separados por espacos.
 */
void mostrar () {
    for(int i = 0; i < n; i++){
        print_show(&array[i]);
    }
}

/**
 * Procura um array e retorna se ele existe.
 * @param x int elemento a ser pesquisado.
 * @return <code>true</code> se o array existir,
 * <code>false</code> em caso contrario.
 */

int main() {
    read_csv("/tmp/disneyplus.csv");

    start();

    char entrada[100];
    while (1) {
        scanf(" %[^\n]", entrada);
        removeN_R(entrada);
        if (strcmp(entrada, "FIM") == 0) break;

        Show* resultado = buscar(entrada);
        if (resultado != NULL) {
           inserirFim(*resultado);
        }
    }
   
    int numero;
    scanf("%d", &numero);
    char operacao[2];
    char id[100]; 
    int posicao;
    for(int i = 0; i < numero; i++){
        scanf("%s", operacao);
        if(operacao[0] == 'I'){
            if(operacao[1] == 'I'){
                scanf(" %[^\n]", id);
                removeN_R(id);
                Show* resultado = buscar(id);
                if (resultado != NULL) {
                inserirInicio(*resultado);
            }
            }

             else if(operacao[1] == 'F'){
                scanf(" %[^\n]", id);
                removeN_R(id);
                Show* resultado = buscar(id);
                if (resultado != NULL) {
                inserirFim(*resultado);
            }
            }

            else if(operacao[1] == '*'){
                scanf("%d", &posicao);
                scanf(" %[^\n]", id);
                removeN_R(id);
                Show* resultado = buscar(id);
                if (resultado != NULL) {
                inserir(*resultado, posicao);
            }
            }
        } 
        
        
        
        else if(operacao[0] == 'R') {
            if(operacao[1] == 'I'){
                
                Show* s = removerInicio();
                printf("(R) %s\n", s->title);

            }
            else if(operacao[1] == 'F'){
                
                Show* s = removerFim();
                printf("(R) %s\n", s->title);

            }
            else if(operacao[1] == '*'){
                scanf("%d", &posicao);
                Show* s = remover(posicao);
                printf("(R) %s\n", s->title);

            }
        }

    }
    mostrar();
    return 0;
}