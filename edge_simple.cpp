/* 
topic: Simple Edge Detection
author: Jun Zhuang
runs: 
  1. Define the WIDTH, HEIGHT, SIZE_FILTER, THRESHOLD, and the input image;
  2. Input the command line as follows:
    gcc edge_simple.cpp -o edge
    ./edge
*/
#include <stdio.h>
#include <stdlib.h>  /* abs */
#include <fstream>
#include<cmath>
using namespace std;

// Need to be modified
#define WIDTH 480 // The width of input image: 480
#define HEIGHT 640 // The height of input image: 640
int SIZE_FILTER = 3; // The size of the filter: 3/5;
int THRESHOLD = 30; // The threshold for edge detection \in [0, 255]; {kernel3: 30, kernel5: 20}
int SCALE; // {kernel3: 8, kernel5: 64}
#define PI 3.14159265
// ----------
FILE *file_in, *file_out1, *file_out2;
unsigned char img[HEIGHT][WIDTH]; // Original image
unsigned char img_Gx[HEIGHT][WIDTH]; // The gradient along x direction
unsigned char img_Gy[HEIGHT][WIDTH]; // The gradient along y direction
unsigned char img_G[HEIGHT][WIDTH]; // The gradient image
unsigned char img_out[HEIGHT][WIDTH]; // Output image (should have the same size as input)
// Define the kernel (squared shape)
int Kernel_3x[3][3] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
int Kernel_3y[3][3] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
int Kernel_5x[5][5] = {-1,-2,0,2,1,-2,-4,0,4,2,-4,-8,0,8,4,-2,-4,0,4,2,-1,-2,0,2,1};
int Kernel_5y[5][5] = {-1,-2,-4,-2,-1,-2,-4,-8,-4,-2,0,0,0,0,0,2,4,8,4,2,1,2,4,2,1};


int main(){
    // OPen the raw file with given file name
    file_in =fopen("building.raw","rb"); // Need to change the input file name
    // Read the raw data into the 2D image
    fread(img, 1, WIDTH*HEIGHT, file_in); // IMG_SIZE = WIDTH*HEIGHT
    fclose(file_in);

    // Compute the gradient image
    const int margin = SIZE_FILTER / 2; // rows = cols
    int i, j;
    for (i = 0; i< HEIGHT; i++){
        for (j = 0; j< WIDTH; j++){
        	// conv operation
            int g_x = 0, g_y = 0;
            int k, l;
            for (k=0; k<SIZE_FILTER; k++){
            	for (l=0; l<SIZE_FILTER; l++){
            		// Compute the offset for x & y directions
                    int x = i-margin+k;
                    int y = j-margin+l;
                    // Make sure the conv will not be out of range
                    if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH){
                    	// Compute the gradient along x and y directions with different kernel
                    	if (SIZE_FILTER == 3){
                    		g_x += img[x][y] * Kernel_3x[k][l];
                    		g_y += img[x][y] * Kernel_3y[k][l];
                            SCALE = 8;
                    	}
                    	if (SIZE_FILTER == 5){
                    		g_x += img[x][y] * Kernel_5x[k][l];
                    		g_y += img[x][y] * Kernel_5y[k][l];
                            SCALE = 64;
                    	}
                    }else{
                    	g_x = img[x][y];
                    	g_y = img[x][y];
                    }
            	}
            }
			// Assign g_x, g_y to the array;
			img_Gx[i][j] = (char)abs(g_x)/SCALE;
			img_Gy[i][j] = (char)abs(g_y)/SCALE;
            // Compute the gradient G = sqrt((pow(g_x, 2.0) + pow(g_y, 2.0)))/(0.5*SCALE);
            img_G[i][j] = (int)sqrt((g_x*g_x + g_y*g_y))/(0.5*SCALE);
            //printf("img_G[i][j]: %d", img_G[i][j]);
        }
    }

    // Simple Edge Detection
    for (i = 1; i< HEIGHT-1; i++){
        for (j = 1; j< WIDTH-1; j++){
        	// Here we only consider two directions
        	if(img_Gx[i][j] < img_Gy[i][j]){ // More close to the horizontal line
        		if ((img_G[i][j] > img_G[i-1][j]) && (img_G[i][j] >= img_G[i+1][j]) && (img_G[i][j] > THRESHOLD)){
        		// The edge should have higher intensity than its neighbors.
        			img_out[i][j] = 250; // max <= 255
        		}else{
        			img_out[i][j] = 0;
        		}
        	}
        	//if(img_Gx[i][j] > img_Gy[i][j]){ // More close to the vertical line
        	else{
            	if ((img_G[i][j] > img_G[i][j-1]) && (img_G[i][j] >= img_G[i][j+1]) && (img_G[i][j] > THRESHOLD)){
        		// The edge should have higher intensity than its neighbors.
        			img_out[i][j] = 250;
        		}else{
        			img_out[i][j] = 0;
        		}
        	}
        }
    }

    // Output the result (here we only output raw file)
    file_out1 = fopen("img_grad.raw", "wb");
    fwrite(img_G, 1, WIDTH*HEIGHT, file_out1);
    fclose(file_out1);
    file_out2 = fopen("img_edge.raw", "wb");
    fwrite(img_out, 1, WIDTH*HEIGHT, file_out2);
    fclose(file_out2);
    return 0;
}
