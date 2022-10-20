#include <iostream>
#include <Windows.h>
#include <string>
#include <cstdint>
#include <random>
using namespace std;

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

class DH {
public:
    int64_t g, p, public_key;

    void gen_g() {
        g = generatorPrime(10000);
    }

    void gen_p() {
        p = generatorPrime(10000);
    }

    void selectPrivateKey() {
        private_key = generator(10000);
    }

    void calcPublicKey() {
        public_key = mod(g, private_key, p);
    }

    void calcKey(int64_t public_interlocutor_key) {
        key = mod(public_interlocutor_key, private_key, p);
    }

    int64_t Get_key() {
        return key;
    }
private:
    int64_t private_key, key;
};

class RSA {
public:
    int64_t p, q, public_key, n;
    
    void gen_p() {
        p = generatorPrime(10000);
    }

    void gen_q() {
        q = generatorPrime(10000);
    }

    void calc_n() {
        n = p * q;
    }

    void calcEulerFunc() {
        euler_func = (p - 1) * (q - 1);
    }

    void selectPublicKey() {
        public_key = mutuallyPrime(euler_func);
    }

    void calcPrivateKey() {
        private_key = mutuallyInverse(public_key, euler_func);
    }

    int64_t GetPrivateKey() {
        return private_key;
    }
private:
    int64_t euler_func, private_key;
};

class Shamir {
public:
    int64_t p;
    string encrypt_x1(string text) {
        string source_text = text, encrypted_text = "";
        for (auto x : source_text) {
            encrypted_text += to_string(mod(x, ax, p)) + ' ';
        }
        return encrypted_text;
    }

    string encrypt_x2(string text) {
        string source_text = text, encrypted_text = "";
        int64_t value = 0;
        for (auto x : source_text) {
            if (x != ' ') {
                value *= 10;
                value += x - '0';
            }
            else {
                encrypted_text += to_string(mod(value, bx, p)) + ' ';
                value = 0;
            }
        }
        return encrypted_text;
    }

    string encrypt_x3(string text) {
        string source_text = text, encrypted_text = "";
        int64_t value = 0;
        for (auto x : source_text) {
            if (x != ' ') {
                value *= 10;
                value += x - '0';
            }
            else {
                encrypted_text += to_string(mod(value, ay, p)) + ' ';
                value = 0;
            }
        }
        return encrypted_text;
    }
    
    string decrypt_x4(string text) {
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
        return decrypted_text;
    }

    int64_t Get_ax() {
        return ax;
    }

    void Set_ax(int64_t ax0) {
        ax = ax0;
    }

    int64_t Get_ay() {
        return ay;
    }

    void Set_ay(int64_t ay0) {
        ay = ay0;
    }

    int64_t Get_bx() {
        return bx;
    }

    void Set_bx(int64_t bx0) {
        bx = bx0;
    }

    int64_t Get_by() {
        return by;
    }

    void Set_by(int64_t by0) {
        by = by0;
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
};

class ElGamal {
public:
    int64_t p, g, y;
    string encrypt(string text) {
        p = generatorPrime(10000);
        g = generatorPrime(p);
        x = generator(p);
        y = mod(g, x, p);
        k = generator(p - 1);
        a = mod(g, k, p);
        string source_text = text, encrypted_text;

        cout << source_text << " : ";
        for (auto i : source_text) {
            cout << int(i) << " ";
        }
        cout << endl;

        for (auto i : source_text) {
            encrypted_text += to_string(i * mod(y, k, p) % p) + ' ';
            cout << char(i * mod(y, k, p) % p);
        }

        cout << " : ";

        cout << encrypted_text << endl;

        return encrypted_text;
    }
    string decrypt(string text) {
        string encrypted_text = text, decrypted_text;
        int64_t value = 0;
        for (auto i : encrypted_text) {
            if (i != ' ') {
                value *= 10;
                value += i - '0';
            }
            else {
                decrypted_text += char(value * mod(a, p - 1 - x, p) % p);
                value = 0;
            }
        }
        cout << endl;
        return decrypted_text;
    }
private:
    int64_t x, k, a;
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

    int64_t generator(int64_t p) {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<> dist(256, p);
        int64_t x = dist(gen);

        return x;
    }

