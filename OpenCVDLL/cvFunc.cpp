#include "cvFunc.h"

//ĥƤ
void mopiImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);

	Mat dst;
	int value1 = 3, value2 = 1;// ĥƤ�̶���ϸ�ڳ̶ȵ�ȷ��
	int dx = value1 * 5; // ˫���˲�����֮һ
	double fc = value1 * 12.5; // ˫���˲�����֮һ
	double p = 0.1f; // ͸����
	Mat temp1;

	// ˫���˲�
	bilateralFilter(src, temp1, dx, fc, fc);


	Mat temp22;
	//temp2 = (temp1 - src + 128);
	subtract(temp1, src, temp22);


	// Core.subtract(temp22, new Scalar(128), temp2);
	//Mat temp222(temp22.rows, temp22.cols, temp22.channels(), Scalar(10, 10, 10, 128));
	//imwrite(outputPath, temp222);

	Mat temp2;
	add(temp22, (128, 128, 128, 128), temp2);


	// ��˹ģ��
	Mat temp3;
	GaussianBlur(temp2, temp3, Size(2 * value2 - 1, 2 * value2 - 1), 0, 0);


	// temp4 = image + 2 * temp3 - 255;
	Mat temp44;
	Mat temp4;
	temp3.convertTo(temp44, temp3.type(), 2, -255);


	add(src, temp44, temp4);

	// dst = (image*(100 - p) + temp4*p) / 100;
	addWeighted(src, p, temp4, 1 - p, 0.0, dst);

	//Mat temp5(temp4.cols, temp4.rows, temp4.type(), Scalar(10, 10, 10));
	add(dst, (10, 10, 10), dst);

	imwrite(outputPath, dst);

	waitKey(0);

}

//��10
void yuhuaImage(char* inputPath, char* outputPath)
{
	float mSize = 0.7;
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	int centerX = width >> 1;
	int centerY = heigh >> 1;

	int maxV = centerX * centerX + centerY * centerY;
	int minV = (int)(maxV * (1 - mSize));
	int diff = maxV - minV;
	float ratio = width > heigh ? (float)heigh / (float)width : (float)width / (float)heigh;

	Mat img;
	src.copyTo(img);

	Scalar avg = mean(src);
	Mat dst(img.size(), CV_8UC3);
	Mat mask1u[3];
	float tmp, r;
	for (int y = 0; y < heigh; y++)
	{
		uchar* imgP = img.ptr<uchar>(y);
		uchar* dstP = dst.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			int b = imgP[3 * x];
			int g = imgP[3 * x + 1];
			int r = imgP[3 * x + 2];

			float dx = centerX - x;
			float dy = centerY - y;

			if (width > heigh)
				dx = (dx * ratio);
			else
				dy = (dy * ratio);

			int dstSq = dx * dx + dy * dy;

			float v = ((float)dstSq / diff) * 255;

			r = (int)(r + v);
			g = (int)(g + v);
			b = (int)(b + v);
			r = (r > 255 ? 255 : (r < 0 ? 0 : r));
			g = (g > 255 ? 255 : (g < 0 ? 0 : g));
			b = (b > 255 ? 255 : (b < 0 ? 0 : b));

			dstP[3 * x] = (uchar)b;
			dstP[3 * x + 1] = (uchar)g;
			dstP[3 * x + 2] = (uchar)r;
		}
	}
	imwrite(outputPath, dst);

	waitKey(0);
}

//�λ�9
void menghuanImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float newR = 0.8 * R + 0.3 * G + 0.1 * B + 46.5;
			float newG = 0.1 * R + 0.9 * G + 0.0 * B + 46.5;
			float newB = 0.1 * R + 0.3 * G + 0.7 * B + 46.5;
			if (newB < 0)newB = 0;
			if (newB > 255)newB = 255;
			if (newG < 0)newG = 0;
			if (newG > 255)newG = 255;
			if (newR < 0)newR = 0;
			if (newR > 255)newR = 255;
			P1[3 * x] = (uchar)newB;
			P1[3 * x + 1] = (uchar)newG;
			P1[3 * x + 2] = (uchar)newR;
		}
	}
	//imshow("����ɫ", img);
	imwrite(outputPath, img);
	waitKey(0);
}

//����8
void geteImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float newR = 1.9 * R + (-0.3) * G + (-0.2) * B - 87.0;
			float newG = (-0.2) * R + 1.7 * G + (-0.1) * B - 87.0;
			float newB = (-0.1) * R + (-0.6) * G + 2.0 * B - 87.0;
			if (newB < 0)newB = 0;
			if (newB > 255)newB = 255;
			if (newG < 0)newG = 0;
			if (newG > 255)newG = 255;
			if (newR < 0)newR = 0;
			if (newR > 255)newR = 255;
			P1[3 * x] = (uchar)newB;
			P1[3 * x + 1] = (uchar)newG;
			P1[3 * x + 2] = (uchar)newR;
		}
	}
	//imshow("����ɫ", img);
	imwrite(outputPath, img);
	waitKey(0);
}

