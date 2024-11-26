#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 100
#define COLS 100

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
void printMatrixWithHighlights(char matrix[ROWS][COLS], int path[][2], int pathLength) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // Verifica se a posição (i, j) está no caminho da palavra
            bool isHighlighted = false;
            for (int k = 0; k < pathLength; k++) {
                if (path[k][0] == i && path[k][1] == j) {
                    isHighlighted = true;
                    break;
                }
            }

            // Imprime com destaque (ANSI color para terminal)
            if (isHighlighted) {
                printf("\033[1;31m%c\033[0m ", matrix[i][j]); // Vermelho
            } else {
                printf("%c ", matrix[i][j]);
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
    int path[100][2]; // Caminho da palavra na matriz (máximo de 100 caracteres)

    // Procurando por cada palavra na matriz
    for (int i = 0; i < wordCount; i++) {
        if (searchWordSnakeWithPath(matrix, ROWS, COLS, words[i], path)) {
            printf("A palavra '%s' foi encontrada na matriz nas coordenadas:\n", words[i]);
            for (int k = 0; k < strlen(words[i]); k++) {
                printf("(%d, %d) ", path[k][0], path[k][1]);
            }
            printf("\n");
            printf("Matriz destacada:\n");
            printMatrixWithHighlights(matrix, path, strlen(words[i]));
        } else {
            printf("A palavra '%s' não foi encontrada na matriz.\n", words[i]);
        }
        printf("\n");
    }

    return 0;
}
