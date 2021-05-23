#include "cpu/hooks/ProgressBar.hpp"
#include "cpu/simul/Simulation.hpp"
#include <cmath>
#include <iostream>
using namespace mdk;
using namespace std;
using namespace std::chrono;

ProgressBar::ProgressBar(double totalTime, int width) {
    this->totalTime = totalTime;
    this->width = width;
    realTime0 = high_resolution_clock::now();
}

void ProgressBar::execute(Simulation &system) {
    double progress = system.state.t / totalTime;

    auto now = high_resolution_clock::now();
    auto diff = duration_cast<microseconds>(now - realTime0).count();

    cout << "\r" << "[";
    int pos = floor(width * progress);
    for (int i = 0; i < width; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << system.state.t << " / " << totalTime;
    cout << " V = " << system.dyn.V;
    cout << " t = " << diff/1000 << " ms";

    cout.flush();
}