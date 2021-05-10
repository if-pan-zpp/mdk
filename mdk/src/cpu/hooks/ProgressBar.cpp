#include "cpu/hooks/ProgressBar.hpp"
#include "cpu/simul/Simulation.hpp"
#include <cmath>
#include <iostream>
using namespace mdk;
using namespace std;

void ProgressBar::execute(Simulation &system) {
    double progress = system.state.t / totalTime;

    cout << "[";
    int pos = floor(width * progress);
    for (int i = 0; i < width; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    std::cout << "] " << system.state.t << " / " << totalTime << "\r";
    cout.flush();
}