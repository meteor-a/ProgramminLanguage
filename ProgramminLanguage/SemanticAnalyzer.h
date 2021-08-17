#pragma once
#include <string>
#include<vector>
#include <iostream>
#include "Constants.h"
#include <stack>
#include "Types.h"
#include "GenCode.h"

class SemanticAnalyzer : public Types {
private:
    TID_* CUR_TID_;
    bool StartCheckFunc;
    bool AnalyzeCallFunc;
    bool AnalyzeExpr;

    FUNC_ new_func;
    FUNC_ call_func;
    
    std::vector<std::string> expr;
    std::string check_compatible_op_and_st_(std::string, std::string, std::string, bool);
    bool may_unary;
    
    std::string getTypeByCode_(short int);
    short int getCodeType(std::string, short int type);

    GenCode* generator;

public:
    SemanticAnalyzer(GenCode&);
    ~SemanticAnalyzer();
    
    void pushID(std::string, std::string);
    bool deepSearchID(std::string);
    void IsIDDeclarring(std::string);

    void pushFUNC(std::string, std::string);
    void pushParamsNewFUNC(std::string, std::string);
    void finishPushFunc();
    bool deepSearchFUNC(FUNC_);
    void backLVLTID();

    void goNextLVLTID(short int);

    void CheckParamsOperatorOutput(std::string);
    IDENT_* getIdent(std::string name) {
        IDENT_ *ident = nullptr;
        if (MAIN_TID_ == nullptr) {
            MAIN_TID_ = new TID_(nullptr);
            CUR_TID_ = MAIN_TID_;
            return nullptr;
        }
        TID_* TMP = CUR_TID_;
        while (TMP != nullptr) {
            int ss = TMP->identifiers.num_of_ident;
            for (int ii = 0; ii < ss; ++ii) {
                if (name == TMP->identifiers.idents[ii]->id_) {
                    ident = TMP->identifiers.idents[ii];
                    return ident;
                }
            }
            TMP = TMP->prev_;
        }
        return nullptr;
    }

    void CheckParamsOperatorInput(std::string);

    void setNameCallFunc(std::string);
    void addCallParamsFunc(std::string name, short int type, std::string);
    int stopAnalyzeCallFunc();

    void start_expr();
    void addOperand(std::string, short int, int, std::string);
    void addOperation(std::string);
    short int calc_expr();

    bool FindTypeTID(short int);
    void setGeneratorTid();
};

