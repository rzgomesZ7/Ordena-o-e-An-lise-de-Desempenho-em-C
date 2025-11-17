#include <stdio.h>      // Biblioteca para funções de entrada/saída (printf, scanf)
#include <stdlib.h>     // Biblioteca para alocação de memória dinâmica (malloc, free)
#include <string.h>     // Biblioteca para manipulação de strings (strlen)
#include <time.h>       // Biblioteca para medir tempo usando clock()

// Estrutura para armazenar métricas de execução do algoritmo
typedef struct {
    long long cmp;      // Quantidade de comparações feitas
    long long swp;      // Quantidade de trocas realizadas
} Metrics;

// Macros que ampliam a contagem de comparações e trocas
#define COUNT_CMP(m) (m->cmp++)    // Conta 1 passo para cada comparação
#define COUNT_SWP(m) (m->swp++)    // Conta 1 passo para cada troca

// Função que zera as métricas antes de uma nova execução
void reset_metrics(Metrics *m) {
    m->cmp = 0;         // Zera contador de comparações
    m->swp = 0;         // Zera contador de trocas
}

/* ============================================================
 * FUNÇÃO ESCOLHIDA: selection_sort
 * Implementa o algoritmo Selection Sort com contagem de passos.
 * ============================================================ */
void selection_sort(int *v, size_t n, Metrics *m) {

    // Loop externo percorre cada posição do vetor até a penúltima
    for (size_t i = 0; i < n - 1; i++) {

        size_t min = i; // Assume que o menor elemento está inicialmente na posição i

        // Loop interno procura o menor elemento no restante do vetor
        for (size_t j = i + 1; j < n; j++) {

            COUNT_CMP(m);           // Conta a comparação realizada no if abaixo

            if (v[j] < v[min])      // Se encontrar elemento menor
                min = j;            // Atualiza posição do menor elemento
        }

        // Se a menor posição mudou, realiza troca
        if (min != i) {
            int temp = v[i];        // Variável temporária usada para a troca tradicional
            v[i] = v[min];
            v[min] = temp;
            COUNT_SWP(m);           // Conta a troca realizada
        }
    }
}

/* ============================================================
 * Função: converter_rgm
 * Converte uma string de RGM em um vetor de inteiros (um dígito por posição).
 * ============================================================ */
int* converter_rgm(const char *rgm, size_t *out_n) {

    *out_n = strlen(rgm);               // Calcula quantos dígitos o RGM possui
    int *vetor = malloc((*out_n) * sizeof(int));  // Aloca vetor de inteiros dinamicamente

    // Converte cada caractere '0'-'9' para seu valor numérico correspondente
    for (size_t i = 0; i < *out_n; i++)
        vetor[i] = rgm[i] - '0';        // Subtrai '0' para transformar char em int

    return vetor;                       // Retorna o vetor pronto
}

/* ============================================================
 * Função: imprimir_vetor
 * Imprime todos os elementos do vetor sem espaços (para mostrar o RGM ordenado).
 * ============================================================ */
void imprimir_vetor(int *v, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d", v[i]);             // Imprime cada número do vetor
}

/* ============================================================
 * Função: rodar_sort
 * Executa o sort, mede tempo e retorna duração em milissegundos.
 * ============================================================ */
double rodar_sort(
    void (*fn)(int*, size_t, Metrics*), // Ponteiro para função de ordenação
    int *v, size_t n,                   // Vetor e tamanho
    Metrics *m                          // Estrutura de métricas
) {
    reset_metrics(m);                    // Reseta métricas antes da execução

    clock_t t0 = clock();                // Marca tempo inicial
    fn(v, n, m);                         // Executa o algoritmo de ordenação
    clock_t t1 = clock();                // Marca tempo final

    // Converte diferença de clocks para milissegundos
    return 1000.0 * (t1 - t0) / CLOCKS_PER_SEC;
}

int main() {

    char rgm_str[64];                    // Buffer para armazenar RGM digitado pelo usuário

    printf("Digite seu RGM: ");          // Mensagem solicitando entrada
    scanf("%63s", rgm_str);              // Lê string do RGM com segurança (máx. 63 chars)

    size_t n;                            // Variável que receberá o tamanho do vetor
    int *v = converter_rgm(rgm_str, &n); // Converte string para vetor de inteiros

    Metrics mt;                           // Estrutura para armazenar métricas
    double tempo = rodar_sort(selection_sort, v, n, &mt);  // Executa sort + coleta tempo

    printf("\nRGM ordenado: ");
    imprimir_vetor(v, n);                // Imprime RGM ordenado

    // Calcula passos totais: soma de comparações + trocas
    long long passos = mt.cmp + mt.swp;

    // Imprime saída no formato CSV, como pedido no enunciado
    printf("\n\nCSV (para relatório)\n");
    printf("metodo,N,caso,comparacoes,trocas,passos,tempo_ms\n");
    printf("selection,%zu,rgm,%lld,%lld,%lld,%.4f\n",
        n, mt.cmp, mt.swp, passos, tempo);       // Linha de dados do CSV

    // Resumo amigável para leitura humana
    printf("\nResumo humano:\n");
    printf("Comparações: %lld\n", mt.cmp);    // Exibe número de comparações
    printf("Trocas: %lld\n", mt.swp);         // Exibe número de trocas
    printf("Passos totais: %lld\n", passos);  // Exibe passos (comparações + trocas)
    printf("Tempo: %.4f ms\n", tempo);        // Exibe tempo total

    free(v);                           // Libera memória alocada dinamicamente

    return 0;                          // Encerra o programa
}


