
#include "edge_detection.h"

using namespace std;
using namespace cv;



Mat extractEdges(Mat &img){

	// Difference of Gaussian
	Mat g1, g2, dog;

	/*
	 * k1 is the size of kernel 1
	 * k2 is the size of kernel 2
	 * by experimentation, we found that best results are obtained for k1=3 ad k2=13
	 */

	int k1 = 3, k2 = 11, s1 = 2 , s2 = 10;
	g1 = Gaussian(img, k1, s1);
	g2 = Gaussian(img, k2, s2);
	subtract(g2 , g1 , dog);


	/* 
	 * Edge Linking by Canny's approach
	 */

	// otsu's method for getting threshold
		auto otsu_max_threshold = [](Mat& img){

		int num_pixels = 0;

		// create histogram (histogram[i] = num of pixels with intensity i)
		vector<int> histogram(256);
		for (int i=0; i<img.rows; i++){
			for (int j=0; j<img.cols; j++){
				histogram[img.at<uchar>(i,j)]++;
			}
		}

		// find total mean
		double total_mean = 0;
		for (int i=0;i<=255;i++){
			total_mean += (histogram[i]*i);
			num_pixels += histogram[i];

		}
		total_mean /= num_pixels;

		int max_var_threshold = -1;
		double max_variance_sq = -1;

		for (int k=0;k<=255;k++){
			double m0 = 0;         // mean of class 1
			double w0 = 0;         // prob. of pixel being in class 1
			for (int i=0;i<=k;i++){
				w0 += histogram[i];
				m0 += (histogram[i]*i);
			}
			w0 /= num_pixels;
			m0 /= num_pixels;
			double variance_sq = ((total_mean * w0 - m0) * (total_mean * w0 - m0)) / (w0 * (num_pixels - w0));
			if (variance_sq > max_variance_sq){
				max_variance_sq = variance_sq;
				max_var_threshold = k;
			}
		}
		return max_var_threshold;
	};


	// th = max threshold as given by otsu
	// tl = half of th
	dog = enhance(dog, LOG_OF_INTENSITY);
	int th = otsu_max_threshold(img);
	int tl = th/2;

	for (int i=0; i<dog.rows; i++){
		for (int j=0; j<dog.cols; j++){
			if (dog.at<uchar>(i,j) >= th){
				dog.at<uchar>(i,j) = 255;       // its a strong edge
			} else if (dog.at<uchar>(i,j) >= tl && dog.at<uchar>(i,j) < th){
				dog.at<uchar>(i,j) = tl;        // its a weak edge
			} else {
				dog.at<uchar>(i,j) = 0;         // its not an edge
			}
		}
	}

	for (int i=1; i<dog.rows-1; i++){
		for (int j=1; j<dog.cols-1; j++){
			// check if the pixel belongs to a weak edge
			if (dog.at<uchar>(i,j) == tl){
				// check the 8 neighbourhood for strong edge pixel
				if ((dog.at<uchar>( i-1, j-1 ) == 255) || \
				(dog.at<uchar>( i-1, j   ) == 255) || \
				(dog.at<uchar>( i-1, j+1 ) == 255) || \
				(dog.at<uchar>( i,   j-1 ) == 255) || \
				(dog.at<uchar>( i,   j+1 ) == 255) || \
				(dog.at<uchar>( i+1, j-1 ) == 255) || \
				(dog.at<uchar>( i+1, j   ) == 255) || \
				(dog.at<uchar>( i+1, j+1 ) == 255))
					dog.at<uchar>(i,j) = 255;
				else
					dog.at<uchar>(i,j) = 0;
			}
		}
	}

	return dog;
}
