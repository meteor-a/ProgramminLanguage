#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(std::vector<std::pair<int, std::string>> tokens, GenCode& gen_) {
    cur_token_ = 0;
    list_of_tokens_ = tokens;
    flag_is_main = false;
    generator = &gen_;
    semantic_analyzer = new SemanticAnalyzer(*generator);
}

SyntaxAnalyzer::~SyntaxAnalyzer(){
    list_of_tokens_.clear();
}

void SyntaxAnalyzer::StartSyntaxAnalyzer() {
    getReadyToken_();
    try {
        AnalyzeProgramm_();
        semantic_analyzer->setGeneratorTid();
    }
    catch (const short int err_code) {
        throw std::make_pair(err_code, LINE_ERR_ + getLineOfError_());
    }
    if (!flag_is_main) {
        //не было main + сделать проверку что Main последний
    }
}

std::string SyntaxAnalyzer::getLineOfError_() {
    unsigned int sum_word = 0;
    for (int ii = 0; ii < num_of_word_in_each_str_.size(); ++ii) {
        sum_word += num_of_word_in_each_str_[ii];
        if (sum_word > cur_token_) {
            return std::to_string((ii + 1));
        }
    }
    return std::to_string(-1);
}

void SyntaxAnalyzer::getReadyToken_() {
    int num_of_str = 0;
    num_of_word_in_each_str_.resize(1);
    for (int ii = 0; ii < list_of_tokens_.size(); ++ii) {
        if (list_of_tokens_[ii].first == TYPE_NUMBER_NEW_STRING_CATEGORY_) {
            ++num_of_str;
            num_of_word_in_each_str_.resize(num_of_str + 1);
            list_of_tokens_.erase(list_of_tokens_.begin() + ii);
            --ii;
        }
        else {
            ++num_of_word_in_each_str_[num_of_str];
        }
    }
}

