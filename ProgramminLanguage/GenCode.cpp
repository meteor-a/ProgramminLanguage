#include "GenCode.h"
#include "Types.h"

void GenCode::insert_start_block() {
    PolizItem item(TYPE_START_BLOCK_);
    POLIZ_.push_back(item);
}

void GenCode::insert_end_block() {
    PolizItem item(TYPE_END_BLOCK_);
    POLIZ_.push_back(item);
}

void GenCode::insert_write_func() {
    PolizItem item(TYPE_WRITE_);
    POLIZ_.push_back(item);
}

void GenCode::insert_read_func() {
    PolizItem item(TYPE_READ_);
    POLIZ_.push_back(item);
}

void GenCode::insert_semicolon() {
    PolizItem item(TYPE_SEMICOLON_);
    POLIZ_.push_back(item);
}

void GenCode::insert_return() {
    PolizItem item(TYPE_RETURN_COMMAND_);
    POLIZ_.push_back(item);
    PolizItem item2_(TYPE_SEMICOLON_);
    POLIZ_.push_back(item2_);
}

void GenCode::insert_declaration_func(FUNC_ func) {
    functions.push_back(std::make_pair(POLIZ_.size(), func));
    PolizItem item(TYPE_FUNC_DECLARATION_, functions.size() - 1);
    POLIZ_.push_back(item);
    for (int ii = 0; ii < func.params.size(); ++ii) {
        PolizItem tmp_item(TYPE_IDENTIFICATOR_AD_, *func.params[ii]);

        if (func.params[ii]->type_ == CODE_TYPE_BOOLEAN_) {
            func.params[ii]->cur_val.push_back("false");
        }
        else if (func.params[ii]->type_ == CODE_TYPE_DOUBLE_) {
            func.params[ii]->cur_val.push_back("0.0");
        }
        else if (func.params[ii]->type_ == CODE_TYPE_INT_) {
            func.params[ii]->cur_val.push_back("0");
        }
        else if (func.params[ii]->type_ == CODE_TYPE_STRING_) {
            func.params[ii]->cur_val.push_back("");
        }

        POLIZ_.push_back(tmp_item);
    }
    PolizItem tmp_item(TYPE_SEMICOLON_);
    POLIZ_.push_back(tmp_item);
}

void GenCode::insert_expr_poliz(short int type) {
    expr_poliz.push_back(PolizItem(type));
}

void GenCode::insert_expr_poliz(short int type, IDENT_& id) {
    expr_poliz.push_back(PolizItem(type, id));
}

void GenCode::insert_expr_poliz(short int type, IDENT_& id, bool is_call_func_param) {
    expr_poliz.push_back(PolizItem(type, id, is_call_func_param));
}

void GenCode::insert_expr_poliz(short int type, short int type_const, std::string val) {
    IDENT_ *id = new IDENT_("", CODE_TYPE_STRING_, val);
    expr_poliz.push_back(PolizItem(type, *id));
}

void GenCode::insert_expr_poliz(short int type, std::string oper) {
    expr_poliz.push_back(PolizItem(type, oper));
}

size_t GenCode::insert_conditional_jump() {
    PolizItem item(TYPE_CONDITIONAL_JUMP_);
    size_t insert_pos = POLIZ_.size();
    POLIZ_.push_back(item);
    return insert_pos;
}

size_t GenCode::insert_conditional_jump_dowhile(size_t pos_to_jump) {
    PolizItem item(TYPE_CONDITIONAL_JUMP_DOWHILE_);
    item.setPos(pos_to_jump);
    size_t insert_pos = POLIZ_.size();
    POLIZ_.push_back(item);
    return insert_pos;
}

void GenCode::pop_pos_break() {
    pos_break_.pop();
}

void GenCode::insert_index_jump(size_t pos_poliz) {
    POLIZ_[pos_poliz].setPos(POLIZ_.size());
}

size_t GenCode::insert_unconditional_jump() {
    PolizItem item(TYPE_UNCONDITIONAL_JUMP_);
    size_t insert_pos = POLIZ_.size();
    POLIZ_.push_back(item);
    return insert_pos;
}

