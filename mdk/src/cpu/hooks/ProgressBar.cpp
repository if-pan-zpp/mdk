#include "cpu/hooks/ProgressBar.hpp"
#include "cpu/sys/System.hpp"
#include <cmath>
#include <iostream>
using namespace mdk;
using namespace std;

void ProgressBar::execute(System &system) {
    double progress = system.state.t / totalTime;
    int curPct = floor(100.0 * progress);

    if (curPct != pct) {
        cout << "[";
        int pos = floor(width * progress);
        for (int i = 0; i < width; ++i) {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";
        }
        std::cout << "] " << curPct << " %\r";
        cout.flush();

        pct = curPct;
    }
}