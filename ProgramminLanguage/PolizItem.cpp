#include "PolizItem.h"

PolizItem::PolizItem(short int type) {
    type_item_ = type;
    pos_ = 0;
    unary_ = false;
    ident = nullptr;
    operation = "";
}

PolizItem::PolizItem(short int type, IDENT_&id) {
    type_item_ = type;
    pos_ = 0;
    unary_ = false;
    ident = &id;
    operation = "";
}

PolizItem::PolizItem(short int type, IDENT_& id, bool is_call_func_param) {
    type_item_ = type;
    pos_ = 0;
    unary_ = is_call_func_param;
    ident = &id;
    operation = "";
}

PolizItem::PolizItem(short int type, long long pos) {
    type_item_ = type;
    pos_ = pos;
    unary_ = false;
    ident = nullptr;
    operation = "";
}

PolizItem::PolizItem(short int type, std::string oper) {
    type_item_ = type;
    pos_ = 0;
    unary_ = false;
    ident = nullptr;
    operation = oper;
}

PolizItem& PolizItem::operator=(const PolizItem& poliz_item) {
    if (ident->type_ == poliz_item.ident->type_) {
        ident->cur_val[ident->cur_val.size() - 1] = poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1];
    }
    else {
        if (ident->type_ == CODE_TYPE_INT_) {
            if (poliz_item.ident->type_ == CODE_TYPE_BOOLEAN_) {
                int res = 0;
                if (poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1] == "true") {
                    res = 1;
                }
                else {
                    res = 0;
                }
                ident->cur_val[ident->cur_val.size() - 1] = std::to_string(res);
            }
            else if (poliz_item.ident->type_ == CODE_TYPE_DOUBLE_) {
                ident->cur_val[ident->cur_val.size()- 1] = poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1].substr(0, poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1].find('.'));
            }
            else {
                throw CODE_ERR_INCOMPATIBLE_TYPES_;
            }
        }
        else if (ident->type_ == CODE_TYPE_DOUBLE_) {
            if (poliz_item.ident->type_ == CODE_TYPE_BOOLEAN_) {
                double res = 0;
                if (poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1] == "true") {
                    res = 1.0;
                }
                else {
                    res = 0.0;
                }
                ident->cur_val[ident->cur_val.size() - 1] = std::to_string(res);
            }
            else if (poliz_item.ident->type_ == CODE_TYPE_DOUBLE_) {
                ident->cur_val[ident->cur_val.size() - 1] = poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1];
            } 
            else if (poliz_item.ident->type_ == CODE_TYPE_INT_) {
                ident->cur_val[ident->cur_val.size() - 1] = poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1] + ".0";
            }
            else {
                throw CODE_ERR_INCOMPATIBLE_TYPES_;
            }
           
        }
        else if (ident->type_ == CODE_TYPE_BOOLEAN_) {
            int res = 0;
            if (poliz_item.ident->type_ == CODE_TYPE_DOUBLE_) {
                res = (std::stod(poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1])) > 0 ? 1 : 0;
            }
            else if (poliz_item.ident->type_ == CODE_TYPE_INT_) {
                res = (std::stoi(poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1])) > 0 ? 1 : 0;
            }
            else {
                throw CODE_ERR_INCOMPATIBLE_TYPES_;
            }
            ident->cur_val[ident->cur_val.size() - 1] = std::to_string(res);
        }
        else if (ident->type_ == CODE_TYPE_STRING_) {
            if (poliz_item.ident->type_ == CODE_TYPE_STRING_) {
                ident->cur_val = poliz_item.ident->cur_val;
            }
            else {
                throw CODE_ERR_INCOMPATIBLE_TYPES_;
            }
        }
        else {
            throw CODE_ERR_INCOMPATIBLE_TYPES_;
        }
    }
    return *this;
}

PolizItem& PolizItem::operator+=(const PolizItem& poliz_item) {
    *this = *this + poliz_item;
    return *this;
}

PolizItem& PolizItem::operator-=(const PolizItem& poliz_item) {
    *this = *this - poliz_item;
    return *this;
}

PolizItem& PolizItem::operator*=(const PolizItem& poliz_item) {
    *this = *this * poliz_item;
    return *this;
}

