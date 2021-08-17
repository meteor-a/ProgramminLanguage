#pragma once
#include "Types.h"
#include "Constants.h"

class PolizItem : public Types {
public:
    PolizItem(short int type); // ��� ������
    PolizItem(short int type, IDENT_&);  // ��� ���������� �������� ������ �������
    PolizItem(short int type, IDENT_&, bool); // ��� ��������� ������
    PolizItem(short int type, long long); // ��� �������
    PolizItem(short int type, std::string); // ��� ��������

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

    // �������� ������������
    PolizItem& operator=(const PolizItem& poliz_item);
    PolizItem& operator+=(const PolizItem& poliz_item);
    PolizItem& operator-=(const PolizItem& poliz_item);
    PolizItem& operator*=(const PolizItem& poliz_item);
    PolizItem& operator/=(const PolizItem& poliz_item);
    PolizItem& operator%=(const PolizItem& poliz_item);

    // �������������� ��������
    PolizItem operator+(const PolizItem& poliz_item) const;
    PolizItem operator-(const PolizItem& poliz_item) const;
    PolizItem operator*(const PolizItem& poliz_item) const;
    PolizItem operator/(const PolizItem& poliz_item) const;
    PolizItem operator%(const PolizItem& poliz_item) const;

    // �������� ���������
    const bool operator==(const PolizItem& poliz_item) const;
    const bool operator==(const bool& boolean) const;
    const bool operator!=(const PolizItem& poliz_item) const;

    const bool operator<(const PolizItem& poliz_item) const;
    const bool operator>(const PolizItem& poliz_item) const;
    const bool operator<=(const PolizItem& poliz_item) const;
    const bool operator>=(const PolizItem& poliz_item) const;

    // ���������� �����
    PolizItem& operator++(); // ++ii
    PolizItem& operator--(); // --ii

    // ������
    const bool operator||(const PolizItem& poliz_item) const;
    const bool operator&&(const PolizItem& poliz_item) const;

    // �������
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
    bool unary_; // ���� �������� �� ������� ��� ���; ���� �������������, �� �������� �� �� ���������� ������ ��� ���
};

