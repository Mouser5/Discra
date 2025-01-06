#include <iostream>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

using namespace boost::multiprecision;
using namespace boost::random;

// Функция для возведения в степень по модулю
cpp_int mod_exp(const cpp_int& base, const cpp_int& exp, const cpp_int& mod) {
    cpp_int result = 1;
    cpp_int b = base % mod;
    cpp_int e = exp;
    while (e > 0) {
        if (e % 2 == 1) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        e /= 2;
    }
    return result;
}

// Функция для нахождения НОД (алгоритм Евклида)
cpp_int gcd(const cpp_int& a, const cpp_int& b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Расширенный алгоритм Евклида для поиска обратного по модулю
cpp_int mod_inverse(const cpp_int& e, const cpp_int& phi) {
    cpp_int t = 0, new_t = 1;
    cpp_int r = phi, new_r = e;

    while (new_r != 0) {
        cpp_int quotient = r / new_r;
        std::swap(t, new_t -= quotient * t);
        std::swap(r, new_r -= quotient * r);
    }
    if (t < 0) {
        t += phi;
    }
    return t;
}

// Функция для проверки, является ли число простым (тест Миллера-Рабина)
bool is_prime(const cpp_int& n, size_t rounds = 25) {
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0)
        return false;

    cpp_int d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    boost::random::random_device rd;
    boost::random::mt19937 gen(rd());

    for (size_t i = 0; i < rounds; ++i) {
        boost::random::uniform_int_distribution<cpp_int> dist(2, n - 2);
        cpp_int a = dist(gen);
        cpp_int x = mod_exp(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        bool composite = true;
        cpp_int temp_d = d;
        while (temp_d != n - 1) {
            x = mod_exp(x, 2, n);
            temp_d *= 2;

            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

// Функция для генерации случайного простого числа длиной не менее 1024 бит
cpp_int generate_prime(size_t bits) {
    boost::random::random_device rd;
    boost::random::mt19937 gen(rd());
    boost::random::uniform_int_distribution<cpp_int> dist(1 << (bits - 1), (cpp_int(1) << bits) - 1);

    cpp_int prime;
    do {
        prime = dist(gen);
    } while (!is_prime(prime));

    return prime;
}

// Генерация ключей
void generate_keys(cpp_int& n, cpp_int& e, cpp_int& d) {
    size_t bits = 1024;

    // Генерируем два простых числа p и q
    cpp_int p = generate_prime(bits);
    cpp_int q = generate_prime(bits);

    // Вычисляем n = p * q
    n = p * q;

    // Вычисляем функцию Эйлера phi(n) = (p-1) * (q-1)
    cpp_int phi = (p - 1) * (q - 1);

    // Задаем значение e, которое взаимно просто с phi
    e = 65537;  // Часто используемое значение

    // Вычисляем d, такое что (d * e) % phi == 1
    d = mod_inverse(e, phi);
}

// Шифрование сообщения
cpp_int encrypt(const cpp_int& message, const cpp_int& e, const cpp_int& n) {
    return mod_exp(message, e, n);
}

// Расшифрование сообщения
cpp_int decrypt(const cpp_int& ciphertext, const cpp_int& d, const cpp_int& n) {
    return mod_exp(ciphertext, d, n);
}

// Функция для сохранения ключа в файл
void save_key_to_file(const std::string& filename, const cpp_int& key) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << key;
        file.close();
    }
}

// Функция для загрузки ключа из файла
void load_key_from_file(const std::string& filename, cpp_int& key) {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> key;
        file.close();
    }
}

int main() {
    cpp_int n, e, d;

    // Генерация ключей
    generate_keys(n, e, d);
    bool rd = is_prime(40193);

    // Сохранение ключей в файлы
    save_key_to_file("public_key.txt", e);
    save_key_to_file("private_key.txt", d);
    save_key_to_file("modulus.txt", n);

    // Пример сообщения для шифрования
    std::string message = "123456789012345678901234567890";
    cpp_int plaintext(message);

    // Шифрование
    cpp_int encrypted_message = encrypt(plaintext, e, n);
    std::cout << "Encrypted message: " << encrypted_message << std::endl;

    // Расшифрование
    cpp_int decrypted_message = decrypt(encrypted_message, d, n);
    std::cout << "Decrypted message: " << decrypted_message << std::endl;

    return 0;
}
