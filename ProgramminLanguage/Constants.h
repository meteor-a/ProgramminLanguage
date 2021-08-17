#pragma once
#include <string>

const std::string SERVICE_WORD_CATEGORY_ = "service_word";
const std::string SERVICE_TYPE_CATEGORY_ = "service_type";
const std::string SYSTEM_CONSTANT_CATEGORY_ = "system_constant";
const std::string LOGICAL_OPERATIONS_CATEGORY_ = "logical_operation";
const std::string INDEXING_OPERATION_CATEGORY_ = "indexing_operation";
const std::string PUNCTUATION_CATEGORY_ = "punctuation";
const std::string SEPARATORS_CATEGORY_ = "separator";
const std::string SEMICOLON_CATEGORY_ = "semicolon";
const std::string OPENING_PARENTHESIS_ = "opening_parenthesis";
const std::string CLOSING_PARENTHESIS_ = "closing_parenthesis";
const std::string BITWISE_OPERATION_CATEGORY_ = "bitwise_operation";
const std::string COMPARISON_OPERATION_CATEGORY_ = "comparison_operation";
const std::string ARITHM_OPERATION_CATEGORY_ = "arithmetic_operation";
const std::string ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_ = "assignment_operation_with_arithmetic";
const std::string ASSIGNMENT_OPERATION_CATEGORY_ = "assignment_operation";

const std::string CONSTANT_STR_CATEGORY_ = "constant_str";
const std::string CONSTANT_NUMBER_CATEGORY_ = "constant_num";
const std::string IDENTIFIER_CATEGORY_ = "identifier";
const std::string NOT_DEFINED_CATEGORY_ = "NOT_DEFINED";
const std::string NEW_STRING_CATEGORY_ = "NEW_STR";

const int TYPE_NUMBER_SERVICE_WORD_CATEGORY_ = 0;
const int TYPE_NUMBER_SERVICE_TYPE_CATEGORY_ = 1;
const int TYPE_NUMBER_SYSTEM_CONSTANT_CATEGORY_ = 2;
const int TYPE_NUMBER_LOGICAL_OPERATIONS_CATEGORY_ = 3;
const int TYPE_NUMBER_INDEXING_OPERATION_CATEGORY_ = 4;
const int TYPE_NUMBER_PUNCTUATION_CATEGORY_ = 5;
const int TYPE_NUMBER_SEPARATORS_CATEGORY_ = 6;
const int TYPE_NUMBER_SEMICOLON_CATEGORY_ = 7;
const int TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_ = 8;
const int TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_ = 9;
const int TYPE_NUMBER_BITWISE_OPERATION_CATEGORY_ =10;
const int TYPE_NUMBER_COMPARISON_OPERATION_CATEGORY_ = 11;
const int TYPE_NUMBER_ARITHM_OPERATION_CATEGORY_ = 12;
const int TYPE_NUMBER_ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_ = 13;
const int TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_ = 14;
const int TYPE_NUMBER_CONSTANT_STR_CATEGORY_ = 15;
const int TYPE_NUMBER_CONSTANT_NUM_CATEGORY_ = 16;
const int TYPE_NUMBER_IDENTIFIER_CATEGORY_ = 17;
const int TYPE_NUMBER_NOT_DEFINED_ = 18;
const int TYPE_NUMBER_NEW_STRING_CATEGORY_ = 19;

const int NUMBER_OF_CATEGORIES_ = 20;

const std::string FILE_NAME_CODE_TEXT_ = "code_text.txt";
const std::string FILE_NAME_RESERVED_WORDS_AND_OTHER_ = "reserved_words_and_other.txt";
const std::string FILE_NAME_ERRORS = "errors_list.txt";
const std::string FILE_NAME_OUTPUT_TOKEN_AND_CATEGORY_ = "output_token.txt";
const std::string DATABASE_NAME_ = "LanguageDataBase.db";


