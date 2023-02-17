#include <iostream>
#include "Math.h"
#include <Windows.h>
#include <string>
#include <cstdint>
using namespace std;

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
    int64_t p, q, public_key, n, M, C;
    
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

    void encrypt(int64_t message) {
        C = mod(message, public_key, n);
    }

    void decrypt(int64_t ciphertext) {
        M = mod(ciphertext, private_key, n);
    }

    int64_t GetPrivateKey() {
        return private_key;
    }
private:
    int64_t euler_func, private_key;
};

class Shamir {
public:
    int64_t p, M;

    void gen_p() {
        p = generatorPrime(100000);
    }

    void select_ax() {
        ax = mutuallyPrime(p - 1);
    }

    void select_ay() {
        ay = mutuallyInverse(ax, p - 1);
    }

    void calc_M(int64_t message, int64_t a) {
        M = mod(message, a, p);
    }

    int64_t Get_ax() {
        return ax;
    }

    int64_t Get_ay() {
        return ay;
    }
private:
    int64_t ax, ay;
};

class ElGamal {
public:
    int64_t p, g, public_key, a, b, M;

    void gen_p() {
        p = generatorPrime(10000);
    }

    void gen_g() {
        g = generatorPrime(10000);
    }

    void selectPrivateKey() {
        private_key = generator(p - 1);
    }

    void calcPublicKey() {
        public_key = mod(g, private_key, p);
    }    

    void select_k() {
        k = mutuallyPrime(p - 1);
    }

    void calc_A() {
        a = mod(g, k, p);
    }

    void calc_B() {
        b = (M * mod(public_key, k, p)) % p;
    }

    void calc_M() {
        M = b * mod(a, p - 1 - private_key, p) % p;
    }

    int64_t Get_k() {
        return k;
    }
private:
    int64_t private_key, k;
};

void diffiehellman(string str) {
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

    string encrypted_text, decrypted_text;

    for (int i = 0; i < str.length(); i++) {
        encrypted_text += str[i] + Bob.Get_key() % 27;
    }

    for (int i = 0; i < encrypted_text.length(); i++) {
        decrypted_text += encrypted_text[i] - Alice.Get_key() % 27;
    }

    cout << "Alice: " << str << " --> Message: " << encrypted_text << " --> Bob: " << decrypted_text;
}

void rsa(string str) {
    RSA Alice, Bob;
    Alice.p = generatorPrime(10000);
    Alice.q = generatorPrime(10000);
    Alice.n = Alice.p * Alice.q;
    Alice.calcEulerFunc();
    Alice.selectPublicKey();
    Alice.calcPrivateKey();

    Bob.public_key = Alice.public_key;
    Bob.n = Alice.n;

    string decrypted_text;

    cout << "Bob: " << str << " --> Message: ";
    for (auto x : str) {
        Bob.encrypt(x);
        cout << Bob.C << " ";
        Alice.decrypt(Bob.C);
        decrypted_text += char(Alice.M);
    }
    cout << " --> Alice: " << decrypted_text;
}

void shamir(string str) {
    Shamir Alice, Bob;
    Alice.gen_p();
    Alice.select_ax();
    Alice.select_ay();

    Bob.p = Alice.p;
    Bob.select_ax();
    Bob.select_ay();

    string x1, x2, x3, decrypted_text;

    for (auto x : str) {
        Alice.calc_M(x, Alice.Get_ax());
        x1 += char(Alice.M);
        Bob.calc_M(Alice.M, Bob.Get_ax());
        x2 += char(Bob.M);
        Alice.calc_M(Bob.M, Alice.Get_ay());
        x3 += char(Alice.M);
        Bob.calc_M(Alice.M, Bob.Get_ay());
        decrypted_text += char(Bob.M);
    }

    cout << "Alice: " << str << endl;
    cout << "Alice --> Bob: " << x1 << endl;
    cout << "Bob --> Alice: " << x2 << endl;
    cout << "Alice --> Bob: " << x3 << endl;
    cout << "Bob: " << decrypted_text << endl;
}

void elgamal(string str) {
    ElGamal Alice, Bob;
    Alice.gen_p();
    Alice.gen_g();
    Alice.selectPrivateKey();
    Alice.calcPublicKey();

    Bob.p = Alice.p;
    Bob.g = Alice.g;
    Bob.public_key = Alice.public_key;

    Bob.select_k();
    Bob.calc_A();
    Alice.a = Bob.a;

    string decrypted_text;

    cout << "Bob: " << str << " --> Message: ";
    for (auto x : str) {
        Bob.M = x;
        Bob.calc_B();
        cout << Bob.b << " ";
        Alice.b = Bob.b;
        Alice.calc_M();
        decrypted_text += char(Alice.M);
    }
    cout << " --> Alice: " << decrypted_text;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Выберите протокол шифрования:\n";
    cout << "1) Алгоритм Диффи - Хеллмана;\n";
    cout << "2) RSA;\n";
    cout << "3) Трёхэтапный протокол Шамира;\n";
    cout << "4) Схема Эль-Гамаля.\n";
    int num = 0;
    cin >> num;
    while (num < 1 || num > 4) {
        cout << "Введите число 1-4!" << endl;
        cin >> num;
    }
    
    cin.get();

    cout << "Введите текст, который хотите зашифровать-дешифровать: ";
    string str, decrypted_text;
    getline(cin, str);
    for (int i = 0; i < 100; i++) cout << '/';
    cout << endl;

    switch (num) {
    case 1:
        cout << "Алгоритм Диффи - Хеллмана:\n";
        diffiehellman(str);
        break;
    case 2:
        cout << "RSA:\n";
        rsa(str);
        break;
    case 3: 
        cout << "Трёхэтапный протокол Шамира:\n";
        shamir(str);
        break;
    case 4:
        cout << "Схема Эль-Гамаля:\n";
        elgamal(str);
        break;
    }
}