void SyntaxAnalyzer::AnalyzeProgramm_() {
    if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SERVICE_TYPE_CATEGORY_) {
        ++cur_token_;
        AnalyzeGlobalIdentifierOrFunc_();
    }
    else if (list_of_tokens_[cur_token_].second == "def") {
        ++cur_token_;
        AnalyzeName_();
        generator->insert_start_block();
        semantic_analyzer->pushFUNC(list_of_tokens_[cur_token_ - 2].second, list_of_tokens_[cur_token_ - 1].second);
        AnalyzeParamsOfFuncProcedure_(false, false);
        semantic_analyzer->finishPushFunc();
        AnalyzeCompoundOperator_(true);
        semantic_analyzer->backLVLTID();
        generator->insert_end_block();
    }
    else if (list_of_tokens_[cur_token_].second == "main") {
        flag_is_main = true;
        ++cur_token_;
        generator->setStartPos();
        generator->insert_start_block();
        semantic_analyzer->pushFUNC(list_of_tokens_[cur_token_ - 1].second, list_of_tokens_[cur_token_ - 1].second);
        AnalyzeParamsMain_();
        semantic_analyzer->finishPushFunc();
        AnalyzeCompoundOperator_(true);
        semantic_analyzer->backLVLTID();
        generator->insert_end_block();
        if ((cur_token_ == list_of_tokens_.size())) {
            return;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_IDENTIFIER_CATEGORY_) {
        AnalyzeOperatorExpression_(true);
        if (cur_token_ >= list_of_tokens_.size()) {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
        if (list_of_tokens_[cur_token_].first != TYPE_NUMBER_SEMICOLON_CATEGORY_) {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
        else {
            ++cur_token_;
        }
    }
    else if ((cur_token_ == list_of_tokens_.size() - 1)) {
        throw std::make_pair(CODE_ERR_NOT_FINED_MAIN_, LINE_ERR_ + getLineOfError_());
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
    }

    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_IDENTIFIER_CATEGORY_ ||
            list_of_tokens_[cur_token_].first == TYPE_NUMBER_SERVICE_TYPE_CATEGORY_ ||
            list_of_tokens_[cur_token_].second == "def" ||
            list_of_tokens_[cur_token_].second == "main") {

            AnalyzeProgramm_();
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else if ((cur_token_ == list_of_tokens_.size()) && !flag_is_main) {
        throw std::make_pair(CODE_ERR_NOT_FINED_MAIN_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeGlobalIdentifierOrFunc_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzeName_();
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
                generator->insert_start_block();
                semantic_analyzer->pushFUNC(list_of_tokens_[cur_token_ - 2].second, list_of_tokens_[cur_token_ - 1].second);
                AnalyzeParamsOfFuncProcedure_(false, false);
                semantic_analyzer->finishPushFunc();
                AnalyzeCompoundOperator_(true);
                semantic_analyzer->backLVLTID();
                generator->insert_end_block();
            }
            else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                semantic_analyzer->pushID(list_of_tokens_[cur_token_- 2].second, list_of_tokens_[cur_token_ - 1].second);
                ++cur_token_;
                return;
            }
            else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_ ||
                list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_) {
                
                if (cur_token_ - 2 >= 0) {
                    if (list_of_tokens_[cur_token_ - 2].first == TYPE_NUMBER_SERVICE_TYPE_CATEGORY_) {
                        cur_token_ -= 2;
                    }
                    else {
                        cur_token_ -= 1;
                    }
                }
                else {
                    cur_token_ -= 1;
                }
                AnalyzeOperatorExpression_(true);
                generator->insert_semicolon();
                if (cur_token_ >= list_of_tokens_.size()) {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
                }
                if (list_of_tokens_[cur_token_].first != TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
                }
                else {
                    ++cur_token_;
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeParamsOfFuncProcedure_(bool is_was_identif, bool is_was_opening_parenth) {
    if (cur_token_ < list_of_tokens_.size()) {
        if (is_was_opening_parenth) {
            if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SERVICE_TYPE_CATEGORY_ && !is_was_identif) {
                AnalyzeIdentificator_();
                semantic_analyzer->pushParamsNewFUNC(list_of_tokens_[cur_token_ - 2].second, list_of_tokens_[cur_token_ - 1].second);
                AnalyzeParamsOfFuncProcedure_(true, true);
            }
            else if (list_of_tokens_[cur_token_].second == "," && is_was_identif) {
                ++cur_token_;
                AnalyzeParamsOfFuncProcedure_(false, true);
            }
            else if ((list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_ && is_was_identif) ||
                (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_ && 
                    list_of_tokens_[cur_token_ - 1].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_)) {

                ++cur_token_;
                return;
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (!is_was_opening_parenth && list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
            ++cur_token_;
            AnalyzeParamsOfFuncProcedure_(false, true);
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeParamsMain_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
            ++cur_token_;
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_) {
                    ++cur_token_;
                    return;
                }
                else {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeIdentificator_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzeServiceType_();
        AnalyzeName_();
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeName_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_IDENTIFIER_CATEGORY_) {
            ChechCorrectName_(list_of_tokens_[cur_token_].second);
            ++cur_token_;
            
            return;
        }
        else {
            throw std::make_pair(CODE_ERR_NAME_IDENTIFIER_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeServiceType_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SERVICE_TYPE_CATEGORY_) {
            ++cur_token_;
            return;
        }
        else {
            throw std::make_pair(CODE_ERR_SPECIFY_IDENTIFIER_TYPE_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeCompoundOperator_(bool is_open_bracket) {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].second == "{" && is_open_bracket) {
            is_open_bracket = false;
            ++cur_token_;
            AnalyzeOperators_();
        }

        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "}" && !is_open_bracket) {
                ++cur_token_;
                return;
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeOperators_() {
    tryAgainOperator:
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].second == "read") {
            ++cur_token_;
            AnalyzeOperatorInput_();
            generator->insert_semicolon();
        }
        else if (list_of_tokens_[cur_token_].second == "write") {
            ++cur_token_;
            AnalyzeOperatorOutput_();
            generator->insert_semicolon();
        }
        else if ((list_of_tokens_[cur_token_].first == TYPE_NUMBER_SERVICE_TYPE_CATEGORY_) || 
            (list_of_tokens_[cur_token_].second == "++" || list_of_tokens_[cur_token_].second == "--")) {
            AnalyzeOperatorExpression_(false);
            generator->insert_semicolon();
            if (cur_token_ >= list_of_tokens_.size()) {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
            if (list_of_tokens_[cur_token_].first != TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
            }
            else {
                ++cur_token_;
            }
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_IDENTIFIER_CATEGORY_) {
            AnalyzeName_();
            AnalyzeCallFuncOrIdentifier_();
            semantic_analyzer->calc_expr();
            generator->insert_semicolon();
        }
        else if (list_of_tokens_[cur_token_].second == "while") {
            ++cur_token_;
            AnalyzeOperatorWhile_();
        }
        else if (list_of_tokens_[cur_token_].second == "if") {
            ++cur_token_;
            AnalyzeOperatorIf_();
        }
        else if (list_of_tokens_[cur_token_].second == "do") {
            ++cur_token_;
            AnalyzeOperatorDoWhile_();
        }
        else if (list_of_tokens_[cur_token_].second == "for") {
            ++cur_token_;
            AnalyzeOperatorFor_();
        }
        else if (list_of_tokens_[cur_token_].second == "elfor") {
            ++cur_token_;
            AnalyzeOperatorElFor_();
        }
        else if (list_of_tokens_[cur_token_].second == "}") {
            return;
        }
        else if (list_of_tokens_[cur_token_].second == "break") {
            ++cur_token_;
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                    if (semantic_analyzer->FindTypeTID(CODE_TYPE_TID_FOR_) || semantic_analyzer->FindTypeTID(CODE_TYPE_TID_ELFOR_) ||
                        semantic_analyzer->FindTypeTID(CODE_TYPE_TID_WHILE_) || semantic_analyzer->FindTypeTID(CODE_TYPE_TID_DO_WHILE_)) {
                        
                        generator->push_pos_break(generator->insert_unconditional_jump());
                        ++cur_token_;
                    }
                    else {
                        throw CODE_ERR_INCORRECT_PLACE_BREAK_;
                    }
                }
                else {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (list_of_tokens_[cur_token_].second == "return") {
            ++cur_token_;
            AnalyzeOperatorReturn_();
            generator->insert_return();
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }

        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "}") {
                return;
            }
            else {
                goto tryAgainOperator;
            }
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeCallFuncOrIdentifier_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
            semantic_analyzer->setNameCallFunc(list_of_tokens_[cur_token_ - 1].second);
            ++cur_token_;
            AnalyzeCallFuncParams_(false, list_of_tokens_[cur_token_ - 2].second, 0);
            int id_func_call = semantic_analyzer->stopAnalyzeCallFunc();
            semantic_analyzer->addOperand(list_of_tokens_[cur_token_ - 1].second, 4, id_func_call, "");
            if (cur_token_ >= list_of_tokens_.size()) {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
            if (list_of_tokens_[cur_token_].first != TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
            }
            else {
                ++cur_token_;
            }
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_ || 
            list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_) {

            --cur_token_;
            AnalyzeOperatorExpression_(false);
            generator->insert_semicolon();
            if (cur_token_ >= list_of_tokens_.size()) {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
            if (list_of_tokens_[cur_token_].first != TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
            }
            else {
                ++cur_token_;
            }
        }
        else {

        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeOperatorInput_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
            ++cur_token_;
            AnalyzeName_();
            semantic_analyzer->CheckParamsOperatorInput(list_of_tokens_[cur_token_ - 1].second);
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_) {
                    ++cur_token_;
                    if (cur_token_ < list_of_tokens_.size()) {
                        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                            ++cur_token_;
                            generator->genPolizExpr();
                            generator->insert_read_func();
                            return;
                        }
                        else {
                            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
                        }
                    }
                    else {
                        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
                    }
                }
                else {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeOperatorOutput_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
            ++cur_token_;
            AnalyzeParamsOperatorOutput_(false);
            generator->genPolizExpr();
            generator->insert_write_func();
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                    ++cur_token_;
                    return;
                }
                else {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeParamsOperatorOutput_(bool is_waiting_comma) {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_IDENTIFIER_CATEGORY_ && !is_waiting_comma) {
            AnalyzeName_();
            generator->insert_expr_poliz(TYPE_IDENTIFICATOR_, *semantic_analyzer->getIdent(list_of_tokens_[cur_token_ - 1].second));
            semantic_analyzer->CheckParamsOperatorOutput(list_of_tokens_[cur_token_ - 1].second);
            AnalyzeParamsOperatorOutput_(true);
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CONSTANT_STR_CATEGORY_ && !is_waiting_comma) {
            generator->insert_expr_poliz(TYPE_CONST_, CODE_TYPE_STRING_, list_of_tokens_[cur_token_].second);
            ++cur_token_;
            AnalyzeParamsOperatorOutput_(true);
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_ && is_waiting_comma) {
            ++cur_token_;
            return;
        }
        else if (list_of_tokens_[cur_token_].second == "," && is_waiting_comma) {
            ++cur_token_;
            AnalyzeParamsOperatorOutput_(false);
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

short int SyntaxAnalyzer::AnalyzeOperatorExpression_(bool is_global) {
    if (cur_token_ < list_of_tokens_.size()) {
        semantic_analyzer->start_expr();
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_IDENTIFIER_CATEGORY_ && !is_global) {
            AnalyzeName_();
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_ ||
                    list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_) {

                    semantic_analyzer->IsIDDeclarring(list_of_tokens_[cur_token_ - 1].second);
                    semantic_analyzer->addOperand(list_of_tokens_[cur_token_ - 1].second, 0, -1, "");
                    AnalyzePriority_1_();
                    AnalyzePriority_2_();
                }
                else {
                    --cur_token_;
                    AnalyzePriority_2_();
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SERVICE_TYPE_CATEGORY_) {
            AnalyzeIdentificator_();
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_ && !is_global) {
                    
                    semantic_analyzer->pushID(list_of_tokens_[cur_token_ - 2].second, list_of_tokens_[cur_token_ - 1].second);
                    semantic_analyzer->addOperand(list_of_tokens_[cur_token_ - 1].second, TYPE_IDENTIFICATOR_AD_, -1, "");
                    AnalyzePriorityAssignable_();
                }
                else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                    semantic_analyzer->pushID(list_of_tokens_[cur_token_ - 2].second, list_of_tokens_[cur_token_ - 1].second);
                    semantic_analyzer->addOperand(list_of_tokens_[cur_token_ - 1].second, TYPE_IDENTIFICATOR_AD_, -1, "");
                    return semantic_analyzer->calc_expr();
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
            AnalyzePriority_2_();
        }
        else  if (!is_global) {
            AnalyzePriority_2_();
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }

    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }

    return semantic_analyzer->calc_expr();
}
// выше переделать ошибки
void SyntaxAnalyzer::AnalyzePriorityAssignable_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_) {
            semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
            ++cur_token_;
            return;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_1_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_ ||
            list_of_tokens_[cur_token_].first == TYPE_NUMBER_ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_) {

            semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
            ++cur_token_;
            return;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_2_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_3_();
        tryAgain_priority_2_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "||") {
                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_3_();
            }
            else {
                return;
            }
            goto tryAgain_priority_2_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_3_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_4_();
        tryAgain_priority_3_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "&&") {
                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_4_();
            }
            else {
                return;
            }
            goto tryAgain_priority_3_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_4_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_5_();
        tryAgain_priority_4_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "|") {
                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_5_();
            }
            else {
                return;
            }
            goto tryAgain_priority_4_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_5_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_6_();
        tryAgain_priority_5_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "^") {
                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_6_();
            }
            else {
                return;
            }
            goto tryAgain_priority_5_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_6_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_7_();
    tryAgain_priority_6_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "&") {
                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_7_();
            }
            else {
                return;
            }
            goto tryAgain_priority_6_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_7_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_8_();
    tryAgain_priority_7_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "==" || list_of_tokens_[cur_token_].second == "<>") {
                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_8_();
            }
            else {
                return;
            }
            goto tryAgain_priority_7_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_8_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_9_();
    tryAgain_priority_8_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "<" || list_of_tokens_[cur_token_].second == ">" || 
                list_of_tokens_[cur_token_].second == "<=" || list_of_tokens_[cur_token_].second == ">=") {

                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_9_();
            }
            else {
                return;
            }
            goto tryAgain_priority_8_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_9_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_10_();
    tryAgain_priority_9_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "<<" || list_of_tokens_[cur_token_].second == ">>") {
                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_10_();
            }
            else {
                return;
            }
            goto tryAgain_priority_9_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_10_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_11_();
    tryAgain_priority_10_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "+" || list_of_tokens_[cur_token_].second == "-") {
                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_11_();
            }
            else {
                return;
            }
            goto tryAgain_priority_10_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_11_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_12_();
    tryAgain_priority_11_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "*" || list_of_tokens_[cur_token_].second == "/" ||
                list_of_tokens_[cur_token_].second == "%") {

                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_12_();
            }
            else {
                return;
            }
            goto tryAgain_priority_11_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_12_() {
    if (cur_token_ < list_of_tokens_.size()) {
    tryAgain_priority_12_:
        if (list_of_tokens_[cur_token_].second == "!") {
            semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
            ++cur_token_;
            AnalyzePriority_13_();
        }
        else if (list_of_tokens_[cur_token_].second == "-") {
            semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
            ++cur_token_;
            AnalyzePriority_13_();
        }
        else if (list_of_tokens_[cur_token_].second == "~") {
            semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
            ++cur_token_;
            AnalyzePriority_13_();
        }
        else {
            AnalyzePriority_13_();
        }

        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "!" || list_of_tokens_[cur_token_].second == "-" || list_of_tokens_[cur_token_].second == "~") {
                goto tryAgain_priority_12_;
            }
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
        
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_13_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].second == "++" || list_of_tokens_[cur_token_].second == "--") {
            semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
            ++cur_token_;
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_IDENTIFIER_CATEGORY_) {
                    ++cur_token_;
                    if (cur_token_ < list_of_tokens_.size()) {
                        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
                            throw CODE_ERR_INCRIMENT_DECRIMENT_VARIABLE;
                        }
                        else {
                            --cur_token_;
                        }
                    }
                    else {
                        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
                    }
                }
                else {
                    throw CODE_ERR_INCRIMENT_DECRIMENT_VARIABLE;
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
            AnalyzePriority_14_();
        }
        else {
            AnalyzePriority_14_();
        }
        
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_14_() {
    if (cur_token_ < list_of_tokens_.size()) {
        AnalyzePriority_15_();
    tryAgain_priority_14_:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "+" || list_of_tokens_[cur_token_].second == "-") {
                semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                ++cur_token_;
                AnalyzePriority_15_();
            }
            else {
                return;
            }
            goto tryAgain_priority_14_;
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzePriority_15_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CONSTANT_NUM_CATEGORY_) {
            CheckCorrectConstant_(1);
            semantic_analyzer->addOperand("", 3, -1, list_of_tokens_[cur_token_].second);
            ++cur_token_;
            if (cur_token_ >= list_of_tokens_.size()) {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CONSTANT_STR_CATEGORY_) {
            semantic_analyzer->addOperand("", 1, -1, list_of_tokens_[cur_token_].second);
            ++cur_token_;
            if (cur_token_ >= list_of_tokens_.size()) {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SYSTEM_CONSTANT_CATEGORY_) {
            semantic_analyzer->addOperand("", 2, -1, list_of_tokens_[cur_token_].second);
            ++cur_token_;
            if (cur_token_ >= list_of_tokens_.size()) {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
            semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
            ++cur_token_;
            AnalyzePriority_2_();
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_) {
                    semantic_analyzer->addOperation(list_of_tokens_[cur_token_].second);
                    ++cur_token_;
                }
                else {
                    throw std::make_pair(CODE_ERR_PARENTHESES_IN_EXPRESSION_, LINE_ERR_ + getLineOfError_());
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_IDENTIFIER_CATEGORY_) {
            AnalyzeName_();
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
                    semantic_analyzer->setNameCallFunc(list_of_tokens_[cur_token_ - 1].second);
                    ++cur_token_;
                    AnalyzeCallFuncParams_(false, list_of_tokens_[cur_token_ - 2].second, 0);
                    int id_func_call = semantic_analyzer->stopAnalyzeCallFunc();
                    semantic_analyzer->addOperand(list_of_tokens_[cur_token_ - 1].second, 4, id_func_call, "");
                }
                else {
                    semantic_analyzer->IsIDDeclarring(list_of_tokens_[cur_token_ - 1].second);
                    semantic_analyzer->addOperand(list_of_tokens_[cur_token_ - 1].second, 0, -1, "");
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (list_of_tokens_[cur_token_].second == "!") {
            AnalyzePriority_2_();
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeCallFuncParams_(bool is_was_comma, std::string name_func, int num_of_params) {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_IDENTIFIER_CATEGORY_) {
            AnalyzeName_();
            semantic_analyzer->addCallParamsFunc(list_of_tokens_[cur_token_ - 1].second, 0, "");
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CONSTANT_STR_CATEGORY_) {
            ++cur_token_;
            semantic_analyzer->addCallParamsFunc("", 1, list_of_tokens_[cur_token_].second);
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SYSTEM_CONSTANT_CATEGORY_) {
            CheckCorrectConstant_(0);
            semantic_analyzer->addCallParamsFunc("", 2, list_of_tokens_[cur_token_].second);
            ++cur_token_;
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CONSTANT_NUM_CATEGORY_) {
            CheckCorrectConstant_(0);
            semantic_analyzer->addCallParamsFunc("", 3, list_of_tokens_[cur_token_].second);
            ++cur_token_;
        }
        else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_ && !is_was_comma) {
            ++cur_token_;
            return;
        }
        else {
            throw std::make_pair(CODE_ERR_INVALID_FUNC_ARGUMENTS_, LINE_ERR_ + getLineOfError_());
        }

        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == ",") {
                ++cur_token_;
                AnalyzeCallFuncParams_(true, name_func, num_of_params + 1);
            }
            else if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_) {
                ++cur_token_;
                return;
            }
            else {
                throw std::make_pair(CODE_ERR_INVALID_FUNC_ARGUMENTS_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeOperatorWhile_() {
    unsigned int index_unconditional_jump = generator->getSizePoliz();
    short int type_expr = AnalyzeOperatorExpression_(false);
    if (type_expr == CODE_TYPE_STRING_) {
        throw CODE_ERR_INCORRECT_TYPE_EXPR_WHILE_;
    }
    semantic_analyzer->goNextLVLTID(CODE_TYPE_TID_WHILE_);
    unsigned int index_conditional_jump = generator->insert_conditional_jump();
    generator->insert_semicolon();
    AnalyzeCompoundOperator_(true);
    generator->insert_unconditional_jump(index_unconditional_jump);
    generator->insert_index_jump(index_conditional_jump);
    if (generator->getSizePosBreak() > 0) {
        generator->insert_index_jump(generator->top_pos_break());
    }
    generator->insert_semicolon();
    semantic_analyzer->backLVLTID();
}

void SyntaxAnalyzer::AnalyzeOperatorIf_() {
    if (cur_token_ < list_of_tokens_.size()) {
        short int type_expr = AnalyzeOperatorExpression_(false);
        if (type_expr == CODE_TYPE_STRING_) {
            throw CODE_ERR_INCORRECT_TYPE_EXPR_IF_;
        }
        semantic_analyzer->goNextLVLTID(CODE_TYPE_TID_IF_);
        unsigned int index_gen = generator->insert_conditional_jump();
        generator->insert_semicolon();
        AnalyzeCompoundOperator_(true);
        std::vector<unsigned int> index_gen_2_;
        index_gen_2_.push_back(generator->insert_unconditional_jump());
        generator->insert_index_jump(index_gen);
        generator->insert_semicolon();
        semantic_analyzer->backLVLTID();
        tryAgain_if:
        if (cur_token_ < list_of_tokens_.size()) {
            if (list_of_tokens_[cur_token_].second == "elseif") {
                ++cur_token_;
                short int type_expr = AnalyzeOperatorExpression_(false);
                if (type_expr == CODE_TYPE_STRING_) {
                    throw CODE_ERR_INCORRECT_TYPE_EXPR_IF_;
                }
                semantic_analyzer->goNextLVLTID(CODE_TYPE_TID_IF_);
                index_gen = generator->insert_conditional_jump();
                generator->insert_semicolon();
                AnalyzeCompoundOperator_(true);
                index_gen_2_.push_back(generator->insert_unconditional_jump());
                generator->insert_index_jump(index_gen);
                generator->insert_semicolon();
                semantic_analyzer->backLVLTID();
            }
            else if (list_of_tokens_[cur_token_].second == "else") {
                ++cur_token_;
                semantic_analyzer->goNextLVLTID(CODE_TYPE_TID_ELSE_);
                AnalyzeCompoundOperator_(true);
                for (int ii = 0; ii < index_gen_2_.size(); ++ii) {
                    generator->insert_index_jump(index_gen_2_[ii]);
                }
                semantic_analyzer->backLVLTID();
                if (cur_token_ < list_of_tokens_.size()) {
                    if (list_of_tokens_[cur_token_].second == "else" ||
                        list_of_tokens_[cur_token_].second == "elseif") {

                        throw std::make_pair(CODE_ERR_ELSE_MUST_BE_ONE_TIME_AND_LAST_ONE_, LINE_ERR_ + getLineOfError_());
                    }
                    else {
                        return;
                    }
                }
                else {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
                }
            }
            else {
                for (int ii = 0; ii < index_gen_2_.size(); ++ii) {
                    generator->insert_index_jump(index_gen_2_[ii]);
                }
                return;
            }

            if (list_of_tokens_[cur_token_].second == "elseif") {
                goto tryAgain_if;
            }
            else if (list_of_tokens_[cur_token_].second == "else") {
                ++cur_token_;
                semantic_analyzer->goNextLVLTID(CODE_TYPE_TID_ELSE_);
                AnalyzeCompoundOperator_(true);
                for (int ii = 0; ii < index_gen_2_.size(); ++ii) {
                    generator->insert_index_jump(index_gen_2_[ii]);
                }
                semantic_analyzer->backLVLTID();
                if (cur_token_ < list_of_tokens_.size()) {
                    if (list_of_tokens_[cur_token_].second == "else" ||
                        list_of_tokens_[cur_token_].second == "elseif") {

                        throw std::make_pair(CODE_ERR_ELSE_MUST_BE_ONE_TIME_AND_LAST_ONE_, LINE_ERR_ + getLineOfError_());
                    }
                    else {
                        return;
                    }
                }
                else {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
                }
            }
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
        }

        for (int ii = 0; ii < index_gen_2_.size(); ++ii) {
            generator->insert_index_jump(index_gen_2_[ii]);
        }
        index_gen_2_.clear();
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeOperatorDoWhile_() {
    semantic_analyzer->goNextLVLTID(CODE_TYPE_TID_DO_WHILE_);
    unsigned int index_compound_jump = generator->getSizePoliz();
    generator->insert_semicolon();
    AnalyzeCompoundOperator_(true);
    semantic_analyzer->backLVLTID();
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].second == "while") {
            ++cur_token_;
            short int type_expr = AnalyzeOperatorExpression_(false);
            generator->insert_conditional_jump_dowhile(index_compound_jump);
            if (type_expr == CODE_TYPE_STRING_) {
                throw CODE_ERR_INCORRECT_TYPE_EXPR_DO_WHILE_;
            }
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first != TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
                }
                else {
                    ++cur_token_;
                    return;
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_EXPECT_WHILE_AFTER_DO_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeOperatorFor_(){
    if (cur_token_ < list_of_tokens_.size()) {
        semantic_analyzer->goNextLVLTID(CODE_TYPE_TID_FOR_);
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
            ++cur_token_;
            AnalyzeParamsFor_(0);
            unsigned int unconditional_ind_jump_if = generator->getSizePoliz();
            generator->insert_semicolon();
            AnalyzeParamsFor_(1);
            unsigned int conditional_ind_jump_out = generator->insert_conditional_jump();
            unsigned int unconditional_ind_jump_in = generator->insert_unconditional_jump();
            unsigned int unconditional_ind_jump_third_expr = generator->getSizePoliz();
            generator->insert_semicolon();
            AnalyzeParamsFor_(2);
            generator->insert_unconditional_jump(unconditional_ind_jump_if);
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_) {
                    ++cur_token_;
                    generator->insert_index_jump(unconditional_ind_jump_in);
                    generator->insert_semicolon();
                    AnalyzeCompoundOperator_(true);
                    generator->insert_unconditional_jump(unconditional_ind_jump_third_expr);
                    generator->insert_index_jump(conditional_ind_jump_out);
                    if (generator->getSizePosBreak() > 0) {
                        generator->insert_index_jump(generator->top_pos_break());
                    }
                    semantic_analyzer->backLVLTID();
                }
                else {
                    throw std::make_pair(CODE_ERR_DESCRIPT_FOR_, LINE_ERR_ + getLineOfError_());
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_DESCRIPT_FOR_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeParamsFor_(short int num_param) {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SEMICOLON_CATEGORY_) {
            ++cur_token_;
            return;
        }
        else if (num_param == 2 && list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_) {
            return;
        }
        else {
            short int type_expr = AnalyzeOperatorExpression_(false);
            if (num_param == 0) {
                if (type_expr == CODE_TYPE_STRING_) {
                    throw CODE_ERR_INCORRECT_TYPE_EXPR_1_FOR_;
                }
            }
            else if (num_param == 1) {
                if (type_expr == CODE_TYPE_STRING_) {
                    throw CODE_ERR_INCORRECT_TYPE_EXPR_2_FOR_;
                }
            }
            else if (num_param == 2) {
                if (type_expr == CODE_TYPE_STRING_) {
                    throw CODE_ERR_INCORRECT_TYPE_EXPR_3_FOR_;
                }
            }
            if (cur_token_ < list_of_tokens_.size()) {
                if (num_param != 2) {
                    if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                        ++cur_token_;
                        return;
                    }
                    else {
                        throw std::make_pair(CODE_ERR_DESCRIPT_FOR_, LINE_ERR_ + getLineOfError_());
                    }
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
    }
}

void SyntaxAnalyzer::AnalyzeOperatorElFor_() {
    if (cur_token_ < list_of_tokens_.size()) {
        semantic_analyzer->goNextLVLTID(CODE_TYPE_TID_ELFOR_);
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) {
            ++cur_token_;
            AnalyzeParamsFor_(0);
            unsigned int unconditional_ind_jump_if = generator->getSizePoliz();
            AnalyzeParamsFor_(1);
            unsigned int conditional_ind_jump_out = generator->insert_conditional_jump();
            unsigned int unconditional_ind_jump_in = generator->insert_unconditional_jump();
            unsigned int unconditional_ind_jump_third_expr = generator->getSizePoliz();
            AnalyzeParamsFor_(2);
            generator->insert_unconditional_jump(unconditional_ind_jump_if);
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_) {
                    ++cur_token_;
                    generator->insert_index_jump(unconditional_ind_jump_in);
                    AnalyzeCompoundOperator_(true);
                    generator->insert_unconditional_jump(unconditional_ind_jump_third_expr);
                    generator->insert_index_jump(conditional_ind_jump_out);
                    semantic_analyzer->backLVLTID();
                    AnalyzeOperatorElseElFor_();
                    if (generator->getSizePosBreak() > 0) {
                        generator->insert_index_jump(generator->top_pos_break());
                    }
                }
                else {
                    throw std::make_pair(CODE_ERR_DESCRIPT_FOR_, LINE_ERR_ + getLineOfError_());
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_DESCRIPT_FOR_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeOperatorElseElFor_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].second == "else") {
            ++cur_token_;
            semantic_analyzer->goNextLVLTID(CODE_TYPE_TID_ELSE_);
            AnalyzeCompoundOperator_(true);
            semantic_analyzer->backLVLTID();
        }
        else {
            throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::AnalyzeOperatorReturn_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SEMICOLON_CATEGORY_) {
            if (semantic_analyzer->FindTypeTID(CODE_TYPE_TID_DEF_) || semantic_analyzer->FindTypeTID(CODE_TYPE_TID_MAIN_)) {
                ++cur_token_;
                return;
            }
            else {
                throw CODE_ERR_INCORRECT_RETURN_TYPE_;
            }
        }
        else {
            short int type_expr = AnalyzeOperatorExpression_(false);
            if (cur_token_ < list_of_tokens_.size()) {
                if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SEMICOLON_CATEGORY_) {
                    if (type_expr == CODE_TYPE_STRING_ && semantic_analyzer->FindTypeTID(CODE_TYPE_TID_STRING_FUNC_)) {
                        ++cur_token_;
                        return;
                    }
                    else if (type_expr != CODE_TYPE_STRING_ && (semantic_analyzer->FindTypeTID(CODE_TYPE_TID_INT_FUNC_) ||
                        semantic_analyzer->FindTypeTID(CODE_TYPE_TID_DOUBLE_FUNC_) || semantic_analyzer->FindTypeTID(CODE_TYPE_TID_BOOLEAN_FUNC_))) {

                        ++cur_token_;
                        return;
                    }
                    else {
                        throw CODE_ERR_INCORRECT_RETURN_TYPE_;
                    }
                }
                else {
                    throw std::make_pair(CODE_ERR_UNEXPEXT_SYMB_, LINE_ERR_ + getLineOfError_());
                }
            }
            else {
                throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
            }
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::ChechCorrectName_(std::string name) {
    bool flag_contains_last_dig = false;
    if (std::isdigit(name[0])) {
        throw std::make_pair(CODE_ERR_NAME_IDENTIFIER_, LINE_ERR_ + getLineOfError_());
    }
    if (std::isdigit(name[name.length() - 1])) {
        flag_contains_last_dig = true;
    }
    bool flag_contains_letter = false;
    for (int ii = 0; ii < name.length(); ++ii) {
        if (std::isalpha(name[ii]) || name[ii] == '_') {
            flag_contains_letter = true;
        }
    }
    if (flag_contains_last_dig || !flag_contains_letter) {
        throw std::make_pair(CODE_ERR_NAME_IDENTIFIER_, LINE_ERR_ + getLineOfError_());
    }
}

void SyntaxAnalyzer::CheckCorrectConstant_(short int type_const){
    if (cur_token_ < list_of_tokens_.size()) {
        if (type_const == 0) {
            if (CheckCorrectNumberConstant_() || CheckCorrectLogicalConstant_()) {
                return;
            }
            else {
                throw std::make_pair(CODE_ERR_INCORRECT_CONST_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (type_const == 1) {
            if (!CheckCorrectNumberConstant_()) {
                throw std::make_pair(CODE_ERR_INCORRECT_NUMBER_CONST_, LINE_ERR_ + getLineOfError_());
            }
        }
        else if (type_const == 2) {
            if (!CheckCorrectLogicalConstant_()) {
                throw std::make_pair(CODE_ERR_INCORRECT_LOGICAL_CONST_, LINE_ERR_ + getLineOfError_());
            }
        }
        else {
            throw std::make_pair(CODE_ERR_INCORRECT_CONST_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
}

bool SyntaxAnalyzer::CheckCorrectNumberConstant_() {
    if (cur_token_ < list_of_tokens_.size()) {
        std::string token = list_of_tokens_[cur_token_].second;
        bool flag_is_first_dig = false;
        bool flag_is_point = false;
        for (int ii = 0; ii < token.length(); ++ii) {
            if (std::isdigit(token[ii]) && !flag_is_first_dig) {
                flag_is_first_dig = true;
            }
            else if (token[ii] == '.' && flag_is_first_dig) {
                flag_is_point = true;
            }
            else if (!std::isdigit(token[ii])) {
                return false;
            }
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
    return true;
}
 
bool SyntaxAnalyzer::CheckCorrectLogicalConstant_() {
    if (cur_token_ < list_of_tokens_.size()) {
        if (list_of_tokens_[cur_token_].first == TYPE_NUMBER_SYSTEM_CONSTANT_CATEGORY_) {
            return true;
        }
        else {
            throw std::make_pair(CODE_ERR_INCORRECT_LOGICAL_CONST_, LINE_ERR_ + getLineOfError_());
        }
    }
    else {
        throw std::make_pair(CODE_ERR_UNEXPEXT_END_PROG_, LINE_ERR_ + getLineOfError_());
    }
    return false;
}


