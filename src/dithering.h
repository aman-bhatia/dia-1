#ifndef DITHERING_H
#define DITHERING_H


#include "enhance.h"

#define UNIFORM_DIST 1
#define MEDIAN_CUT 2


/*
 * Median Cut for single channel
 * return vector of intensities calculated by median cut algorithm
 */
std::vector<int>* medianCut(cv::Mat &img, int num_colors);




/*
 * Uniformly Distributed Intensities for single channel
 * return vector of intensities calculated by uniform distribution of intensities
 */
std::vector<int>* uniformDist(int nc);




/*
 * Given an intensity and a color palette,
 * it returns nearest intensity from the color palette
 */
int nearest_intensity(int intensity, std::vector<int>* palette);




/*
 * Applies Quatization to the given image
 * Input :-
 *		num_colors 	:	number of colors in the quantized image
 *		method 		:	UNIFORM_DIST / MEDIAN_CUT
 */
cv::Mat quantize(cv::Mat& image, int num_colors, int method);




/*
 * Floyd-Steinberg Dithering Algorithm
 */
cv::Mat floyd(cv::Mat& image, int num_colors, int method);

#endif
