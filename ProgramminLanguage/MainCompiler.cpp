#include "MainCompiler.h"

void MainCompiler::StartCompilation(int mode) {
	try {
        if (mode != 1) {
            std::cout << "DEBUG INFO:" << std::endl;
            std::vector<std::pair<int, std::string>> list_of_tokens;
            if (mode == 0) {
                list_of_tokens = StartLexicalAnalyze_(FILE_NAME_CODE_TEXT_);
            }
            else if (mode == 3) {
                list_of_tokens = StartLexicalAnalyze_(EXAMPLE_RECUR_AKKERMAN_);
            }
            else if (mode == 4) {
                list_of_tokens = StartLexicalAnalyze_(EXAMPLE_RECUR_FACTORIAL_);
            }
            else if (mode == 5) {
                list_of_tokens = StartLexicalAnalyze_(EXAMPLE_RECUR_FIBONACHI_);
            }
            else if (mode == 6) {
                list_of_tokens = StartLexicalAnalyze_(EXAMPLE_RECUR_GCD_);
            }
            else if (mode == 7) {
                list_of_tokens = StartLexicalAnalyze_(EXAMPLE_RECUR_HANOI_);
            }
            else if (mode == 8) {
                list_of_tokens = StartLexicalAnalyze_(EXAMPLE_PEREBOR_);
            }
            else if (mode == 9) {
                list_of_tokens = StartLexicalAnalyze_(EXAMPLE_ARITHM_);
            }
            StartSyntaxAnalyzer_(list_of_tokens);
            std::cout << "END DEBUG!" << std::endl;
            std::cout << "Start Programm" << std::endl;
            try {
                generator.StartProgramm();
                generator.RELOAD();
            }
            catch (short int err_num) {
                std::cout << getTextErrorByCode(err_num) << std::endl;
            }
            std::cout << std::endl;
        }
        else if (mode == 1) {
            UpdateTableReservedWords_();
            UpdateErrorsTable_();
        }
	}
	catch (std::pair<short int, std::string> error_info) {
        std::cout << std::to_string(error_info.first) << ": " << getTextErrorByCode(error_info.first) << error_info.second << std::endl;
	}
}

// Steps of compilation

std::vector<std::pair<int, std::string>> MainCompiler::StartLexicalAnalyze_(std::string file_name) {
    std::cout << "1. Start Lexical Analyzer -> ";
    std::ifstream in;
    in.open(file_name);
    std::string code_text = "";
    if (in.is_open()) {
        char cc = 0;
        while ((cc = in.get()) != EOF) {
            code_text += cc;
        }
    }
    else {
        throw std::make_pair(CODE_ERR_OP_FILE_START_COMP_, file_name);
    }
    in.close();
    LexicalAnalyzer lex_analyzer;

    /* вывод токенов с их группами
    std::vector<std::pair<int, std::string>> check_tmp;
    check_tmp = lex_analyzer.analyze(code_text);

    
    std::ofstream out;
    out.open(FILE_NAME_OUTPUT_TOKEN_AND_CATEGORY_);
    if (out.is_open()) {
        for (int ii = 0; ii < check_tmp.size(); ++ii) {
            out << check_tmp[ii].first << "  " << check_tmp[ii].second << std::endl;
        }
    }
    else {
        throw std::make_pair(CODE_ERR_OP_FILE_START_COMP_, FILE_NAME_OUTPUT_TOKEN_AND_CATEGORY_);
    }
    */
    std::vector<std::pair<int, std::string>> result_lex = lex_analyzer.analyze(code_text);

    std::cout << "Successfull" << std::endl;
    return result_lex;
}

void MainCompiler::StartSyntaxAnalyzer_(std::vector<std::pair<int, std::string>> tokens) {
    std::cout << "2. Start Syntax and Semantic Analyzer -> ";
    SyntaxAnalyzer syntax_analyzer(tokens, generator);
    syntax_analyzer.StartSyntaxAnalyzer();
    std::cout << "Successfull" << std::endl;
}

// Steps of compilation



// SQLITE

void MainCompiler::CreateReserverWordsTable_() {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    if (exit) {
        throw std::make_pair(CODE_ERR_OPEN_DB_SQL_, DATABASE_NAME_ + " " + sqlite3_errmsg(DB));
    }
    else {
        std::cout << "Opened Database Successfully!" << std::endl;
    }

    std::string sql = "drop table if exists " + TABLE_RESERVER_WORDS_AND_OTHER_;
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        throw std::make_pair(CODE_ERR_DROP_TABLE_IF_EX_, TABLE_RESERVER_WORDS_AND_OTHER_ + " " + messaggeError);
    }
    else {
        std::cout << "Table drop Successfully" << std::endl;
    }

    sql = "CREATE TABLE " + TABLE_RESERVER_WORDS_AND_OTHER_ + "(" + COLUMN_ID + 
        " INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " + COLUMN_WORD + " TEXT NOT NULL, " + 
        COLUMN_TYPE + " TEXT NOT NULL, " + COLUMN_TYPE_ID + " INTEGER NOT NULL);";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

    if (exit != SQLITE_OK) {
        throw std::make_pair(CODE_ERR_CREATE_TABLE_, TABLE_RESERVER_WORDS_AND_OTHER_ + " " + messaggeError);
    }
    else {
        std::cout << "Table created Successfully" << std::endl;
    }
    sqlite3_close(DB);
    return;
}

