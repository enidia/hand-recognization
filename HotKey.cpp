#include <iostream>
#include "HotKey.h"


void HotKey::postKey(Gesture g) {
    switch (g) {
        case UP://初始化，往下移，获取状态，然后向上移动，触发UP
                for(int i=0;i<3;i++) {
                                      mouse_event(MOUSEEVENTF_WHEEL, 0, 0, 120, 0);
                }
                                         std::cout << "UP" << std::endl;
            break;
        case DOWN://中心初始化，往上移动，获取上方区域的状态，然后向下移动出，触发；
                 for(int i=0;i<3;i++) {
                                          mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -120, 0);
                 }
                                        std::cout << "DOWN" << std::endl;
            break;
        case LEFT://中心初始化,往右移动，获取状态，向左效应，由于电脑摄像头调反，实际向左,在判断Move时调回来
                                          keybd_event(VK_CONTROL,0,0,0);
                                          keybd_event(VK_F4,0,0,0);
                                          keybd_event(VK_CONTROL,0, KEYEVENTF_KEYUP,0);
                                          keybd_event(VK_F4,0, KEYEVENTF_KEYUP,0);
                                          std::cout << "TO LEFT" << std::endl;
            break;
        case RIGHT:
                                         keybd_event(VK_CONTROL,0,0,0);
                                         keybd_event(78,0,0,0);
                                         keybd_event(VK_CONTROL,0, KEYEVENTF_KEYUP,0);
                                         keybd_event(78,0, KEYEVENTF_KEYUP,0);
                                         std::cout << "TO RIGHT" << std::endl;
            break;
        case NOOPERATION:break;
    }
}