size_t GenCode::insert_unconditional_jump(size_t pos_to_jump) {
    PolizItem item(TYPE_UNCONDITIONAL_JUMP_);
    item.setPos(pos_to_jump);
    POLIZ_.push_back(item);
    return POLIZ_.size();
}

void GenCode::push_pos_break(size_t pos_break) {
    pos_break_.push(pos_break);
}

size_t GenCode::top_pos_break() {
    return pos_break_.top();
}

size_t GenCode::getSizePosBreak() {
    return pos_break_.size();
}

void GenCode::genPolizExpr() {
    std::vector<PolizItem> polish_expr;
    std::stack<PolizItem> tmp;
    bool unary = false;
    for (unsigned int ii = 0; ii < expr_poliz.size(); ++ii) {
        unary = false;
        if (expr_poliz[ii].getTypeItem() == TYPE_IDENTIFICATOR_ || expr_poliz[ii].getTypeItem() == TYPE_IDENTIFICATOR_AD_ ||expr_poliz[ii].getTypeItem() == TYPE_CONST_) {
            polish_expr.push_back(expr_poliz[ii]);
        }
        else if (expr_poliz[ii].getTypeItem() == TYPE_CALL_FUNC_) { // после скобочки функции + считается унарным
            PolizItem tmp_item = expr_poliz[ii];
            ii += 2;
            while (expr_poliz[ii].getOper() != ")") {
                polish_expr.push_back(expr_poliz[ii]);
                ++ii;
            }
            expr_poliz[ii].setType(TYPE_FUNC_CALL_BRACKETS);
            polish_expr.push_back(tmp_item);
        }
        else if (expr_poliz[ii].getOper() == "(") {
            tmp.push(expr_poliz[ii]);
        }
        else if (expr_poliz[ii].getOper() == ")") {
            while (tmp.top().getOper() != "(") {
                polish_expr.push_back(tmp.top());
                tmp.pop();
            }
            tmp.pop();
        }
        else {
            if (expr_poliz[ii].getOper() == "+" || expr_poliz[ii].getOper() == "-") {
                if (ii == 0) {
                    unary = true;
                }
                else if (expr_poliz[ii - 1].getOper() == "(") {
                    unary = true;
                }
                else if (expr_poliz[ii - 1].getTypeItem() != TYPE_CONST_ && expr_poliz[ii - 1].getTypeItem() != TYPE_IDENTIFICATOR_AD_ && expr_poliz[ii - 1].getTypeItem() != TYPE_IDENTIFICATOR_ && expr_poliz[ii - 1].getTypeItem() != TYPE_CALL_FUNC_ && 
                    expr_poliz[ii - 1].getTypeItem() != TYPE_FUNC_CALL_BRACKETS && expr_poliz[ii - 1].getOper() != ")") {

                    unary = true;
                }
                else {
                    unary = false;
                }
            }

            if (tmp.size() > 0) {
                if (getPriority(tmp.top().getOper(), tmp.top().getUnary()) > getPriority(expr_poliz[ii].getOper(), unary)) {
                    PolizItem tmp_item = expr_poliz[ii];
                    tmp_item.setUnary(unary);
                    tmp.push(tmp_item);
                }
                else {
                    while (getPriority(tmp.top().getOper(), tmp.top().getUnary()) <= getPriority(expr_poliz[ii].getOper(), unary)) {
                        PolizItem out_stack = tmp.top();
                        tmp.pop();
                        polish_expr.push_back(out_stack);
                        if (tmp.size() == 0) {
                            break;
                        }
                    }
                    PolizItem tmp_item = expr_poliz[ii];
                    tmp_item.setUnary(false);
                    tmp.push(tmp_item);
                }
            }
            else {
                PolizItem tmp_item = expr_poliz[ii];
                tmp_item.setUnary(unary);
                tmp.push(tmp_item);
            }
        }
    }

    for (; tmp.size() > 0;) {
        polish_expr.push_back(tmp.top());
        tmp.pop();
    }

    expr_poliz.clear();

    for (int ii = 0; ii < polish_expr.size(); ++ii) {
        POLIZ_.push_back(polish_expr[ii]);
    }
}

