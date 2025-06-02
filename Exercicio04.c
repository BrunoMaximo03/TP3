#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h> // Necessário para round

#define MAX_SHOWS 10000
#define MAX_FIELD 512
#define MAX_LIST 50
#define TAM_FILA 5

typedef struct
{
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

void removeN_R(char *texto) {
    int tamanho = strlen(texto);
    if (tamanho > 0 && (texto[tamanho - 1] == '\n' || texto[tamanho - 1] == '\r'))
        texto[tamanho - 1] = '\0';
}

void split_list(char *entrada, char lista[MAX_LIST][MAX_FIELD], int *total_itens) {
    *total_itens = 0;
    char *parte = strtok(entrada, ",");
    while (parte != NULL && *total_itens < MAX_LIST) {
        while (*parte == ' ')
            parte++;
        strcpy(lista[*total_itens], parte);
        (*total_itens)++;
        parte = strtok(NULL, ",");
    }
}

void sort_list(char lista[MAX_LIST][MAX_FIELD], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < quantidade; j++) {
            if (strcmp(lista[j], lista[menor]) < 0)
                menor = j;
        }
        if (menor != i) {
            char temp[MAX_FIELD];
            strcpy(temp, lista[i]);
            strcpy(lista[i], lista[menor]);
            strcpy(lista[menor], temp);
        }
    }
}

void print_show(const Show *registro, int pos) {
    printf("[%d] ", pos);
    printf("=> %s ## %s ## %s ## ", registro->show_id, registro->title, registro->type);
    for (int i = 0; i < registro->director_count; i++) {
        printf("%s", registro->director[i]);
        if (i < registro->director_count - 1)
            printf(", ");
    }
    printf(" ## [");
    for (int i = 0; i < registro->cast_count; i++) {
        printf("%s", registro->cast[i]);
        if (i < registro->cast_count - 1)
            printf(", ");
    }
    printf("] ## ");
    for (int i = 0; i < registro->country_count; i++) {
        printf("%s", registro->country[i]);
        if (i < registro->country_count - 1)
            printf(", ");
    }
    printf(" ## %s ## %d ## %s ## %s ## [", 
           registro->date[0] ? registro->date : "March 1, 1900", 
           registro->release_year != -1 ? registro->release_year : 0, 
           registro->rating, 
           registro->duration);
    for (int i = 0; i < registro->listen_count; i++) {
        printf("%s", registro->listen_in[i]);
        if (i < registro->listen_count - 1)
            printf(", ");
    }
    printf("] ##\n");
}

void read_csv(const char *caminho) {
    FILE *arquivo = fopen(caminho, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char linha[4000];
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) && show_count < MAX_SHOWS) {
        char *campos[13] = {NULL};
        int indice_campo = 0;
        int entre_aspas = 0;
        char *inicio_token = linha;

        for (char *ptr = linha; *ptr && indice_campo < 13; ptr++) {
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
        if (indice_campo < 13)
            campos[indice_campo++] = inicio_token;
        while (indice_campo < 13)
            campos[indice_campo++] = "";

        Show *registro = &shows[show_count];
        removeN_R(campos[11]);

        strcpy(registro->show_id, strlen(campos[0]) > 0 ? campos[0] : "NaN");
        strcpy(registro->type, strlen(campos[1]) > 0 ? campos[1] : "NaN");
        strcpy(registro->title, strlen(campos[2]) > 0 ? campos[2] : "NaN");

        split_list(strlen(campos[3]) > 0 ? campos[3] : "NaN", registro->director, &registro->director_count);
        sort_list(registro->director, registro->director_count);

        split_list(strlen(campos[4]) > 0 ? campos[4] : "NaN", registro->cast, &registro->cast_count);
        sort_list(registro->cast, registro->cast_count);

        split_list(strlen(campos[5]) > 0 ? campos[5] : "NaN", registro->country, &registro->country_count);
        sort_list(registro->country, registro->country_count);

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

Show *buscarIdCSV(const char *id) {
    for (int i = 0; i < show_count; i++) {
        if (strcmp(shows[i].show_id, id) == 0)
            return &shows[i];
    }
    return NULL;
}

// =============== Fila Circular ===============
typedef struct {
    Show *array[TAM_FILA];
    int inicio, fim, tamanho;
} FilaCircular;

void start_fila(FilaCircular *fila) {
    fila->inicio = fila->fim = fila->tamanho = 0;
}

bool isVazia(FilaCircular *fila) { return fila->tamanho == 0; }

bool isCheia(FilaCircular *fila) { return fila->tamanho == TAM_FILA; }

void inserir(FilaCircular *fila, Show *show) {
    if (isCheia(fila)) {
        fila->inicio = (fila->inicio + 1) % TAM_FILA;
        fila->tamanho--;
    }
    fila->array[fila->fim] = show;
    fila->fim = (fila->fim + 1) % TAM_FILA;
    fila->tamanho++;
    printf("[Media] %d\n", mediaReleaseYear(fila)); // imprime média após inserção
}

Show *remover(FilaCircular *fila) {
    if (isVazia(fila)) {
        printf("Erro ao remover!\n");
        exit(1);
    }
    Show *removido = fila->array[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAM_FILA;
    fila->tamanho--;
    return removido;
}

int mediaReleaseYear(FilaCircular *fila) {
    if (fila->tamanho == 0)
        return 0;
    int soma = 0;
    for (int i = 0, idx = fila->inicio; i < fila->tamanho; i++, idx = (idx + 1) % TAM_FILA)
        soma += fila->array[idx]->release_year;
    return (int)round(soma / (double)fila->tamanho);
}

void print_fila(FilaCircular *fila) {
    for (int i = 0, idx = fila->inicio; i < fila->tamanho; i++, idx = (idx + 1) % TAM_FILA)
        print_show(fila->array[idx], i);
}

// =============== Main ===============
int main() {
    read_csv("/tmp/disneyplus.csv");

    FilaCircular fila;
    start_fila(&fila);

    char entrada[100];
    while (1) {
        scanf(" %[^\n]", entrada);
        removeN_R(entrada);
        if (strcmp(entrada, "FIM") == 0) break;

        Show *s = buscarIdCSV(entrada);
        if (s != NULL) {
            inserir(&fila, s);
        }
    }

    int numero;
    scanf("%d", &numero);
    char operacao[3], id[100];

    for (int i = 0; i < numero; i++) {
        scanf(" %s", operacao);
        if (strcmp(operacao, "I") == 0) {
            scanf(" %[^\n]", id);
            removeN_R(id);
            Show *s = buscarIdCSV(id);
            if (s != NULL)
                inserir(&fila, s);
        } else if (strcmp(operacao, "R") == 0) {
            Show *removido = remover(&fila);
            printf("(R) %s\n", removido->title);
        }
    }

    print_fila(&fila);
    return 0;
}
