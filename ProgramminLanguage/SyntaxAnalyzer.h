#pragma once
#include "SemanticAnalyzer.h"
#include "GenCode.h"

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer(std::vector<std::pair<int, std::string>>, GenCode&);
    ~SyntaxAnalyzer();

    void StartSyntaxAnalyzer();

private:
    std::vector<std::pair<int, std::string>> list_of_tokens_;
    std::vector<int> num_of_word_in_each_str_;
    unsigned int cur_token_;
    bool flag_is_main;

    std::string getLineOfError_();
    void getReadyToken_();

    void AnalyzeProgramm_();
    void AnalyzeGlobalIdentifierOrFunc_();
    void AnalyzeParamsOfFuncProcedure_(bool, bool);
    void AnalyzeParamsMain_();
    void AnalyzeIdentificator_();
    void AnalyzeName_();
    void AnalyzeServiceType_();
    void AnalyzeCompoundOperator_(bool);
    void AnalyzeOperators_();
    void AnalyzeOperatorInput_();
    void AnalyzeOperatorOutput_();
    void AnalyzeParamsOperatorOutput_(bool);

    short int AnalyzeOperatorExpression_(bool);
    void AnalyzePriorityAssignable_();
    void AnalyzePriority_1_();
    void AnalyzePriority_2_();
    void AnalyzePriority_3_();
    void AnalyzePriority_4_();
    void AnalyzePriority_5_();
    void AnalyzePriority_6_();
    void AnalyzePriority_7_();
    void AnalyzePriority_8_();
    void AnalyzePriority_9_();
    void AnalyzePriority_10_();
    void AnalyzePriority_11_();
    void AnalyzePriority_12_();
    void AnalyzePriority_13_();
    void AnalyzePriority_14_();
    void AnalyzePriority_15_();

    void AnalyzeCallFuncOrIdentifier_();

    void AnalyzeCallFuncParams_(bool, std::string, int);

    void AnalyzeOperatorWhile_();
    void AnalyzeOperatorIf_();
    void AnalyzeOperatorDoWhile_();
    void AnalyzeOperatorFor_();
    void AnalyzeParamsFor_(short int);
    void AnalyzeOperatorElFor_();
    void AnalyzeOperatorElseElFor_();

    void AnalyzeOperatorReturn_();

    void ChechCorrectName_(std::string);
    void CheckCorrectConstant_(short int);// 0 любая константа 1 - число 2 - логика
    bool CheckCorrectNumberConstant_();
    bool CheckCorrectLogicalConstant_();

    SemanticAnalyzer* semantic_analyzer;
    GenCode* generator;
};

