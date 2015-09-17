#include <iostream>
#include "dithering.h"
#include "edge_detection.h"
#include "enhance.h"
#include "segmentation.h"
#include "pattern_mapping.h"

using namespace std;
using namespace cv;

void help()	{
	cout << "1 : Quantization with Median Cut\n";
	cout << "2 : Quantization with Uniform Distribution\n";
	cout << "3 : Quantization with Median Cut with Floyd-Steinberg Dithering\n";
	cout << "4 : Quantization with Uniform Distribution with Floyd-Steinberg Dithering\n";
	cout << "5 : Image Enhancement\n";
	cout << "6 : Kmeans Segmentation\n";
	cout << "7 : Otsu Segmentation\n";
	cout << "8 : Edge Detection\n";
	cout << "9 : Pattern Mapping\n";
	cout << "0 : Quit Program\n\n";
}


int main(int argc , char **argv)
{
	srand(time(0));
	cout << "\n\n";
	int method;
	Mat img,res;
	while(true){
		cout << "Image FileName : ";
		string imagename;
		cin >> imagename;
		
		img = imread(imagename,0);
		if (img.empty()){
			cout << "Unable to read the image. Exitting Program...\n\n";
			exit(0);
		}

		help();
		cout << "Method : ";
		cin >> method;
		string filename = "";
		switch(method){
			case(1):
				filename += "quantize_median";
				cout << "Number of colors in quatized image : ";
				int num_colors_qm;
				cin >> num_colors_qm;

				res = quantize(img , num_colors_qm , MEDIAN_CUT);
				break;
			case(2):
				filename += "quantize_uniform";
				cout << "Number of colors in quatized image : ";
				int num_colors_qu;
				cin >> num_colors_qu;

				res = quantize(img , num_colors_qu , UNIFORM_DIST);
				break;
			case(3):
				filename += "floyd_median";
				cout << "Number of colors in quatized image : ";
				int num_colors_fm;
				cin >> num_colors_fm;

				res = floyd(img , num_colors_fm , MEDIAN_CUT);
				break;
			case(4):
				filename += "floyd_uniform";
				cout << "Number of colors in quatized image : ";
				int num_colors_fu;
				cin >> num_colors_fu;

				res = floyd(img , num_colors_fu , UNIFORM_DIST);
				break;
			case(5):
				cout << "	Choose Enhancement : \n\n";
				cout << "	a : Gaussian Blur\n";
				cout << "	b : Negative Image\n";
				cout << "	c : Contrast Streching\n";
				cout << "	d : Log of Intensity\n";
				cout << "	e : Inverse Log of Intensity\n";
				cout << "	f : Emboss Image\n";
				cout << "	g : Silhoutte Enhancement\n";
				cout << "	Method : ";
				char enhancement;
				cin >> enhancement;
				switch(enhancement){
					case('a'):
						filename += "gaussian";
						cout << "	Kernel Size : ";
						int k;
						cin >> k;

						cout << "	Standard Deviation : ";
						double sigma;
						cin >> sigma;

						res = Gaussian(img , k , sigma);
						break;
					case('b'):
						filename += "negative";
						res = enhance(img , NEGATIVE);
						break;
					case('c'):
						filename += "contrast_streching";
						res = enhance(img , CONTRAST_STRECHING);
						break;
					case('d'):
						filename += "log_intensity";
						res = enhance(img , LOG_OF_INTENSITY);
						break;
					case('e'):
						filename += "inverse_log_intensity";
						res = enhance(img , INVERSE_LOG_OF_INTENSITY);
						break;
					case('f'):
						filename += "embossing";
						res = emboss(img);
						break;
					case('g'):
						filename += "silhoutte";
						res = silhoutte(img);
						break;
					default:
						cout << "	Please choose correct input method\n\n";
						break;
				}
				break;
			case(6):
				filename += "kmeans";
				cout << "Number of clusters in Segmented Image : ";
				int num_clusters;
				cin >> num_clusters;

				res = kmeans(img,num_clusters);
				break;
			case(7):
				filename += "otsu";
				cout << "Number of bits for clusters in Segmented Image (3 bits = 8 clusters) : ";
				int num_bits;
				cin >> num_bits;

				res = otsu(img,num_bits);
				break;
			case(8):
				filename += "edges";
				res = extractEdges(img);
				break;
			case(9):
				filename += "pattern";
				res = patternMapping(img);
				break;
			case(0):
				cout << "Quitting Program...\n\n";
				exit(0);
				break;
			default:
				cout << "Please choose correct input method\n\n";
				exit(0);
				break;
		}
		
		imwrite("result/"+filename+".png",res);
		cout << "Done!!!\nCheck the result folder...\n\n";
	}
}

