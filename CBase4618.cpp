#include "CBase4618.h"


void CBase4618::run() {
    while(_key != 'q') {
        auto end_time = std::chrono::system_clock::now() + std::chrono::milliseconds(16);
        update();
        draw();
        std::this_thread::sleep_until(end_time);
        _key = cv::waitKey(1);
    }
}