//����7
void danyaImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float newR = 0.6 * R + 0.3 * G + 0.1 * B - 73.3;
			float newG = 0.2 * R + 0.7 * G + 0.1 * B - 73.3;
			float newB = 0.2 * R + 0.3 * G + 0.4 * B - 73.3;
			if (newB < 0)newB = 0;
			if (newB > 255)newB = 255;
			if (newG < 0)newG = 0;
			if (newG > 255)newG = 255;
			if (newR < 0)newR = 0;
			if (newR > 255)newR = 255;
			P1[3 * x] = (uchar)newB;
			P1[3 * x + 1] = (uchar)newG;
			P1[3 * x + 2] = (uchar)newR;
		}
	}
	//imshow("����ɫ", img);
	imwrite(outputPath, img);
	waitKey(0);
}

//�ֶ�6
void heduImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float newB = R * 0.393 + G * 0.769 + B * 0.189;
			float newG = R * 0.349 + G * 0.686 + B * 0.168;
			float newR = R * 0.272 + G * 0.534 + B * 0.131;
			if (newB < 0)newB = 0;
			if (newB > 255)newB = 255;
			if (newG < 0)newG = 0;
			if (newG > 255)newG = 255;
			if (newR < 0)newR = 0;
			if (newR > 255)newR = 255;
			P1[3 * x] = (uchar)newB;
			P1[3 * x + 1] = (uchar)newG;
			P1[3 * x + 2] = (uchar)newR;
		}
	}
	//imshow("����ɫ", img);
	imwrite(outputPath, img);
	waitKey(0);
}

//�Ҷ�5
void huiduImage(char* inputPath, char* outputPath)   //self
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float avg = 0.3 * R + 0.59 * G + 0.11 * B;
			if (avg > 255) avg = 255;
			else if (avg < 0) avg = 0;
			P1[3 * x] = avg;
			P1[3 * x + 1] = avg;
			P1[3 * x + 2] = avg;
		}

	}
	//imshow("�ڰ�", img);
	imwrite(outputPath, img);
	waitKey(0);
}

//����4
void huaijiuImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float newB = 0.272 * R + 0.534 * G + 0.131 * B;
			float newG = 0.349 * R + 0.686 * G + 0.168 * B;
			float newR = 0.393 * R + 0.769 * G + 0.189 * B;
			if (newB < 0)newB = 0;
			if (newB > 255)newB = 255;
			if (newG < 0)newG = 0;
			if (newG > 255)newG = 255;
			if (newR < 0)newR = 0;
			if (newR > 255)newR = 255;
			P1[3 * x] = (uchar)newB;
			P1[3 * x + 1] = (uchar)newG;
			P1[3 * x + 2] = (uchar)newR;
		}
	}
	//imshow("����ɫ", img);
	imwrite(outputPath, img);
	waitKey(0);
}

//ë����3
void maoboliImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 1; y < heigh - 1; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 1; x < width - 1; x++)
		{
			int tmp = rng.uniform(0, 9);
			P1[3 * x] = src.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3));
			P1[3 * x + 1] = src.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3) + 1);
			P1[3 * x + 2] = src.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3) + 2);
		}

	}
	//imshow("��ɢ", img);
	imwrite(outputPath, img);
	waitKey(0);
}

//����2
void fudiaoImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	Mat img(src.size(), CV_8UC3);
	
	for (int y = 1; y < src.rows - 1; y++)
	{
		uchar* p0 = src.ptr<uchar>(y);
		uchar* p1 = src.ptr<uchar>(y + 1);

		uchar* q0 = img.ptr<uchar>(y);
		for (int x = 1; x < src.cols - 1; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				int tmp0 = p1[3 * (x + 1) + i] - p0[3 * (x - 1) + i] + 128;//����
				if (tmp0 < 0)
					q0[3 * x + i] = 0;
				else if (tmp0 > 255)
					q0[3 * x + i] = 255;
				else
					q0[3 * x + i] = tmp0;
			}
		}
	}
	imwrite(outputPath, img);
	//imshow("src", src);
	//imshow("����", img);
	waitKey(0);

}

//����1
void sumiaoImage(char* inputPath, char* outputPath)
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
	string winName = "camera";
	srand(time(0));
	winName += to_string(rand());

	//namedWindow("empty");
	namedWindow(winName, CV_WINDOW_NORMAL);
	moveWindow(winName, 300, 400);

	while (true)
	{
		Mat frame;
		capture >> frame;
		if (filterType == 0)
		{
			imshow(winName, frame);
		}
		
		//moveWindow(winName, 300, 400);
		//imwrite("images/Video.png", frame);
		waitKey(30);
		frame.release();
		//remove("images/Video.png");
	}
}

void releaseCamera()
{
	VideoCapture capture(0);
	capture.release();
	destroyAllWindows();
}

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

void saveImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	imwrite(outputPath, src);
}

/*int main()
{
	
	return 0;
}*/