void MainCompiler::UpdateTableReservedWords_() {
    CreateReserverWordsTable_();
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    if (exit) {
        throw std::make_pair(CODE_ERR_OPEN_DB_SQL_, DATABASE_NAME_ + " " + sqlite3_errmsg(DB));
    }
    else {
        std::cout << "Opened Database Successfully!" << std::endl;
    }

    char* messaggeError;
    std::string sql = "DELETE FROM " + TABLE_RESERVER_WORDS_AND_OTHER_;
    int rc = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

    if (rc != SQLITE_OK) {
        throw std::make_pair(CODE_ERR_DEL_DB_SQL_, TABLE_RESERVER_WORDS_AND_OTHER_ + " " + messaggeError);
    }
    else {
        std::cout << "Operation DELETE OK!" << std::endl;
    }

    std::ifstream in;          // поток для записи
    std::string line = "";
    in.open(FILE_NAME_RESERVED_WORDS_AND_OTHER_); // окрываем файл для записи
    std::string code_text = "";
    if (in.is_open()) {
        char cc = 0;
        while (getline(in, line)) {
            std::string category = "";
            unsigned int ii = 0;
            for (ii = 0; ii < line.length(); ++ii) {
                if (line[ii] == ':') {
                    break;
                }
                category += line[ii];
            }
            ++ii;
            char cur_elem = 0;
            std::string cur_token = "";
            for (; ii < line.length(); ++ii) {
                cur_elem = line[ii];
                if ((cur_elem == ',' && !cur_token.empty()) || (cur_elem == ';' && !cur_token.empty())) {
                    if (cur_token == "\\r") {
                        cur_token = "\r";
                    }
                    else if (cur_token == "\\n") {
                        cur_token = "\n";
                    }
                    else if (cur_token == "\\t") {
                        cur_token = "\t";
                    }
                    if (category == SERVICE_WORD_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + SERVICE_WORD_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_SERVICE_WORD_CATEGORY_) + ");";
                    }
                    else if (category == SERVICE_TYPE_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + SERVICE_TYPE_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_SERVICE_TYPE_CATEGORY_) + ");";
                    }
                    else if (category == SYSTEM_CONSTANT_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + SYSTEM_CONSTANT_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_SYSTEM_CONSTANT_CATEGORY_) + ");";
                    }
                    else if (category == LOGICAL_OPERATIONS_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + LOGICAL_OPERATIONS_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_LOGICAL_OPERATIONS_CATEGORY_) + ");";
                    }
                    else if (category == INDEXING_OPERATION_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + INDEXING_OPERATION_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_INDEXING_OPERATION_CATEGORY_) + ");";
                    }
                    else if (category == SEMICOLON_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + PUNCTUATION_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_SEMICOLON_CATEGORY_) + ");";
                    }
                    else if (category == OPENING_PARENTHESIS_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + PUNCTUATION_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_OPENING_PARENTHESIS_CATEGORY_) + ");";
                    }
                    else if (category == CLOSING_PARENTHESIS_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + PUNCTUATION_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_CLOSING_PARENTHESIS_CATEGORY_) + ");";
                    }
                    else if (category == PUNCTUATION_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + PUNCTUATION_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_PUNCTUATION_CATEGORY_) + ");";
                    }
                    else if (category == SEPARATORS_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + SEPARATORS_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_SEPARATORS_CATEGORY_) + ");";
                    }
                    else if (category == BITWISE_OPERATION_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + BITWISE_OPERATION_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_BITWISE_OPERATION_CATEGORY_) + ");";
                    }
                    else if (category == COMPARISON_OPERATION_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + COMPARISON_OPERATION_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_COMPARISON_OPERATION_CATEGORY_) + ");";
                    }
                    else if (category == ARITHM_OPERATION_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + ARITHM_OPERATION_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_ARITHM_OPERATION_CATEGORY_) + ");";
                    }
                    else if (category == ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_ASSIGNMENT_OPERATION_WITH_ARITHMETIC_CATEGORY_) + ");";
                    }
                    else if (category == ASSIGNMENT_OPERATION_CATEGORY_) {
                        sql = "INSERT INTO " + TABLE_RESERVER_WORDS_AND_OTHER_ + " (" + COLUMN_WORD + "," + COLUMN_TYPE + "," + COLUMN_TYPE_ID + ")" + " VALUES(" + "'" + cur_token + "'" + "," + "'" + ASSIGNMENT_OPERATION_CATEGORY_ + "'" + "," + std::to_string(TYPE_NUMBER_ASSIGNMENT_OPERATION_CATEGORY_) + ");";
                    }

                    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
                    if (exit != SQLITE_OK) {
                        throw std::make_pair(CODE_ERR_INS_DB_SQL_, TABLE_RESERVER_WORDS_AND_OTHER_ + " " + messaggeError);
                    }
                    else {
                        std::cout << "Records created Successfully!" << std::endl;
                    }
                    cur_token = "";
                }
                else {
                    cur_token += cur_elem;
                }
            }
        }
        std::cout << "UPDATE COMPLETE!" << std::endl;
    }
    else {
        throw std::make_pair(CODE_ERR_OP_FILE_SQL, FILE_NAME_RESERVED_WORDS_AND_OTHER_);
    }
    in.close();

    sqlite3_close(DB);
}

