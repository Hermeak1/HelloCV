#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;

void affine_transform();

int main()
{
	affine_transform();
	return 0;
}

void affine_transform()
{
	Mat src = imread("tekapo.bmp");
	Mat src2 = imread("tekapo.bmp");
	Mat src3 = imread("tekapo.bmp");

	if(src.empty())
	{
		cerr << "Image load failed!" << endl;
		return;
	}

	double mx = 0.3;
	Point2f srcPts[3], dstPts[3];

	srcPts[0] = Point2f(0, 0); // 원본파일의 좌표
	srcPts[1] = Point2f(src.cols - 1, 0); // 영상의 폭길이는 0 우측 상단점
	srcPts[2] = Point2f(src.cols - 1, src.rows -1); // 영상의 하단 우측 꼭지점

	dstPts[0] = Point2f(50,50); // 첫번째 좌표
	dstPts[1] = Point2f(src.cols - 100, 100); // 두번째 좌표
	dstPts[2] = Point2f(src.cols - 50, src.rows - 50); // 세번째 좌표

	Mat M = getAffineTransform(srcPts, dstPts);
	Mat M2 = Mat_<double>({ 2,3 }, { 1,0,150,0,1,100 });
	Mat M3 = Mat_<double>({ 2,3 }, { 1,mx,0,0,1,0 });

	Mat dst; // affine
	Mat dst2;//move
	Mat dst3; // shear
	warpAffine(src, dst, M, Size()); // 원본파일만큼 dst 파일, m 사이즈를 동일시 한다.
	warpAffine(src2, dst2, M2, Size()); // 원본파일만큼 dst 파일, m 사이즈를 동일시 한다.
	warpAffine(src3, dst3, M3, Size(cvRound(src.cols + src.rows * mx),src.rows)); // 원본파일만큼 dst 파일, m 사이즈를 동일시 한다.

	imshow("src", src);
	imshow("src2", src2);
	imshow("src3", src3);

	imshow("dst", dst);
	imshow("dst2", dst2);
	imshow("dst3", dst3);

	waitKey();
	destroyAllWindows();

}

#pragma region precode
/*void filter_embossing();
void filter_blurring();
void gausian_filter();
void sharpen_mask();
void filter_median();

int main()
{
	filter_embossing();
	//filter_blurring();
	//gausian_filter();
	//sharpen_mask();
	//filter_median();
	return 0;
}

void filter_blurring()
{
	Mat src = imread("rose.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	imshow("src", src);

	Mat dst;
	for (int ksize = 3; ksize <= 7; ksize += 2)
	{
		blur(src, dst, Size(ksize, ksize));

		String desc = format("Mean : %dx%d", ksize, ksize);
		putText(dst, desc, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
		imshow("dst", dst);
		waitKey(0);
	}

	destroyAllWindows();
}

// 엠보 효과 embo_filter
void filter_embossing()
{
	Mat src = imread("rose.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	float data[] = { 0.1,0.1,0,0.1,0,0.1,0,0.1,0.1 };
	Mat emboss(3, 3, CV_32FC1, data);

	Mat dst;
	filter2D(src, dst, -1, emboss, Point(-1, -1), 0);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	destroyAllWindows();
}

void gausian_filter()
{
	Mat src = imread("rose.bmp", IMREAD_GRAYSCALE);
	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	imshow("src", src);
	Mat dst;
	for (int sigma = 1; sigma <= 5; sigma++)
	{
		GaussianBlur(src, dst, Size(), (double)sigma);

		String text = format("sigma = %d", sigma);
		putText(dst, text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);

		imshow("dst", dst);
		waitKey(0);
	}
	destroyAllWindows();
}

void sharpen_mask()
{
	Mat src = imread("rose.bmp", IMREAD_GRAYSCALE);

	if (src.empty())
	{
		cerr << "Image load failed!" << endl;
		return;
	}
	imshow("src", src);

	for (int sigma = 1; sigma <= 5; sigma++)
	{
		Mat blurred;
		GaussianBlur(src, blurred, Size(), sigma);

		float alpha = 1.f;
		Mat dst = (1 + alpha) * src - alpha * blurred;

		String desc = format("sigma: %d", sigma);
		putText(dst, desc, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);

		imshow("dst", dst);
		waitKey();

	}
	destroyAllWindows();
}

void filter_median()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty())
	{
		cerr << "Image load failed!" << endl;
		return;
	}

	int num = (int)(src.total() * 0.1);
	for (int i = 0; i < num; i++)
	{
		int x = rand() % src.cols;
		int y = rand() % src.rows;
		src.at<uchar>(y, x) = (i % 2) * 255;

	}

	Mat dst1;
	GaussianBlur(src, dst1, Size(), 1);

	Mat dst2;
	medianBlur(src, dst2, 3);

	imshow("src", src);
	imshow("dst1", dst1);

	imshow("dst2", dst2);

	waitKey();
	destroyAllWindows();
}

void filter_bilateral()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty())
	{
		cerr << "Image load failed!" << endl;
		return;
	}
	Mat noise(src.size(), CV_32SC1);
	randn(noise, 0, 5);
	add(src, noise, src, Mat(), CV_8U);

	Mat dst1;
	GaussianBlur(src, dst1, Size(), 5);

	Mat dst2;
	bilateralFilter(src, dst2, -1, 10, 5);

	imshow("src", src);
	imshow("dst1", dst1);
	imshow("dst2", dst2);

	waitKey();
	destroyAllWindows();
}*/
#pragma endregion

