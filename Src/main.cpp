#include <chrono>
#include <thread>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "KeyboardManager.hpp"
#include "GameManager.hpp"

using namespace std;
using namespace std::chrono;

class StopWatch {
public:
    void start() noexcept { mBeginTime = steady_clock::now(); }
    void stop() noexcept { mEndTime = steady_clock::now(); }
    void restart() noexcept { start(); }
    steady_clock::duration elapsed() const noexcept { return mEndTime - mBeginTime; }
    int elapsed_ms() const noexcept { return static_cast<int>(duration_cast<milliseconds>(elapsed()).count()); }
    int elapsed_s() const noexcept { return static_cast<int>(duration_cast<seconds>(elapsed()).count()); }
private:
    steady_clock::time_point mBeginTime, mEndTime;
};

void pauseUntilPress(const char* info, char key) {
    std::cout << info << std::endl;
    while (true) {
        if (isKeyDown(key))return;
        std::this_thread::sleep_for(10ms);
    }
}

int main() {
    try {
        KeyboardManager::init();
        auto game = std::make_shared<GameManager>();
        bool quit = false;
        std::cout << "准备完成" << std::endl;
        pauseUntilPress("请将焦点放在风神录窗口上，开始游戏，然后按C开启AI", 'C');
        std::cout << "已开始游戏，按Q键退出" << std::endl;
        unsigned long long frameCount = 0;
        StopWatch watch;
        while (!quit) {
            watch.start();
            frameCount++;
            game->update(frameCount);
            watch.stop();
            if (isKeyDown('Q'))
                quit = true;
			cout << watch.elapsed_ms()/16.0*100<<"%"<< endl;
            std::this_thread::sleep_for(milliseconds(std::max(0, 16 - watch.elapsed_ms())));
        }
        system("pause");
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        system("pause");
    }
}
