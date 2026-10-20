#include <cassert>
#include <iostream>
#include "BinaryTracker.h"

int main() {
    BestFitRequestHandler bestFit;
    BinaryTracker tracker(100, &bestFit);

    const int a = tracker.request(10);
    const int b = tracker.request(30);
    const int c = tracker.request(50);   // leaves only [90, 100) free
    assert(a == 0 && b == 10 && c == 40);

    tracker.giveBack(b, 30);
    tracker.giveBack(a, 10);   // merges with the freed block after it -> [0, 40) free
    assert(tracker.request(40) == 0);

    std::cout << "a=" << a << " b=" << b << " c=" << c << '\n';
}