    int64_t generatorPrime(int64_t p) {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<> dist(256, p);

        int64_t x = dist(gen);
        while (!checkPrime(x)) {
            x++;
        }
        return x;
    }
};

void diffiehellman() {
    DH Alice, Bob;
    Alice.gen_g();
    Alice.gen_p();
    Alice.selectPrivateKey();
    Alice.calcPublicKey();

    Bob.g = Alice.g;
    Bob.p = Alice.p;
    Bob.selectPrivateKey();
    Bob.calcPublicKey();

    Alice.calcKey(Bob.public_key);
    Bob.calcKey(Alice.public_key);

    string str, encrypted_text, decrypted_text;
    getline(cin, str);

    for (int i = 0; i < str.length(); i++) {
        encrypted_text += str[i] + Alice.Get_key() % 27;
    }

    for (int i = 0; i < encrypted_text.length(); i++) {
        decrypted_text += encrypted_text[i] - Alice.Get_key() % 27;
    }

    cout << "Alice: " << str << " --> Message: " << encrypted_text << " --> Bob: " << decrypted_text;
}

void rsa() {
    RSA Alice, Bob;
    Alice.p = generatorPrime(10000);
    Alice.q = generatorPrime(10000);
    Alice.n = Alice.p * Alice.q;
    Alice.calcEulerFunc();
    Alice.selectPublicKey();
    Alice.calcPrivateKey();

    Bob.public_key = Alice.public_key;
    Bob.n = Alice.n;

    string str, encrypted_text, decrypted_text;
    getline(cin, str);
    cout << "Bob: " << str << " --> Message: ";
    for (auto x : str) {
        encrypted_text += to_string(mod(x, Bob.public_key, Bob.n)) + ' ';
        //cout << char(mod(x, Bob.public_key, Bob.public_key));
    }
    int64_t value = 0;
    for (auto x : encrypted_text) {
        if (x != ' ') {
            value *= 10;
            value += x - '0';
        }
        else {
            value = mod(value, Alice.GetPrivateKey(), Alice.n);
            decrypted_text += char(value);
            value = 0;
        }
    }
    cout << encrypted_text << " --> Alice: " << decrypted_text;
}

void shamir() {
    Shamir Alice, Bob;
    string str = "Hello", x1, x2, x3, x4;

    Alice.p = Alice.generatorPrime();
    Bob.p = Alice.p;

    Alice.Set_ax(mutuallyPrime(Alice.p - 1));
    for (auto x : str) {
        x1 += to_string(mod(x, Alice.Get_ax(), Alice.p)) + ' ';
    }

    int64_t value = 0;
    Bob.Set_bx(mutuallyPrime(Bob.p - 1));
    for (auto x : x1) {
        if (x != ' ') {
            value *= 10;
            value += x - '0';
        }
        else {
            x2 += to_string(mod(value, Bob.Get_bx(), Bob.p)) + ' ';
            value = 0;
        }
    }

    value = 0;
    Alice.Set_ay(mutuallyInverse(Alice.Get_ax(), Alice.p - 1));
    for (auto x : x2) {
        int64_t value = 0;
        if (x != ' ') {
            value *= 10;
            value += x - '0';
        }
        else {
            x3 += to_string(mod(value, Alice.Get_ay(), Alice.p)) + ' ';
            cout << value << endl;
            value = 0;
        }
    }

    value = 0;
    Bob.Set_by(mutuallyInverse(Bob.Get_bx(), Bob.p - 1));
    cout << Bob.Get_bx() << " " << Bob.Get_by() << " " << mutuallyInverse(Bob.Get_by(), Bob.p - 1) << endl;
    for (auto x : x3) {
        int64_t value = 0;
        if (x != ' ') {
            value *= 10;
            value += x - '0';
        }
        else {
            x4 += to_string(mod(value, Bob.Get_by(), Bob.p)) + ' ';
            value = 0;
        }
    }

    cout << "Alice: " << str << endl;
    cout << "Alice --> Bob: " << x1 << endl;
    cout << "Bob --> Alice: " << x2 << endl;
    cout << "Alice --> Bob: " << x3 << endl;
    cout << "Bob: " << x4 << endl;
}

string print_msg(string input) {
    int64_t value = 0;
    string output;
    for (auto x : input) {
        if (x != ' ') {
            value *= 10;
            value += x - '0';
        }
        else {
            output += char(value);
            value = 0;
        }
    }
    return output;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    //diffiehellman();

    rsa();

    //shamir();

   /* string str = "Hello, World!", enc_text, dec_text;
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

    /*Shamir shamir;
    string str = "Hello, World!", enc_text, dec_text;
    enc_text = shamir.encrypt(str);
    cout << enc_text << endl;
    dec_text = shamir.decrypt(enc_text);
    cout << dec_text << endl;*/

    /*ElGamal elgamal;
    string str = "Hello, World!", enc_text, dec_text;
    enc_text = elgamal.encrypt(str);
    cout << enc_text << endl;
    dec_text = elgamal.decrypt(enc_text);
    cout << dec_text << endl;*/
}