const std::string EXAMPLE_RECUR_AKKERMAN_ = "examples/akkerman_recur.txt";
const std::string EXAMPLE_RECUR_FACTORIAL_ = "examples/factorial_recur.txt";
const std::string EXAMPLE_RECUR_FIBONACHI_ = "examples/fibonachi_recur.txt";
const std::string EXAMPLE_RECUR_GCD_ = "examples/gcd_recur.txt";
const std::string EXAMPLE_RECUR_HANOI_ = "examples/hanoi_recur.txt";
const std::string EXAMPLE_PEREBOR_ = "examples/perebor.txt";
const std::string EXAMPLE_ARITHM_ = "examples/arithm.txt";


const std::string TABLE_RESERVER_WORDS_AND_OTHER_ = "reserved_words";
const std::string COLUMN_ID = "ID";
const std::string COLUMN_WORD = "WORD";
const std::string COLUMN_TYPE = "TYPE";
const std::string COLUMN_TYPE_ID = "TYPE_ID";


const std::string TABLE_ERRORS = "errors";
const std::string COLUMN_ID_TABLE_ERRORS = "ID";
const std::string COLUMN_CODE_ERROR = "CODE";
const std::string COLUMN_TEXT_ERROR_EN = "ERROR_EN";
const std::string COLUMN_TEXT_ERROR_RUS = "ERROR_RUS";


const std::string DEBUG_OK = "OK";

const short int CODE_ERR_OPEN_DB_SQL_ = 1001;
const short int CODE_ERR_DEL_DB_SQL_ = 1002;
const short int CODE_ERR_INS_DB_SQL_ = 1003;
const short int CODE_ERR_OP_FILE_SQL = 1104;
const short int CODE_ERR_CREATE_TABLE_ = 1005;
const short int CODE_ERR_DROP_TABLE_IF_EX_ = 1006;

const short int CODE_ERR_OP_FILE_START_COMP_ = 2101;
const short int CODE_ERR_OPEN_DB_START_COMP_ = 2101;

const short int CODE_ERR_SYN_STR_CONST_ = 3201;
const short int CODE_ERR_OPEN_DB_LA_ = 3000;
const short int CODE_ERR_NOT_DEFINED_TOKEN = 3202;

const std::string LINE_ERR_ = " line ";

const short int CODE_ERR_UNEXPEXT_END_PROG_ = 4200;
const short int CODE_ERR_UNEXPEXT_SYMB_ = 4201;
const short int CODE_ERR_NAME_IDENTIFIER_ = 4202;
const short int CODE_ERR_SPECIFY_IDENTIFIER_TYPE_ = 4203;
const short int CODE_ERR_INCORRECT_CONST_ = 4204;
const short int CODE_ERR_INCORRECT_LOGICAL_CONST_ = 4205;
const short int CODE_ERR_INCORRECT_NUMBER_CONST_ = 4206;
const short int CODE_ERR_DESCRIPT_FOR_ = 4207;
const short int CODE_ERR_EXPECT_WHILE_AFTER_DO_ = 4208;
const short int CODE_ERR_ELSE_MUST_BE_ONE_TIME_AND_LAST_ONE_ = 4209;
const short int CODE_ERR_INVALID_FUNC_ARGUMENTS_ = 4210;
const short int CODE_ERR_PARENTHESES_IN_EXPRESSION_ = 4211;
const short int CODE_ERR_INCRIMENT_DECRIMENT_VARIABLE = 4212;
const short int CODE_ERR_NOT_FINED_MAIN_ = 4213;


