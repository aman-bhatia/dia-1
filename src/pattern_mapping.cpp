
#include "pattern_mapping.h"
#include "segmentation.h"

using namespace std;
using namespace cv;


Mat patternMapping(Mat &img){
	Mat seg_img = otsu(img,2);
	vector<int> intensities(0);

	for (int i=0;i<img.rows;i++){
		for (int j=0;j<img.cols;j++){
			if (find(intensities.begin(),intensities.end(),seg_img.at<uchar>(i,j)) == intensities.end())
				intensities.push_back(seg_img.at<uchar>(i,j));
		}
	}

	for (int i=0;i<seg_img.rows-3;i+=3){
		for (int j=0;j<seg_img.cols-3;j+=3){
			if (seg_img.at<uchar>(i,j) == intensities[0]){
				seg_img.at<uchar>(i,j) = 255;
				seg_img.at<uchar>(i,j+1) = 255;
				seg_img.at<uchar>(i,j+2) = 255;
				seg_img.at<uchar>(i+1,j) = 0;
				seg_img.at<uchar>(i+1,j+1) = 0;
				seg_img.at<uchar>(i+1,j+2) = 0;
				seg_img.at<uchar>(i+2,j) = 0;
				seg_img.at<uchar>(i+2,j+1) = 0;
				seg_img.at<uchar>(i+2,j+2) = 0;
			} else if (seg_img.at<uchar>(i,j) == intensities[1]){
				seg_img.at<uchar>(i,j) = 255;
				seg_img.at<uchar>(i,j+1) = 0;
				seg_img.at<uchar>(i,j+2) = 0;
				seg_img.at<uchar>(i+1,j) = 255;
				seg_img.at<uchar>(i+1,j+1) = 0;
				seg_img.at<uchar>(i+1,j+2) = 0;
				seg_img.at<uchar>(i+2,j) = 255;
				seg_img.at<uchar>(i+2,j+1) = 0;
				seg_img.at<uchar>(i+2,j+2) = 0;
			} else if (seg_img.at<uchar>(i,j) == intensities[2]){
				seg_img.at<uchar>(i,j) = 255;
				seg_img.at<uchar>(i,j+1) = 0;
				seg_img.at<uchar>(i,j+2) = 0;
				seg_img.at<uchar>(i+1,j) = 0;
				seg_img.at<uchar>(i+1,j+1) = 255;
				seg_img.at<uchar>(i+1,j+2) = 0;
				seg_img.at<uchar>(i+2,j) = 0;
				seg_img.at<uchar>(i+2,j+1) = 0;
				seg_img.at<uchar>(i+2,j+2) = 255;
			} else if (seg_img.at<uchar>(i,j) == intensities[3]){
				seg_img.at<uchar>(i,j) = 0;
				seg_img.at<uchar>(i,j+1) = 0;
				seg_img.at<uchar>(i,j+2) = 255;
				seg_img.at<uchar>(i+1,j) = 0;
				seg_img.at<uchar>(i+1,j+1) = 255;
				seg_img.at<uchar>(i+1,j+2) = 0;
				seg_img.at<uchar>(i+2,j) = 255;
				seg_img.at<uchar>(i+2,j+1) = 0;
				seg_img.at<uchar>(i+2,j+2) = 0;
			}
		}
	}

	return seg_img;
}