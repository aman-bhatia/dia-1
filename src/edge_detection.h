#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include "enhance.h"

/*
 * Extract edges by using difference of gaussian and link the egdes as done by Canny
 * Thresholds for Canny Edge linking are calculated using Otsu's Thresholding Method
 */
cv::Mat extractEdges(cv::Mat &img);

#endif