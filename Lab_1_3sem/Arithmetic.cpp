#include <iostream>
#include "Header.h"
using namespace std;

int64_t mod(int64_t a0, int64_t x0, int64_t p0) {
    int64_t a = a0, x = x0, q = 1, p = p0;
    while (x > 0) {
        if (x % 2 == 0) {
            x /= 2;
            a = (a * a) % p;
        }
        else {
            x--;
            q = (a * q) % p;
        }
    }
    return q;
}