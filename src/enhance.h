#ifndef ENHANCE_H
#define ENHANCE_H


#include <map>
#include <math.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#define NEGATIVE 3
#define CONTRAST_STRECHING 4
#define LOG_OF_INTENSITY 5
#define INVERSE_LOG_OF_INTENSITY 6




/*
 * General Enhancement Function
 * Method can have the following values :-
 *		- NEGATIVE
 *		- CONTRAST_STRECHING
 *		- LOG_OF_INTENSITY
 *		- INVERSE_LOG_OF_INTENSITY
 */
cv::Mat enhance(cv::Mat& img, int method);




/*
 * Implementation of Gaussian Function for blurring
 * Input :-
 *		k 		:	kernel size
 *		sigma 	:	standard deviation
 */
cv::Mat Gaussian(cv::Mat &img , int k , double sigma);




/*
 * Embossing filter which detects changes in the upper-left direction
 *      1	0	0
 *      0	0	0
 *      0	0	-1
 */
cv::Mat emboss(cv::Mat &img);



/*
 * Enhances the edges/boundaries of the image
 */
cv::Mat silhoutte(cv::Mat &image);


#endif