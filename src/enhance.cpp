
#include "enhance.h"

using namespace std;
using namespace cv;




cv::Mat enhance(cv::Mat& img, int method){
	Mat result = img.clone();
	if (method == NEGATIVE){
		for (int i=0;i<result.cols;i++){
			for (int j=0;j<result.rows;j++){
				result.at<uchar>(i,j) = 255 - result.at<uchar>(i,j);
			}
		}
		return result;
	} else if (method == CONTRAST_STRECHING){
		int min=255,max=0;
		for (int i=0;i<result.cols;i++){
			for (int j=0;j<result.rows;j++){
				int intensity = result.at<uchar>(i,j);
				if (intensity < min)
					min = intensity;
				if (intensity > max)
					max = intensity;
			}
		}
		for (int i=0;i<result.cols;i++){
			for (int j=0;j<result.rows;j++){
				result.at<uchar>(i,j) = (255 * (result.at<uchar>(i,j) - min))/(max-min);
			}
		}
		return result;
	} else if (method == LOG_OF_INTENSITY){
		for (int i=0;i<result.cols;i++){
			for (int j=0;j<result.rows;j++){
				result.at<uchar>(i,j) = (int)(255 * (log10(1+result.at<uchar>(i,j)))/log10(256));
			}
		}
		return result;
	} else if (method == INVERSE_LOG_OF_INTENSITY){
		for (int i=0;i<result.cols;i++){
			for (int j=0;j<result.rows;j++){
				result.at<uchar>(i,j) = (int)(pow(10.0,(log10(256) * result.at<uchar>(i,j) / 255)) - 1);
			}
		}
		return result;
	} else{
		cout << "Please input appropriate enhancing method\n\n";
		exit(0);
	}
}



cv::Mat Gaussian(Mat &img , int k , double sigma){

	Mat result(img.rows , img.cols , DataType<uchar>::type);
	
	for(int i = 0 ; i < img.cols ; ++i){
		for(int j = 0 ; j < img.rows ; ++j){

			double accumulatedintensity = 0;
			double accumulatedweights = 0;

			for(int q = -(k/2) ; q <= (k/2) ; ++q){
				for(int w = -(k/2) ; w <= (k/2) ; ++w){

					double temp = exp(((-1) * (pow(abs(w) , 2) + pow(abs(q) , 2)) )/ (2 * (pow(sigma,2))));

					if(j+q >= 0 and j+q < img.rows and i+w>=0 and i+w<img.cols){
						accumulatedweights += temp;
						accumulatedintensity += (img.at<uchar>(j+q,i+w) * temp);
					}
				}
			}
			
			result.at<uchar>(j,i) = int(accumulatedintensity / accumulatedweights);
		}
	}

	return result;
}

Mat emboss(Mat &img){

	Mat embossed_image = img.clone();

	for (int i=1; i<img.rows-1; i++){
		for(int j=1; j<img.cols-1; j++){
			embossed_image.at<uchar>(i,j) = min(255, max(0, 127 + img.at<uchar>(i-1,j-1) - img.at<uchar>(i+1,j+1)));
		}
	}
	return embossed_image;
}



Mat silhoutte(Mat &image)
{
	Mat averageimage = Gaussian(image, 7, 2);
	Mat result = 3*image-2*averageimage;
	return result;
}