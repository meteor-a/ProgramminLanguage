#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "sqlite3.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "Constants.h"
#include "GenCode.h"

class MainCompiler
{
public:
    void StartCompilation(int);

private:
    void CreateReserverWordsTable_();
    void UpdateTableReservedWords_();
    void CreateErrorsTable_();
    void UpdateErrorsTable_();

    std::string getTextErrorByCode(short int);

    std::vector<std::pair<int, std::string>> StartLexicalAnalyze_(std::string);
    void StartSyntaxAnalyzer_(std::vector<std::pair<int, std::string>>);

    GenCode generator;
};

