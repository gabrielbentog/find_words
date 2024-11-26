#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

#define ROWS 1000
#define COLS 1000
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

// Função para buscar uma palavra na matriz
bool buscarPalavraCobrinha(const char matriz[ROWS][COLS], int rows, int cols, const char* word, int path[][2]) {
    int len = strlen(word);

    // Direções possíveis (horizontal, vertical, diagonal)
    const int direcoes[8][2] = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}, // Horizontal e vertical
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonais
    };

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Testar todas as direções
            for (int d = 0; d < 8; d++) {
                int x = i, y = j, k;

                for (k = 0; k < len; k++) {
                    // Ajuste de coordenadas circulares
                    x = (x + rows) % rows;
                    y = (y + cols) % cols;

                    if (matriz[x][y] != word[k]) break;

                    // Salvar coordenada atual
                    path[k][0] = x;
                    path[k][1] = y;

                    x += direcoes[d][0];
                    y += direcoes[d][1];
                }

                // Palavra encontrada
                if (k == len) return true;
            }
        }
    }
    return false;
}

// Função para carregar matriz do arquivo
bool carregarMatriz(const char* filename, char matriz[ROWS][COLS]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return false;
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (fscanf(file, " %c", &matriz[i][j]) != 1) {
                fprintf(stderr, "Erro ao ler a matriz do arquivo.\n");
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);
    return true;
}

int main() {
    char matriz[ROWS][COLS];
    const char* words[] = {"algoritmos", "bubblesort", "quicksort", "mergesort", "arvore", "openmp", "prova"};
    const int wordCount = sizeof(words) / sizeof(words[0]);

    // Carregar a matriz do arquivo
    if (!carregarMatriz("../exemplo_palavras2.txt", matriz)) {
        return 1;
    }

    char foundWords[MAX_WORDS][MAX_WORD_LENGTH];
    int paths[MAX_WORDS][MAX_WORD_LENGTH][2];
    int pathLengths[MAX_WORDS];
    int foundThreads[MAX_WORDS]; // Armazena qual thread encontrou cada palavra
    int foundCount = 0;

    double start_time = omp_get_wtime(); // Início da medição de tempo

    // Buscando as palavras sequencialmente
    for (int i = 0; i < wordCount; i++) {
        int path[MAX_WORD_LENGTH][2];

        // Captura do tempo inicial para a palavra
        double thread_start_time = omp_get_wtime();
        
        if (buscarPalavraCobrinha(matriz, ROWS, COLS, words[i], path)) {
            strcpy(foundWords[foundCount], words[i]);
            pathLengths[foundCount] = strlen(words[i]);
            memcpy(paths[foundCount], path, sizeof(path));
            foundThreads[foundCount] = -1; // Indicador de que foi encontrado sequencialmente
            foundCount++;

            // Exibindo o tempo que a busca levou
            printf("Palavra '%s' encontrada em %.6f segundos.\n", foundWords[foundCount-1], omp_get_wtime() - thread_start_time);
        }
    }

    double end_time = omp_get_wtime(); // Fim da medição de tempo

    // Exibir resultados
    printf("\nResultados:\n");
    for (int i = 0; i < foundCount; i++) {
        printf("Palavra '%s' encontrada nas coordenadas: ", foundWords[i]);
        for (int k = 0; k < pathLengths[i]; k++) {
            printf("(%d, %d) ", paths[i][k][0], paths[i][k][1]);
        }
        printf("\n");
    }

    // Exibir tempo total de execução
    printf("\nTempo total de execução: %.6f segundos\n", end_time - start_time);

    return 0;
}