#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 30
#define COLS 30
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

// Função para verificar se a palavra existe na matriz e retornar o caminho
bool searchWordSnakeWithPath(char matrix[ROWS][COLS], int rows, int cols, const char* word, int path[][2]) {
    int len = strlen(word);

    // Direções possíveis (horizontal, vertical, diagonal)
    int directions[8][2] = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}, // Horizontal e vertical
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonais
    };

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Verifica todas as direções
            for (int d = 0; d < 8; d++) {
                int x = i, y = j, k;

                // Verifica se a palavra pode ser formada
                for (k = 0; k < len; k++) {
                    // Ajusta as coordenadas para serem circulares
                    if (x < 0) x = rows - 1;
                    if (x >= rows) x = 0;
                    if (y < 0) y = cols - 1;
                    if (y >= cols) y = 0;

                    if (matrix[x][y] != word[k]) {
                        break;
                    }

                    // Salva a coordenada do caractere atual
                    path[k][0] = x;
                    path[k][1] = y;

                    x += directions[d][0];
                    y += directions[d][1];
                }

                // Se a palavra foi encontrada
                if (k == len) {
                    return true;
                }
            }
        }
    }

    return false;
}

// Função para exibir a matriz com destaques
void printMatrixWithHighlights(char matrix[ROWS][COLS], int paths[MAX_WORDS][MAX_WORD_LENGTH][2], int pathLengths[], int foundCount) {
    // Array para contar quantos destaques cada posição tem
    int highlightCount[ROWS][COLS] = {{0}};

    // Contabiliza os destaques para cada posição
    for (int w = 0; w < foundCount; w++) {
        for (int k = 0; k < pathLengths[w]; k++) {
            int x = paths[w][k][0];
            int y = paths[w][k][1];
            highlightCount[x][y]++;
        }
    }

    // Exibe a matriz, com base nos destaques
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // Se a posição (i, j) tem destaque
            if (highlightCount[i][j] > 0) {
                printf("\033[1;31m%c\033[0m ", matrix[i][j]); // Destacar em vermelho
            } else {
                printf("%c ", matrix[i][j]); // Imprimir normalmente
            }
        }
        printf("\n");
    }
}

int main() {
    char matrix[ROWS][COLS];
    FILE* file = fopen("exemplo_palavras.txt", "r");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lendo a matriz do arquivo
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(file, " %c", &matrix[i][j]);
        }
    }

    fclose(file);

    // Lista de palavras para buscar
    const char* words[] = {"algoritmos", "bubblesort", "quicksort", "mergesort", "arvore", "openmp", "prova"};
    int wordCount = sizeof(words) / sizeof(words[0]);

    // Arrays para armazenar os resultados
    char foundWords[MAX_WORDS][MAX_WORD_LENGTH];
    int paths[MAX_WORDS][MAX_WORD_LENGTH][2];
    int pathLengths[MAX_WORDS];
    int foundCount = 0;

    // Procurando por cada palavra na matriz
    for (int i = 0; i < wordCount; i++) {
        int path[MAX_WORD_LENGTH][2]; // Caminho da palavra na matriz
        if (searchWordSnakeWithPath(matrix, ROWS, COLS, words[i], path)) {
            // Salva a palavra encontrada
            strcpy(foundWords[foundCount], words[i]);
            pathLengths[foundCount] = strlen(words[i]);
            memcpy(paths[foundCount], path, sizeof(path));
            foundCount++;
        }
    }

    // Exibindo os resultados ao final
    printf("\nResultados:\n");
    for (int i = 0; i < foundCount; i++) {
        printf("Palavra '%s' encontrada nas coordenadas: ", foundWords[i]);
        for (int k = 0; k < pathLengths[i]; k++) {
            printf("(%d, %d) ", paths[i][k][0], paths[i][k][1]);
        }
        printf("\n");
    }

    // Exibir a matriz com destaques
    printf("\nMatriz com palavras destacadas:\n");
    printMatrixWithHighlights(matrix, paths, pathLengths, foundCount);

    return 0;
}
