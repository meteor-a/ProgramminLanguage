#pragma once
#include <set>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <iostream>
#include "Constants.h"

class LexicalAnalyzer {
public:
    LexicalAnalyzer();
    ~LexicalAnalyzer();
    std::vector<std::pair<int, std::string>> analyze(std::string&);

private:
    std::vector<std::set<std::string>> ArrOfSetsOfLangCategory;
    std::vector<std::string> ArrOfSpecialSymb{ ".", "'", "\"" };
    
    enum class StateLA_ {
        START_STATE,
        SEPARATOR,
        PUNCT,
        LET_DIG,
        CONST_STR,
        ARITHM,
        LOGICAL,
        BITWISE,
        COMPAR,
        INDEXING,
        ASSIGNMENT
    };

    StateLA_ cur_state_;

    int getTypeCategory(std::string token);
    void LoadDataBase();
};

