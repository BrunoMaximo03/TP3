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

// Struct Celula
typedef struct Celula
{
    Show elemento;
    struct Celula *prox;
} Celula;

// LISTA PROPRIAMENTE DITA
Celula *primeiro;
Celula *ultimo;

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
int tamanho()
{
    int tamanho = 0;
    Celula *i;
    for (i = primeiro->prox; i != NULL; i = i->prox)
    {
        tamanho++;
    }
    return tamanho;
}

// Construtor Celula
Celula *novaCelula(Show elemento)
{
    Celula *nova = (Celula *)malloc(sizeof(Celula));
    nova->elemento = elemento;
    nova->prox = NULL;
    return nova;
}

// Cria a lista sem elementos
void start()
{
    primeiro = novaCelula((Show){0});
    ultimo = primeiro;
}

// Inserir elemento no inicio da lista
void inserirInicio(Show elemento)
{
    Celula *nova = novaCelula(elemento);
    nova->prox = primeiro->prox;
    primeiro->prox = nova;

    if (ultimo == primeiro)
    {
        ultimo = nova;
    }
    nova = NULL;
}

// Inserir elemento no final da lista
void inserirFim(Show elemento)
{
    ultimo->prox = novaCelula(elemento);
    ultimo = ultimo->prox;
}

// Inserir elemento em uma posição específica
void inserir(Show elemento, int posicao)
{

    int tam = tamanho();
    if (posicao < 0 || posicao > tam)
    {
        printf("Erro ao inserir (posição inválida)!\n");
        exit(1);
    }
    else if (posicao == 0)
    {
        inserirInicio(elemento);
    }
    else if (posicao == tam)
    {
        inserirFim(elemento);
    }
    else
    {

        // Caminha ate a posicao anterior a insercao
        Celula *i = primeiro;
        int j;

        for (j = 0; j < posicao; j++, i = i->prox)
            ;
        Celula *nova = novaCelula(elemento);
        nova->prox = i->prox;
        i->prox = nova;
        nova = i = NULL;
    }
}

// Remover elemento do inicio da lista
Show removerInicio()
{

    if (primeiro == ultimo)
    {
        printf("Erro ao remover (lista vazia)!\n");
        exit(1);
    }
    Celula *tmp = primeiro->prox;
    Show elemento = tmp->elemento;
    primeiro->prox = tmp->prox;
    if (tmp == ultimo)
    ultimo = primeiro;
    free(tmp);
    return elemento;
}

// Remover elemento do final da lista
Show removerFim()
{

    if (primeiro == ultimo)
    {
        printf("Erro ao remover (lista vazia)!\n");
        exit(1);
    }
    // Caminado até a penúltima célula
    Celula *i;
    for (i = primeiro; i->prox != ultimo; i = i->prox)
        ;

    Show elemento = ultimo->elemento;
    ultimo = i;
    free(ultimo->prox);
    i = ultimo->prox = NULL;
    return elemento;
}

// Remover elemento de uma posição específica
Show remover(int posicao)
{

    Show resposta;
    int tam = tamanho();

    if (primeiro == ultimo)
    {
        printf("Erro ao remover (lista vazia)!\n");
        exit(1);
    }
    else if (posicao < 0 || posicao >= tam)
    {
        printf("Erro ao remover (posicao invalida)!\n");
        exit(1);
    }
    else if (posicao == 0)
    {
        resposta = removerInicio();
    }
    else if (posicao == tam - 1)
    {
        resposta = removerFim();
    }
    else
    {
        // Caminhar até a célula anterior à inserção
        Celula *i = primeiro;
        int j;

        for (j = 0; j < posicao; j++, i = i->prox)
            ;
        Celula *nova = i->prox;
        resposta = nova->elemento;
        i->prox = nova->prox;
        nova->prox = NULL;
        free(nova);
        i = nova = NULL;

        return resposta;
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

// Mostra a lista
void mostrar()
{
    Celula *i;
    for (i = primeiro->prox; i != NULL; i = i->prox)
    {
        print_show(&(i->elemento));
    }
}
// Main
int main()
{
    read_csv("/tmp/disneyplus.csv");
    start(); // inicializa a lista

    // Leitura
    char entrada[256];
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0;
    while (!isFim(entrada))
    {
        Show *selecionado = buscarID(entrada);
        if (selecionado)
        {
            inserirInicio(*selecionado);
        }
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0;
    }

    int N;
    scanf("%d\n", &N);
    char id[100];
    char operacao[4];
    int posicao;

    for (int i = 0; i < N; i++)
    {
        scanf("%s", &operacao);
        if (operacao[0] == 'I')
        {
            if (operacao[1] == 'I')
            {
                scanf(" %[^\n]", id);
                removeN_R(id);
                Show *resultado = buscarID(id);
                if (resultado)
                    inserirInicio(*resultado);
            }
            else if (operacao[1] == 'F')
            {
                scanf(" %[^\n]", id);
                removeN_R(id);
                Show *resultado = buscarID(id);
                if (resultado != NULL)
                {
                    inserirFim(*resultado);
                }
            }

            else if (operacao[1] == '*')
            {
                scanf("%d", &posicao);
                scanf(" %[^\n]", id);
                removeN_R(id);
                Show *resultado = buscarID(id);
                if (resultado != NULL)
                {
                    inserir(*resultado, posicao);
                }
            }
        }
            else if(operacao[0] == 'R') {
                if(operacao[1] == 'I')
                {
                    Show show = removerInicio();
                    printf("(R) %s\n", show.title);
                }
                else if(operacao[1] == 'F')
                {
                    Show show = removerFim();
                    printf("(R) %s\n", show.title);
                }
                else if(operacao[1] == '*')
                {
                    scanf("%d", &posicao);
                    Show show = remover(posicao);
                    printf("(R) %s\n", show.title);
                }
            }
    }
    
      mostrar();
      return 0; 
 }



 