size_t GenCode::getSizePoliz() {
    return POLIZ_.size();
}

void GenCode::StartProgramm() {
    InterputProgrammMain_(start_pos_, MAIN_TID_);
}

void GenCode::setStartPos() {
    start_pos_ = POLIZ_.size();
}

void GenCode::setTID(TID_* tid) {
    MAIN_TID_ = tid;
}

void GenCode::RELOAD() {
    expr_poliz.clear();
    start_pos_ = -1;
    POLIZ_.clear();
    functions.clear();
    while (pos_break_.size() > 0) {
        pos_break_.pop();
    }
}


void GenCode::InterputProgrammMain_(size_t pos, TID_* CUR_TID) {
    if (POLIZ_[pos].getTypeItem() == TYPE_START_BLOCK_) {
        ++pos;
    }

    if (POLIZ_[pos].getTypeItem() == TYPE_FUNC_DECLARATION_) {
        //unsigned int num_tid_children = FindFuncNum(pos);
        //TID_* tmp = CUR_TID->children[num_tid_children];
        std::vector<IDENT_> params;
        InterputFunc_(++pos, params, true, 0);
    }
}

unsigned int GenCode::FindFuncNum(int num) {
    for (unsigned int ii = 0; ii < functions.size(); ++ii) {
        if (functions[ii].first == num) {
            return ii;
        }
    }
    return -1;
}

