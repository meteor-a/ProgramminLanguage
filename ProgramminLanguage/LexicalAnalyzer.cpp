#pragma once
#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer() {
    cur_state_ = StateLA_::START_STATE;
    ArrOfSetsOfLangCategory.resize(NUMBER_OF_CATEGORIES_);
}

LexicalAnalyzer::~LexicalAnalyzer() {
    ArrOfSetsOfLangCategory.clear();
}

std::vector<std::pair<int, std::string>> LexicalAnalyzer::analyze(std::string& text_code) {
    LoadDataBase();
    std::string token = "";
    std::vector<std::pair<int, std::string>> list_of_tokens;
    std::string cur_symb_str = "";
    int number_of_cur_string =1;
    for (unsigned int ii = 0; ii < text_code.length(); ++ii) {
        cur_symb_str = text_code[ii];
        if (std::isalpha(cur_symb_str[0]) || std::isdigit(cur_symb_str[0]) || cur_symb_str[0] == '_') {
            if (cur_state_ != StateLA_::LET_DIG) {
                if (!token.empty()) {
                    list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
                }
                token = cur_symb_str;
            }
            else {
                token += cur_symb_str;
            }
            cur_state_ = StateLA_::LET_DIG;
        }
        else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_PUNCTUATION_CATEGORY_].count(cur_symb_str)) {
            if (!token.empty()) {
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
            }
            token = cur_symb_str;
            list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
            token = "";
            cur_state_ = StateLA_::PUNCT;
        }
        else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_SEPARATORS_CATEGORY_].count(cur_symb_str)) {
            if (!token.empty()) {
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
            }
            if (cur_symb_str == "\n") {
                list_of_tokens.push_back(std::make_pair(TYPE_NUMBER_NEW_STRING_CATEGORY_, NEW_STRING_CATEGORY_));
                ++number_of_cur_string;
            }
            token = "";
            cur_state_ = StateLA_::SEPARATOR;
        }
        else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_ARITHM_OPERATION_CATEGORY_].count(cur_symb_str)) {
            if ((cur_state_ != StateLA_::ARITHM && cur_state_ != StateLA_::ASSIGNMENT) && !token.empty()) {
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
                token = "";
            }
            token += cur_symb_str;
            if (token == "//") {
                for (; ii < text_code.length(); ++ii) {
                    if (text_code[ii] == 13 || text_code[ii] == 10) {
                        break;
                    }
                }
                token = "";
            }
            cur_state_ = StateLA_::ARITHM;
        }
        else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_].count(cur_symb_str)) {
            if ((cur_state_ == StateLA_::LET_DIG) && !token.empty()) {
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
                token = "";
            }
            token += cur_symb_str;
            cur_state_ = StateLA_::ASSIGNMENT;
        }
        else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_BITWISE_OPERATION_CATEGORY_].count(cur_symb_str)) {
            if (cur_state_ == StateLA_::LET_DIG && !token.empty()) {
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
                token = "";
            }
            token += cur_symb_str;
            cur_state_ = StateLA_::BITWISE;
        }
        else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_INDEXING_OPERATION_CATEGORY_].count(cur_symb_str)) {
            if (!token.empty()) {
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
            }
            token = cur_symb_str;
            list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
            token = "";
            cur_state_ = StateLA_::INDEXING;
        }
        else if (ArrOfSpecialSymb[0] == cur_symb_str) {
            bool flag_is_ident = false;
            bool flag_is_double = false;
            for (unsigned int jj = 0; jj < token.length(); ++jj) {
                if (std::isalpha(token[jj]) || token[jj] == '_') {
                    list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
                    token = "";
                    flag_is_ident = true;
                    break;
                }
                else if (std::isdigit(token[jj])) {
                    flag_is_double = true;
                }
            }
            if (!flag_is_ident && flag_is_double) {
                token += cur_symb_str;
            }
            else if (flag_is_ident) {
                if (!token.empty()) {
                    list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
                }
                token = cur_symb_str;
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
                token = "";
                cur_state_ = StateLA_::PUNCT;
            }
        }
        else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_COMPARISON_OPERATION_CATEGORY_].count(cur_symb_str)) {
            if (cur_state_ == StateLA_::LET_DIG && !token.empty()) {
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
                token = "";
            }
            token += cur_symb_str;
            cur_state_ = StateLA_::COMPAR;
        }
        else if (ArrOfSpecialSymb[1] == cur_symb_str || ArrOfSpecialSymb[2] == cur_symb_str) {
            short int flag_quotation = (ArrOfSpecialSymb[1] == cur_symb_str) ? 1 : 2;
            if (!token.empty()) {
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
            }
            token = "";
            (ii < text_code.length()) ? ++ii : 0;
            bool flag_escaping = false;
            bool flag_end = false;
            for (; ii < text_code.length(); ++ii) {
                cur_symb_str = text_code[ii];
                if (cur_symb_str[0] == 10 || cur_symb_str[0] == 13) {
                    throw std::make_pair(CODE_ERR_SYN_STR_CONST_, " line " + std::to_string(number_of_cur_string));
                }
                else if (cur_symb_str[0] == '\\') {
                    if (ii < text_code.length()) {
                        ++ii;
                        cur_symb_str = text_code[ii];
                        if (cur_symb_str == "n") {
                            token.push_back('\n');
                        }
                        else if (cur_symb_str == "r") {
                            token.push_back('\r');
                        }
                        else if (cur_symb_str == "t") {
                            token.push_back('\t');
                        }
                        else if (cur_symb_str == "\'") {
                            token.push_back('\'');
                        }
                        else if (cur_symb_str == "\"") {
                            token.push_back('\"');
                        }
                        else if (cur_symb_str == "\\") {
                            token.push_back('\\');
                        }
                        else {
                            token.push_back(cur_symb_str[0]);
                        }
                    }
                    else {
                        throw std::make_pair(CODE_ERR_SYN_STR_CONST_, " line " + std::to_string(number_of_cur_string));
                    }
                }
                else if (ArrOfSpecialSymb[1] == cur_symb_str && flag_quotation == 1 || (ArrOfSpecialSymb[2] == cur_symb_str && flag_quotation == 2)) {
                    flag_end = true;
                    break;
                }
                else {
                    token += cur_symb_str;
                    flag_escaping = true;
                }
            }
            if (!flag_end) {
                throw std::make_pair(CODE_ERR_SYN_STR_CONST_, " line " + std::to_string(number_of_cur_string));
            }
            list_of_tokens.push_back(std::make_pair(TYPE_NUMBER_CONSTANT_STR_CATEGORY_, token));
            token = "";
        }
        else {
            if (!token.empty()) {
                list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
            }
            token = cur_symb_str;
        }
    }
    if (!token.empty()) {
        list_of_tokens.push_back(std::make_pair(getTypeCategory(token), token));
        token = "";
    }
    return list_of_tokens;
}

