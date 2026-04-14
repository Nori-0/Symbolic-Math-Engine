#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "plotter.h"

void disegna_linea_bresenham(char griglia[PLOT_HEIGHT][PLOT_WIDTH], int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x0 >= 0 && x0 < PLOT_WIDTH && y0 >= 0 && y0 < PLOT_HEIGHT) {
            griglia[y0][x0] = '*';
        }
        
        if (x0 == x1 && y0 == y1) break;
        
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void disegna_grafico_ascii(NodoAST* radice, TabellaSimboli* tabella, const char* var_nome) {
    char input[64];
    double x_min = -10.0, x_max = 10.0;

    printf("\n--- ASCII PLOTTER ---\n");
    printf("Enter lower bound for %s (e.g. -10): ", var_nome);
    if (fgets(input, sizeof(input), stdin) != NULL) x_min = strtod(input, NULL);
    
    printf("Enter upper bound for %s (e.g. 10): ", var_nome);
    if (fgets(input, sizeof(input), stdin) != NULL) x_max = strtod(input, NULL);

    if (x_min >= x_max) {
        printf("[ERROR] The lower bound has to be lower than the upper bound.\n");
        return;
    }

    int indice_var = -1;
    for (int i = 0; i < tabella->conteggio; i++) {
        if (strcmp(tabella->array[i].nome, var_nome) == 0) {
            indice_var = i; break;
        }
    }
    if (indice_var == -1) return;

    double valore_originale = tabella->array[indice_var].valore;

    double y_min = INFINITY, y_max = -INFINITY;
    double step_x = (x_max - x_min) / (PLOT_WIDTH - 1);
    double y_valori[PLOT_WIDTH];

    for (int col = 0; col < PLOT_WIDTH; col++) {
        double x = x_min + col * step_x;
        tabella->array[indice_var].valore = x;
        double y = valuta_albero(radice, tabella);
        
        y_valori[col] = y;

        if (!isnan(y) && !isinf(y)) {
            if (y < y_min) y_min = y;
            if (y > y_max) y_max = y;
        }
    }

    if (y_max == y_min) { y_max += 1.0; y_min -= 1.0; }
    if (isinf(y_min) || isinf(y_max)) {
        printf("[ERROR] Impossible to determine the bounds of the graph.\n");
        return;
    }

    char griglia[PLOT_HEIGHT][PLOT_WIDTH];
    for (int r = 0; r < PLOT_HEIGHT; r++) {
        for (int c = 0; c < PLOT_WIDTH; c++) {
            griglia[r][c] = ' ';
        }
    }

    int asse_x_row = -1;
    if (0.0 >= y_min && 0.0 <= y_max) {
        asse_x_row = (int)round((PLOT_HEIGHT - 1) - (0.0 - y_min) / (y_max - y_min) * (PLOT_HEIGHT - 1));
        for (int c = 0; c < PLOT_WIDTH; c++) griglia[asse_x_row][c] = '-';
    }
    
    int asse_y_col = -1;
    if (0.0 >= x_min && 0.0 <= x_max) {
        asse_y_col = (int)round((0.0 - x_min) / (x_max - x_min) * (PLOT_WIDTH - 1));
        for (int r = 0; r < PLOT_HEIGHT; r++) {
            if (griglia[r][asse_y_col] == '-') griglia[r][asse_y_col] = '+';
            else griglia[r][asse_y_col] = '|';
        }
    }

int prev_col = -1;
    int prev_row = -1;

    for (int col = 0; col < PLOT_WIDTH; col++) {
        double y = y_valori[col];
        
        if (!isnan(y) && !isinf(y)) {
            int row = (int)round((PLOT_HEIGHT - 1) - (y - y_min) / (y_max - y_min) * (PLOT_HEIGHT - 1));
            
            if (prev_col != -1 && prev_row != -1) {
                disegna_linea_bresenham(griglia, prev_col, prev_row, col, row);
            } else {
                if (row >= 0 && row < PLOT_HEIGHT) griglia[row][col] = '*';
            }
            
            prev_col = col;
            prev_row = row;
        } else {
            prev_col = -1;
            prev_row = -1;
        }
    }

    printf("\nf(%s) Graph in [%.2f, %.2f]. Range Y: [%.2f, %.2f]\n\n", var_nome, x_min, x_max, y_min, y_max);
    for (int r = 0; r < PLOT_HEIGHT; r++) {
        for (int c = 0; c < PLOT_WIDTH; c++) {
            putchar(griglia[r][c]);
        }
        putchar('\n');
    }

    tabella->array[indice_var].valore = valore_originale;
}