PolizItem& PolizItem::operator/=(const PolizItem& poliz_item) {
    *this = *this / poliz_item;
    return *this;
}

PolizItem& PolizItem::operator%=(const PolizItem& poliz_item) {
    *this = *this % poliz_item;
    return *this;
}

PolizItem PolizItem::operator+(const PolizItem& poliz_item) const {
    PolizItem tmp = poliz_item;
    IDENT_* id = nullptr;
    if (ident->type_ == CODE_TYPE_INT_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            long long first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first + second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            long long first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            long long second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first + second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            long long first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            long long second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first + second));
        }
        else {
            long long first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            long long second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first + second));
        }
    }
    else if (ident->type_ == CODE_TYPE_DOUBLE_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first + second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            long long second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first + second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            long long second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first + second));
        }
        else {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            long long second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first + second));
        }
    }
    else if (ident->type_ == CODE_TYPE_BOOLEAN_) {
        long long first;
        if (ident->cur_val[ident->cur_val.size() - 1] == "true") {
            first = 1;
        }
        else {
            first = 0;
        }
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first + second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            long long second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first + second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            long long second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first + second));
        }
        else {
            long long second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first + second));
        }
    }
    else if (ident->type_ == CODE_TYPE_STRING_) {
        long long first = ident->cur_val[ident->cur_val.size() - 1].length();
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first + second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            id = new IDENT_("", CODE_TYPE_STRING_, ident->cur_val[ident->cur_val.size() - 1] + tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            long long second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first + second));
        }
        else {
            long long second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first + second));
        }
    }

    return PolizItem(TYPE_CONST_, *id);
}

PolizItem PolizItem::operator-(const PolizItem& poliz_item) const {
    PolizItem tmp = poliz_item;
    IDENT_* id = nullptr;
    if (ident->type_ == CODE_TYPE_INT_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first - second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first - second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first - second));
        }
        else {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first - second));
        }
    }
    else if (ident->type_ == CODE_TYPE_DOUBLE_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first - second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first - second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first - second));
        }
        else {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first - second));
        }
    }
    else if (ident->type_ == CODE_TYPE_BOOLEAN_) {
        int first;
        if (ident->cur_val[ident->cur_val.size() - 1] == "true") {
            first = 1;
        }
        else {
            first = 0;
        }
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first - second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first - second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first - second));
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first - second));
        }
    }
    else if (ident->type_ == CODE_TYPE_STRING_) {
        int first = ident->cur_val[ident->cur_val.size() - 1].length();
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first - second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_STRING_, std::to_string(first - second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first - second));
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first - second));
        }
    }

    return PolizItem(TYPE_CONST_, *id);
}

PolizItem PolizItem::operator*(const PolizItem& poliz_item) const {
    PolizItem tmp = poliz_item;
    IDENT_* id = nullptr;
    if (ident->type_ == CODE_TYPE_INT_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first * second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first * second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first * second));
        }
        else {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first * second));
        }
    }
    else if (ident->type_ == CODE_TYPE_DOUBLE_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first * second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first * second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first * second));
        }
        else {
            double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first * second));
        }
    }
    else if (ident->type_ == CODE_TYPE_BOOLEAN_) {
        int first;
        if (ident->cur_val[ident->cur_val.size() - 1] == "true") {
            first = 1;
        }
        else {
            first = 0;
        }
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first * second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first * second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first * second));
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first * second));
        }
    } else if (ident->type_ == CODE_TYPE_STRING_) {
        int first = ident->cur_val[ident->cur_val.size() - 1].length();
        if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first * second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first * second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first * second));
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first * second));
        }
    }

    return PolizItem(TYPE_CONST_, *id);
}

