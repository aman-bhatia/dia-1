#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "enhance.h"



/*
 * Otsu Algorithm for finding threshold automatically
 * Given an image and number of bit for colors (3 bits = 8 colors), it returns the segmented image
 */
cv::Mat otsu(cv::Mat &img, int numbits);




/*
 * K-mean clustering algorithm for image segmentation
 */
cv::Mat kmeans(cv::Mat &image, int num_clusters);


#endif