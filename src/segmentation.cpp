
#include "segmentation.h"

using namespace std;
using namespace cv;


Mat otsu(Mat &img, int numbits){

	auto otsu_helper = [](Mat& img, vector<int> histogram, int start_intensity, int end_intensity){
		int num_pixels = 0;

		// find total mean
		double total_mean = 0;
		for (int i=start_intensity;i<=end_intensity;i++){
			total_mean += (histogram[i]*i);
			num_pixels += histogram[i];

		}
		total_mean /= num_pixels;

		int max_var_threshold = -1;
		double max_variance_sq = -1;

		for (int k=start_intensity;k<=end_intensity;k++){
			double m0 = 0;         // mean of class 1
			double w0 = 0;         // prob. of pixel being in class 1
			for (int i=start_intensity;i<=k;i++){
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

	Mat res = img.clone();

	vector<int> thresholds(0);
	thresholds.push_back(0);
	thresholds.push_back(255);

	// create histogram (histogram[i] = num of pixels with intensity i)
	vector<int> histogram(256);
	for (int i=0; i<img.rows; i++){
		for (int j=0; j<img.cols; j++){
			histogram[img.at<uchar>(i,j)]++;
		}
	}

	// calculate otsu thresholds
	for (int i=0; i<numbits; i++){
		vector<int>::iterator it = thresholds.begin();
		for (int j=0; j<pow(2.0,(double)i); j++){
			thresholds.insert(it+(j+1), otsu_helper(img, histogram, *(it + j), *(it + j+1)) );
			it++;
		}
	}

	for (int i=0; i<thresholds.size()-1; i++){
		int mean_intensity = 0;
		int temp_num_pixels = 0;
		for (int intensity=thresholds[i]; intensity<((i+1==thresholds.size()-1) ? 256 : thresholds[i+1]) ; intensity++){
			mean_intensity += (intensity * histogram[intensity]);
			temp_num_pixels += histogram[intensity];
		}
		if (temp_num_pixels == 0)
			mean_intensity = 0;
		else
			mean_intensity /= temp_num_pixels;
		for (int row=0;row<img.rows;row++){
			for (int col=0; col<img.cols;col++){
				if ((res.at<uchar>(row,col) >= thresholds[i]) && (res.at<uchar>(row,col) < ((i+1==thresholds.size()-1) ? 256 : thresholds[i+1])))
					res.at<uchar>(row,col) = mean_intensity;
			}
		}
	}

	return res;
}




Mat kmeans(Mat &image, int num_clusters) {
	
	Mat segmentedimage(image.rows , image.cols , DataType<uchar>::type); //segmented image of same size as the image
	Mat clusterimage(image.rows , image.cols , DataType<uchar>::type);

	int *cluster_posx = new int[num_clusters + 1];
	int *cluster_posy = new int[num_clusters + 1];
	double initialvalues[num_clusters + 1];
	
	for(int i = 0 ; i <= num_clusters ; ++i)
		initialvalues[i] = -1;
	
	
	int min = 255 , max = 0;
	
	for(int i = 0 ; i < image.rows ; ++i) {
		for(int j = 0 ; j < image.cols ; ++j){
			if(image.at<uchar>(i,j) <= min)
				min = image.at<uchar>(i,j);
			if(image.at<uchar>(i,j) >= max)
				max = image.at<uchar>(i,j);
		}
	}

	for(int i = 0 ; i < image.rows ; ++i){
		for(int j = 0 ; j < image.cols ; ++j){

			double quotient = (image.at<uchar>(i,j)-min)*(num_clusters-1)/(max-min);
			
			if(abs(quotient-round(quotient)) < initialvalues[int(round(quotient))+1]){
				int t = int(round(quotient));
				while(initialvalues[t] != -1 and t > 0)
					--t;
				if(t > 0){
					initialvalues[t] = initialvalues[int(round(quotient))+1];
					cluster_posx[t] = cluster_posx[int(round(quotient))+1];
					cluster_posy[t] = cluster_posy[int(round(quotient))+1];
				} else {
					t = int(round(quotient));
					while(initialvalues[t] != -1 and t <= num_clusters)
						++t;
					if(t <= num_clusters) {
						initialvalues[t] = initialvalues[int(round(quotient))+1];
						cluster_posx[t] = cluster_posx[int(round(quotient))+1];
						cluster_posy[t] = cluster_posy[int(round(quotient))+1];
					}
				}
				initialvalues[int(round(quotient))+1] = abs(quotient-round(quotient));
				cluster_posx[int(round(quotient))+1] = j;
				cluster_posy[int(round(quotient))+1] = i;
			}
		}
	}

	//select the cluster centers from the image
	for(int i = 1 ; i <= num_clusters ; ++i) {
		if(initialvalues[i] == -1)
		{
			int x = rand() % image.cols;
			int y = rand() % image.rows;
			int j;
			for(j = 1 ; j < i ; ++j){
				if(cluster_posx[j] == x && cluster_posy[j] == y)
					break;
			}
			cluster_posx[i] = x;
			cluster_posy[i] = y;
			if(j != i)
				--i;
		}
	}

	long long prevclusterx[num_clusters + 1];
	long long prevclustery[num_clusters + 1];

	long long accumulatedvalue[num_clusters + 1];
	long long accumulatedx[num_clusters + 1];
	long long accumulatedy[num_clusters + 1];
	long long totalnumber[num_clusters + 1];
	long long counter = 0;

	while(counter < 10) {
		++counter;
		for(int rows = 0 ; rows < image.rows ; ++rows) {
			for(int cols = 0 ; cols < image.cols ; ++cols) {
				clusterimage.at<uchar>(rows , cols) = 0;
			}
		}
		for(int i = 1 ; i <= num_clusters ; ++i) {
			clusterimage.at<uchar>(cluster_posy[i] , cluster_posx[i]) = i;
		}
		for(int rows = 0 ; rows < image.rows ; ++rows) {
			for(int cols = 0 ; cols < image.cols ; ++cols) {
				if(int(clusterimage.at<uchar>(rows , cols)) == 0) {
					long long dist = 47483648;
					for (int i = 1 ; i <= num_clusters ; ++i) {
						long long intensitydifference = abs(image.at<uchar>(rows , cols) - image.at<uchar>(cluster_posx[i] , cluster_posy[i]));
						if ((intensitydifference) < dist) {
							dist = intensitydifference;
							clusterimage.at<uchar>(rows , cols) = i;
						}
					}
				}
			}
		}

		for(int i = 1 ; i <= num_clusters ; ++i) {
			accumulatedvalue[i] = 0;
			accumulatedx[i] = 0;
			accumulatedy[i] = 0;
			totalnumber[i] = 0;
		}
		
		for(int rows = 0 ; rows < image.rows ; ++rows) {
			for(int cols = 0 ; cols < image.cols ; ++cols) {
				accumulatedvalue[clusterimage.at<uchar>(rows , cols)] += image.at<uchar>(rows , cols);
				accumulatedx[clusterimage.at<uchar>(rows , cols)] += cols;
				accumulatedy[clusterimage.at<uchar>(rows , cols)] += rows;
				totalnumber[clusterimage.at<uchar>(rows , cols)] += 1;
			}
		}
		
		bool stopiterations = true;
		for(int i = 1 ; i < num_clusters ; ++i) {
			prevclusterx[i] = cluster_posx[i];
			prevclustery[i] = cluster_posy[i];
			cluster_posx[i] = (accumulatedx[i] / (totalnumber[i] + 1));
			cluster_posy[i] = (accumulatedy[i] / (totalnumber[i] + 1));
			stopiterations = stopiterations && (prevclusterx[i] == cluster_posx[i]) && (prevclustery[i] == cluster_posy[i]);
		}
		if(stopiterations)
			break;
	}

	for(int rows = 0 ; rows < image.rows ; ++rows) {
		for(int cols = 0 ; cols < image.cols ; ++cols) {
			segmentedimage.at<uchar>(rows,cols) = accumulatedvalue[clusterimage.at<uchar>(rows,cols)]/totalnumber[clusterimage.at<uchar>(rows,cols)];
		}
	}
	
	return segmentedimage;
}



