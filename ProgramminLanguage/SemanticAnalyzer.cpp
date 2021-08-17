#include "SemanticAnalyzer.h"

SemanticAnalyzer::SemanticAnalyzer(GenCode& gen_) {
    MAIN_TID_ = nullptr;
    CUR_TID_ = nullptr;
    StartCheckFunc = false;
    AnalyzeExpr = false;
    AnalyzeCallFunc = false;
    may_unary = true;
    generator = &gen_;
}

SemanticAnalyzer::~SemanticAnalyzer() {
    CUR_TID_ = nullptr;
    MAIN_TID_ = nullptr;
}

short int SemanticAnalyzer::getCodeType(std::string type_str, short int type) {
    if (type == 0) { // переменные
        if (type_str == TYPE_INT_) {
            return CODE_TYPE_INT_;
        }
        else if (type_str == TYPE_STRING_) {
            return CODE_TYPE_STRING_;
        }
        else if (type_str == TYPE_BOOLEAN_) {
            return CODE_TYPE_BOOLEAN_;
        }
        else if (type_str == TYPE_DOUBLE_) {
            return CODE_TYPE_DOUBLE_;
        }
    }
    else if (type == 1) { // func
        if (type_str == TYPE_INT_) {
            return CODE_TYPE_TID_INT_FUNC_;
        }
        else if (type_str == TYPE_STRING_) {
            return CODE_TYPE_TID_STRING_FUNC_;
        }
        else if (type_str == TYPE_BOOLEAN_) {
            return CODE_TYPE_TID_BOOLEAN_FUNC_;
        }
        else if (type_str == TYPE_DOUBLE_) {
            return CODE_TYPE_TID_DOUBLE_FUNC_;
        }
        else if (type_str == "main") {
            return CODE_TYPE_TID_MAIN_;
        }
        else if (type_str == "def") {
            return CODE_TYPE_TID_DEF_;
        }
    }
   
}

void SemanticAnalyzer::pushID(std::string type, std::string name) {
    if (MAIN_TID_ == nullptr) {
        MAIN_TID_ = new TID_(nullptr);
        CUR_TID_ = MAIN_TID_;
    }
    if (!deepSearchID(name)) {
        short int code_type_id = getCodeType(type, 0);
        std::string val;
        if (code_type_id == CODE_TYPE_INT_) {
            val = "0";
        }
        else if (code_type_id == CODE_TYPE_BOOLEAN_) {
            val = "false";
        }
        else if (code_type_id == CODE_TYPE_DOUBLE_) {
            val = "0.0";
        }
        else if (code_type_id == CODE_TYPE_STRING_) {
            val = "";
        }
        IDENT_* ident = new IDENT_(name, code_type_id, val);
        CUR_TID_->identifiers.idents.push_back(ident);
        ++CUR_TID_->identifiers.num_of_ident;
    }
    else {
        throw CODE_ERR_RE_DECLARING_VAR_;
    }
}

bool SemanticAnalyzer::deepSearchID(std::string name) {
    if (MAIN_TID_ == nullptr) {
        MAIN_TID_ = new TID_(nullptr);
        CUR_TID_ = MAIN_TID_;
        return false;
    }
    TID_* TMP = CUR_TID_;
    while (TMP != nullptr) {
        int ss = TMP->identifiers.num_of_ident;
        for (int ii = 0; ii < ss; ++ii) {
            if (name == TMP->identifiers.idents[ii]->id_) {
                return true;
            }
        }
        TMP = TMP->prev_;
    }
    return false;
}

void SemanticAnalyzer::IsIDDeclarring(std::string name) {
    if (!deepSearchID(name)) {
        throw CODE_ERR_NOT_FINED_ID_;
    }
}

void SemanticAnalyzer::pushFUNC(std::string type, std::string name) {
    if (MAIN_TID_ == nullptr) {
        MAIN_TID_ = new TID_(nullptr);
        CUR_TID_ = MAIN_TID_;
    }
    new_func.name_ = name;
    new_func.type_ = getCodeType(type, 1);
    StartCheckFunc = true;
}

void SemanticAnalyzer::pushParamsNewFUNC(std::string type, std::string name) {
    if (StartCheckFunc) {
        IDENT_ *ident = new IDENT_(name, getCodeType(type, 0));
        new_func.params.push_back(ident);
    }
    else {
        throw CODE_ERR_INTERNAL_SEMANTIC_ANALYZER_;
    }
}

