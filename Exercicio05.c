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

// Struct Lista Sequencial com alocação dinâmica
typedef struct Lista {
    Show** shows;
    int tam; 
    int n;  
} Lista;

Show* catalogo[MAX_SHOWS];
int totalShows = 0;

// Remove espaços em branco da string
char* limparString(char* str) {
    if (!str) return strdup("NaN");
    while (isspace(*str)) str++;
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return strdup(str);
}

// Remove aspas de uma string
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

// Junta funções para remover aspas e espaços vazios
char* removeAspasEspacoVazio(char* field) {
    if (!field || strlen(field) == 0) return strdup("NaN");
    char* noQuotes = removeAspas(field);
    char* result = limparString(noQuotes);
    free(noQuotes);
    return result;
}

// Trata listas separadas por vírgula  e ordena cast ou listed_in
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
    // Imprime cast sem espaço 
    printf("=> %s ## %s ## %s ## %s ## [", s->show_id, s->title, s->type, s->director);
    for (int i = 0; i < s->castSize; i++) {
        printf("%s", s->cast[i]);
        if (i < s->castSize - 1) printf(", ");
    }
    printf("] ## %s ## %s %01d, %d ## %d ## %s ## %s ## [", s->country, s->date_added.mes, s->date_added.dia,
           s->date_added.ano, s->release_year, s->rating, s->duration);
    // Imprime listed_in sem espaço 
    for (int i = 0; i < s->listedInSize; i++) {
        printf("%s", s->listed_in[i]);
        if (i < s->listedInSize - 1) printf(", ");
    }
    printf("] ##\n");
}

// Verifica se a entrada é FIM
bool isFim(char* str) {
    return strcasecmp(str, "FIM") == 0;
}

// Métodos da Lista Sequencial dinâmica
Lista* criarLista(int tam) {
    Lista* lista = (Lista*)malloc(sizeof(Lista));
    lista->tam = tam;
    lista->shows = (Show**)malloc(tam * sizeof(Show*));
    lista->n = 0;
    return lista;
}

// Redimensiona a lista se necessário 
void redimensionarLista(Lista* lista) {
    lista->tam *= 2;
    lista->shows = (Show**) realloc(lista->shows, lista->tam * sizeof(Show*));
}

// Insere início
void inserirInicio(Lista* lista, Show* show) {
    if (lista->n >= lista->tam) {
        redimensionarLista(lista);
    }
    for (int i = lista->n; i > 0; i--)
        lista->shows[i] = lista->shows[i - 1];
    lista->shows[0] = show;
    lista->n++;
}

// Insere fim
void inserirFim(Lista* lista, Show* show) {
    if (lista->n >= lista->tam) {
        redimensionarLista(lista);
    }
    lista->shows[lista->n++] = show;
}

// Insere em qualquer posição
void inserir(Lista* lista, Show* show, int posicao) {
    if (lista->n >= lista->tam) {
        redimensionarLista(lista);
    }
    if (posicao < 0 || posicao > lista->n) return;
    for (int i = lista->n; i > posicao; i--)
        lista->shows[i] = lista->shows[i - 1];
    lista->shows[posicao] = show;
    lista->n++;
}

// Remove início
Show* removerInicio(Lista* lista) {
    if (lista->n == 0) return NULL;
    Show* removido = lista->shows[0];
    for (int i = 0; i < lista->n - 1; i++)
        lista->shows[i] = lista->shows[i + 1];
    lista->n--;
    return removido;
}

// Remove fim
Show* removerFim(Lista* lista) {
    if (lista->n == 0) return NULL;
    return lista->shows[--lista->n];
}

// Remove de qualquer posição
Show* remover(Lista* lista, int posicao) {
    if (lista->n == 0 || posicao < 0 || posicao >= lista->n) return NULL;
    Show* removido = lista->shows[posicao];
    for (int i = posicao; i < lista->n - 1; i++)
        lista->shows[i] = lista->shows[i + 1];
    lista->n--;
    return removido;
}

// Imprime a lista sequencial
void imprimirLista(Lista* lista) {
    for (int i = 0; i < lista->n; i++) {
        imprimir(lista->shows[i]);
    }
}

// Procura show por show_id no catálogo
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

    // Lista para armazenar os Shows
    Lista* showLista = criarLista(10); 

    //inserir shows pelo show_id
    char entrada[256];
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0;
    while (!isFim(entrada)) {
        Show* selecionado = buscarNoCatalogo(entrada);
        if (selecionado) {
            inserirFim(showLista, selecionado);
        }
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0;
    }

    int N;
    scanf("%d\n", &N);
    char linha[100];
    for (int i = 0; i < N; i++) {
        fgets(linha, sizeof(linha), stdin);
        linha[strcspn(linha, "\n")] = 0;
        char cmd[10], show_id[100];
        int posicao;
        if (sscanf(linha, "%s", cmd) == 1) {
            //inserir inicio
            if (strcmp(cmd, "II") == 0) {
                sscanf(linha, "%*s %s", show_id);
                Show* show = buscarNoCatalogo(show_id);
                if (show) inserirInicio(showLista, show);
            } 
            //inserir fim
            else if (strcmp(cmd, "IF") == 0) {
                sscanf(linha, "%*s %s", show_id);
                Show* show = buscarNoCatalogo(show_id);
                if (show) inserirFim(showLista, show);
            } 
            //inserir em qualquer posição
            else if (strcmp(cmd, "I*") == 0) {
                sscanf(linha, "%*s %d %s", &posicao, show_id);
                Show* show = buscarNoCatalogo(show_id);
                if (show) inserir(showLista, show, posicao);
            } 
            //remover inicio
            else if (strcmp(cmd, "RI") == 0) {
                Show* show = removerInicio(showLista);
                if (show) printf("(R) %s\n", show->title);
            } 
            //remover fim
            else if (strcmp(cmd, "RF") == 0) {
                Show* show = removerFim(showLista);
                if (show) printf("(R) %s\n", show->title);
            } 
            //remover em qualquer posição
            else if (strcmp(cmd, "R*") == 0) {
                sscanf(linha, "%*s %d", &posicao);
                Show* show = remover(showLista, posicao);
                if (show) printf("(R) %s\n", show->title);
            }
        }
    }

    imprimirLista(showLista);

    // Libera toda a memória alocada
    for (int i = 0; i < totalShows; i++) {
        Show* show = catalogo[i];
        free(show->show_id);
        free(show->type);
        free(show->title);
        free(show->director);
        for (int j = 0; j < show->castSize; j++) free(show->cast[j]);
        free(show->cast);
        free(show->country);
        free(show->rating);
        free(show->duration);
        for (int j = 0; j < show->listedInSize; j++) free(show->listed_in[j]);
        free(show->listed_in);
        free(show->description);
        free(show);
    }
    free(showLista->shows);
    free(showLista);

    return 0;
}