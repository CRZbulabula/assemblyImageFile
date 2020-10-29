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

void openCamera(int filterType)
{
	VideoCapture capture(0);

	//namedWindow("empty", CV_WINDOW_NORMAL);
	//resizeWindow("empty", 10, 10);
	//moveWindow("empty", 300, 40);
	namedWindow("camera", CV_WINDOW_NORMAL);
	moveWindow("camera", 300, 40);

	while (true)
	{
		Mat frame;
		capture >> frame;
<<<<<<< Updated upstream
		imshow("camera", frame);
		moveWindow("camera", 300, 400);
		imwrite("images/Video.png", frame);
		waitKey(300);	//��ʱ30
=======
		if (filterType == 0)
		{
			imshow(winName, frame);
		}
		//moveWindow(winName, 300, 400);
		//imwrite("images/Video.png", frame);
		waitKey(30);
		frame.release();
>>>>>>> Stashed changes
		//remove("images/Video.png");
	}
}

<<<<<<< Updated upstream
void captureFrame()
=======
void captureFrame(int filterType)
{
	VideoCapture capture(0);
	Mat frame;
	capture >> frame;
	if (filterType == 0)
	{
		imwrite("images/tmp.png", frame);
	}
}

void releaseCamera()
>>>>>>> Stashed changes
{
	VideoCapture capture(0);
	capture.release();
	destroyAllWindows();
}

/*int main()
{

	return 0;
}*/