#include"HoughTransform.h"
#include<string>
#include<iostream>

void main()
{
	std::string imgPath;
	std::cout << "Image name: ";
	std::cin >> imgPath;
	
	cv::Mat img = cv::imread(imgPath, -1);
	if (img.empty())
	{
		std::cout << "Image file error!!" << std::endl;
		system("pause");
		return;
	}

	cv::imshow("Original Image", img);
	cv::waitKey(1);

	HoughTrnf hou(img);
}