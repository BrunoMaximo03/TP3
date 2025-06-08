#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

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
    Date date_added;   // pega struct Date
    int release_year;
    char* rating;
    char* duration;
    char** listed_in; 
    int listedInSize;
    char* description;
} Show;

// Struct CelulaDupla
typedef struct CelulaDupla {
    Show* elemento;            
    struct CelulaDupla* prox;   
    struct CelulaDupla* ant;    
} CelulaDupla;

// Variáveis globais do catálogo de shows
Show* catalogo[MAX_SHOWS];
int totalShows = 0;

// Variáveis globais para log do quicksort
int comparacoes = 0;
int movimentacoes = 0;

// Função para criar nova célula dupla
CelulaDupla* novaCelulaDupla(Show* elemento) {
    CelulaDupla* nova = (CelulaDupla*) malloc(sizeof(CelulaDupla));
    nova->elemento = elemento;
    nova->ant = nova->prox = NULL;
    return nova;
}

CelulaDupla* primeiro;
CelulaDupla* ultimo;

// Cria nó cabeca
void start() {
    primeiro = novaCelulaDupla(NULL);
    ultimo = primeiro;
}

// Insere no inicio
void inserirInicio(Show* s) {
    CelulaDupla* tmp = novaCelulaDupla(s);

    tmp->ant = primeiro;
    tmp->prox = primeiro->prox;
    primeiro->prox = tmp;

    if (primeiro == ultimo) {                    
        ultimo = tmp;
    } else {
        tmp->prox->ant = tmp;
    }
    tmp = NULL;
}

// Insere no fim
void inserirFim(Show* s) {
    ultimo->prox = novaCelulaDupla(s);
    ultimo->prox->ant = ultimo;
    ultimo = ultimo->prox;
}

// Remove no inicio
Show* removerInicio() {
    if (primeiro == ultimo) {
        fprintf(stderr, "Erro ao remover (vazia)!\n");
        exit(1);
    }

    CelulaDupla* tmp = primeiro;
    primeiro = primeiro->prox;
    Show* resp = primeiro->elemento;
    tmp->prox = primeiro->ant = NULL;
    free(tmp);
    tmp = NULL;
    return resp;
}

// Remove no fim
Show* removerFim() {
    if (primeiro == ultimo) {
        fprintf(stderr, "Erro ao remover (vazia)!\n");
        exit(1);
    } 
    Show* resp = ultimo->elemento;
    ultimo = ultimo->ant;
    ultimo->prox->ant = NULL;
    free(ultimo->prox);
    ultimo->prox = NULL;
    return resp;
}

// Calcula o da lista
int tamanho() {
    int tamanho = 0; 
    CelulaDupla* i;
    for(i = primeiro; i != ultimo; i = i->prox, tamanho++);
    return tamanho;
}

// Insere um elemento 
void inserir(Show* s, int pos) {
    int tam = tamanho();

    if(pos < 0 || pos > tam){
        fprintf(stderr, "Erro ao inserir (posicao %d/%d invalida)!\n", pos, tam);
        exit(1);
    } else if (pos == 0){
        inserirInicio(s);
    } else if (pos == tam){
        inserirFim(s);
    } else {
        // Caminhar ate a posicao anterior a insercao
        CelulaDupla* i = primeiro;
        int j;
        for(j = 0; j < pos; j++, i = i->prox);

        CelulaDupla* tmp = novaCelulaDupla(s);
        tmp->ant = i;
        tmp->prox = i->prox;
        tmp->ant->prox = tmp->prox->ant = tmp;
        tmp = i = NULL;
    }
}

// Remove um elemento 
Show* remover(int pos) {
    Show* resp;
    int tam = tamanho();

    if (primeiro == ultimo){
        fprintf(stderr, "Erro ao remover (vazia)!\n");
        exit(1);
    } else if(pos < 0 || pos >= tam){
        fprintf(stderr, "Erro ao remover (posicao %d/%d invalida)!\n", pos, tam);
        exit(1);
    } else if (pos == 0){
        resp = removerInicio();
    } else if (pos == tam - 1){
        resp = removerFim();
    } else {
        // Caminhar ate a posicao anterior a insercao
        CelulaDupla* i = primeiro->prox;
        int j;
        for(j = 0; j < pos; j++, i = i->prox);

        i->ant->prox = i->prox;
        i->prox->ant = i->ant;
        resp = i->elemento;
        i->prox = i->ant = NULL;
        free(i);
        i = NULL;
    }

    return resp;
}

