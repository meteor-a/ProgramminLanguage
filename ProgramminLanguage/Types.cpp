#include "Types.h"

Types::Types() {
    MAIN_TID_ = new TID_(nullptr);
}

int Types::getPriority(std::string sign, bool unary) { // от большего к меньшему
    if (sign == ")") {
        return 1;
    }
    else if (sign == "++" || sign == "--" || sign == "!" || sign == "~" || ((sign == "+" || sign == "-") && unary)) {
        return 2;
    }
    else if (sign == "*" || sign == "/" || sign == "%") {
        return 3;
    }
    else if ((sign == "+" || sign == "-") && !unary) {
        return 4;
    }
    else if (sign == "<<" || sign == ">>") {
        return 5;
    }
    else if (sign == "<" || sign == ">" || sign == "<=" || sign == ">=") {
        return 6;
    }
    else if (sign == "==" || sign == "<>") {
        return 7;
    }
    else if (sign == "&") {
        return 8;
    }
    else if (sign == "^") {
        return 9;
    }
    else if (sign == "|") {
        return 10;
    }
    else if (sign == "&&") {
        return 11;
    }
    else if (sign == "||") {
        return 12;
    }
    else if (sign == "=" || sign == "+=" || sign == "-=" || sign == "*=" || sign == "/=" || sign == "%=") {
        return 13;
    }
    else if (sign == "(") {
        return 14;
    }
    else {
        return -1;
    }
}