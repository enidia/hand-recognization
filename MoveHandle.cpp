#include <utility>

#include "MoveHandle.h"

void MoveHandle::addPoint(cv::Point newPoint) {
      this->newPoint = std::move(newPoint);
}

Gesture MoveHandle::analyseLocus() {
    Gesture g = NOOPERATION;

    switch (step)
    {
        case 1:
            if(InRegionCenter(newPoint,winSize)) {
                step++;
                cout << "got" << endl;
            }
            break;
        case 2:
            if(!InRegionCenter(newPoint,winSize)) {
                first = newPoint;
                step++;
                cout << "out" << endl;
            }
            break;
        case 3:
            Step(g);
            break;

        default:break;
    }
    return g;
}

bool MoveHandle::InRegionLeftOfTurn(cv::Point p, cv::Size winSize) {
    return p.x < winSize.width / 3;
}

bool MoveHandle::InRegionRightOfTurn(cv::Point p, cv::Size winSize) {
    return p.x > winSize.width * 2 / 3;
}

bool MoveHandle::InRegionTop(cv::Point p, cv::Size winSize) {
    return p.y < winSize.height / 3;
}

bool MoveHandle::InRegionBottom(cv::Point p, cv::Size winSize) {
    //if( p.y > winSize.height * 2 / 3) cout << "bottom" << endl;
    return p.y > winSize.height * 2 / 3;
}

bool MoveHandle::InRegionCenter(cv::Point p, cv::Size winSize) {
    return p.x > winSize.width / 3 && p.x < winSize.width * 2 / 3 && p.y > winSize.height / 3 && p.y < winSize.height * 2 / 3;
}

void MoveHandle::Step(Gesture &g) {
    if (InRegionRightOfTurn(first, winSize) && InRegionLeftOfTurn(newPoint, winSize)) {
        g = LEFT;
        step = 1;
        cout << "turn right to " << endl;
    }
    else if(InRegionRightOfTurn(first,winSize)&&InRegionTop(newPoint,winSize)){
        g = UP;
        step=1;
        cout<<"turn up to"<<endl;
    }
    else if(InRegionRightOfTurn(first,winSize)&&InRegionBottom(newPoint,winSize)){
        g=DOWN;
        step=1;
        cout<<"turn down to"<<endl;
    }//右边的时候
    else if (InRegionLeftOfTurn(first, winSize) && InRegionRightOfTurn(newPoint, winSize)) {
        g = RIGHT;
        step = 1;
        cout << "turn left to" << endl;
    }
    else if(InRegionLeftOfTurn(first,winSize)&&InRegionTop(newPoint,winSize)){
        g=UP;
        step=1;
        cout<<"turn up to"<<endl;
    }
    else if(InRegionLeftOfTurn(first,winSize)&&InRegionBottom(newPoint,winSize)){
        g=DOWN;
        step=1;
        cout<<"turn down to"<<endl;
    }//左边的时候
    else if (InRegionTop(first, winSize) && InRegionBottom(newPoint, winSize)) {
        g = DOWN;
        step = 1;
        cout << "turn down to" << endl;
    }
    else if(InRegionTop(first,winSize)&&InRegionLeftOfTurn(newPoint,winSize)){
        g=LEFT;
        step=1;
        cout<<"turn left to"<<endl;
    }
    else if(InRegionTop(first,winSize)&&InRegionRightOfTurn(newPoint,winSize)){
        g=RIGHT;
        step=1;
        cout<<"turn right to"<<endl;
    }//上面的时候
    else if (InRegionBottom(first, winSize) && InRegionTop(newPoint, winSize)) {
        g = UP;
        step = 1;
        cout << "turn up to" << endl;
    }
    else if(InRegionBottom(first,winSize)&&InRegionRightOfTurn(newPoint,winSize)){
        g=RIGHT;
        step=1;
        cout<<"turn right to"<<endl;
    }
    else if(InRegionBottom(first,winSize)&&InRegionLeftOfTurn(newPoint,winSize)){
        g=LEFT;
        step=1;
        cout<<"turn left to"<<endl;
    }//在下面的时候
}

void MoveHandle::reset() {
    step = 1;
    cout << "Reset Seccess" <<endl;
}
