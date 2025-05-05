#include <iostream>
#include <cstring>
#include <climits>

// Используем константы для базовых значений
const int HASH_BASE = 31; // База для хэширования (простое число)
const int HASH_MOD = 1000000007; // Модуль для хэширования (большое простое число)
const int MIN_SUBSTR_LEN = 6; // Минимальная длина подстроки (> 5)

// Функция для вычисления степени с модулем
long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Функция для проверки, содержит ли подстрока пробелы
bool hasSpaces(const char* str, int start, int len) {
    for (int i = start; i < start + len; i++) {
        if (str[i] == ' ') return true;
    }
    return false;
}

// Функция для вычисления хэша подстроки
long long computeHash(const char* str, int start, int len, long long basePow) {
    long long hash = 0;
    for (int i = 0; i < len; i++) {
        hash = (hash * HASH_BASE + str[start + i]) % HASH_MOD;
    }
    return hash;
}

// Функция для сравнения двух подстрок (для проверки коллизий хэша)
bool areSubstringsEqual(const char* str, int start1, int start2, int len) {
    for (int i = 0; i < len; i++) {
        if (str[start1 + i] != str[start2 + i]) return false;
    }
    return true;
}

// Функция для копирования подстроки в выходной массив
void appendSubstring(char* dest, int& destPos, const char* src, int start, int len) {
    for (int i = 0; i < len; i++) {
        dest[destPos++] = src[start + i];
    }
    dest[destPos++] = ' '; // Добавляем пробел после подстроки
}

// Функция для удаления подстроки из массива
void removeSubstring(char* str, int& len, int start, int subLen) {
    for (int i = start; i < len - subLen; i++) {
        str[i] = str[i + subLen];
    }
    len -= subLen;
    str[len] = '\0'; // Обновляем конец строки
}

// Функция для поиска и обработки двух одинаковых подстрок
bool findAndProcessSubstrings(char* input, int& inputLen, char* output, int& outputLen, int outputCapacity) {
    if (inputLen < MIN_SUBSTR_LEN * 2) return false; // Если строка слишком короткая, выходим

    bool found = false;
    // Перебираем длины подстрок от inputLen/2 до MIN_SUBSTR_LEN
    for (int subLen = inputLen / 2; subLen >= MIN_SUBSTR_LEN; subLen--) {
        // Проверка, что выходной массив не переполнится
        if (outputLen + subLen + 1 > outputCapacity) {
            std::cerr << "Ошибка: недостаточно места в выходном массиве\n";
            return false;
        }

        // Вычисляем базовую степень для хэширования
        long long basePow = modPow(HASH_BASE, subLen - 1, HASH_MOD);

        // Хэши подстрок
        long long* hashes = new long long[inputLen - subLen + 1];
        for (int i = 0; i <= inputLen - subLen; i++) {
            hashes[i] = computeHash(input, i, subLen, basePow);
        }

        // Ищем одинаковые подстроки
        for (int i = 0; i <= inputLen - subLen; i++) {
            if (hasSpaces(input, i, subLen)) continue;

            for (int j = i + 1; j <= inputLen - subLen; j++) {
                if (hasSpaces(input, j, subLen)) continue;

                // Сравниваем хэши
                if (hashes[i] == hashes[j]) {
                    // Проверяем на коллизии
                    if (areSubstringsEqual(input, i, j, subLen)) {
                        found = true;
                        // Копируем подстроку в выходной массив
                        appendSubstring(output, outputLen, input, i, subLen);
                        // Удаляем обе подстроки
                        removeSubstring(input, inputLen, j, subLen); // Сначала вторую
                        removeSubstring(input, inputLen, i, subLen); // Потом первую
                        delete[] hashes;
                        return found;
                    }
                }
            }
        }
        delete[] hashes;
    }
    return found;
}

int main() {
    // Динамическое выделение памяти для входной и выходной строки
    char* input = nullptr;
    char* output = nullptr;
    int inputLen = 0, outputLen = 0;
    int inputCapacity = 0, outputCapacity = 1024; // Начальная емкость выходного массива

    // Ввод строки
    std::cout << "Введите строку (максимум 2^31 символов, пустая строка для завершения):\n";
    char buffer[1024];
    input = new char[1024];
    inputCapacity = 1024;
    inputLen = 0;
    buffer = "programming program testprogramming";
    while (true) {
        
        // std::cin.getline(buffer, 1024);
        if (strlen(buffer) == 0) break; // Пустая строка — конец ввода

        int bufferLen = strlen(buffer);
        // Проверяем, достаточно ли места в input
        while (inputLen + bufferLen + 1 > inputCapacity) {
            inputCapacity *= 2;
            char* temp = new char[inputCapacity];
            memcpy(temp, input, inputLen);
            delete[] input;
            input = temp;
        }
        memcpy(input + inputLen, buffer, bufferLen);
        inputLen += bufferLen;
        input[inputLen] = '\0'; // Завершаем строку
    }

    if (inputLen == 0) {
        std::cout << "Результат: (пустая строка)\n";
        delete[] input;
        return 0;
    }

    // Инициализируем выходной массив
    output = new char[outputCapacity];
    output[0] = '\0';

    // Обрабатываем строку, пока находятся одинаковые подстроки
    while (findAndProcessSubstrings(input, inputLen, output, outputLen, outputCapacity)) {
        // Увеличиваем выходной массив, если нужно
        if (outputLen + inputLen + 1 > outputCapacity) {
            outputCapacity *= 2;
            char* temp = new char[outputCapacity];
            memcpy(temp, output, outputLen);
            delete[] output;
            output = temp;
        }
    }

    // Добавляем остаток входной строки в выходную
    if (outputLen + inputLen + 1 > outputCapacity) {
        outputCapacity = outputLen + inputLen + 1;
        char* temp = new char[outputCapacity];
        memcpy(temp, output, outputLen);
        delete[] output;
        output = temp;
    }
    memcpy(output + outputLen, input, inputLen);
    outputLen += inputLen;
    output[outputLen] = '\0';

    // Вывод результата
    std::cout << "Результат: " << output << "\n";

    // Освобождаем память
    delete[] input;
    delete[] output;

    return 0;
}
