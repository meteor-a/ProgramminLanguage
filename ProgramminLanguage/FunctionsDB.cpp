#include "sqlite3.h"
#include "Constants.h"
#include <iostream>
#include <fstream>

void CreateReserverWordsTable_() {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return;
        // Ошибка
    }
    else {
        std::cout << "Opened Database Successfully!" << std::endl;
    }

    std::string sql = "CREATE TABLE " + TABLE_RESERVER_WORDS_AND_OTHER_ + "(" + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " + COLUMN_WORD + " TEXT NOT NULL, " + COLUMN_TYPE + " TEXT NOT NULL, " + COLUMN_TYPE_ID + " INTEGER NOT NULL);";
    sqlite3_close(DB);

    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

    if (exit != SQLITE_OK) {
        std::cerr << "Error Create Table: " << messaggeError << std::endl;
        sqlite3_free(messaggeError);
        return;
    }
    else {
        std::cout << "Table created Successfully" << std::endl;
    }
    sqlite3_close(DB);

    return;
}

void UpdateTableReservedWords_() {
    CreateReserverWordsTable_();
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return;
        // Ошибка
    }
    else {
        std::cout << "Opened Database Successfully!" << std::endl;
    }

    char* messaggeError;
    std::string sql = "DELETE FROM reserved_words";
    int rc = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

    if (rc != SQLITE_OK) {
        std::cerr << "Error DELETE DB: " << messaggeError << std::endl;
        return;
    } else {
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
                        std::cerr << "Error Insert: " << messaggeError << std::endl;
                        sqlite3_free(messaggeError);
                        return;
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
        std::cout << "Error open file reserver_words_and_other.txt" << std::endl;
        return;
    }
    in.close();

    sqlite3_close(DB);
}

void CreateErrorsTable_() {
    const std::string sql = "CREATE TABLE " + TABLE_ERRORS + "(" + COLUMN_ID_TABLE_ERRORS + " INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " + COLUMN_NUMBER_ERROR + " TEXT NOT NULL, " + COLUMN_TEXT_ERROR_EN + " TEXT NOT NULL, " + COLUMN_TEXT_ERROR_RUS + " TEXT);";
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return;
    }
    else {
        std::cout << DEBUG_OK << std::endl;
    }
    sqlite3_close(DB);
    exit = sqlite3_open(DATABASE_NAME_.c_str(), &DB);
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Create Table: " << messaggeError << std::endl;
        sqlite3_free(messaggeError);
        return;
    }
    else {
        std::cout << DEBUG_OK << std::endl;
    }
    sqlite3_close(DB);
    return;
}