std::pair<int, std::string> GenCode::InterputFunc_(size_t pos, std::vector<IDENT_> params, bool is_main, unsigned int deapth) {
    if (params.size() != 0 && !is_main) {
        GetReadyFunc_(pos, params);
    }
    if (deapth > 20) {
        throw CODE_ERR_STACK_OVERFLOW;
    }
    std::stack<PolizItem> stack_expr;
    for (size_t ii = pos; ii < POLIZ_.size(); ++ii) {
        if (POLIZ_[ii].getTypeItem() == TYPE_IDENTIFICATOR_ || POLIZ_[ii].getTypeItem() == TYPE_IDENTIFICATOR_AD_ || POLIZ_[ii].getTypeItem() == TYPE_CONST_) {
            stack_expr.push(POLIZ_[ii]);
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_CALL_FUNC_) {
            std::vector<IDENT_> params;
            while (stack_expr.size() > 0) {
                if (stack_expr.top().getUnary() == true) {
                    params.push_back(*stack_expr.top().getIdent());
                    stack_expr.pop();
                }
                else {
                    break;
                }
            }
            std::reverse(params.begin(), params.end());
            unsigned int pos_start = FindPosFunc_(POLIZ_[ii].getIdent()->id_, params);
            FUNC_ call_func = functions[POLIZ_[pos_start].getPos()].second;
            std::pair<int, std::string> result_func = InterputFunc_(pos_start, params, false, deapth + 1);
            GetBackFuncAfterCall_(pos_start);
            std::string res_call_func = "";
            
            if (result_func.first == CODE_TYPE_TID_DEF_) {
                // ничего
            }
            else if (result_func.first == TYPE_RETURN_VAL_MISS) {
                // ничего
            }
            else {
                if (result_func.first == CODE_TYPE_INT_) {
                    if (call_func.type_ == CODE_TYPE_TID_INT_FUNC_) {
                        res_call_func = result_func.second;
                    }
                    else if (call_func.type_ == CODE_TYPE_TID_DOUBLE_FUNC_) {
                        res_call_func = result_func.second + ".0";
                    }
                    else if (call_func.type_ == CODE_TYPE_TID_BOOLEAN_FUNC_) {
                        if (std::stoi(result_func.second) > 0) {
                            res_call_func = "true";
                        }
                        else {
                            res_call_func = "false";
                        }
                    }
                }
                else if (result_func.first == CODE_TYPE_DOUBLE_) {
                    if (call_func.type_ == CODE_TYPE_TID_INT_FUNC_) {
                        res_call_func = result_func.second.substr(0, result_func.second.find('.'));
                    }
                    else if (call_func.type_ == CODE_TYPE_TID_DOUBLE_FUNC_) {
                        res_call_func = result_func.second;
                    }
                    else if (call_func.type_ == CODE_TYPE_TID_BOOLEAN_FUNC_) {
                        if (std::stod(result_func.second) > 0) {
                            res_call_func = "true";
                        }
                        else {
                            res_call_func = "false";
                        }
                    }
                }
                else if (result_func.first == CODE_TYPE_BOOLEAN_) {
                    if (call_func.type_ == CODE_TYPE_TID_INT_FUNC_) {
                        if (result_func.second == "true") {
                            res_call_func = "1";
                        }
                        else {
                            res_call_func = "0";
                        }
                    }
                    else if (call_func.type_ == CODE_TYPE_TID_DOUBLE_FUNC_) {
                        if (result_func.second == "true") {
                            res_call_func = "1.0";
                        }
                        else {
                            res_call_func = "0.0";
                        }
                    }
                    else if (call_func.type_ == CODE_TYPE_TID_BOOLEAN_FUNC_) {
                        res_call_func = result_func.second;
                    }
                }
                else if (result_func.first == CODE_TYPE_STRING_) {
                    res_call_func = result_func.second;
                }

                short int type_of_ans_call_func_id;
                if (call_func.type_ == CODE_TYPE_TID_INT_FUNC_) {
                    type_of_ans_call_func_id = CODE_TYPE_INT_;
                }
                else if (call_func.type_ == CODE_TYPE_TID_DOUBLE_FUNC_) {
                    type_of_ans_call_func_id = CODE_TYPE_DOUBLE_;
                }
                else if (call_func.type_ == CODE_TYPE_TID_BOOLEAN_FUNC_) {
                    type_of_ans_call_func_id = CODE_TYPE_BOOLEAN_;
                }
                else if (call_func.type_ == CODE_TYPE_TID_STRING_FUNC_) {
                    type_of_ans_call_func_id = CODE_TYPE_STRING_;
                }
                IDENT_* id = new IDENT_("", type_of_ans_call_func_id, res_call_func);
                PolizItem res_func(TYPE_CONST_, *id);
                stack_expr.push(res_func);
            }
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_WRITE_) {
            std::vector<std::string> output;
            while (stack_expr.size() > 0) {
                output.push_back(stack_expr.top().getIdent()->cur_val[stack_expr.top().getIdent()->cur_val.size() - 1]);
                stack_expr.pop();
            }
            for (int ii = output.size() - 1; ii >= 0; --ii) {
                std::cout << output[ii];
            }
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_READ_) {
            std::string input = "";
            std::cin >> input;
            if (stack_expr.top().getIdent()->type_ == CODE_TYPE_STRING_) {
                stack_expr.top().getIdent()->cur_val[stack_expr.top().getIdent()->cur_val.size() - 1] = input;
            }
            else {
                stack_expr.top() = input_poliz_item(input);
            }
            stack_expr.pop();
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_SEMICOLON_) {
            while (stack_expr.size() > 0) {
                stack_expr.pop();
            }
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_OPERATION_) {
            if (POLIZ_[ii].getOper() == "=" || POLIZ_[ii].getOper() == "+=" || POLIZ_[ii].getOper() == "-=" || POLIZ_[ii].getOper() == "*=" || POLIZ_[ii].getOper() == "/="
                || POLIZ_[ii].getOper() == "%=") {

                PolizItem n2_ = stack_expr.top();
                stack_expr.pop();
                PolizItem n3_ = stack_expr.top();
                stack_expr.pop();
                make_operation_assignm(n2_, n3_, POLIZ_[ii]);
            }
            else if (POLIZ_[ii].getOper() == "~" || POLIZ_[ii].getOper() == "!" || ((POLIZ_[ii].getOper() == "+" ||
                POLIZ_[ii].getOper() == "-") && POLIZ_[ii].getUnary())) {

                PolizItem n1_ = stack_expr.top();
                stack_expr.pop();
                stack_expr.push(make_operation(n1_, POLIZ_[ii]));
            }
            else if (POLIZ_[ii].getOper() == "++" || POLIZ_[ii].getOper() == "--") {
                PolizItem n1_ = stack_expr.top();
                stack_expr.pop();
                PolizItem result = make_operation(n1_, POLIZ_[ii]);
                if (result.getPos() == 1) {
                    stack_expr.push(result);
                }
            }
            else {
                PolizItem n2_ = stack_expr.top();
                stack_expr.pop();
                PolizItem n3_ = stack_expr.top();
                stack_expr.pop();
                PolizItem it = make_operation(n3_, n2_, POLIZ_[ii]);
                stack_expr.push(it);
            }
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_CONDITIONAL_JUMP_) {
            PolizItem n2_ = stack_expr.top();
            stack_expr.pop();
            if (n2_ == false) {
                ii = POLIZ_[ii].getPos();
                --ii;
            }
            while (stack_expr.size() > 0) {
                stack_expr.pop();
            }
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_CONDITIONAL_JUMP_DOWHILE_) {
            PolizItem n2_ = stack_expr.top();
            stack_expr.pop();
            if (n2_ == true) {
                ii = POLIZ_[ii].getPos();
                --ii;
            }
            while (stack_expr.size() > 0) {
                stack_expr.pop();
            }
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_UNCONDITIONAL_JUMP_) {
            ii = POLIZ_[ii].getPos();
            --ii;
            while (stack_expr.size() > 0) {
                stack_expr.pop();
            }
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_RETURN_COMMAND_) {
            if (stack_expr.size() > 0) {
                return std::make_pair(stack_expr.top().getIdent()->type_, stack_expr.top().getIdent()->cur_val[stack_expr.top().getIdent()->cur_val.size() - 1]);
            }
            else {
                return std::make_pair(CODE_TYPE_TID_DEF_, "");
            }
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_END_BLOCK_) {
            return std::make_pair(TYPE_RETURN_VAL_MISS, "");
        }
    }
}

