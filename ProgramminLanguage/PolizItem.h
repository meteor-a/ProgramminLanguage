#pragma once
#include "Types.h"
#include "Constants.h"

class PolizItem : public Types {
public:
    PolizItem(short int type); // для блоков
    PolizItem(short int type, IDENT_&);  // для переменных констант вызова функций
    PolizItem(short int type, IDENT_&, bool); // для парметров вызова
    PolizItem(short int type, long long); // для функций
    PolizItem(short int type, std::string); // Для операций

    bool getUnary() {
        return unary_;
    }

    void setUnary(bool unar) {
        unary_ = unar;
    }

    short int getTypeItem() {
        return type_item_;
    }

    std::string getOper() {
        return operation;
    }

    IDENT_* getIdent() {
        return ident;
    }

    void setPos(size_t pos) {
        pos_ = pos;
    }

    void setVal(std::string val) {
        ident->cur_val[ident->cur_val.size() - 1] = val;
    }

    void setType(unsigned short int type) {
        type_item_ = type;
    }

    size_t getPos() {
        return pos_;
    }

    // операция присваивания
    PolizItem& operator=(const PolizItem& poliz_item);
    PolizItem& operator+=(const PolizItem& poliz_item);
    PolizItem& operator-=(const PolizItem& poliz_item);
    PolizItem& operator*=(const PolizItem& poliz_item);
    PolizItem& operator/=(const PolizItem& poliz_item);
    PolizItem& operator%=(const PolizItem& poliz_item);

    // арифметические операции
    PolizItem operator+(const PolizItem& poliz_item) const;
    PolizItem operator-(const PolizItem& poliz_item) const;
    PolizItem operator*(const PolizItem& poliz_item) const;
    PolizItem operator/(const PolizItem& poliz_item) const;
    PolizItem operator%(const PolizItem& poliz_item) const;

    // операции сравнения
    const bool operator==(const PolizItem& poliz_item) const;
    const bool operator==(const bool& boolean) const;
    const bool operator!=(const PolizItem& poliz_item) const;

    const bool operator<(const PolizItem& poliz_item) const;
    const bool operator>(const PolizItem& poliz_item) const;
    const bool operator<=(const PolizItem& poliz_item) const;
    const bool operator>=(const PolizItem& poliz_item) const;

    // префиксная форма
    PolizItem& operator++(); // ++ii
    PolizItem& operator--(); // --ii

    // логика
    const bool operator||(const PolizItem& poliz_item) const;
    const bool operator&&(const PolizItem& poliz_item) const;

    // битовые
    PolizItem operator&(const PolizItem& poliz_item) const;
    PolizItem operator|(const PolizItem& poliz_item) const;
    PolizItem operator<<(const PolizItem& poliz_item) const;
    PolizItem operator>>(const PolizItem& poliz_item) const;
    PolizItem& operator~(); // ~ii
    PolizItem operator^(const PolizItem& poliz_item) const;
private:
    short int type_item_;
    IDENT_* ident;
    size_t pos_;
    std::string operation;
    bool unary_; // если операция то унарная или нет; если идентификатор, то является ли он параметром вызова или нет
};

