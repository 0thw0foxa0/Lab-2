#include <iostream>
#include <string>

using namespace std;

// Функция для поиска двух одинаковых подстрок длиной > 5 без пробелов
bool findAndProcess(string &input, string &output) {
    int len = input.length();
    bool found = false;

    // Перебираем возможные длины подстрок от len/2 до 6 (минимальная длина > 5)
    for (int subLen = len / 2; subLen >= 6; subLen--) {
        // Перебираем начальные позиции первой подстроки
        for (int i = 0; i <= len - subLen; i++) {
            // Проверяем, что подстрока не содержит пробелов
            string substr1 = input.substr(i, subLen);
            bool hasSpace = false;
            for (char c : substr1) {
                if (c == ' ') {
                    hasSpace = true;
                    break;
                }
            }
            if (hasSpace) continue;

            // Ищем вторую такую же подстроку
            for (int j = i + 1; j <= len - subLen; j++) {
                string substr2 = input.substr(j, subLen);
                if (substr1 == substr2) {
                    // Проверяем, что вторая подстрока не содержит пробелов
                    hasSpace = false;
                    for (char c : substr2) {
                        if (c == ' ') {
                            hasSpace = true;
                            break;
                        }
                    }
                    if (hasSpace) continue;

                    // Найдены две одинаковые подстроки
                    found = true;
                    // Добавляем подстроку в выходную строку
                    output += substr1 + " ";
                    // Удаляем обе подстроки из входной строки
                    input.erase(j, subLen); // Удаляем вторую подстроку
                    input.erase(i, subLen); // Удаляем первую подстроку
                    return found; // Прерываем, чтобы обработать новую строку
                }
            }
        }
    }
    return found;
}

int main() {
    string input, output;
    
    // Ввод строки
    cout << "Введите строку: "<<endl;
   // getline(cin, input);
    input = "programming program testprogramming";
    // Обрабатываем строку, пока находятся одинаковые подстроки
    while (findAndProcess(input, output)) {
        // Продолжаем цикл
    }
    
    // Добавляем остаток входной строки в выходную
    output += input;
    
    // Вывод результата
    cout << "Результат: " << output << endl;
    
    return 0;
}
