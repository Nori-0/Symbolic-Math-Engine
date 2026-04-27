#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "plotter.h"

#define IMG_WIDTH 800
#define IMG_HEIGHT 600

typedef struct {
	unsigned char r, g, b;
} Pixel;

/* Helper Function for mapping in a secure way the coordinates (x,y) into the 
 * array 1D of the memory*/
void set_pixel(Pixel* buffer, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
	if (x >= 0 && x < IMG_WIDTH && y >= 0 && y < IMG_HEIGHT) {
		int index = y * IMG_WIDTH + x;
		buffer[index].r = r;
		buffer[index].g = g;
		buffer[index].b = b;
	}
}

void draw_pixel_line(Pixel* buffer, int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b) {
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	while (1) {
		set_pixel(buffer, x0, y0, r, g, b);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { 
			err += dy; 
			x0 += sx;
		}
		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void draw_ascii_graph(NodoAST* root, TabellaSimboli* table, const char* var_name) {
	char input[64];
	double x_min = -10.0, x_max = 10.0;

	printf("\n--- HIGH RESOLUTION RASTER GRAPHICS ENGINE ---\n");
	printf("Enter lower limit for %s (e.g. -10):", var_name);
	if (fgets(input, sizeof(input), stdin) != NULL) x_min = strtod(input, NULL);
	printf("Enter upper limit for %s (e.g. 10):", var_name);
	if (fgets(input, sizeof(input), stdin) != NULL) x_max = strtod(input, NULL);

	if (x_min >= x_max) {
		printf("[ERROR] The lower bound must be less than the upper bound.\n");
		return;
	}

	int var_index = -1;
	for (int i = 0; i < table->conteggio; i++) {
		if (strcmp(table->array[i].nome, var_name) == 0) {
			var_index = i;
			break;
		}
	}
	if (var_index == -1)
		return;

	double original_value = table->array[var_index].valore;

	Pixel* buffer = (Pixel*)malloc(IMG_WIDTH * IMG_HEIGHT * sizeof(Pixel));
	if (!buffer) {
		printf("[ERROR] Not enough RAM for the frame buffer.\n");
		return;
	}

	for (int i = 0; i < IMG_WIDTH * IMG_HEIGHT; i++) {
		buffer[i].r = 30;
		buffer[i].g = 30;
		buffer[i].b = 35;
	}

	double y_min = INFINITY, y_max = -INFINITY;
	double step_x = (x_max - x_min) / (IMG_WIDTH - 1);
	double y_values[IMG_WIDTH];

	for (int col = 0; col < IMG_WIDTH; col++) {
		double x = x_min + col * step_x;
		table->array[var_index].valore = x;
		double y = valuta_albero(root, table);
		y_values[col] = y;

		if (!isnan(y) && !isinf(y)) {
			if (y < y_min) y_min = y;
			if (y > y_max) y_max = y;
		}
	}

	if (y_max == y_min) {
		y_max += 1.0;
		y_min -= 1.0;
	}

	int x_axis_row = -1, y_axis_col = -1;
	if (0.0 >= y_min && 0.0 <= y_max) {
		x_axis_row = (int)round((IMG_HEIGHT - 1) - (0.0 - y_min) / (y_max - y_min) * (IMG_HEIGHT - 1));
		draw_pixel_line(buffer, 0, x_axis_row, IMG_WIDTH - 1, x_axis_row, 100, 100, 110);
	}
	if (0.0 >= x_min && 0.0 <= x_max) {
		y_axis_col = (int)round((0.0 - x_min) / (x_max - x_min) * (IMG_WIDTH - 1));
		draw_pixel_line(buffer, y_axis_col, 0, y_axis_col, IMG_HEIGHT - 1, 100, 100, 110);
	}

	int prev_col = -1;
	int prev_row = -1;

	for (int col = 0; col < IMG_WIDTH; col++) {
		double y = y_values[col];
		if (!isnan(y) && !isinf(y)) {
			int row = (int)round((IMG_HEIGHT - 1) - (y - y_min) / (y_max - y_min) * (IMG_HEIGHT - 1));

			if (prev_col != -1 && prev_row != -1 && abs(row - prev_row) < IMG_HEIGHT / 2) {
				draw_pixel_line(buffer, prev_col, prev_row, col, row, 0, 255, 200);
			} else {
				set_pixel(buffer, col, row, 0, 255, 200);
			}
			prev_col = col;
			prev_row = row;
		} else {
			prev_col = -1;
			prev_row = -1;
		}
	}
	
    	const char* filename = "graphs/graph.ppm";
    	FILE *f = fopen(filename, "wb");
    	if (f) {
        	fprintf(f, "P6\n%d %d\n255\n", IMG_WIDTH, IMG_HEIGHT);
        	fwrite(buffer, sizeof(Pixel), IMG_WIDTH * IMG_HEIGHT, f);
        	fclose(f);
        
        	printf("\n[SYSTEM] Frame rendered in: %s (%dx%d pixels)\n\n", filename, IMG_WIDTH, IMG_HEIGHT);
        	system("timg graphs/graph.ppm");

    	} else {
        	printf("\n[ERROR] Unable to access disk to save image.\n");
    	}

	free(buffer);
    	table->array[var_index].valore = original_value;

}
