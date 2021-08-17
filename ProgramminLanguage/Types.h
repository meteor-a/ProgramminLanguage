#pragma once
#include <string>
#include<vector>

class Types {
protected:
    Types();
    struct IDENT_ {
        short int type_;
        std::string id_;
        std::vector<std::string> cur_val;
        bool is_declaration;

        IDENT_(std::string id, short int type) {
            id_ = id;
            type_ = type;
            cur_val.clear();
            is_declaration = false;
        }

        IDENT_(std::string id, short int type, std::string cur_v) {
            id_ = id;
            type_ = type;
            cur_val.push_back(cur_v);
            is_declaration = false;
        }

        IDENT_(std::string id, short int type, std::string cur_v, bool decl) {
            id_ = id;
            type_ = type;
            cur_val.push_back(cur_v);
            is_declaration = decl;
        }

        IDENT_() {
            id_ = "";
            type_ = -1;
            cur_val.clear();
            is_declaration = false;
        }

        bool operator == (const IDENT_& point) const {
            return this->type_ == point.type_ &&
                this->id_ == point.id_;
        }
    };

    struct FUNC_ {
        short int type_;
        std::string name_;
        std::vector<IDENT_*> params;

        FUNC_(std::string name, short int type) {
            type_ = type;
            name_ = name;
            params.clear();
        }

        FUNC_() {
            type_ = -1;
            name_ = "";
            params.clear();
        }

        bool operator == (const FUNC_& point) const {
            bool ans = this->name_ == point.name_;
            std::vector<IDENT_*> left = this->params;
            std::vector<IDENT_*> right = point.params;
            if (left.size() != right.size()) {
                return false;
            }
            else {
                for (int ii = 0; ii < left.size(); ++ii) {
                    if (left[ii]->type_ != right[ii]->type_) {
                        return false;
                    }
                }
                return true && ans;
            }
        }
    };

    struct IDENTIFIER_ {
        int num_of_ident;
        std::vector<IDENT_*> idents;
        int num_of_func;
        std::vector<FUNC_> funcs;

        IDENTIFIER_() {
            num_of_func = 0;
            num_of_ident = 0;
            idents.clear();
            funcs.clear();
            idents.reserve(1000);
            funcs.reserve(1000);
        }
    };

    struct TID_ {
        IDENTIFIER_ identifiers;
        TID_* prev_;
        int num_of_children;
        short int type_tid; // 0 - main; 1 - int func; 2 - string func; 3 - double func; 4 - boolean func; 5 - def; 6 - while; 7 - for; 8 - elfor; 9 - else(elfor); 10 - if; 11 - elseif; 12 - else(if) 
        std::vector<TID_*> children;

        TID_(TID_* prev) {
            prev_ = prev;
            num_of_children = 0;
            children.clear();
            type_tid = -1;
        }
    };



    int getPriority(std::string, bool);
    TID_* MAIN_TID_;
};