PolizItem PolizItem::operator/(const PolizItem& poliz_item) const {
    PolizItem tmp = poliz_item;
    IDENT_* id = nullptr;
    if (ident->type_ == CODE_TYPE_INT_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_STRING_LENGTH_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first / second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_BOOLEAN_FALSE_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first / second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first / second));
        }
        else {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first / second));
        }
    }
    else if (ident->type_ == CODE_TYPE_BOOLEAN_) {
        int first;
        if (ident->cur_val[ident->cur_val.size() - 1] == "true") {
            first = 1;
        }
        else {
            first = 0;
        }

        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_STRING_LENGTH_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first / second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_BOOLEAN_FALSE_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first / second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first / second));
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first / second));
        }
    }
    else if (ident->type_ == CODE_TYPE_STRING_) {
        int first = ident->cur_val[ident->cur_val.size() - 1].length();
        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_STRING_LENGTH_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first / second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_BOOLEAN_FALSE_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first / second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first / second));
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first / second));
        }
    }
    else if (ident->type_ == CODE_TYPE_DOUBLE_) {
        double first = std::stod(ident->cur_val[ident->cur_val.size() - 1]);
        if (tmp.getIdent()->type_ == CODE_TYPE_INT_) {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first / second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            double second = std::stod(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first / second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }

            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first / second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_DOUBLE_, std::to_string(first / second));
        }
    }

    return PolizItem(TYPE_CONST_, *id);
}

PolizItem PolizItem::operator%(const PolizItem& poliz_item) const {
    PolizItem tmp = poliz_item;
    IDENT_* id = nullptr;
    if (ident->type_ == CODE_TYPE_INT_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_STRING_LENGTH_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first % second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_BOOLEAN_FALSE_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first % second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            throw CODE_ERR_CANNOT_MAKE_OPERAT_OSTATOC_DIV_;
        }
        else {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first % second));
        }
    }
    else if (ident->type_ == CODE_TYPE_BOOLEAN_) {
        int first;
        if (ident->cur_val[ident->cur_val.size() - 1] == "true") {
            first = 1;
        }
        else {
            first = 0;
        }

        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first % second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first % second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            throw CODE_ERR_CANNOT_MAKE_OPERAT_OSTATOC_DIV_;
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first % second));
        }
    }
    else if (ident->type_ == CODE_TYPE_STRING_) {
        int first = ident->cur_val[ident->cur_val.size() - 1].length();
        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            if (second == 0) {
                throw CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first % second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first % second));
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_DOUBLE_) {
            throw CODE_ERR_CANNOT_MAKE_OPERAT_OSTATOC_DIV_;
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            id = new IDENT_("", CODE_TYPE_INT_, std::to_string(first % second));
        }
    }
    else if (ident->type_ == CODE_TYPE_DOUBLE_) {
        throw CODE_ERR_CANNOT_MAKE_OPERAT_OSTATOC_DIV_;
    }

    return PolizItem(TYPE_CONST_, *id);
}

const bool PolizItem::operator==(const PolizItem& poliz_item) const {
    PolizItem tmp = poliz_item;
    bool res = false;
    if (ident->type_ == CODE_TYPE_INT_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            res = first == second;
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            res = first == second;
        }
        else {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            res = first == second;
        }
    }
    else if (ident->type_ == CODE_TYPE_BOOLEAN_) {
        int first;
        if (ident->cur_val[ident->cur_val.size() - 1] == "true") {
            first = 1;
        }
        else {
            first = 0;
        }

        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            res = first == second;
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            res = first == second;
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            res = first == second;
        }
    }
    else if (ident->type_ == CODE_TYPE_STRING_) {
        int first = ident->cur_val[ident->cur_val.size() - 1].length();
        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            res = first == second;
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            res = first == second;
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            res = first == second;
        }
    }

    return res;
}

const bool PolizItem::operator==(const bool& boolean) const {
    std::string res;
    if (boolean == true) {
        res = "true";
    }
    else {
        res = "false";
    }
    IDENT_ id("", CODE_TYPE_BOOLEAN_, res);
    PolizItem item(TYPE_CONST_, id);
    return *this == item;
}

const bool PolizItem::operator!=(const PolizItem& poliz_item) const {
     return !(*this == poliz_item);
}

