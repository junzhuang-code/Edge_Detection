/* 
topic: Canny Edge Detection
author: Jun Zhuang
runs: 
  1. Define the WIDTH, HEIGHT, SIZE_FILTER, THRESHOLD, and the input image;
  2. Input the command line as follows:
    gcc edge_canny.cpp -o edge
    ./edge
*/
#include <stdio.h>
#include <fstream>
#include<cmath>
using namespace std;

// Need to be modified
#define WIDTH 750 // The width of input image
#define HEIGHT 500 // The height of input image
int SIZE_FILTER = 3; // The size of the filter: 3, 5;
int THRESHOLD = 100; // The threshold for edge detection \in [0, 255];
#define PI 3.14159265
// ----------
FILE *file_in, *file_out1, *file_out2;
unsigned char img[HEIGHT][WIDTH]; // Original image
unsigned char img_G[HEIGHT][WIDTH]; // The gradient image
unsigned char img_Gdir[HEIGHT][WIDTH]; // The gradient direction
unsigned char img_NMS[HEIGHT][WIDTH]; // Non-Maximum Suppression
// Define the Sobel operator (squared shape)
int Kernel_3x[3][3] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
int Kernel_3y[3][3] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
int Kernel_5x[5][5] = {-1,-2,0,2,1,-2,-4,0,4,2,-4,-8,0,8,4,-2,-4,0,4,2,-1,-2,0,2,1};
int Kernel_5y[5][5] = {-1,-2,-4,-2,-1,-2,-4,-8,-4,-2,0,0,0,0,0,2,4,8,4,2,1,2,4,2,1};


int main(){ // int argc, char *argv[]
    // OPen the raw file with given file name
    file_in =fopen("unesco750.raw","rb"); // Need to change the input file name
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
                    	}
                    	if (SIZE_FILTER == 5){
                    		g_x += img[x][y] * Kernel_5x[k][l];
                    		g_y += img[x][y] * Kernel_5y[k][l];
                    	}
                    }else{
                    	g_x = img[x][y];
                    	g_y = img[x][y];
                    }
            	}
            }
			// Compute the direction of the gradient
			img_Gdir[i][j] = atan2(g_y, g_x) * 180 / PI;
            // Compute the gradient G = sqrt((pow(g_x, 2.0) + pow(g_y, 2.0)))/2;
            img_G[i][j] = (int)sqrt((g_x*g_x + g_y*g_y))/2;
            //printf("img_G[i][j]: %d", img_G[i][j]);
            // Convert to binary image
            if(img_G[i][j] > THRESHOLD){
				img_G[i][j] = 255;
            }else{
            	img_G[i][j] = 0;
            }

        }
    }

    // Perform Non-Maximum Suppression
    float theta; // theta = atan2(Gy, Gx);
    for (int i = 1; i< HEIGHT-1; i++){
        for (int j = 1; j< WIDTH-1; j++){
        	theta = img_Gdir[i][j]; // The gradient direction of current pixel
        	img_NMS[i][j] = img_G[i][j];
        	// For theta = 0
        	if(((337.5 < theta) && (theta <= 22.5)) || ((157.5 < theta) && (theta <= 202.5))){
            	if ((img_G[i][j] <= img_G[i][j+1]) || (img_G[i][j] <= img_G[i][j-1]))
                	img_NMS[i][j] = 0;
        	}
	        // For theta = 45
	        if (((22.5 < theta) && (theta <= 67.5)) || ((202.5 < theta) && (theta <= 247.5))){
	            if ((img_G[i][j] <= img_G[i-1][j+1]) || (img_G[i][j] <= img_G[i+1][j-1]))
	                img_NMS[i][j] = 0;
	        }
        	// For theta = 90
	        if(((67.5 < theta) && (theta <= 112.5)) || ((247.5 < theta) && (theta <= 292.5))){
	            if((img_G[i][j] <= img_G[i+1][j]) || (img_G[i][j] <= img_G[i-1][j]))
	                img_NMS[i][j] = 0;
	        }
	        // For theta = 135
	        if (((112.5 < theta) && (theta <= 157.5)) || ((292.5 < theta) && (theta <= 337.5))){
	            if ((img_G[i][j] <= img_G[i+1][j+1]) || (img_G[i][j] <= img_G[i-1][j-1]))
	                img_NMS[i][j] = 0;
	        }

        }
    }

    // Output the result
    file_out1 = fopen("img_bin.raw", "wb");
    fwrite(img_G, 1, WIDTH*HEIGHT, file_out1);
    fclose(file_out1);
    file_out2 = fopen("img_nms.raw", "wb");
    fwrite(img_NMS, 1, WIDTH*HEIGHT, file_out2);
    fclose(file_out2);
    return 0;
}