void MainCompiler::CreateErrorsTable_() {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    if (exit) {
        throw std::make_pair(CODE_ERR_OPEN_DB_SQL_, DATABASE_NAME_ + " " + sqlite3_errmsg(DB));
    }
    else {
        std::cout << DEBUG_OK << std::endl;
    }

    std::string sql = "drop table if exists " + TABLE_ERRORS;
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        throw std::make_pair(CODE_ERR_DROP_TABLE_IF_EX_, TABLE_ERRORS + " " + messaggeError);
    }
    else {
        std::cout << "Table drop Successfully" << std::endl;
    }

    sql = "CREATE TABLE " + TABLE_ERRORS + "(" + COLUMN_ID_TABLE_ERRORS + 
        " INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " + COLUMN_CODE_ERROR + " INTEGER NOT NULL, " + 
        COLUMN_TEXT_ERROR_EN + " TEXT NOT NULL, " + COLUMN_TEXT_ERROR_RUS + " TEXT);";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        throw std::make_pair(CODE_ERR_CREATE_TABLE_, TABLE_ERRORS + " " + messaggeError);
    }
    else {
        std::cout << DEBUG_OK << std::endl;
    }
    sqlite3_close(DB);
    return;
}

void MainCompiler::UpdateErrorsTable_() {
    CreateErrorsTable_();
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    if (exit) {
        throw std::make_pair(CODE_ERR_OPEN_DB_SQL_, DATABASE_NAME_ + " " + sqlite3_errmsg(DB));
    }
    else {
        std::cout << "Opened Database Successfully!" << std::endl;
    }

    char* messaggeError;
    std::string sql = "DELETE FROM " + TABLE_ERRORS;
    int rc = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

    if (rc != SQLITE_OK) {
        throw std::make_pair(CODE_ERR_DEL_DB_SQL_, TABLE_ERRORS + " " + messaggeError);
    }
    else {
        std::cout << "Operation DELETE OK!" << std::endl;
    }

    std::ifstream in;          // поток для записи
    std::string line = "";
    in.open(FILE_NAME_ERRORS); // окрываем файл для записи
    std::string code_text = "";
    if (in.is_open()) {
        char cc = 0;
        std::string code;
        std::string sql;
        std::string error_text_en;
        while (getline(in, line)) {
            if (line.length() > 2) {
                if (line[0] != '/' && line[1] != '/') {
                    code = line.substr(0,4);
                    error_text_en = line.substr(5, line.length() - 5);
                    sql = "INSERT INTO " + TABLE_ERRORS + " (" + COLUMN_CODE_ERROR + "," + 
                        COLUMN_TEXT_ERROR_EN + ")" + " VALUES(" + "'" + code + "'" + "," + "'" + error_text_en + "'" + ");";
                    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
                    if (exit != SQLITE_OK) {
                        throw std::make_pair(CODE_ERR_INS_DB_SQL_, TABLE_ERRORS + " " + messaggeError);
                    }
                    else {
                        std::cout << "Records created Successfully!" << std::endl;
                    }
                }
            }
        }

    }
    else {
        throw std::make_pair(CODE_ERR_OP_FILE_SQL, FILE_NAME_ERRORS);
    }

    sqlite3_close(DB);
}

std::string MainCompiler::getTextErrorByCode(short int error_code) {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    if (exit) {
        throw std::make_pair(CODE_ERR_OPEN_DB_START_COMP_, DATABASE_NAME_ + " " + sqlite3_errmsg(DB));
    }
    else {
        std::cout << "Opened Database Successfully!" << std::endl;
    }

    sqlite3_stmt* stmt;

    std::string sql = "SELECT " + COLUMN_TEXT_ERROR_EN + " from " + TABLE_ERRORS + 
        " WHERE " + COLUMN_CODE_ERROR + " = " + std::to_string(error_code);
    sqlite3_prepare(DB, sql.c_str(), -1, &stmt, NULL);//preparing the statement
    sqlite3_step(stmt);//executing the statement
    std::string error_text_en = std::string((char*)sqlite3_column_text(stmt, 0));
    sqlite3_close(DB);
    return error_text_en;
}

// SQLITE