void SemanticAnalyzer::finishPushFunc() {
    if (StartCheckFunc) {
        if(!deepSearchFUNC(new_func)) {
            CUR_TID_->identifiers.funcs.push_back(new_func);         
            ++CUR_TID_->identifiers.num_of_func;
            TID_* NEW_TID = new TID_(CUR_TID_);
            
            for (int ii = 0; ii < CUR_TID_->identifiers.funcs[CUR_TID_->identifiers.num_of_func - 1].params.size(); ++ii) {
                NEW_TID->identifiers.idents.push_back(CUR_TID_->identifiers.funcs[CUR_TID_->identifiers.num_of_func - 1].params[ii]);
                ++NEW_TID->identifiers.num_of_ident;
            }
            generator->insert_declaration_func(new_func); // пушатся разные указатели на одну переменную
            CUR_TID_->children.push_back(NEW_TID);
            NEW_TID->type_tid = new_func.type_;
            ++CUR_TID_->num_of_children;
            CUR_TID_ = NEW_TID;
        }
        else {
            throw CODE_ERR_OVERRID_DEF_;
        }
        StartCheckFunc = false;
        new_func.params.clear();
    }
    else {
        throw CODE_ERR_INTERNAL_SEMANTIC_ANALYZER_;
    }

}

bool SemanticAnalyzer::deepSearchFUNC(FUNC_ func) {
    if (MAIN_TID_ == nullptr) {
        MAIN_TID_ = new TID_(nullptr);
        CUR_TID_ = MAIN_TID_;
        return false;
    }
    int ss = MAIN_TID_->identifiers.num_of_func;
    for (int ii = 0; ii < ss; ++ii) {
        if (MAIN_TID_->identifiers.funcs[ii] == func) {
            return true;
        }
    }
    return false;
}

void SemanticAnalyzer::backLVLTID() {
    if (MAIN_TID_ == nullptr) {
        return;
    }
    else {
        CUR_TID_ = CUR_TID_->prev_;
    }
}

void SemanticAnalyzer::goNextLVLTID(short int type_tid) {
    if (MAIN_TID_ == nullptr) {
        MAIN_TID_ = new TID_(nullptr);
        CUR_TID_ = MAIN_TID_;
    }
    else {
        TID_* NEW_TID = new TID_(CUR_TID_);
        NEW_TID->type_tid = type_tid;
        CUR_TID_->children.push_back(NEW_TID);
        ++CUR_TID_->num_of_children;
        CUR_TID_ = NEW_TID;
    }
}

void SemanticAnalyzer::CheckParamsOperatorOutput(std::string name) {
    if (MAIN_TID_ == nullptr) {
        MAIN_TID_ = new TID_(nullptr);
        CUR_TID_ = MAIN_TID_;
    }
    if (deepSearchID(name)) {
        IDENT_ ident = *getIdent(name);
        if (ident.type_ == 3) {
            throw CODE_ERR_INCORRECT_TYPE_OPERATOR_WRITE_;
        }
    }
    else {
        throw CODE_ERR_NOT_FINED_ID_;
    }
}

void SemanticAnalyzer::CheckParamsOperatorInput(std::string name) {
    if (MAIN_TID_ == nullptr) {
        MAIN_TID_ = new TID_(nullptr);
        CUR_TID_ = MAIN_TID_;
    }
    if (deepSearchID(name)) {
        generator->insert_expr_poliz(TYPE_IDENTIFICATOR_, *getIdent(name));
        if (getIdent(name)->type_ == CODE_TYPE_BOOLEAN_) {
            throw CODE_ERR_INCORRECT_TYPE_OPERATOR_READ_;
        }
    }
    else {
        throw CODE_ERR_NOT_FINED_ID_;
    }
}

void SemanticAnalyzer::setNameCallFunc(std::string name) {
    call_func.name_ = name;
    IDENT_* ident = new IDENT_(name, TYPE_CALL_FUNC_);
    generator->insert_expr_poliz(TYPE_CALL_FUNC_, *ident);
    generator->insert_expr_poliz(TYPE_OPERATION_, "(");
    AnalyzeCallFunc = true;
}