const bool PolizItem::operator<(const PolizItem& poliz_item) const {
    PolizItem tmp = poliz_item;
    bool res = false;
    if (ident->type_ == CODE_TYPE_INT_) {
        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            res = first < second;
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            res = first < second;
        }
        else {
            int first = std::stoi(ident->cur_val[ident->cur_val.size() - 1]);
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            res = first < second;
        }
    }
    else if (ident->type_ == CODE_TYPE_BOOLEAN_) {
        int first;
        if (ident->cur_val[ident->cur_val.size() - 1] == "true") {
            first = 1;
        }
        else {
            first = 0;
        }

        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            res = first < second;
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            res = first < second;
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            res = first < second;
        }
    }
    else if (ident->type_ == CODE_TYPE_STRING_) {
        int first = ident->cur_val[ident->cur_val.size() - 1].length();
        if (tmp.getIdent()->type_ == CODE_TYPE_STRING_) {
            int second = tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1].length();
            res = first < second;
        }
        else if (tmp.getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
            int second;
            if (tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1] == "true") {
                second = 1;
            }
            else {
                second = 0;
            }
            res = first < second;
        }
        else {
            int second = std::stoi(tmp.getIdent()->cur_val[tmp.getIdent()->cur_val.size() - 1]);
            res = first < second;
        }
    }

    return res;
}

const bool PolizItem::operator>(const PolizItem& poliz_item) const {
    return !(*this < poliz_item || *this == poliz_item);
}

const bool PolizItem::operator<=(const PolizItem& poliz_item) const {
    return *this < poliz_item || *this == poliz_item;
}

const bool PolizItem::operator>=(const PolizItem& poliz_item) const {
    return *this > poliz_item || *this == poliz_item;
}

PolizItem& PolizItem::operator++() {
    IDENT_ id("", CODE_TYPE_INT_, "1");
    PolizItem it(TYPE_CONST_, id);
    return *this = *this + it;
}

PolizItem& PolizItem::operator--() {
    IDENT_ id("", CODE_TYPE_INT_, "1");
    PolizItem it(TYPE_CONST_, id);
    return *this = *this - it;
}

const bool PolizItem::operator||(const PolizItem& poliz_item) const {
    return poliz_item == true || *this == true;
}

const bool PolizItem::operator&&(const PolizItem& poliz_item) const {
    return poliz_item == true && *this == true;
}

PolizItem PolizItem::operator&(const PolizItem& poliz_item) const {
    int val_first = 1;
    int val_second = 1;
    IDENT_ *id = nullptr;
    if (this->ident->type_ == CODE_TYPE_DOUBLE_ || poliz_item.ident->type_ == CODE_TYPE_DOUBLE_) {
        throw CODE_ERR_CANNOT_MAKE_OPERAITON_BITWISE_DOUBLE_;
    }
    if (this->ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (this->ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_first = 1;
        }
        else {
            val_first = 0;
        }
    } else if (this->ident->type_ == CODE_TYPE_INT_) {
        val_first = std::stoi(this->ident->cur_val[ident->cur_val.size() - 1]);
    }

    if (poliz_item.ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (poliz_item.ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_second = 1;
        }
        else {
            val_second = 0;
        }
    }
    else if (poliz_item.ident->type_ == CODE_TYPE_INT_) {
        val_second = std::stoi(poliz_item.ident->cur_val[ident->cur_val.size() - 1]);
    }

    id = new IDENT_("", CODE_TYPE_INT_, std::to_string(val_first & val_second));
    return PolizItem(TYPE_CONST_, *id);
}

PolizItem PolizItem::operator|(const PolizItem& poliz_item) const {
    int val_first = 1;
    int val_second = 1;
    IDENT_* id = nullptr;
    if (this->ident->type_ == CODE_TYPE_DOUBLE_ || poliz_item.ident->type_ == CODE_TYPE_DOUBLE_) {
        throw CODE_ERR_CANNOT_MAKE_OPERAITON_BITWISE_DOUBLE_;
    }
    if (this->ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (this->ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_first = 1;
        }
        else {
            val_first = 0;
        }
    }
    else if (this->ident->type_ == CODE_TYPE_INT_) {
        val_first = std::stoi(this->ident->cur_val[ident->cur_val.size() - 1]);
    }

    if (poliz_item.ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (poliz_item.ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_second = 1;
        }
        else {
            val_second = 0;
        }
    }
    else if (poliz_item.ident->type_ == CODE_TYPE_INT_) {
        val_second = std::stoi(poliz_item.ident->cur_val[ident->cur_val.size() - 1]);
    }

    id = new IDENT_("", CODE_TYPE_INT_, std::to_string(val_first | val_second));
    return PolizItem(TYPE_CONST_, *id);
}

