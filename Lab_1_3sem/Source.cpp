#include <iostream>
#include <string>
#include <cstdint>
#include <random>
using namespace std;

class DH {
public:
    int64_t g, p, A, B;
    string encrypt(string text) {
        string encrypted_text = text;
        g = generatorPrime();
        p = generatorPrime();
        a = generator();
        b = generator();
        A = mod(g, a, p);
        B = mod(g, b, p);
        key = mod(B, a, p);
        cout << g << " " << p << " " << a << " " << b << " " << A << " " << B << " " << key << endl;
        for (int i = 0; i < encrypted_text.length(); i++) {
            encrypted_text[i] += key;
        }
        return encrypted_text;
    }
    string decrypt(string text) {
        string decrypted_text = text;
        key = mod(A, b, p);
        cout << g << " " << p << " " << a << " " << b << " " << A << " " << B << " " << key << endl;
        for (int i = 0; i < decrypted_text.length(); i++) {
            decrypted_text[i] -= key;
        }
        return decrypted_text;
    }
private:
    int64_t a, b, key;
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

    int64_t generator() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<> dist(256, 100000000);
        int64_t x = dist(gen);

        return x;
    }

    int64_t generatorPrime() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<> dist(256, 100000000);

        int64_t x = dist(gen);
        while (!checkPrime(x)) {
            x++;
        }
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
};

class RSA {
public:
    int64_t p, q, e;
    string encrypt(string text) {
        int64_t p = generatorPrime(), q = generatorPrime(), f = (p - 1) * (q - 1), e = mutuallyPrime(f);
        n = p * q;
        if (mutuallyInverse(e, f) < 0) d = f + mutuallyInverse(e, f);
        else d = mutuallyInverse(e, f);
        cout << "P = " << p << "; Q = " << q << "; N = " << n << "; Phi = " << f << "; E = " << e << "; D = " << d << endl;

        string source_text = text, encrypted_text;

        cout << source_text << " : ";
        for (auto x : source_text) {
            cout << int(x) << " ";
        }
        cout << endl;

        for (auto x : source_text) {
            encrypted_text += to_string(mod(x, e, n)) + ' ';
            cout << char(mod(x, e, n));
        }

        cout << " : ";

        for (auto x : encrypted_text) {
            if (x != ' ') {
                cout << x;
            }
            else {
                cout << ' ';
            }
        }

        return encrypted_text;
    }
    string decrypt(string text) {
        string encrypted_text = text, decrypted_text;
        int64_t value = 0;
        for (auto x : encrypted_text) {
            if (x != ' ') {
                value *= 10;
                value += x - '0';
            }
            else {
                value = mod(value, d, n);
                decrypted_text += char(value);
                value = 0;
            }
        }
        cout << endl;
        return decrypted_text;
    }
private:
    int64_t n, f, d;
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

    int64_t generator() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<> dist(256, 10000);
        int64_t x = dist(gen);

        return x;
    }

    int64_t generatorPrime() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<> dist(256, 10000);

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
};

class Shamir {
public:
    int64_t p;
    string encrypt(string text) {
        p = generatorPrime();
        int64_t ax = mutuallyPrime(p - 1), bx = mutuallyPrime(p - 1), value = 0;
        if (mutuallyInverse(ax, p - 1) < 0) ay = p - 1 + mutuallyInverse(ax, p - 1);
        else ay = mutuallyInverse(ax, p - 1);
        if (mutuallyInverse(bx, p - 1) < 0) by = p - 1 + mutuallyInverse(bx, p - 1);
        else by = mutuallyInverse(bx, p - 1);
        cout << "p = " << p << "; a = " << ax << "; a' = " << ay << "; b = " << bx << "; b' = " << by << endl;

        // M^a

        string source_text = text, encrypted_text = "";

        cout << source_text << " : ";
        for (auto x : source_text) {
            cout << int(x) << " ";
        }
        cout << endl;

        for (auto x : source_text) {
            encrypted_text += to_string(mod(x, ax, p)) + ' ';
            cout << char(mod(x, ax, p));
        }

        cout << " : " << encrypted_text << endl;

        // M^ab
        source_text = encrypted_text;
        encrypted_text = "";

        for (auto x : source_text) {
            if (x != ' ') {
                value *= 10;
                value += x - '0';
            }
            else {
                encrypted_text += to_string(mod(value, bx, p)) + ' ';
                cout << char(mod(value, bx, p));
                value = 0;
            }
        }

        cout << " : " << encrypted_text << endl;

        // (M^ab)^a'

        source_text = encrypted_text;
        encrypted_text = "";

        for (auto x : source_text) {
            if (x != ' ') {
                value *= 10;
                value += x - '0';
            }
            else {
                encrypted_text += to_string(mod(value, ay, p)) + ' ';
                cout << char(mod(value, ay, p));
                value = 0;
            }
        }

        cout << " : " << encrypted_text;

        return encrypted_text;
    }
    string decrypt(string text) {
        string encrypted_text = text, decrypted_text;
        int64_t value = 0;
        for (auto x : encrypted_text) {
            if (x != ' ') {
                value *= 10;
                value += x - '0';
            }
            else {
                decrypted_text += char(mod(value, by, p));
                value = 0;
            }
        }
        cout << endl;
        return decrypted_text;
    }
private:
    int64_t ax, ay, bx, by;
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

    int64_t generator() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<> dist(256, 10000);
        int64_t x = dist(gen);

        return x;
    }

    int64_t generatorPrime() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<> dist(256, 1000000);

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
};

int main() {
    /*DH dh;
    string str = "Hello, World!", enc_text, dec_text;
    enc_text = dh.encrypt(str);
    cout << enc_text << endl;
    dec_text = dh.decrypt(enc_text);
    cout << dec_text << endl;*/

    /*RSA rsa;
    string str = "Hello, World!", enc_text, dec_text;
    enc_text = rsa.encrypt(str);
    cout << enc_text << endl;
    dec_text = rsa.decrypt(enc_text);
    cout << dec_text << endl;*/

    Shamir shamir;
    string str = "Hello, World!", enc_text, dec_text;
    enc_text = shamir.encrypt(str);
    cout << enc_text << endl;
    dec_text = shamir.decrypt(enc_text);
    cout << dec_text << endl;
}