PolizItem GenCode::input_poliz_item(std::string input) {
    IDENT_* id;
    if (input == "true" || input == "false") {
        id = new IDENT_("",CODE_TYPE_BOOLEAN_, input);
        return PolizItem(TYPE_CONST_, *id);
    }
    else if (input.find_first_not_of("0123456789") == std::string::npos) {
        id = new IDENT_("", CODE_TYPE_INT_, input);
        return PolizItem(TYPE_CONST_, *id);
    }
    else {
        for (int ii = 0; ii < input.length(); ++ii) {
            if (std::isalpha(input[ii])) {
                id = new IDENT_("", CODE_TYPE_STRING_, input);
                return PolizItem(TYPE_CONST_, *id);
            }
        }
        int count_point = 0;
        bool is_point = false;
        unsigned int number_before_point = 0;
        unsigned int number_after_point = 0;
        for (int ii = 0; ii < input.length(); ++ii) {
            if (input[ii] == '.') {
                ++count_point;
                is_point = true;
            }
            else if (std::isdigit(input[ii]) && !is_point) {
                ++number_before_point;
            }
            else if (std::isdigit(input[ii]) && is_point) {
                ++number_after_point;
            }
        }
        if (count_point > 1) {
            throw CODE_ERR_INCORRECT_TYPE_INPUT_;
        }
        else if (number_before_point == 0) {
            throw CODE_ERR_INCORRECT_TYPE_INPUT_;
        }
        else if (number_after_point == 0) {
            throw CODE_ERR_INCORRECT_TYPE_INPUT_;
        }

        id = new IDENT_("",CODE_TYPE_DOUBLE_, input);
        return PolizItem(TYPE_CONST_, *id);
    }
}

