#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h> 

#define MAX_SHOWS 10000
#define MAX_FIELD 512
#define MAX_LIST 50
#define TAM_FILA 5
#define MAX_LINE 1024

// Struct Date
typedef struct Date {
    char mes[20];
    int dia;
    int ano;
} Date;

// Struct Show 
typedef struct Show {
    char show_id[MAX_FIELD];
    char type[MAX_FIELD];
    char title[MAX_FIELD];
    char director[MAX_LIST][MAX_FIELD];
    int director_count;
    char cast[MAX_LIST][MAX_FIELD];
    int cast_count;
    char country[MAX_LIST][MAX_FIELD];
    int country_count;
    Date date_added;
    int release_year;
    char rating[MAX_FIELD];
    char duration[MAX_FIELD];
    char listed_in[MAX_LIST][MAX_FIELD];
    int listedInSize;
    char description[MAX_FIELD];
} Show;

Show catalogo[MAX_SHOWS];
int totalShows = 0;

// Remove espaços em branco da string
char* limparString(char* str) {
    if (!str) return NULL;
    while (isspace(*str)) str++;
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

// Remove aspas de uma string 
void removeAspas(char* str) {
    int j = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] != '"')
            str[j++] = str[i];
    }
    str[j] = '\0';
}

// Junta funções para remover aspas e espaços vazios
void campoProcesso(char* field) {
    if (!field || strlen(field) == 0) {
        strcpy(field, "NaN");
        return;
    }
    removeAspas(field);
    limparString(field);
}