// Mostra os elementos da lista 
void mostrarListaDupla() {
    CelulaDupla* i;
    int idx = 0;
    for (i = primeiro->prox; i != NULL; i = i->prox) {
       // printf("[%d] ", idx++);
        Show* s = i->elemento;
        printf("=> %s ## %s ## %s ## %s ## [", s->show_id, s->title, s->type, s->director);
        for (int k = 0; k < s->castSize; k++) {
            printf("%s%s", s->cast[k], (k < s->castSize - 1) ? ", " : "");
        }
        printf("] ## %s ## %s %d, %d ## %d ## %s ## %s ## [", s->country, s->date_added.mes, s->date_added.dia,
            s->date_added.ano, s->release_year, s->rating, s->duration);
        for (int k = 0; k < s->listedInSize; k++) {
            printf("%s%s", s->listed_in[k], (k < s->listedInSize - 1) ? ", " : "");
        }
        printf("] ##\n");
    }
}

// Procura show_id no catálogo
Show* buscarNoCatalogo(char* show_id) {
    for (int i = 0; i < totalShows; i++) {
        if (strcasecmp(catalogo[i]->show_id, show_id) == 0) {
            return catalogo[i];
        }
    }
    return NULL;
}

// Funções auxiliares
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
    char* resultado = malloc(strlen(str) + 1);
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] != '"') {
            resultado[j++] = str[i];
        }
    }
    resultado[j] = '\0';
    return resultado;
}
char* removeAspasEspacoVazio(char* field) {
    if (!field || strlen(field) == 0) return strdup("NaN");
    char* noQuotes = removeAspas(field);
    char* resultado = limparString(noQuotes);
    free(noQuotes);
    return resultado;
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

// Função para verificar se é FIM
bool isFim(char* str) {
    return strcasecmp(str, "FIM") == 0;
}

void preencherCatalogo() {
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
            strcpy(novoShow->date_added.mes, "March");
            novoShow->date_added.dia = 1;
            novoShow->date_added.ano = 1900;
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

//QuickSort 
// Ordem dos meses para comparar os meses pela posição no ano
const char* mesOrdem[] = {"January", "February", "March", "April", "May", "June", 
                          "July", "August", "September", "October", "November", "December"};

// Função auxiliar para comparar datas e desempatar por title
int compareShows(Show* a, Show* b) {
    int mesA = -1, mesB = -1;
    for (int i = 0; i < 12; i++) {
        if (strcasecmp(a->date_added.mes, mesOrdem[i]) == 0) mesA = i;
        if (strcasecmp(b->date_added.mes, mesOrdem[i]) == 0) mesB = i;
    }
    if (a->date_added.ano != b->date_added.ano)
        return a->date_added.ano - b->date_added.ano;
    if (mesA != mesB)
        return mesA - mesB;
    if (a->date_added.dia != b->date_added.dia)
        return a->date_added.dia - b->date_added.dia;
    return strcasecmp(a->title, b->title);
}

// Algoritmo de ordenação quicksort
void quicksort(Show* array[], int esq, int dir) {
    if (esq < dir) { 
        Show* pivo = array[dir];
        int i = esq - 1;
        for (int j = esq; j < dir; j++) {
            comparacoes++;
            if (compareShows(array[j], pivo) < 0) { 
                i++;
                Show* temp = array[i];
                array[i] = array[j];
                array[j] = temp;
                movimentacoes++;
            }
        }
        Show* temp = array[i + 1];
        array[i + 1] = array[dir];
        array[dir] = temp;
        movimentacoes++;
        int pi = i + 1;
        quicksort(array, esq, pi - 1);
        quicksort(array, pi + 1, dir);
    }
}

int main() {
    preencherCatalogo();

    start(); 

    char entrada[50];
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0;

    Show* resultados[MAX_SHOWS];
    int tamanhoResultados = 0;

    // ler enquanto não for "FIM"
    while (!isFim(entrada)) {
        Show* show = buscarNoCatalogo(entrada);
        if (show) {
            resultados[tamanhoResultados++] = show;
        }
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0;
    }

    clock_t inicio = clock();

    // Ordena usando Quicksort 
    quicksort(resultados, 0, tamanhoResultados - 1);

    clock_t fim = clock();

    // Insere os resultados ordenados na lista dupla
    for (int i = 0; i < tamanhoResultados; i++) {
        inserirFim(resultados[i]);
    }

    mostrarListaDupla();

    // Calcula o tempo de execução
    double tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Gera o arquivo de log 
    FILE* log = fopen("matricula_quicksort.txt", "w");
    if (log) {
        fprintf(log, "850847\t%d\t%d\t%lf\n", comparacoes, movimentacoes, tempoExecucao);
        fclose(log);
    }

    // Libera memória da lista dupla
    CelulaDupla* novaCelula = primeiro;
    while (novaCelula != NULL) {
        CelulaDupla* tmp = novaCelula;
        novaCelula = novaCelula->prox;
        free(tmp);
    }

    // Libera memória dos shows do catálogo
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

    return 0;
}