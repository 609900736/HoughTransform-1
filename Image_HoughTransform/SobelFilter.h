//================================================
//    此為 Sobel Filter Algothrim                
//    使用 (3*3) 的Mask來做運算                 　　 
//    Input為 Mat 的 Gray Image point           　
//    Output為 vector<vector<int>> 的二維矩陣     　
//================================================


#ifndef _SOBELFILTERH
#define _SOBELFILTERH

#include<highgui.h>
#include<cv.h>
#include<vector>
#include<fstream>

using std::vector;

const int FEATURE_POINT = 127, BACKGROUND = 255, miniSup = 75;

class Sobel
{
	public:

		vector<vector<int>> SobelFilter(cv::Mat img);

};





vector<vector<int>> Sobel::SobelFilter(cv::Mat img)
{
	int TConvoMask[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
	int PConvoMask[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

	vector<vector<int>> TConvoV(img.rows, vector<int>(img.cols, 0));
	vector<vector<int>> PConvoV(img.rows, vector<int>(img.cols, 0));

	int TNum, PNum, index;

	for (int i = 1; i < img.rows - 1; i++)
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			TNum = 0, PNum = 0, index = 0;
			for (int x = i - 1; x < i + 2; x++)
			{
				for (int y = j - 1; y < j + 2; y++)
				{
					TNum += img.at<uchar>(x, y) * TConvoMask[index];
					PNum += img.at<uchar>(x, y) * PConvoMask[index];
					index++;
				}
			}
			TConvoV[i][j] = TNum;
			PConvoV[i][j] = PNum;
		}
	}

	for (int i = 1; i < img.rows; i++)
	{
		for (int j = 1; j < img.cols; j++)
		{
			if (sqrt(pow(TConvoV[i][j], 2) + pow(PConvoV[i][j], 2)) > miniSup) TConvoV[i][j] = FEATURE_POINT;
			else TConvoV[i][j] = BACKGROUND;
		}
	}

	cv::Mat sobImg = cv::Mat(img.rows, img.cols, CV_8UC1, cv::Scalar(0));
	for (int i = 1; i < img.rows; i++)
	{
		for (int j = 1; j < img.cols; j++)
		{
			if (TConvoV[i][j] == FEATURE_POINT)
			{
				sobImg.at<uchar>(i, j) = FEATURE_POINT;
			}
		}
	}

	cv::imshow("Sobel Fliter Image", sobImg);
	cv::waitKey(1);

	vector<vector<int>>(PConvoV).swap(PConvoV);

	return TConvoV;
}

#endif