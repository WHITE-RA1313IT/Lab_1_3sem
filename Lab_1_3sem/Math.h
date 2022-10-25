#pragma once
#include <cstdint>
#include <random>
using namespace std;

int64_t generator(int64_t num);

bool checkPrime(int64_t num);

int64_t generatorPrime(int64_t num);

int64_t mutuallyPrime(int64_t euler_func);

int64_t mutuallyInverse(int64_t num, int64_t euler_func);

int64_t mod(int64_t a0, int64_t x0, int64_t p0);