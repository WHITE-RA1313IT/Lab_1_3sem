#include "Math.h"

int64_t generator(int64_t num) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<> dist(400, num);
    int64_t x = dist(gen);

    return x;
}

bool checkPrime(int64_t num) {
    for (int i = 2; i < num; i++) {
        if (num % i == 0)
        {
            return false;
            break;
        }
    }
    return true;
}

int64_t generatorPrime(int64_t num) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<> dist(400, num);

    int64_t x = dist(gen);
    while (!checkPrime(x)) {
        x++;
    }
    return x;
}

int64_t mutuallyPrime(int64_t euler_func) {
    random_device rd;
    mt19937_64 gen(rd());

    int64_t a0 = euler_func, a, b = 0, b0;
    uniform_int_distribution<> dist(2, a0);

    while (b != 1) {
        a = a0, b0 = dist(gen), b = b0;
        while (a % b != 0) {
            a %= b;
            swap(a, b);
        }
    }
    return b0;
}

int64_t mutuallyInverse(int64_t num, int64_t euler_func) {
    int64_t a = num, a0 = a, b = euler_func, x0 = 1, y0 = 0, x = 0, y = 1, q;
    while (a % b != 0) {
        q = a / b;
        a %= b;
        swap(a, b);

        x0 -= q * x;
        swap(x0, x);

        y0 -= q * y;
        swap(y0, y);
    }
    if (x < 0) x += euler_func;
    return x;
}

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