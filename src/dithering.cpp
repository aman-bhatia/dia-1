
#include "dithering.h"
#include "segmentation.h"

using namespace std;
using namespace cv;



vector<int>* medianCut(Mat &img, int nc){
	vector<int> intensities(0);
	for (int i=0;i<img.cols;i++){
		for (int j=0;j<img.rows;j++){
			intensities.push_back(img.at<uchar>(i,j));
		}
	}
	sort(intensities.begin(),intensities.end());
	assert (nc < intensities.size());
	vector<int>* ret = new vector<int>(0);
	for (int i=0; i<nc;i++){
		int average = 0;
		int start = (i*intensities.size())/nc;
		int end = ((i+1)*intensities.size())/nc;
		for (int j=start;j<end;j++){
			average += intensities[j];
			if (j == end - 1){
				average /= (end-start);
			}
		}
		ret->push_back(average);
	}
	return ret;
}



vector<int>* uniformDist(int nc){
	vector<int>* ret = new vector<int>(0);
	for (int i=0; i<nc;i++){
		ret->push_back(((255*i)/(nc-1)));
	}
	return ret;
}




int nearest_intensity(int intensity, vector<int>* palette){
	if (intensity <= palette->at(0))
		return palette->at(0);
	if (intensity >= palette->at(palette->size()-1))
		return palette->at(palette->size()-1);
	for (unsigned int i=0; i<palette->size()-1;i++){
		if ((palette->at(i) <= intensity) && (intensity <= palette->at(i+1))){
			int diff1 = intensity-palette->at(i);
			int diff2 = palette->at(i+1)-intensity;
			if (diff1 == min(diff1, diff2))
				return palette->at(i);
			else
				return palette->at(i+1);
		}
	}
}




Mat quantize(Mat& image, int num_colors, int method)
{
	// check for valid range of number of colors
	if (num_colors<=1 || num_colors>256){
		cout << "Specify a value of num_colors in range (2,256) both included\n\n";
		exit(0);
	}

	// check if the image is grayscale
	if (image.channels() != 1){
		cout << "Give Grayscale Image as input\n\n";
		exit(0);
	}
	Mat img = image.clone();
	vector<int>* palette;
	if (method == MEDIAN_CUT)
		palette = medianCut(img,num_colors);
	else if (method == UNIFORM_DIST)
		palette = uniformDist(num_colors);
	else {
		cout << "Select a method from MEDIAN_CUT or UNIFORM_DIST only\n\n";
		exit(0);
	}
	for (int i=1;i<img.rows-1;i++){
		for (int j=1; j<img.cols-1;j++){
			img.at<uchar>(i,j) = nearest_intensity(img.at<uchar>(i,j), palette);
		}
	}
	return img;
}




Mat floyd(Mat& image, int num_colors, int method)
{
	// check for valid range of number of colors
	if (num_colors<=1 || num_colors>256){
		cout << "Specify a value of num_colors in range (2,256) both included\n\n";
		exit(0);
	}

	Mat img = image.clone();
	vector<int>* palette;
	if (method == MEDIAN_CUT)
		palette = medianCut(img,num_colors);
	else if (method == UNIFORM_DIST)
		palette = uniformDist(num_colors);
	else {
		cout << "Select a method from MEDIAN_CUT or UNIFORM_DIST only\n\n";
		exit(0);
	}

	for (int i=1;i<img.rows-1;i++){
		for (int j=1; j<img.cols-1;j++){
			int new_intensity = nearest_intensity(img.at<uchar>(i,j), palette);
			int error = img.at<uchar>(i,j) - new_intensity;
		   img.at<uchar>(i,j) = new_intensity;
			if (error<0){
				img.at<uchar>(i,j+1) = max(0, img.at<uchar>(i,j+1) + (error * 7)/16);
				img.at<uchar>(i+1,j+1) = max(0, img.at<uchar>(i+1,j+1) + (error)/16);
				img.at<uchar>(i+1,j) = max(0, img.at<uchar>(i+1,j) + (error * 5)/16);
				img.at<uchar>(i+1,j-1) = max(0, img.at<uchar>(i+1,j-1) + (error * 3)/16);
			} else if (error>0){
				img.at<uchar>(i,j+1) = min(255, img.at<uchar>(i,j+1) + (error * 7)/16);
				img.at<uchar>(i+1,j+1) = min(255, img.at<uchar>(i+1,j+1) + (error)/16);
				img.at<uchar>(i+1,j) = min(255, img.at<uchar>(i+1,j) + (error * 5)/16);
				img.at<uchar>(i+1,j-1) = min(255, img.at<uchar>(i+1,j-1) + (error * 3)/16);
			}
		}
	}
	return img;
}