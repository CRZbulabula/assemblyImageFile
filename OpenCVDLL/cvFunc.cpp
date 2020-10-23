#include "cvFunc.h"

void smImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	Mat gray0, gray1;
	//imshow("src", src);
	//ȥɫ
	cvtColor(src, gray0, CV_BGR2GRAY);
	//��ɫ
	addWeighted(gray0, -1, NULL, 0, 255, gray1);
	//��˹ģ��,��˹�˵�Size������Ч���й�
	GaussianBlur(gray1, gray1, Size(11, 11), 0);

	//�ںϣ���ɫ����
	Mat img(gray1.size(), CV_8UC1);
	for (int y = 0; y < heigh; y++)
	{

		uchar* P0 = gray0.ptr<uchar>(y);
		uchar* P1 = gray1.ptr<uchar>(y);
		uchar* P = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			int tmp0 = P0[x];
			int tmp1 = P1[x];
			P[x] = (uchar)min((tmp0 + (tmp0 * tmp1) / (256 - tmp1)), 255);
		}

	}
	imwrite(outputPath, img);
	//imshow("����", img);
	waitKey(0);
}

/*int main()
{
	//string A, B;
	char A[] = "images/WGY.png";
	char B[] = "images/smWGY.png";

	smImage(A, B);
	return 0;
}*/