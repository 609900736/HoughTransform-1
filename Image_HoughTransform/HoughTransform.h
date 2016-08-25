#ifndef _HOUGHTRANSFORMH
#define _HOUGHTRANSFORMH

#include"SobelFilter.h"

using::cv::Mat;
using::std::vector;

class HoughTrnf
{
	private:
	
		Sobel sob;
		int thetaSize;
		float phiSize, pi;
		vector<float> cosTab, sinTab;
		vector<vector<int>> houMtx;

	public:

		HoughTrnf(Mat originalImg);
		~HoughTrnf();
		void TransForm(vector<vector<int>> binaryImgMtx);
		void DrawHoughValueImage();
		void ShowLongestLine(Mat originalImg, vector<vector<int>> binaryImgMtx);

};






HoughTrnf::HoughTrnf(Mat img)
{
	Mat grayImg;
	cv::cvtColor(img, grayImg, CV_BGR2GRAY);

	phiSize = sqrt(pow((float)img.rows, 2) + pow((float)img.cols, 2));
	thetaSize = 180;
	pi = 3.14159265358979;


	//***************************                         
	//     計算每個theta的弧度                               
	//***************************
	float theta;
	cosTab.resize(180);
	sinTab.resize(180);
	for (int i = 0; i < thetaSize; i++)
	{
		theta = i * pi / 180.;
		cosTab[i] = cos(theta);
		sinTab[i] = sin(theta);
	}

	vector<int> _hou(thetaSize, 0);
	houMtx.resize(int(2 * phiSize), _hou);

	vector<vector<int>> binaryImgMtx;
	binaryImgMtx = sob.SobelFilter(grayImg);

	TransForm(binaryImgMtx);
	DrawHoughValueImage();
	ShowLongestLine(img, binaryImgMtx);

	vector<vector<int>>(binaryImgMtx).swap(binaryImgMtx);
}

HoughTrnf::~HoughTrnf()
{
	vector<vector<int>>(houMtx).swap(houMtx);
	vector<float>(sinTab).swap(sinTab);
	vector<float>(cosTab).swap(cosTab);
}

void HoughTrnf::TransForm(vector<vector<int>> imgMtx)
{
	float phi;

	for (int i = 0; i < imgMtx.size(); i++)
	{
 		for (int j = 0; j < imgMtx[i].size(); j++)
		{
 			if (imgMtx[i][j] == FEATURE_POINT)
			{
				for (int nTheta = 0; nTheta < thetaSize; nTheta++)
				{
					phi = i * sinTab[nTheta] + j * cosTab[nTheta]; 
					houMtx[int(phi + phiSize)][nTheta]++;
				}
			}
		}
	}
}

void HoughTrnf::DrawHoughValueImage()
{
	Mat houImg = Mat(int(2 * phiSize), thetaSize, CV_8UC1, cv::Scalar(0));

	for (int i = 0; i < houImg.rows; i++)
	{
		for (int j = 0; j < houImg.cols; j++)
		{
			if (houMtx[i][j] > 0)
			{
				houImg.at<uchar>(i, j) = 255;
			}
		}
	}

 	cv::imshow("Hough Transform Value", houImg);
	cv::waitKey(1);
}

void HoughTrnf::ShowLongestLine(Mat img, vector<vector<int>> imgMtx)
{
	//***********************************************
	//     尋找Hough Matrix 的 Maximums Value
	//***********************************************
	vector<int> buf(3, 0);
	for (int i = 0; i < (int)phiSize; i++)
	{
		for (int j = 0; j < thetaSize; j++)
		{
			if (houMtx[i][j] > buf[2]) 
				buf[0] = i, buf[1] = j, buf[2] = houMtx[i][j];
		}
	}


	//**********************************
	//     輸出 Longest Line Image
	//**********************************
	float num;
	Mat newImg = Mat(img.rows, img.cols, CV_8UC1, cv::Scalar(255));
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (imgMtx[i][j] == FEATURE_POINT)
			{
				num = i * sinTab[buf[1]] + j * cosTab[buf[1]];
				if (int(num + phiSize) == buf[0])
				{
					newImg.at<uchar>(i, j) = 0;
				}
			}
		}
	}

	cv::imshow("Longest Line Image", newImg);
	cv::waitKey(0);
}

#endif