#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // funções strcmp e strcpy
#include <math.h>

// Responsável: EDUARDO
// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE 1
#define FALSE 0

// Definição do tipo string
typedef char *string;

#define MAX_STRING_LEN 20

unsigned converter(string s)
{
    unsigned h = 0;
    for (int i = 0; s[i] != '\0'; i++)
        h = h * 256 + s[i];
    return h;
}

string *ler_strings(const char *arquivo, const int n)
{
    FILE *f = fopen(arquivo, "r");

    string *strings = (string *)malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++)
    {
        strings[i] = (string)malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double)(_fim - _ini)) / CLOCKS_PER_SEC;
}

unsigned h_div(unsigned x, unsigned i, unsigned B)
{
    return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i, unsigned B)
{
    const double A = 0.6180;
    return ((int)((fmod(x * A, 1) * B) + i)) % B;
}

string *criar_hash(unsigned B)
{

    string *tabela_hash = calloc(sizeof(string), B);
    // tabela_hash[i] == null representa bucket vazio

    return tabela_hash;
}

int inserir_hash(string *tabela_hash, unsigned B, unsigned *colisoes, string insercao, bool is_div)
{

    unsigned convertido = converter(insercao);
    int index;

    for (int i = 0; i < B; i++)
    {

        if (is_div)
        {
            index = h_div(convertido, i, B);
        }
        else
        {
            index = h_mul(convertido, i, B);
        }

        if (tabela_hash[index] == NULL)
        {
            tabela_hash[index] = malloc(sizeof(char) * MAX_STRING_LEN);
            strcpy(tabela_hash[index], insercao);
            return 0; // Inserido
        }

        if (!strcmp(tabela_hash[index], insercao))
        {
            (*colisoes) -= i;
            return 0; // Elemento já presente
        }

        (*colisoes)++; // Colisao, continua insercao
    }
}

int buscar_hash(string *tabela_hash, unsigned B, int *encontrados, string buscado, bool is_div)
{

    unsigned convertido = converter(buscado);
    int index;

    for (int i = 0; i < B; i++)
    {

        if (is_div)
        {
            index = h_div(convertido, i, B);
        }
        else
        {
            index = h_mul(convertido, i, B);
        }

        if (tabela_hash[index] == NULL)
        {
            return 0; // Nao encontrado
        }

        if (!strcmp(tabela_hash[index], buscado))
        {
            (*encontrados)++;
            return index; // Elemento encontrado
        }
    }
    return 1; // Não encontrado
}

int main(int argc, char const *argv[])
{
    unsigned N = 50000;
    unsigned M = 70000;
    unsigned B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string *insercoes = ler_strings("strings_entrada.txt", N);
    string *consultas = ler_strings("strings_busca.txt", M);

    // cria tabela hash com hash por divisão
    string *tabela_hash_div = criar_hash(B);

    // inserção dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++)
    {
        // inserir insercoes[i] na tabela hash
        inserir_hash(tabela_hash_div, B, &colisoes_h_div, insercoes[i], TRUE);
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // consulta dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++)
    {
        // buscar consultas[i] na tabela hash
        buscar_hash(tabela_hash_div, B, &encontrados_h_div, consultas[i], TRUE);
    }
    double tempo_busca_h_div = finaliza_tempo();

    // limpa a tabela hash com hash por divisão
    for (int i = 0; i < B; i++)
    {
        // if(tabela_hash_div[i] != NULL)s
        free(tabela_hash_div[i]);
    }
    free(tabela_hash_div);

    // cria tabela hash com hash por multiplicação
    string *tabela_hash_mul = criar_hash(B);

    // inserção dos dados na tabela hash usando hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++)
    {
        // inserir insercoes[i] na tabela hash
        inserir_hash(tabela_hash_mul, B, &colisoes_h_mul, insercoes[i], FALSE);
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++)
    {
        // buscar consultas[i] na tabela hash
        buscar_hash(tabela_hash_mul, B, &encontrados_h_mul, consultas[i], FALSE);
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // limpa a tabela hash com hash por multiplicação
    for (int i = 0; i < B; i++)
    {
        // if(tabela_hash_mul[i] != NULL)
        free(tabela_hash_mul[i]);
    }
    free(tabela_hash_mul);

    // limpa insercoes
    for (int i = 0; i < N; i++)
    {
        free(insercoes[i]);
    }
    free(insercoes);

    // limpa consultas
    for (int i = 0; i < N; i++)
    {
        free(consultas[i]);
    }
    free(consultas);

    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    return 0;
}