void SemanticAnalyzer::addCallParamsFunc(std::string name, short int type, std::string val_if_const) {
    if (AnalyzeCallFunc) {
        if (type == 0) {
            IDENT_* id = getIdent(name);
            if (id == nullptr) {
                throw CODE_ERR_NOT_FINED_ID_;
            }
            generator->insert_expr_poliz(TYPE_IDENTIFICATOR_, *id, true);
            call_func.params.push_back(id);
        }
        else if (type == 1) {
            IDENT_* ident = new IDENT_("", getCodeType(TYPE_STRING_, 0), val_if_const);
            call_func.params.push_back(ident);
            generator->insert_expr_poliz(TYPE_CONST_, *ident, true);
        }
        else if (type == 2) {
            IDENT_* ident = new IDENT_("", getCodeType(TYPE_BOOLEAN_, 0), val_if_const);
            call_func.params.push_back(ident);
            generator->insert_expr_poliz(TYPE_CONST_, *ident, true);
        }
        else if (type == 3) {
            int pos = name.find(".", 0);
            if (pos != -1) {
                IDENT_* ident = new IDENT_("", getCodeType(TYPE_DOUBLE_, 0), val_if_const);
                call_func.params.push_back(ident);
                generator->insert_expr_poliz(TYPE_CONST_, *ident, true);
            }
            else {
                IDENT_* ident = new IDENT_("", getCodeType(TYPE_INT_, 0), val_if_const);
                call_func.params.push_back(ident);
                generator->insert_expr_poliz(TYPE_CONST_, *ident, true);
            }
        }
    }
}

int SemanticAnalyzer::stopAnalyzeCallFunc() {
    generator->insert_expr_poliz(TYPE_OPERATION_, ")");
    if (AnalyzeCallFunc) {
        std::vector<FUNC_> tmp_func = MAIN_TID_->identifiers.funcs;
        for (int ii = 0; ii < tmp_func.size(); ++ii) {
            if (tmp_func[ii].name_ == call_func.name_) {
                std::vector<IDENT_*> tmp_ident = tmp_func[ii].params;
                if (tmp_ident.size() == call_func.params.size()) {
                    int jj = 0;
                    for (jj = 0; jj < tmp_ident.size(); ++jj) {
                        if ((tmp_ident[jj]->type_ == 2 && call_func.params[jj]->type_ != 2) ||
                            (tmp_ident[jj]->type_ != 2 && call_func.params[jj]->type_ == 2)) {

                            break;
                        }
                    }
                    if (jj == tmp_ident.size()) {
                        AnalyzeCallFunc = false;
                        call_func.params.clear();
                        return ii;
                    }
                }
            }
        }
        throw CODE_ERR_NOT_FINED_FUNC_;
    }
}

std::string SemanticAnalyzer::getTypeByCode_(short int code_type) {
    if (code_type == CODE_TYPE_INT_) {
        return TYPE_INT_;
    }
    else if (code_type == CODE_TYPE_STRING_) {
        return TYPE_STRING_;
    }
    else if (code_type == CODE_TYPE_BOOLEAN_) {
        return TYPE_BOOLEAN_;
    }
    else if (code_type == CODE_TYPE_DOUBLE_) {
        return TYPE_DOUBLE_;
    }
    else if (code_type == 5) {
        return TYPE_INT_;
    }
    else if (code_type == 6) {
        return TYPE_STRING_;
    }
    else if (code_type == 7) {
        return TYPE_BOOLEAN_;
    }
    else if (code_type == 8) {
        return TYPE_DOUBLE_;
    }
    else if (code_type == 9) {
        return "main";
    }
    else if (code_type == 10) {
        return "def";
    }
    else {
        throw CODE_ERR_INTERNAL_SEMANTIC_ANALYZER_;
    }
}