const short int CODE_ERR_OVERRID_DEF_ = 5200;
const short int CODE_ERR_RE_DECLARING_VAR_ = 5201;
const short int CODE_ERR_INTERNAL_SEMANTIC_ANALYZER_ = 5202;
const short int CODE_ERR_INCORRECT_TYPE_OPERATOR_WRITE_ = 5203;
const short int CODE_ERR_NOT_FINED_ID_ = 5204;
const short int CODE_ERR_NOT_FINED_FUNC_ = 5205;
const short int CODE_ERR_INCORRECT_TYPE_OPERATOR_READ_ = 5206;
const short int CODE_ERR_INCORRECT_TYPE_OPERATOR_EXPR_ = 5207;
const short int CODE_ERR_OPERATIONS_WITH_DOUBLE_ = 5208;
const short int CODE_ERR_OPERATIONS_WITH_DOUBLE_ANS_STRING_ = 5209;
const short int CODE_ERR_OPERATIONS_TO_STRING_ = 5210;
const short int CODE_ERR_OPERATIONS_WITH_STRING_ = 5211;
const short int CODE_ERR_INCORRECT_TYPE_EXPR_DO_WHILE_ = 5212;
const short int CODE_ERR_INCORRECT_TYPE_EXPR_1_FOR_ = 5213;
const short int CODE_ERR_INCORRECT_TYPE_EXPR_2_FOR_ = 5214;
const short int CODE_ERR_INCORRECT_TYPE_EXPR_3_FOR_ = 5215;
const short int CODE_ERR_INCORRECT_TYPE_EXPR_WHILE_ = 5216;
const short int CODE_ERR_INCORRECT_TYPE_EXPR_IF_ = 5217;
const short int CODE_ERR_INCORRECT_RETURN_TYPE_ = 5218;
const short int CODE_ERR_INCORRECT_PLACE_BREAK_ = 5219;

const short int CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_BOOLEAN = 6200;
const short int CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_NULL_ = 6201;
const short int CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_STRING_LENGTH_NULL_ = 6202;
const short int CODE_ERR_CANNOT_MAKE_OPERAT_DIVISION_TO_BOOLEAN_FALSE_ = 6203;
const short int CODE_ERR_CANNOT_MAKE_OPERAT_OSTATOC_DIV_ = 6204;
const short int CODE_ERR_CANNOT_MAKE_OPERAITON_BITWISE_DOUBLE_ = 6205;
const short int CODE_ERR_INCORRECT_TYPE_INPUT_ = 6206;
const short int CODE_ERR_INCOMPATIBLE_TYPES_ = 6207;
const short int CODE_ERR_STACK_OVERFLOW = 6208;

// semantic internal
const std::string TYPE_INT_ = "int";
const std::string TYPE_STRING_ = "string";
const std::string TYPE_BOOLEAN_ = "boolean";
const std::string TYPE_DOUBLE_ = "double";
const std::string TYPE_DEF_ = "def";

const short int CODE_TYPE_INT_ = 1;
const short int CODE_TYPE_STRING_ = 2;
const short int CODE_TYPE_BOOLEAN_ = 3;
const short int CODE_TYPE_DOUBLE_ = 4;

const short int CODE_TYPE_TID_WHILE_ = 0;
const short int CODE_TYPE_TID_FOR_ = 1;
const short int CODE_TYPE_TID_ELFOR_ = 2;
const short int CODE_TYPE_TID_IF_ = 5;
const short int CODE_TYPE_TID_ELSE_ = 4;
const short int CODE_TYPE_TID_INT_FUNC_ = 5;
const short int CODE_TYPE_TID_STRING_FUNC_ = 6;
const short int CODE_TYPE_TID_BOOLEAN_FUNC_ = 7;
const short int CODE_TYPE_TID_DOUBLE_FUNC_ = 8;
const short int CODE_TYPE_TID_MAIN_ = 9;
const short int CODE_TYPE_TID_DEF_ = 10;
const short int CODE_TYPE_TID_DO_WHILE_ = 11;

// Generator

const short int TYPE_START_BLOCK_ = 101;
const short int TYPE_END_BLOCK_ = 102;
const short int TYPE_FUNC_DECLARATION_ = 103;
const short int TYPE_IDENTIFICATOR_ = 104;
const short int TYPE_CONST_ = 105;
const short int TYPE_OPERATION_ = 106;
const short int TYPE_CALL_FUNC_ = 107;
const short int TYPE_CONDITIONAL_JUMP_ = 108;
const short int TYPE_UNCONDITIONAL_JUMP_ = 109;
const short int TYPE_IDENTIFICATOR_AD_ = 110;
const short int TYPE_WRITE_ = 111;
const short int TYPE_CONDITIONAL_JUMP_DOWHILE_ = 112;
const short int TYPE_READ_ = 113;
const short int TYPE_SEMICOLON_ = 114;
const short int TYPE_RETURN_COMMAND_ = 115;
const short int TYPE_RETURN_VAL_MISS = 116;
const short int TYPE_FUNC_CALL_BRACKETS = 117;

