#pragma once
#include "Types.h"
#include <vector>
#include <string>
#include <stack>
#include <iostream>
#include "Constants.h"
#include "PolizItem.h"



class GenCode : public Types {
public:
    void insert_start_block();
    void insert_end_block();
    void insert_write_func();
    void insert_read_func();
    void insert_semicolon();
    void insert_return();
    void insert_declaration_func(FUNC_);
    void insert_expr_poliz(short int type);
    void insert_expr_poliz(short int type, IDENT_& id);
    void insert_expr_poliz(short int type, IDENT_& id, bool is_call_func_param);
    void insert_expr_poliz(short int type, std::string);
    void insert_expr_poliz(short int, short int, std::string);
    size_t insert_conditional_jump();
    size_t insert_conditional_jump_dowhile(size_t);
    size_t insert_unconditional_jump();
    size_t insert_unconditional_jump(size_t);
    void push_pos_break(size_t);
    size_t top_pos_break();
    size_t getSizePosBreak();
    void pop_pos_break();
    void insert_index_jump(size_t);
    void genPolizExpr();
    size_t getSizePoliz();
    void StartProgramm();
    void setStartPos();
    void setTID(TID_*);
    void RELOAD();
private:
    std::vector<PolizItem> expr_poliz;
    int start_pos_;
    std::vector<PolizItem> POLIZ_;
    std::vector<std::pair<int, FUNC_>> functions;

    std::stack<size_t> pos_break_;
    void InterputProgrammMain_(size_t, TID_*);
    unsigned int FindFuncNum(int);
    std::pair<int, std::string> InterputFunc_(size_t, std::vector<IDENT_>, bool, unsigned int);
    PolizItem input_poliz_item(std::string);
    
    PolizItem make_operation(PolizItem, PolizItem, PolizItem);
    void make_operation_assignm(PolizItem, PolizItem, PolizItem);
    PolizItem make_operation(PolizItem, PolizItem);

    unsigned int FindPosFunc_(std::string, std::vector<IDENT_>);
    void GetReadyFunc_(int, std::vector<IDENT_>);
    void GetBackFuncAfterCall_(int);

};