std::string SemanticAnalyzer::check_compatible_op_and_st_(std::string left, std::string right, std::string op, bool may_unary) {
    if (may_unary) {
        if ((op == "++" || op == "--") && (right == TYPE_DOUBLE_ || right == TYPE_INT_)) {
            return right;
        }
        else if (op == "!" && (right != TYPE_STRING_)) {
            return right;
        }
        else if (op == "~" && right == TYPE_INT_) {
            return right;
        }
        else if ((op == "+" || op == "-") && (right == TYPE_INT_ || right == TYPE_DOUBLE_ || right == TYPE_BOOLEAN_)) {
            return right;
        }
        else {
            throw CODE_ERR_INTERNAL_SEMANTIC_ANALYZER_;
        }
    }
    else {
        if (left == TYPE_DOUBLE_ || right == TYPE_DOUBLE_) {
            if (op == "%" || op == "<<" || op == ">>" || op == "&" || op == "|" || op == "%=", op == "^") {
                throw CODE_ERR_OPERATIONS_WITH_DOUBLE_;
            }
            if (left == TYPE_STRING_ || right == TYPE_STRING_) {
                throw CODE_ERR_OPERATIONS_WITH_DOUBLE_ANS_STRING_;
            }
            return TYPE_DOUBLE_;
        }
        else if (left == TYPE_STRING_ && right == TYPE_STRING_) {
            if (op == "+" || op == "=" || op == "+=") {
                return TYPE_STRING_;
            } else {
                throw CODE_ERR_OPERATIONS_TO_STRING_;
            }
        }
        else if (left == TYPE_STRING_ || right == TYPE_STRING_) {
            if (((left == TYPE_STRING_) * (right == TYPE_STRING_)) == 0) {
                throw CODE_ERR_OPERATIONS_WITH_STRING_;
            }
        }
        else if (left == TYPE_INT_) {
            if (right == TYPE_BOOLEAN_) {
                return TYPE_INT_;
            }
            else if (right == TYPE_DOUBLE_) {
                return TYPE_DOUBLE_;
            }
            else if (right == TYPE_INT_) {
                return TYPE_INT_;
            }
        }
        else if (right == TYPE_INT_) {
            if (left == TYPE_BOOLEAN_) {
                return TYPE_INT_;
            }
            else if (left == TYPE_DOUBLE_) {
                return TYPE_DOUBLE_;
            }
            else if (left == TYPE_INT_) {
                return TYPE_INT_;
            }
        }
        else if (left == TYPE_BOOLEAN_) {
            if (right == TYPE_BOOLEAN_) {
                return TYPE_BOOLEAN_;
            }
            else if (right == TYPE_DOUBLE_) {
                return TYPE_DOUBLE_;
            }
            else if (right == TYPE_INT_) {
                return TYPE_INT_;
            }
        }
        else if (right == TYPE_BOOLEAN_) {
            if (left == TYPE_BOOLEAN_) {
                return TYPE_BOOLEAN_;
            }
            else if (left == TYPE_DOUBLE_) {
                return TYPE_DOUBLE_;
            }
            else if (left == TYPE_INT_) {
                return TYPE_INT_;
            }
        }
    }
}

void SemanticAnalyzer::start_expr() {
    may_unary = true;
}

void SemanticAnalyzer::addOperand(std::string name, short int type, int id_func, std::string val_if_const) {
    if (type == 0) {
        expr.push_back(getTypeByCode_(getIdent(name)->type_));
        IDENT_* id = getIdent(name);
        generator->insert_expr_poliz(TYPE_IDENTIFICATOR_, *id);
    }
    else if (type == TYPE_IDENTIFICATOR_AD_) {
        expr.push_back(getTypeByCode_(getIdent(name)->type_));
        IDENT_* id = getIdent(name);
        generator->insert_expr_poliz(TYPE_IDENTIFICATOR_AD_, *id);
    }
    else if (type == 1) {
        expr.push_back(getTypeByCode_(CODE_TYPE_STRING_));
        IDENT_* ident = new IDENT_("", getCodeType(TYPE_STRING_, 0), val_if_const);
        generator->insert_expr_poliz(TYPE_CONST_, *ident);
    }
    else if (type == 2) {
        expr.push_back(getTypeByCode_(CODE_TYPE_BOOLEAN_));
        IDENT_* ident = new IDENT_("", getCodeType(TYPE_BOOLEAN_, 0), val_if_const);
        generator->insert_expr_poliz(TYPE_CONST_, *ident);
    }
    else if (type == 3) {
        int pos = val_if_const.find(".", 0);
        if (pos != -1) {
            expr.push_back(getTypeByCode_(CODE_TYPE_DOUBLE_));
            IDENT_* ident = new IDENT_("", getCodeType(TYPE_DOUBLE_, 0), val_if_const);
            generator->insert_expr_poliz(TYPE_CONST_, *ident);
        }
        else {
            expr.push_back(getTypeByCode_(CODE_TYPE_INT_));
            IDENT_* ident = new IDENT_("", getCodeType(TYPE_INT_, 0), val_if_const);
            generator->insert_expr_poliz(TYPE_CONST_, *ident);
        }
    }
    else if (type == 4) {
        expr.push_back(getTypeByCode_(MAIN_TID_->identifiers.funcs[id_func].type_));
    }
    else {
        // throw err
    }
}

