#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>

std::string evaluate_expression(const std::string& line) {
    std::stack<std::string> st;
    size_t i = 0;

    while (i < line.size()) {
        // Пропускаем пробелы
        if (line[i] == ' ') {
            i++;
            continue;
        }

        std::string token = "";

        // Если это скобка или оператор, берём один символ
        if (line[i] == '(' || line[i] == ')' || line[i] == '+' || line[i] == '*') {
            token = line[i];
            i++;
        }
        // Иначе это число (может быть отрицательным)
        else {
            // Считываем всё число целиком
            while (i < line.size() && (std::isdigit(line[i]) || (line[i] == '-' && token.empty()))) {
                token += line[i];
                i++;
            }

            // Если считали пустую строку, значит неизвестный символ
            if (token.empty()) {
                throw std::runtime_error("Invalid character");
            }
        }

        // Если это не закрывающая скобка - просто кладем в стек
        if (token != ")") {
            st.push(token);
        }
        else {
            // Встретили ')' — начинаем вычисление
            std::vector<long long> args;

            // Достаем из стека всё, пока не наткнемся на оператор
            while (!st.empty() && st.top() != "+" && st.top() != "*") {
                // stoll бросит исключение, если строку нельзя конвертировать в число
                args.push_back(std::stoll(st.top()));
                st.pop();
            }

            if (st.empty()) {
                throw std::runtime_error("Missing operator");
            }

            std::string op = st.top();
            st.pop();

            // После оператора обязательно должна быть открывающая скобка
            if (st.empty() || st.top() != "(") {
                throw std::runtime_error("Missing opening parenthesis");
            }
            st.pop(); // Удаляем '('

            // Вычисляем результат
            long long result = 0;
            if (op == "+") {
                for (long long num : args) {
                    result += num;
                }
            } else { // op == "*"
                result = 1;
                for (long long num : args) {
                    result *= num;
                }
            }

            // Результат кладем обратно в стек как строку
            st.push(std::to_string(result));
        }
    }

    // В конце в стеке должен остаться ровно один элемент — итоговый результат
    if (st.size() != 1) {
        throw std::runtime_error("Invalid expression structure");
    }

    return st.top();
}

int main() {
    std::string line;
    // Читаем выражения построчно
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        try {
            std::cout << evaluate_expression(line) << "\n";
        } catch (...) {
            // Любая ошибка (неверный символ, лишние скобки, деление и т.д.)
            // перехватывается и превращается в ERROR
            std::cout << "ERROR\n";
        }
    }

    return 0;
}