// Trata listas separadas por vírgula e ordena cast ou listed_in 
void trataLista(char* str, char lista[MAX_LIST][MAX_FIELD], int* size) {
    *size = 0;
    if (!str || strlen(str) == 0) {
        strcpy(lista[(*size)++], "NaN");
        return;
    }

    char buffer[4096];
    strncpy(buffer, str, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char* token = strtok(buffer, ",");
    while (token != NULL && *size < MAX_LIST) {
        limparString(token);
        removeAspas(token);
        if (!strlen(token)) strcpy(lista[*size], "NaN");
        else strncpy(lista[*size], token, MAX_FIELD-1);
        lista[*size][MAX_FIELD-1] = '\0';
        (*size)++;
        token = strtok(NULL, ",");
    }

    // Bubble Sort 
    for (int i = 0; i < *size - 1; i++) {
        for (int j = 0; j < *size - i - 1; j++) {
            if (strcasecmp(lista[j], lista[j + 1]) > 0) {
                char temp[MAX_FIELD];
                strcpy(temp, lista[j]);
                strcpy(lista[j], lista[j + 1]);
                strcpy(lista[j + 1], temp);
            }
        }
    }

    if (*size == 0) {
        strcpy(lista[(*size)++], "NaN");
    }
}

// Função para separar os campos de uma linha respeitando aspas duplas
int separarCampos(char* linha, char* campos[], int maxCampos) {
    int i = 0;
    char* ptr = linha;
    while (*ptr && i < maxCampos) {
        if (*ptr == '"') {
            ptr++;
            char* start = ptr;
            while (*ptr && !(*ptr == '"' && (*(ptr + 1) == ',' || *(ptr + 1) == '\0'))) {
                ptr++;
            }
            int len = ptr - start;
            campos[i] = malloc(len + 1);
            strncpy(campos[i], start, len);
            campos[i][len] = '\0';
            if (*ptr == '"') ptr++;
            if (*ptr == ',') ptr++;
        } else {
            char* start = ptr;
            while (*ptr && *ptr != ',') ptr++;
            int len = ptr - start;
            campos[i] = malloc(len + 1);
            strncpy(campos[i], start, len);
            campos[i][len] = '\0';
            if (*ptr == ',') ptr++;
        }
        i++;
    }
    return i;
}

// Preenche o catálogo
void preencherCatalogo() {
    FILE* file = fopen("/tmp/disneyplus.csv", "r"); // 
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[MAX_LINE];
    fgets(linha, MAX_LINE, file); // Ignora o cabeçalho

    while (fgets(linha, MAX_LINE, file)) {
        linha[strcspn(linha, "\n")] = 0; // Remove o caractere de nova linha

        char* campos[15];
        int fieldCount = separarCampos(linha, campos, 15);

        Show* novoShow = &catalogo[totalShows];

        strncpy(novoShow->show_id, campos[0], MAX_FIELD - 1);
        novoShow->show_id[MAX_FIELD-1] = '\0';
        campoProcesso(novoShow->show_id);

        strncpy(novoShow->type, campos[1], MAX_FIELD - 1);
        novoShow->type[MAX_FIELD-1] = '\0';
        campoProcesso(novoShow->type);

        strncpy(novoShow->title, campos[2], MAX_FIELD - 1);
        novoShow->title[MAX_FIELD-1] = '\0';
        campoProcesso(novoShow->title);

        trataLista(campos[3], novoShow->director, &novoShow->director_count);
        trataLista(campos[4], novoShow->cast, &novoShow->cast_count);
        trataLista(campos[5], novoShow->country, &novoShow->country_count);

        // Trata campo de data
        if (strlen(campos[6]) > 0 && strcmp(campos[6], "NaN") != 0) {
            char buffer[MAX_FIELD];
            strncpy(buffer, campos[6], MAX_FIELD-1);
            buffer[MAX_FIELD-1] = '\0';
            char* parte = strtok(buffer, " ");
            strcpy(novoShow->date_added.mes, parte ? parte : "NaN");
            parte = strtok(NULL, ",");
            novoShow->date_added.dia = parte ? atoi(parte) : -1;
            parte = strtok(NULL, "");
            novoShow->date_added.ano = parte ? atoi(parte) : -1;
        } else {
            strcpy(novoShow->date_added.mes, "NaN");
            novoShow->date_added.dia = -1;
            novoShow->date_added.ano = -1;
        }

        novoShow->release_year = (fieldCount > 7 && strlen(campos[7]) > 0) ? atoi(campos[7]) : -1;

        strncpy(novoShow->rating, campos[8], MAX_FIELD - 1);
        novoShow->rating[MAX_FIELD-1] = '\0';
        campoProcesso(novoShow->rating);

        strncpy(novoShow->duration, campos[9], MAX_FIELD - 1);
        novoShow->duration[MAX_FIELD-1] = '\0';
        campoProcesso(novoShow->duration);

        trataLista(campos[10], novoShow->listed_in, &novoShow->listedInSize);

        if (fieldCount > 11 && strlen(campos[11]) > 0) {
            strncpy(novoShow->description, campos[11], MAX_FIELD - 1);
            novoShow->description[MAX_FIELD-1] = '\0';
            campoProcesso(novoShow->description);
        } else {
            strcpy(novoShow->description, "NaN");
        }

        for (int i = 0; i < fieldCount; i++) free(campos[i]);
        totalShows++;
    }

    fclose(file);
}

// Função que imprime o catálogo
void imprimirShow(const Show* registro, int pos) {
    printf("[%d] ", pos);
    printf("=> %s ## %s ## %s ## ", registro->show_id, registro->title, registro->type);

    for (int i = 0; i < registro->director_count; i++) {
        if (i > 0) printf(",");
        printf("%s", registro->director[i]);
    }
    printf(" ## [");
    for (int i = 0; i < registro->cast_count; i++) {
        if (i > 0) printf(",");
        printf("%s", registro->cast[i]);
    }
    printf("] ## ");
    for (int i = 0; i < registro->country_count; i++) {
        if (i > 0) printf(",");
        printf("%s", registro->country[i]);
    }
    printf(" ## %s %01d, %d ## %d ## %s ## %s ## [", 
           registro->date_added.mes, registro->date_added.dia, registro->date_added.ano,
           registro->release_year, registro->rating, registro->duration);
    for (int i = 0; i < registro->listedInSize; i++) {
        if (i > 0) printf(",");
        printf("%s", registro->listed_in[i]);
    }
    printf("] ##\n");
}

// Procura show_id no catálogo
Show* buscarNoCatalogo(char* show_id) {
    for (int i = 0; i < totalShows; i++) {
        if (strcasecmp(catalogo[i].show_id, show_id) == 0) {
            return &catalogo[i];
        }
    }
    return NULL;
}

//Fila Circular Sequencial
typedef struct {
    Show *array[TAM_FILA];
    int tamanho;
} FilaSequencial;

void inicializarFila(FilaSequencial *fila) {
    fila->tamanho = 0;
}

bool isVazia(FilaSequencial *fila){
    return fila->tamanho == 0; 
}

bool isCheia(FilaSequencial *fila){ 
    return fila->tamanho == TAM_FILA; 
}

// Inserir na fila
void inserir(FilaSequencial *fila, Show *show) {
    if (isCheia(fila)) {
        for (int i = 1; i < fila->tamanho; i++)
            fila->array[i-1] = fila->array[i];
        fila->tamanho--;
    }
    fila->array[fila->tamanho] = show;
    fila->tamanho++;
    printf("[Media] %d\n", mediaReleaseYear(fila));
}

// Remover na fila
Show *remover(FilaSequencial *fila) {
    if (isVazia(fila)) {
        printf("Erro ao remover!\n");
        exit(1);
    }
    Show *removido = fila->array[0];
    for (int i = 1; i < fila->tamanho; i++)
        fila->array[i-1] = fila->array[i];
    fila->tamanho--;
    return removido;
}

int mediaReleaseYear(FilaSequencial *fila) {
    if (fila->tamanho == 0)
        return 0;
    int soma = 0;
    for (int i = 0; i < fila->tamanho; i++)
        soma += fila->array[i]->release_year;
    return soma / fila->tamanho;
}

// bubble sort
void bubbleSortFila(FilaSequencial *fila) {
    for (int i = 0; i < fila->tamanho - 1; i++) {
        for (int j = 0; j < fila->tamanho - i - 1; j++) {
            if (strcasecmp(fila->array[j]->title, fila->array[j + 1]->title) > 0) {
                Show* temp = fila->array[j];
                fila->array[j] = fila->array[j + 1];
                fila->array[j + 1] = temp;
            }
        }
    }
}

// Imprimir na fila
void imprimirFila(FilaSequencial *fila) {
    bubbleSortFila(fila);
    for (int i = 0; i < fila->tamanho; i++)
        imprimirShow(fila->array[i], i);
}

// Main
int main() {
    preencherCatalogo();

    FilaSequencial fila;
    inicializarFila(&fila);

    char entrada[100];
    while (1) {
        scanf(" %[^\n]", entrada);
        if (strcmp(entrada, "FIM") == 0) break;

        Show *s = buscarNoCatalogo(entrada);
        if (s != NULL) {
            inserir(&fila, s);
        }
    }

    int num;
    scanf("%d", &num);
    char op[3], id[100];

    for (int i = 0; i < num; i++) {
        scanf(" %s", op);
        if (strcmp(op, "I") == 0) {
            scanf(" %[^\n]", id);
            Show *s = buscarNoCatalogo(id);
            if (s != NULL)
                inserir(&fila, s);
        } else if (strcmp(op, "R") == 0) {
            Show *removido = remover(&fila);
            printf("(R) %s\n", removido->title);
        }
    }

    imprimirFila(&fila);
    return 0;
}