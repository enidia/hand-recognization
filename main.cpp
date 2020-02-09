#include <iomanip>  // for controlling float print precision
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> // OpenCV window I/O
#include <opencv2/imgproc/imgproc.hpp> // Gaussian Blur
#include "MoveHandle.h"
#include "HotKey.h"

using namespace std;
cv::Mat getContours(const cv::Mat &src, cv::Rect &rect);//图像集

cv::Mat skindetect(cv::Mat &src);


int main(int argc, char *argv[]) {

    cv::VideoCapture captRefrnc(0);
    if (!captRefrnc.isOpened()) {
        return -1;
    }
    const char *WIN_SRC = "Source";
    const char *WIN_RESULT = "Result";

    // Windows
    namedWindow(WIN_SRC, cv::WINDOW_AUTOSIZE);
    //namedWindow(WIN_RESULT, cv::WINDOW_AUTOSIZE);

    cv::Mat frame;                                 // 输入视频帧序列
    cv::Size winSize(640, 480);
    MoveHandle moveHandle(winSize);

    while (true) // 显示窗口
    {
        captRefrnc >> frame;

        if (frame.empty()) {
            cout << " < < <  Game over!  > > > ";
            break;
        }

        // 中值滤波，去除噪声
        medianBlur(frame, frame, 5);

        cv::Mat dst = skindetect(frame);

        cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        erode(dst, dst, element);
        morphologyEx(dst, dst, cv::MORPH_OPEN, element);
        dilate(dst, dst, element);
        morphologyEx(dst, dst, cv::MORPH_CLOSE, element);

        cv::Rect rect;
        dst = getContours(dst, rect);


        cv::Point center(rect.x + rect.width / 2, rect.y + rect.height / 2);
        if (center.x != 0 && center.y != 0) {//判断中心点
           cv::circle(frame,center,4,cv::Scalar(0,0,255),2);
            moveHandle.addPoint(center);
            Gesture g = moveHandle.analyseLocus();
            HotKey ht;
            ht.postKey(g);
        }//画出矩形框
        line(frame, cv::Point(winSize.width/3, winSize.height / 3), cv::Point(winSize.width*2/3, winSize.height / 3), cv::Scalar(255, 255, 255), 3);
        line(frame, cv::Point(winSize.width/3, winSize.height * 2 / 3), cv::Point(winSize.width*2/3, winSize.height * 2 / 3), cv::Scalar(255, 255, 255),
             3);
        line(frame, cv::Point(winSize.width / 3, winSize.height / 3), cv::Point(winSize.width / 3, winSize.height * 2 / 3), cv::Scalar(255, 255, 255), 3);
        line(frame, cv::Point(winSize.width * 2 / 3, winSize.height / 3), cv::Point(winSize.width * 2 / 3, winSize.height * 2 / 3), cv::Scalar(255, 255, 255),
             3);
        imshow(WIN_SRC, frame);

        //imshow(WIN_RESULT, dst);
        dst.release();

        int c = cv::waitKey(1);
        if (c == 27)//esc退出
            break;
        else if(c == 'q') {
            moveHandle.reset();
            c = -1;
        }
    }
}

cv::Mat skindetect(cv::Mat &src) {
    cv::Mat hsv_image;
    int h = 0;
    int s = 1;
    int v = 2;
    cvtColor(src, hsv_image, cv::COLOR_BGR2HSV); //转换成YCrCb空间
    cv::Mat output_mask = cv::Mat::zeros(src.size(), CV_8UC1);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            auto *p_mask = output_mask.ptr<uchar>(i, j);
            auto *p_src = hsv_image.ptr<uchar>(i, j);
            if (p_src[h] >= 0 && p_src[h] <= 20 && p_src[s] >= 48 && p_src[v] >= 50) {
                p_mask[0] = 255;//获得白色的肉色转换
            }
        }
    }
    cv::Mat detect;
    src.copyTo(detect, output_mask);
    return output_mask;
}

cv::Mat getContours(const cv::Mat &src, cv::Rect &rect) {
    if (src.channels() != 1)
        throw "FindTargets : 通道数必须为 1";
    cv::Mat dst = src.clone();
    vector<vector<cv::Point>> contours;       // 轮廓
    contours.clear();

    // 得到手的轮廓
    findContours(src, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 去除伪轮廓
    double MaxArea = 0;
    vector<vector<cv::Point>> MaxContour;
    MaxContour.clear();
    for (const auto &contour : contours) {
        double area = contourArea(contour);
        //判断手进入区域的阈值
        if (area > 25000 && area > MaxArea) {
            MaxArea = area;
            MaxContour.push_back(contour);
        }
    }
    // 画轮廓
    if (MaxContour.empty()) {
        return dst;
    } else {
        drawContours(dst, MaxContour, -1, cv::Scalar(0, 0, 255), 3);
        rect = boundingRect(MaxContour.back());
        rectangle(dst, rect, cv::Scalar(255, 255, 255), 3);
    }
    return dst;
}