PolizItem PolizItem::operator<<(const PolizItem& poliz_item) const {
    int val_first = 1;
    int val_second = 1;
    IDENT_* id = nullptr;
    if (this->ident->type_ == CODE_TYPE_DOUBLE_ || poliz_item.ident->type_ == CODE_TYPE_DOUBLE_) {
        throw CODE_ERR_CANNOT_MAKE_OPERAITON_BITWISE_DOUBLE_;
    }
    if (this->ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (this->ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_first = 1;
        }
        else {
            val_first = 0;
        }
    }
    else if (this->ident->type_ == CODE_TYPE_INT_) {
        val_first = std::stoi(this->ident->cur_val[ident->cur_val.size() - 1]);
    }

    if (poliz_item.ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1] == "true") {
            val_second = 1;
        }
        else {
            val_second = 0;
        }
    }
    else if (poliz_item.ident->type_ == CODE_TYPE_INT_) {
        val_second = std::stoi(poliz_item.ident->cur_val[poliz_item.ident->cur_val.size() - 1]);
    }

    id = new IDENT_("", CODE_TYPE_INT_, std::to_string(val_first << val_second));
    return PolizItem(TYPE_CONST_, *id);
}

PolizItem PolizItem::operator>>(const PolizItem& poliz_item) const {
    int val_first = 1;
    int val_second = 1;
    IDENT_* id = nullptr;
    if (this->ident->type_ == CODE_TYPE_DOUBLE_ || poliz_item.ident->type_ == CODE_TYPE_DOUBLE_) {
        throw CODE_ERR_CANNOT_MAKE_OPERAITON_BITWISE_DOUBLE_;
    }
    if (this->ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (this->ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_first = 1;
        }
        else {
            val_first = 0;
        }
    }
    else if (this->ident->type_ == CODE_TYPE_INT_) {
        val_first = std::stoi(this->ident->cur_val[ident->cur_val.size() - 1]);
    }

    if (poliz_item.ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (poliz_item.ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_second = 1;
        }
        else {
            val_second = 0;
        }
    }
    else if (poliz_item.ident->type_ == CODE_TYPE_INT_) {
        val_second = std::stoi(poliz_item.ident->cur_val[ident->cur_val.size() - 1]);
    }

    id = new IDENT_("", CODE_TYPE_INT_, std::to_string(val_first >> val_second));
    return PolizItem(TYPE_CONST_, *id);
}

PolizItem& PolizItem::operator~() {
    int val_first = 1;
    IDENT_* id = nullptr;
    if (this->ident->type_ == CODE_TYPE_DOUBLE_) {
        throw CODE_ERR_CANNOT_MAKE_OPERAITON_BITWISE_DOUBLE_;
    }
    if (this->ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (this->ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_first = 1;
        }
        else {
            val_first = 0;
        }
    }
    else if (this->ident->type_ == CODE_TYPE_INT_) {
        val_first = std::stoi(this->ident->cur_val[ident->cur_val.size() - 1]);
    }
    val_first = ~val_first;
    this->ident->cur_val[ident->cur_val.size() - 1] = std::to_string(val_first);
    return *this;
}

PolizItem PolizItem::operator^(const PolizItem& poliz_item) const {
    int val_first = 1;
    int val_second = 1;
    IDENT_* id = nullptr;
    if (this->ident->type_ == CODE_TYPE_DOUBLE_ || poliz_item.ident->type_ == CODE_TYPE_DOUBLE_) {
        throw CODE_ERR_CANNOT_MAKE_OPERAITON_BITWISE_DOUBLE_;
    }
    if (this->ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (this->ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_first = 1;
        }
        else {
            val_first = 0;
        }
    }
    else if (this->ident->type_ == CODE_TYPE_INT_) {
        val_first = std::stoi(this->ident->cur_val[ident->cur_val.size() - 1]);
    }

    if (poliz_item.ident->type_ == CODE_TYPE_BOOLEAN_) {
        if (poliz_item.ident->cur_val[ident->cur_val.size() - 1] == "true") {
            val_second = 1;
        }
        else {
            val_second = 0;
        }
    }
    else if (poliz_item.ident->type_ == CODE_TYPE_INT_) {
        val_second = std::stoi(poliz_item.ident->cur_val[ident->cur_val.size() - 1]);
    }

    id = new IDENT_("", CODE_TYPE_INT_, std::to_string(val_first ^ val_second));
    return PolizItem(TYPE_CONST_, *id);
}