PolizItem GenCode::make_operation(PolizItem n2_, PolizItem n3_, PolizItem oper) {
    if (oper.getOper() == "*") {
        return n2_ * n3_;
    }
    else if (oper.getOper() == "/") {
        return n2_ / n3_;
    }
    else if (oper.getOper() == "%") {
        return n2_ % n3_;
    }
    else if (oper.getOper() == "+") {
        return n2_ + n3_;
    }
    else if (oper.getOper() == "-") {
        return n2_ - n3_;
    }
    else if (oper.getOper() == "<") {
        std::string res;
        if (n2_ < n3_) {
            res = "true";
        } else {
            res = "false";
        }
        IDENT_* id = new IDENT_("", CODE_TYPE_BOOLEAN_, res);
        PolizItem item(TYPE_CONST_, *id);
        return item;
    }
    else if (oper.getOper() == ">") {
        std::string res;
        if (n2_ > n3_) {
            res = "true";
        }
        else {
            res = "false";
        }
        IDENT_* id = new IDENT_("", CODE_TYPE_BOOLEAN_, res);
        PolizItem item(TYPE_CONST_, *id);
        return item;
    }
    else if (oper.getOper() == "<=") {
        std::string res;
        if (n2_ <= n3_) {
            res = "true";
        }
        else {
            res = "false";
        }
        IDENT_* id = new IDENT_("", CODE_TYPE_BOOLEAN_, res);
        PolizItem item(TYPE_CONST_, *id);
        return item;
    }
    else if (oper.getOper() == ">=") {
        std::string res;
        if (n2_ >= n3_) {
            res = "true";
        }
        else {
            res = "false";
        }
        IDENT_* id = new IDENT_("", CODE_TYPE_BOOLEAN_, res);
        PolizItem item(TYPE_CONST_, *id);
        return item;
    }
    else if (oper.getOper() == "==") {
        std::string res;
        if (n2_ == n3_) {
            res = "true";
        }
        else {
            res = "false";
        }
        IDENT_* id = new IDENT_("", CODE_TYPE_BOOLEAN_, res);
        PolizItem item(TYPE_CONST_, *id);
        return item;
    }
    else if (oper.getOper() == "<>") {
        std::string res;
        if (n2_ != n3_) {
            res = "true";
        }
        else {
            res = "false";
        }
        IDENT_* id = new IDENT_("", CODE_TYPE_BOOLEAN_, res);
        PolizItem item(TYPE_CONST_, *id);
        return item;
    }
    else if (oper.getOper() == "|") {
        return n2_ | n3_;
    }
    else if (oper.getOper() == "&") {
        return n2_ & n3_;
    }
    else if (oper.getOper() == "^") {
        return n2_ ^ n3_;
    }
    else if (oper.getOper() == "<<") {
        return n2_ << n3_;
    }
    else if (oper.getOper() == ">>") {
        return n2_ >> n3_;
    }
    else if (oper.getOper() == "&&") {
        std::string res;
        if (n2_ && n3_) {
            res = "true";
        }
        else {
            res = "false";
        }
        IDENT_* id = new IDENT_("", CODE_TYPE_BOOLEAN_, res);
        PolizItem item(TYPE_CONST_, *id);
        return item;
    }
    else if (oper.getOper() == "||") {
        std::string res;
        if (n2_ || n3_) {
            res = "true";
        }
        else {
            res = "false";
        }
        IDENT_* id = new IDENT_("", CODE_TYPE_BOOLEAN_, res);
        PolizItem item(TYPE_CONST_, *id);
        return item;
    }

    return NULL;
}

void GenCode::make_operation_assignm(PolizItem n2_, PolizItem n3_, PolizItem oper) {
    if (oper.getOper() == "=") {
        n3_ = n2_;
    }
    else if (oper.getOper() == "+=") {
        n3_ += n2_;
    }
    else if (oper.getOper() == "-=") {
        n3_ -= n2_;
    }
    else if (oper.getOper() == "*=") {
        n3_ *= n2_;
    }
    else if (oper.getOper() == "/=") {
        n3_ /= n2_;
    }
    else if (oper.getOper() == "%=") {
        n3_ %= n2_;
    }
}

PolizItem GenCode::make_operation(PolizItem n2_, PolizItem oper) {
    if (oper.getOper() == "++") {
        return ++n2_;
    }
    else if (oper.getOper() == "--") {
        return --n2_;
    }
    else if (oper.getOper() == "-") {
        std::string res = "-1";
        IDENT_* id = new IDENT_("", CODE_TYPE_INT_, res);
        PolizItem item(TYPE_CONST_, *id);
        return n2_ * item;
    }
    else if (oper.getOper() == "+") {
        return n2_;
    }
    else if (oper.getOper() == "!") {

    }
    else if (oper.getOper() == "~") {
        return ~n2_;
    }
}

