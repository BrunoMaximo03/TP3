#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

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

// Struct Lista Sequencial com alocação dinâmica
typedef struct Lista {
    Show** shows;
    int tam; 
    int n;  
} Lista;

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

// Função para verificar se é FIM
bool isFim(char *str)
{
    return (strcasecmp(str, "FIM") == 0);
}

// Retorna o tamanho da lista
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
void inserir(Lista* lista, Show* show, int pos) {
    if (lista->n >= lista->tam) {
        redimensionarLista(lista);
    }
    if (pos < 0 || pos > lista->n) return;
    for (int i = lista->n; i > pos; i--)
        lista->shows[i] = lista->shows[i - 1];
    lista->shows[pos] = show;
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
Show* remover(Lista* lista, int pos) {
    if (lista->n == 0 || pos < 0 || pos >= lista->n) return NULL;
    Show* removido = lista->shows[pos];
    for (int i = pos; i < lista->n - 1; i++)
        lista->shows[i] = lista->shows[i + 1];
    lista->n--;
    return removido;
}

// Imprime a lista
void imprimirLista(Lista* lista) {
    for(int i = 0; i < lista->n;i++) {
        print_show(lista->shows[i]);
    }
}

// Busca no catálogo por show_id
Show* buscarID(const char* id) {
    for (int i = 0; i < show_count; i++) {
        if (strcmp(shows[i].show_id, id) == 0) {
            return &shows[i];
        }
    }
    return NULL;
}

// Main
int main()
{
    read_csv("/tmp/disneyplus.csv");
    
    Lista* showLista = criarLista(10);

    // Leitura
    char entrada[256];
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0;
    while (!isFim(entrada))
    {
        Show *selecionado = buscarID(entrada);
        if (selecionado)
        {
            inserirInicio(showLista, selecionado);
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
        int pos;
        if (sscanf(linha, "%s", cmd) == 1) {
            //inserir inicio
            if (strcmp(cmd, "II") == 0) {
                sscanf(linha, "%*s %s", show_id);
                Show* s = buscarNoCatalogo(show_id);
                if (s) inserirInicio(showLista, s);
            } 
            //inserir fim
            else if (strcmp(cmd, "IF") == 0) {
                sscanf(linha, "%*s %s", show_id);
                Show* s = buscarNoCatalogo(show_id);
                if (s) inserirFim(showLista, s);
            } 
            //inserir em qualquer posição
            else if (strcmp(cmd, "I*") == 0) {
                sscanf(linha, "%*s %d %s", &pos, show_id);
                Show* s = buscarNoCatalogo(show_id);
                if (s) inserir(showLista, s, pos);
            } 
            //remover inicio
            else if (strcmp(cmd, "RI") == 0) {
                Show* s = removerInicio(showLista);
                if (s) printf("(R) %s\n", s->title);
            } 
            //remover fim
            else if (strcmp(cmd, "RF") == 0) {
                Show* s = removerFim(showLista);
                if (s) printf("(R) %s\n", s->title);
            } 
            //remover em qualquer posição
            else if (strcmp(cmd, "R*") == 0) {
                sscanf(linha, "%*s %d", &pos);
                Show* s = remover(showLista, pos);
                if (s) printf("(R) %s\n", s->title);
            }
        }
    }

    
      mostrar();
      return 0; 
 }



 


