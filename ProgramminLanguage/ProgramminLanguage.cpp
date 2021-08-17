#include <iostream>
#include "MainCompiler.h"

void choose_examples(MainCompiler& compiler) {
    std::string n = "";
    std::cout << "RECUR programms:" << "\n";
    std::cout << "3 - Akkerman" << "\n";
    std::cout << "4 - Factorial" << "\n";
    std::cout << "5 - Fibonachi" << "\n";
    std::cout << "6 - GCD" << "\n";
    std::cout << "7 - Hanoi" << "\n";
    std::cout << "No Recur Programms:" << "\n";
    std::cout << "8 - Perebor 4-number" << "\n";
    std::cout << "9 - Arithmetic" << "\n";
    std::cout << "back - Main menu" << "\n";
    std::cout << "Type: ";
    std::cin >> n;
    std::cout << "\n";
    if (n == "3") {
        compiler.StartCompilation(3);
    }
    else if (n == "4") {
        compiler.StartCompilation(4);
    }
    else if (n == "5") {
        compiler.StartCompilation(5);
    }
    else if (n == "6") {
        compiler.StartCompilation(6);
    }
    else if (n == "7") {
        compiler.StartCompilation(7);
    }
    else if (n == "8") {
        compiler.StartCompilation(8);
    }
    else if (n == "9") {
        compiler.StartCompilation(9);
    }
    else if (n == "back") {
        return;
    }
}

int main() {
    std::string input = "";
    MainCompiler compiler;
    while (true) {
        std::cout << "\n";
        std::cout << "0 - Compile programm in code_text.txt" << "\n" << "1 - Update DataBases" << "\n" << "2 - examples" << "\n" << "exit - close programm" << "\n" << "\n";
        std::cout << "TYPE start: ";
        std::cin >> input;
        if (input == "0") {
            compiler.StartCompilation(0);
        }
        else if (input == "1") {
            compiler.StartCompilation(1);
        }
        else if (input == "2") {
            choose_examples(compiler);
        }
        else if (input == "exit") {
            return 0;
        }
    }
}
