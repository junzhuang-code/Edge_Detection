# Edge Detection

#### Author: Jun Zhuang

### 0. About The Project
In this project, I implement edge detection algorithms from scratch using c++.  

### 1. Methods
Basically, I use Sobel operators to make convolutional operations with input images to compute the partial derivatives along x and y directions. After that, I compute the gradient image and then detect the edges at the peaks of gradient values. The total process follows [this site](http://mmlab.ie.cuhk.edu.hk/archive/gbq/csc5280_project_1.htm) (without pre-smoothing).  

Here I implement two versions. For the canny edge detector, I convert the gradient image to the binary image and then perform Non-Maximum Suppression. For the simple version, I only consider horizontal and vertical directions. Note that these are basic re-implementation as I don't put much time into tuning the scale number for avoiding the overflow problem. 

### 2. Run the scripts
I use GNU Compiler Collection (GCC) to run the scripts on MacOS.
> ```gcc script_name.cpp -o script_name``` \
> ```./script_name```

Before running, the user needs to set up the WIDTH and HEIGHT of the image, the size of the filter, the threshold, and the name of the input image inside the script (Cannot pass the arguments at this version).

### 3. Results
Original images are displayed as follows. Here we use the raw file as input and output. The image needs to be converted to jpg format for human vision.  
![unesco750.jpg](https://github.com/junzhuang-code/Edge_Detection/blob/main/unesco750.jpg)
![building.jpg](https://github.com/junzhuang-code/Edge_Detection/blob/main/building.jpg)

The result is presented as follows:
![canny](https://github.com/junzhuang-code/Edge_Detection/blob/main/results/canny.png)

![simple](https://github.com/junzhuang-code/Edge_Detection/blob/main/results/simple.png)


### Acknowledgments
 Thanks [professor Zheng](https://cs.iupui.edu/~jzheng/) for his patient instruction.