/*
unsigned int GenCode::FindPosFunc_(std::string id_func, std::vector<IDENT_> params) {
    for (int ii = 0; ii < functions.size(); ++ii) {
        if (functions[ii].second.name_ == id_func) {
            return functions[ii].first;
        }
    }

    return -1;
}
*/
unsigned int GenCode::FindPosFunc_(std::string id_func, std::vector<IDENT_> params) {
    for (int ii = 0; ii < functions.size(); ++ii) {
        if (functions[ii].second.name_ == id_func) {
            bool find = true;
            for (int jj = 0; jj < params.size(); ++jj) {
                if (functions[ii].second.params[jj]->type_ != params[jj].type_) {
                    find = false;
                    break;
                }
            }
            if (find) {
                return functions[ii].first;
            }
        }
    }

    for (int ii = 0; ii < functions.size(); ++ii) {
        if (functions[ii].second.name_ == id_func) {
            bool find = true;
            for (int jj = 0; jj < params.size(); ++jj) {
                if (functions[ii].second.params[jj]->type_ == CODE_TYPE_STRING_) {
                    if (params[jj].type_ == CODE_TYPE_STRING_) {
                        return functions[ii].first;
                    }
                    else {
                        find = false;
                        break;
                    }
                }
                else if (functions[ii].second.params[jj]->type_ == CODE_TYPE_INT_ || functions[ii].second.params[jj]->type_ == CODE_TYPE_DOUBLE_ ||
                    functions[ii].second.params[jj]->type_ == CODE_TYPE_BOOLEAN_) {

                    if (params[jj].type_ == CODE_TYPE_INT_ || params[jj].type_ == CODE_TYPE_BOOLEAN_ || params[jj].type_ == CODE_TYPE_DOUBLE_) {
                        return functions[ii].first;
                    }
                    else {
                        find = false;
                        break;
                    }
                }
            }
            if (find) {
                return functions[ii].first;
            }
        }
    }

    return -1;
}


void GenCode::GetReadyFunc_(int pos, std::vector<IDENT_> params) {
    for (int ii = pos; ii < POLIZ_.size(); ++ii) {
        if (POLIZ_[ii].getTypeItem() == TYPE_END_BLOCK_) {
            break;
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_IDENTIFICATOR_AD_) {
            if (POLIZ_[ii].getIdent()->type_ == CODE_TYPE_BOOLEAN_) {
                POLIZ_[ii].getIdent()->cur_val.push_back("false");
            }
            else if (POLIZ_[ii].getIdent()->type_ == CODE_TYPE_DOUBLE_) {
                POLIZ_[ii].getIdent()->cur_val.push_back("0.0");
            }
            else if (POLIZ_[ii].getIdent()->type_ == CODE_TYPE_INT_) {
                POLIZ_[ii].getIdent()->cur_val.push_back("0");
            }
            else if (POLIZ_[ii].getIdent()->type_ == CODE_TYPE_STRING_) {
                POLIZ_[ii].getIdent()->cur_val.push_back("");
            }
        }
    }

    std::vector<IDENT_*> tmp = functions[POLIZ_[pos].getPos()].second.params;
    for (int ii = 0; ii < tmp.size(); ++ii) {
        PolizItem left(TYPE_CONST_, *tmp[ii]);
        PolizItem right(TYPE_CONST_, params[ii]);
        left = right;
    }
}

void GenCode::GetBackFuncAfterCall_(int pos) {
    for (int ii = pos; ii < POLIZ_.size(); ++ii) {
        if (POLIZ_[ii].getTypeItem() == TYPE_END_BLOCK_) {
            break;
        }
        else if (POLIZ_[ii].getTypeItem() == TYPE_IDENTIFICATOR_AD_) {
            POLIZ_[ii].getIdent()->cur_val.pop_back();
        }
    }
}

