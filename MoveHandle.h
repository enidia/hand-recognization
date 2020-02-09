#include <utility>

#ifndef OPENCV_MOVEHANDLE_H
#define OPENCV_MOVEHANDLE_H

#include "opencv2/opencv.hpp"

using namespace std;

enum Gesture {
    NOOPERATION, UP, DOWN, LEFT, RIGHT
};

class MoveHandle {
    vector<cv::Point> centers;
    cv::Point newPoint;
    cv::Point first;

    cv::Size winSize;
    int step = 1;

    bool InRegionLeftOfTurn(cv::Point p, cv::Size winSize);

    bool InRegionRightOfTurn(cv::Point p, cv::Size winSize);

    bool InRegionTop(cv::Point p, cv::Size winSize);

    bool InRegionBottom(cv::Point p, cv::Size winSize);

    bool InRegionCenter(cv::Point p, cv::Size winSize);

    void Step(Gesture &g);

public:
    MoveHandle() = default;

    explicit MoveHandle(cv::Size winS) : winSize(std::move(winS)) {}

    void addPoint(cv::Point newPoint);

    Gesture analyseLocus();

    void reset();
};


#endif //OPENCV_MOVEHANDLE_H