void SemanticAnalyzer::addOperation(std::string operation) {
    expr.push_back(operation);
    generator->insert_expr_poliz(TYPE_OPERATION_, operation);
}

short int SemanticAnalyzer::calc_expr() {
    if (expr.size() == 0) {
        return -1;
    }
    std::vector<std::pair<std::string, bool>> polish_expr;
    std::stack<std::pair<std::string, bool>> tmp;
    bool unary = false;
    for (unsigned int ii = 0; ii < expr.size(); ++ii) {
        unary = false;
        if (expr[ii] == TYPE_INT_ || expr[ii] == TYPE_DOUBLE_ || expr[ii] == TYPE_STRING_ || expr[ii] == TYPE_BOOLEAN_) {
            polish_expr.push_back(std::make_pair(expr[ii], false));
        }
        else if (expr[ii] == TYPE_DEF_) {
            short int ans_type = getCodeType(TYPE_DEF_, 1);
            expr.clear();
            generator->genPolizExpr();
            return ans_type;
        }
        else if (expr[ii] == "(") {
            tmp.push(std::make_pair(expr[ii], false));
        }
        else if (expr[ii] == ")") {
            while (tmp.top().first != "(") {
                polish_expr.push_back(tmp.top());
                tmp.pop();
            }
            tmp.pop();
        }
        else {
            if (expr[ii] == "+" || expr[ii] == "-") {
                if (ii == 0) {
                    unary = true;
                }
                else if (expr[ii - 1] == "(") {
                    unary = true;
                }
                else if (expr[ii - 1] != TYPE_INT_ && expr[ii - 1] != TYPE_DOUBLE_ && expr[ii - 1] != TYPE_STRING_ && expr[ii - 1] != TYPE_BOOLEAN_ && expr[ii - 1] != ")") {
                    unary = true;
                }
                else {
                    unary = false;
                }
            }
            if (tmp.size() > 0) {
                if (getPriority(tmp.top().first, tmp.top().second) > getPriority(expr[ii], unary)) {
                    tmp.push(std::make_pair(expr[ii], unary));
                }
                else {
                    while (getPriority(tmp.top().first, tmp.top().second) <= getPriority(expr[ii], unary)) {
                        polish_expr.push_back(tmp.top());
                        tmp.pop();
                        if (tmp.size() == 0) {
                            break;
                        }
                    }
                    tmp.push(std::make_pair(expr[ii], false));
                }
            }
            else {
                tmp.push(std::make_pair(expr[ii], unary));
            }
        }
    }
    for (; tmp.size() > 0;) {
        polish_expr.push_back(tmp.top());
        tmp.pop();
    }
    expr.clear();
    std::stack<std::string> types;
    for (int ii = 0; ii < polish_expr.size(); ++ii) {
        if (polish_expr[ii].first == TYPE_INT_ || polish_expr[ii].first == TYPE_DOUBLE_ || polish_expr[ii].first == TYPE_BOOLEAN_ ||
            polish_expr[ii].first == TYPE_STRING_) {

            types.push(polish_expr[ii].first);
        }
        else if (polish_expr[ii].first == "++" || polish_expr[ii].first == "--" || polish_expr[ii].first == "~"
            || polish_expr[ii].first == "!" || ((polish_expr[ii].first == "+" || polish_expr[ii].first == "-") && polish_expr[ii].second)) {
            
            std::string n1_ = types.top();
            types.pop();
            types.push(check_compatible_op_and_st_("", n1_, polish_expr[ii].first, true));
        }
        else {
            std::string n2_ = types.top();
            types.pop();
            std::string n3_ = types.top();
            types.pop();
            types.push(check_compatible_op_and_st_(n2_, n3_, polish_expr[ii].first, polish_expr[ii].second));
        }
    }
    short int ans_type = getCodeType(types.top(), 0);
    expr.clear();
    generator->genPolizExpr();
    return ans_type;
}

bool SemanticAnalyzer::FindTypeTID(short int type_tid) {
    TID_* TMP_ = CUR_TID_;
    while (TMP_ != nullptr) {
        if (TMP_->type_tid == type_tid) {
            return true;
        }
        TMP_ = TMP_->prev_;
    }
    return false;
}

void SemanticAnalyzer::setGeneratorTid() {
    generator->setTID(MAIN_TID_);
}

