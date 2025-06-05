#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_SHOWS 10000
#define MAX_ITEMS 50

// Struct Date
typedef struct Date {
    char mes[20];
    int dia;
    int ano;
} Date;

// Struct show
typedef struct Show {
    char* show_id;
    char* type;
    char* title;
    char* director;
    char** cast;
    int castSize;
    char* country;
    Date date_added;
    int release_year;
    char* rating;
    char* duration;
    char** listed_in;
    int listedInSize;
    char* description;
} Show;

// Struct Pilha com alocação flexível
typedef struct {
    int tam;          
    Show** shows;     
    int n;          
} Pilha;

// Funções auxiliares para tratar strings
char* limparString(char* str) {
    if (!str) return strdup("NaN");
    while (isspace(*str)) str++;
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return strdup(str);
}

char* removeAspas(char* str) {
    if (!str) return NULL;
    char* result = malloc(strlen(str) + 1);
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] != '"') {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    return result;
}

char* removeAspasEspacoVazio(char* field) {
    if (!field || strlen(field) == 0) return strdup("NaN");
    char* noQuotes = removeAspas(field);
    char* result = limparString(noQuotes);
    free(noQuotes);
    return result;
}

char** trataLista(char* str, int* size) {
    char** items = malloc(MAX_ITEMS * sizeof(char*));
    *size = 0;
    if (!str || strlen(str) == 0) {
        items[(*size)++] = strdup("NaN");
        return items;
    }
    char* clear = removeAspas(str);
    char* token = strtok(clear, ",");
    while (token != NULL && *size < MAX_ITEMS) {
        char* item = removeAspasEspacoVazio(token);
        items[(*size)++] = item;
        token = strtok(NULL, ",");
    }
    free(clear);
    //Bubble Sort
    for (int i = 0; i < *size - 1; i++) {
        for (int j = 0; j < *size - i - 1; j++) {
            if (strcasecmp(items[j], items[j + 1]) > 0) {
                char* temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }
    if (*size == 0) {
        items[(*size)++] = strdup("NaN");
    }
    return items;
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
Show* catalogo[MAX_SHOWS];
int totalShows = 0;

void readCSV() {
    FILE* file = fopen("/tmp/disneyplus.csv", "r");
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

        Show* novoShow = malloc(sizeof(Show));
        novoShow->show_id = removeAspasEspacoVazio(campos[0]);
        novoShow->type = removeAspasEspacoVazio(campos[1]);
        novoShow->title = removeAspasEspacoVazio(campos[2]);
        novoShow->director = removeAspasEspacoVazio(campos[3]);
        novoShow->cast = trataLista(campos[4], &novoShow->castSize);
        novoShow->country = removeAspasEspacoVazio(campos[5]);

        // Trata campo de data
        char* dateStr = removeAspasEspacoVazio(campos[6]);
        if (strcmp(dateStr, "NaN") != 0) {
            char* parte = strtok(dateStr, " ");
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
        free(dateStr);

        novoShow->release_year = (fieldCount > 7 && strlen(campos[7]) > 0) ? atoi(campos[7]) : -1;
        novoShow->rating = removeAspasEspacoVazio(campos[8]);
        novoShow->duration = removeAspasEspacoVazio(campos[9]);
        novoShow->listed_in = trataLista(campos[10], &novoShow->listedInSize);
        novoShow->description = removeAspasEspacoVazio(campos[11]);

        for (int i = 0; i < fieldCount; i++) free(campos[i]);
        catalogo[totalShows++] = novoShow;
    }

    fclose(file);
}

// Função que imprime o catálogo
void imprimir(Show* s) {
    printf("=> %s ## %s ## %s ## %s ## [", s->show_id, s->title, s->type, s->director);
    for (int i = 0; i < s->castSize; i++) {
        printf("%s", s->cast[i]);
        if (i < s->castSize - 1) printf(", ");
    }
    printf("] ## %s ## %s %01d, %d ## %d ## %s ## %s ## [", s->country, s->date_added.mes, s->date_added.dia,
           s->date_added.ano, s->release_year, s->rating, s->duration);
    for (int i = 0; i < s->listedInSize; i++) {
        printf("%s", s->listed_in[i]);
        if (i < s->listedInSize - 1) printf(", ");
    }
    printf("] ##\n");
}

// Função para verificar se é FIM
bool isFim(char* str) {
    return (strcasecmp(str, "FIM") == 0);
}

// Inicializa pilha dinâmica
void inicializarPilha(Pilha* pilha, int tamInicial) {
    pilha->tam = tamInicial;
    pilha->n = 0;
    pilha->shows = (Show**) malloc(pilha->tam * sizeof(Show*));
}

// Redimensiona pilha dinâmica
void redimensionarPilha(Pilha* pilha) {
    pilha->tam *= 2;
    pilha->shows = (Show**) realloc(pilha->shows, pilha->tam * sizeof(Show*));
}

// Empilhar 
void empilhar(Pilha* pilha, Show* show) {
    if (pilha->n >= pilha->tam) {
        redimensionarPilha(pilha);
    }
    pilha->shows[pilha->n++] = show;
}

// Desempilhar 
Show* desempilhar(Pilha* pilha) {
    if (pilha->n == 0) {
        printf("Erro ao desempilhar!\n");
        exit(1);
    }
    return pilha->shows[--(pilha->n)];
}

// Imprimir pilha do topo para o fundo
void imprimirPilha(Pilha* pilha) {
    for (int i = pilha->n - 1; i >= 0; i--) {
        printf("[%d] ", i);
        imprimir(pilha->shows[i]);
    }
}

// Busca no catálogo por show_id
Show* buscarNoCatalogo(char* show_id) {
    for (int i = 0; i < totalShows; i++) {
        if (strcasecmp(catalogo[i]->show_id, show_id) == 0) {
            return catalogo[i];
        }
    }
    return NULL;
}

// Main
int main() {
    readCSV();

    Pilha pilha;
    inicializarPilha(&pilha, 10); 

    // Leitura 
    char entrada[256];
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0;
    while (!isFim(entrada)) {
        Show* selecionado = buscarNoCatalogo(entrada);
        if (selecionado) {
            empilhar(&pilha, selecionado);
        }
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0;
    }

    int Numero;
    scanf("%d\n", &Numero);
    char linha[100];
    for (int i = 0; i < Numero; i++) {
        fgets(linha, sizeof(linha), stdin);
        linha[strcspn(linha, "\n")] = 0;
        if (linha[0] == 'I') {
            char show_id[100];
            sscanf(linha + 2, "%s", show_id);
            Show* show = buscarNoCatalogo(show_id);
            if (show) empilhar(&pilha, show);
        } 
        else if (linha[0] == 'R') {
            Show* removido = desempilhar(&pilha);
            if (removido) printf("(R) %s\n", removido->title);
        }
    }

    imprimirPilha(&pilha);

    // Libera memória
    for (int i = 0; i < totalShows; i++) {
        Show* s = catalogo[i];
        free(s->show_id);
        free(s->type);
        free(s->title);
        free(s->director);
        for (int j = 0; j < s->castSize; j++) free(s->cast[j]);
        free(s->cast);
        free(s->country);
        free(s->rating);
        free(s->duration);
        for (int j = 0; j < s->listedInSize; j++) free(s->listed_in[j]);
        free(s->listed_in);
        free(s->description);
        free(s);
    }
    free(pilha.shows);

    return 0;
}