int LexicalAnalyzer::getTypeCategory(std::string token) {
    if (ArrOfSetsOfLangCategory[TYPE_NUMBER_SERVICE_WORD_CATEGORY_].count(token)) {
        return TYPE_NUMBER_SERVICE_WORD_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_SERVICE_TYPE_CATEGORY_].count(token)) {
        return TYPE_NUMBER_SERVICE_TYPE_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_SYSTEM_CONSTANT_CATEGORY_].count(token)) {
        return TYPE_NUMBER_SYSTEM_CONSTANT_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_LOGICAL_OPERATIONS_CATEGORY_].count(token)) {
        return TYPE_NUMBER_LOGICAL_OPERATIONS_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_INDEXING_OPERATION_CATEGORY_].count(token)) {
        return TYPE_NUMBER_INDEXING_OPERATION_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_SEMICOLON_CATEGORY_].count(token)) {
        return TYPE_NUMBER_SEMICOLON_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_].count(token)) {
        return TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_].count(token)) {
        return TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_PUNCTUATION_CATEGORY_].count(token) || token == ArrOfSpecialSymb[0] || token == ArrOfSpecialSymb[1] || token == ArrOfSpecialSymb[2]) {
        return TYPE_NUMBER_PUNCTUATION_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_SEPARATORS_CATEGORY_].count(token)) {
        return TYPE_NUMBER_SEPARATORS_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_BITWISE_OPERATION_CATEGORY_].count(token)) {
        return TYPE_NUMBER_BITWISE_OPERATION_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_COMPARISON_OPERATION_CATEGORY_].count(token)) {
        return TYPE_NUMBER_COMPARISON_OPERATION_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_ARITHM_OPERATION_CATEGORY_].count(token)) {
        return TYPE_NUMBER_ARITHM_OPERATION_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_].count(token)) {
        return TYPE_NUMBER_ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_;
    }
    else if (ArrOfSetsOfLangCategory[TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_].count(token)) {
        return TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_;
    }
    else {
        bool flag_is_letter = false;
        bool flag_is_digit_const = false;
        for (unsigned int ii = 0; ii < token.length(); ++ii) {
            if (std::isalpha(token[ii]) || token[ii] == '_') {
                flag_is_letter = true;
                break;
            }
            else if (std::isdigit(token[ii]) || token[ii] == '.') {
                flag_is_digit_const = true;
            }
            else {
                //return TYPE_NUMBER_NOT_DEFINED_;
                throw std::make_pair(CODE_ERR_NOT_DEFINED_TOKEN, " " + token);
            }
        }
        if (flag_is_letter) {
            return TYPE_NUMBER_IDENTIFIER_CATEGORY_;
        }
        else {
            return TYPE_NUMBER_CONSTANT_NUM_CATEGORY_;
        }
    }
    throw std::make_pair(CODE_ERR_NOT_DEFINED_TOKEN, " " + token);

    //return TYPE_NUMBER_NOT_DEFINED_;
}

void LexicalAnalyzer::LoadDataBase() {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    if (exit) {
        throw std::make_pair(CODE_ERR_OPEN_DB_LA_, DATABASE_NAME_ + " " + sqlite3_errmsg(DB));
    }

    sqlite3_stmt* stmt;

    std::string sql = "SELECT " + COLUMN_WORD + ", " + COLUMN_TYPE_ID + " from " + TABLE_RESERVER_WORDS_AND_OTHER_;
    sqlite3_prepare(DB, sql.c_str(), -1, &stmt, NULL);//preparing the statement
    sqlite3_step(stmt);//executing the statement
    std::string word = "";
    int type_word = 0;
    while (sqlite3_column_text(stmt, 0)) {
        for (int ii = 0; ii < 2; ++ii) {
            if (ii == 0) {
                word = std::string((char*)sqlite3_column_text(stmt, ii));
            }
            else {
                type_word = std::stoi(std::string((char*)sqlite3_column_text(stmt, ii)));
                ArrOfSetsOfLangCategory[type_word].insert(word);
            }
        }
        sqlite3_step(stmt);
    }
    sqlite3_close(DB);
}
