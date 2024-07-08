#pragma once
#include <variant>
#include "tokenization.hpp"
#include "arena_alloc.hpp"
using namespace std;


//******************
//expr structs
//******************
struct expr;
struct feature;

struct expr_t {
    string name = "expr_t";
};
struct expr_f {
    string name = "expr_f";
};
struct expr_str {
    string name = "expr_str";
    token str;
};

struct expr_int {
    string name = "expr_int";
    token integer;
};

struct expr_id{
    string name = "expr_id";
    token id;
};

struct expr_paren{
    string name = "expr_paren";
    expr* side;
};

struct expr_not{
    string name = "expr_not";
    expr* side;
};

struct expr_equal{
    string name = "expr_equal";
    expr* lh;
    expr* rh;
};

struct expr_assign{
    string name = "expr_assign";
    expr* lh;
    expr* rh;
};

struct expr_less{
    string name = "expr_less";
    expr* lh;
    expr* rh;
};

struct expr_neg{
    string name = "expr_neg";
    expr* side;
};

struct expr_div{
    string name = "expr_div";
    expr* lh;
    expr* rh;
};

struct expr_mul{
    string name = "expr_mul";
    expr* lh;
    expr* rh;
};

struct expr_sub{
    string name = "expr_sub";
    expr* lh;
    expr* rh;
};

struct expr_add{
    string name = "expr_add";
    expr* lh;
    expr* rh;
};

struct expr_pow{
    string name = "expr_pow";
    expr* lh;
    expr* rh;
};

struct expr_isvoid{
    string name = "expr_isvoid";
    expr* side;
};

struct expr_new{
    string name = "expr_new";
    token type;
};

struct expr_case{
    string name = "expr_case";
    expr* expr1;
    vector <token> ids;
    vector <token> types;
    vector <expr*> exprs;
};

struct expr_let{
    string name = "expr_let";
    token id;
    token type1;
    expr* expr1;
    vector<token> ids;
    vector<token> types;
    vector <expr*> exprs;
    expr* expr2;   
};

struct expr_cur{
    string name = "expr_cur";
    vector <expr*> exprs; 
};

struct expr_while{
    string name = "expr_while";
    expr* expr1; 
    expr* expr2; 
};

struct expr_if{
    string name = "expr_if";
    expr* expr1; 
    expr* expr2; 
    expr* expr3; 
};

struct expr_idb{
    string name = "expr_idb";
    token id;
    expr* expr1; 
    vector <expr*> exprs; 
};

struct expr_expr{
    string name = "expr_expr";
    expr* expr1; 
    token type;
    token id;
    expr* expr2; 
    vector <expr*> exprs;  
};

struct expr_ida{
    string name = "expr_ida";
    token id;
    expr* expr1;
};

struct expr{
    string name = "expr";
    variant <expr_pow* ,expr_t* ,expr_f* , expr_str* , expr_int* , expr_id* , expr_paren* , expr_not* , expr_equal* , expr_assign*, expr_less* , expr_neg* , expr_div* , expr_mul* , expr_sub* , expr_add* , expr_isvoid* , expr_new* , expr_case* , expr_let* , expr_cur* , expr_while* , expr_if* , expr_idb* , expr_expr* , expr_ida*> my_expr;
};

//******************
//struct formal
//******************
struct formal{
    string name = "formal";
    token id;
    token type;
};

//******************
//struct feature
//******************
struct feature_d{
    string name = "feature_d";
    token id;
    token type;
    expr* expr1;
};

struct feature_e{
    string name = "feature_e";
    token id;
    formal* formal1;
    vector <formal*> formals;
    token type;
    expr* expr1;
};

struct feature{
    string name = "feature";
    variant <feature_d* , feature_e*> my_feature;
};

//******************
//struct class
//******************
struct cool_class{
    string name = "cool_class";
    token type1;
    token type2;
    vector <feature*> features;
};

//******************
//struct program
//******************
struct program{
    string name = "program";
    vector <cool_class*> classes;
};



class parser{
    public:
        //**********constructor **********//
        inline explicit parser (vector<token> tokens)
        : my_tokens(move(tokens))
        , parse_tree(1024 * 1024 * 4){}
        //**********constructor **********//   

        inline optional<expr*> check_expr (){ 
            cout <<"EXPR" <<endl;
            if (try_consume_value("True")){
                cout <<"expr_True" <<endl;                                        
                auto my_expr_t = parse_tree.emplace<expr_t>("expr_t");
                auto my_expr = parse_tree.emplace<expr>("expr" , my_expr_t);
                return my_expr;
            }
            else if (try_consume_value("False")){ 
                cout <<"expr_False" <<endl;                                         
                auto my_expr_f = parse_tree.emplace<expr_f>("expr_f");
                auto my_expr = parse_tree.emplace<expr>("expr" , my_expr_f);
                return my_expr;
            }
            else if (auto str_token = try_consume_type(token_type :: Strings)){
                cout <<"expr_string" <<endl;
                auto my_string = parse_tree.emplace <expr_str>("expr_str" , str_token.value());
                auto str = parse_tree.emplace <expr> ("expr" , my_string);
                return str;
            }
            else if (auto int_token = try_consume_type(token_type :: Integers)){
                cout <<"expr_integer" <<endl;
                auto my_int = parse_tree.emplace <expr_int>("expr_int" , int_token.value());
                auto cint = parse_tree.emplace <expr> ("expr" , my_int);
                return cint;
            }
            else if (auto id_token = try_consume_type(token_type :: Type_identifiers)){
                cout <<"expr_Type_identifier" <<endl;
                auto my_id = parse_tree.emplace <expr_id>("expr_id" , id_token.value());
                auto id = parse_tree.emplace <expr> ("expr" , my_id);
                return id;
            }
            else if (auto id_token = try_consume_type(token_type :: object_identifiers)){ 
                cout <<"expr_Object_identifier" <<endl;                            
                auto my_id = parse_tree.emplace <expr_id>("expr_id" , id_token.value());
                auto id = parse_tree.emplace <expr> ("expr" , my_id);
                return id;
            }
            else if (auto open_paren = try_consume_value("(")){
                if (auto node = check_expr()){
                    if (auto closeparen = try_consume_value(")")){
                        cout <<"expr_parenthis" <<endl;
                        auto my_paren = parse_tree.emplace <expr_paren>("expr_paren" , node.value());
                        auto paren = parse_tree.emplace <expr> ("expr" , my_paren);
                        return paren;
                    }
                }
                
            }
            else if (auto cool_not = try_consume_value("not")){
                if (auto node = check_expr()){
                    cout <<"not_expr" <<endl;
                    auto my_not = parse_tree.emplace <expr_not>("expr_not" , node.value());
                    auto not1 = parse_tree.emplace <expr> ("expr" , my_not);
                    return not1;
                }               
            }
            else if (auto op1 = try_consume_value("=")){
                if (auto node1 = check_expr()){
                    if (auto op2 = try_consume_value("=")){
                        if (auto node2 = check_expr()){
                            cout <<"expr_equal" <<endl;
                            auto my_op = parse_tree.emplace <expr_equal>("expr_equal" , node1.value() , node2.value());
                            auto op = parse_tree.emplace <expr> ("expr" , my_op);
                            return op;
                        }
                    }
                }               
            }
            else if (auto op1 = try_consume_value("<=")){
                if (auto node1 = check_expr()){
                    if (auto op2 = try_consume_value("<=")){
                        if (auto node2 = check_expr()){
                            cout <<"expr_assign" <<endl;
                            auto my_op = parse_tree.emplace <expr_assign>("expr_assign" , node1.value() , node2.value());
                            auto op = parse_tree.emplace <expr> ("expr" , my_op);
                            return op;
                        }
                    }
                }               
            }
            else if (auto op1 = try_consume_value("<")){
                if (auto node1 = check_expr()){
                    if (auto op2 = try_consume_value("<")){
                        if (auto node2 = check_expr()){
                            cout <<"expr_less than" <<endl;
                            auto my_op = parse_tree.emplace <expr_less>("expr_less" , node1.value() , node2.value());
                            auto op = parse_tree.emplace <expr> ("expr" , my_op);
                            return op;
                        }
                    }
                }               
            }
            else if (auto op1 = try_consume_value("/")){
                if (auto node1 = check_expr()){
                    if (auto op2 = try_consume_value("/")){
                        if (auto node2 = check_expr()){
                            cout <<"expr_divide" <<endl;
                            auto my_op = parse_tree.emplace <expr_div>("expr_div" , node1.value() , node2.value());
                            auto op = parse_tree.emplace <expr> ("expr" , my_op);
                            return op;
                        }
                    }
                }               
            }
            else if (auto op1 = try_consume_value("*")){
                if (auto node1 = check_expr()){
                    if (auto op2 = try_consume_value("*")){
                        if (auto node2 = check_expr()){
                            cout <<"expr_*" <<endl;
                            auto my_op = parse_tree.emplace <expr_mul>("expr_mul" , node1.value() , node2.value());
                            auto op = parse_tree.emplace <expr> ("expr" , my_op);
                            return op;
                        }
                    }
                }               
            }
            else if (auto op1 = try_consume_value("-")){
                if (auto node1 = check_expr()){
                    if (auto op2 = try_consume_value("-")){
                        if (auto node2 = check_expr()){
                            cout <<"expr_not" <<endl;
                            auto my_op = parse_tree.emplace <expr_sub>("expr_sub" , node1.value() , node2.value());
                            auto op = parse_tree.emplace <expr> ("expr" , my_op);
                            return op;
                        }
                    }
                }               
            }
            else if (auto op1 = try_consume_value("+")){
                if (auto node1 = check_expr()){
                    if (auto op2 = try_consume_value("+")){
                        if (auto node2 = check_expr()){
                            cout <<"expr_+" <<endl;
                            auto my_op = parse_tree.emplace <expr_add>("expr_add" , node1.value() , node2.value());
                            auto op = parse_tree.emplace <expr> ("expr" , my_op);
                            return op;
                        }
                    }
                }               
            }
            else if (auto cool_not = try_consume_value("~")){
                if (auto node = check_expr()){
                    cout <<"~expr" <<endl;
                    auto my_not = parse_tree.emplace <expr_neg>("expr_neg" , node.value());
                    auto not1 = parse_tree.emplace <expr> ("expr" , my_not);
                    return not1;
                }               
            }
            else if (auto is_void = try_consume_value("isvoid")){
                if (auto node = check_expr()){
                    cout <<"expr_isvoid" <<endl;
                    auto my_void = parse_tree.emplace <expr_isvoid>("expr_isvoid" , node.value());
                    auto not1 = parse_tree.emplace <expr> ("expr" , my_void);
                    return not1;
                }               
            }
            else if (auto is_new = try_consume_value("new")){
                if (auto is_type = try_consume_type(token_type :: Type)){
                    cout <<"expr_new" <<endl;
                    auto my_new = parse_tree.emplace <expr_new>("expr_new" , is_type.value());
                    auto node = parse_tree.emplace <expr> ("expr" , my_new);
                    return node;
                }               
            }
            else if (auto is_curl = try_consume_value("{")){
                auto curl = parse_tree.emplace<expr_cur> ("expr_cur");

                if (auto x = check_expr()){
                    if (auto is_semi = try_consume_value(";")){
                        curl -> exprs.push_back(x.value());
                    }else{
                        return {};
                    }
                    while(x = check_expr()){
                        curl -> exprs.push_back(x.value());
                        if (auto is_semi = try_consume_value(";")){
                            curl -> exprs.push_back(x.value());
                        }else{
                            return {};
                        }
                    }
                    cout <<"{expr}"<<endl;
                    auto expr_node = parse_tree.emplace<expr> ("expr" , curl);
                    return (expr_node);
                }else{
                    return {};
                }
            }
            else if (auto is_while = try_consume_value("while")){
                if(auto expr1 = check_expr()){
                    if(auto is_loop = try_consume_value("loop")){
                        if(auto expr2 = check_expr()){
                            if(auto is_loop = try_consume_value("pool")){
                                cout << "while expression" <<endl;
                                auto wh = parse_tree.emplace<expr_while>("expr_while" , expr1.value() , expr2.value());
                                auto wef = parse_tree.emplace<expr>("expr" , wh);
                                return(wef);
                            }
                        }
                    }
                }
            }

            //if expr then expr else expr fi
            else if (auto is_if = try_consume_value("if")){
                if(auto expr1 = check_expr()){
                    if(auto is_then = try_consume_value("then")){
                        if(auto expr2 = check_expr()){
                            if(auto is_else = try_consume_value("else")){
                                if(auto expr3 = check_expr()){
                                    if(auto is_fi = try_consume_value("fi")){
                                        cout<<"if expression" <<endl;
                                        auto iff = parse_tree.emplace<expr_if>("expr_if" , expr1.value() , expr2.value() , expr3.value());
                                        auto ifff = parse_tree.emplace<expr>("expr" , iff);
                                        return(ifff);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // & ID( expr , expr  )
            else if (auto is_and = try_consume_value("&")){
                auto my_expr = parse_tree.emplace<expr_idb>("expr_idb");
                if (auto type1 = try_consume_type(token_type::Type_identifiers)){
                    my_expr -> id = type1.value();
                    if(auto x = try_consume_value("(")){
                        if (auto exprc = check_expr()){
                            my_expr -> expr1 = exprc.value();
                            while (auto is_comma = try_consume_value(",")){
                                auto my_exprs = check_expr();
                                if (!my_exprs.has_value()){
                                    return {};
                                }
                                else{
                                    my_expr ->exprs.push_back(my_exprs.value());
                                }
                            }
                            auto x = try_consume_value(")");
                            if (!x.has_value()){
                                return {};                                   
                            }
                            cout << "& ID( expr , expr  )" << endl;
                            auto final_expr = parse_tree.emplace<expr>("expr" , my_expr);
                            return (final_expr);
                        }
                    }                   
                }
            }
            //@ID <- expr
            else if (auto is_ass = try_consume_value("@")){
                if (auto is_type = try_consume_type(token_type :: Type_identifiers)){
                    if(auto is_less = try_consume_value("<")){
                        if(auto is_d = try_consume_value("-")){
                            if (auto my_expr = check_expr()){
                                cout << "@ID <- expr" <<endl;
                                auto node1 = parse_tree.emplace <expr_ida> ("expr_ida" , is_type.value() , my_expr.value());
                                auto node = parse_tree.emplace<expr>("expr" , node1);
                                return(node);
                            }
                        }
                    }
                }
            } 
            //************
            // power part
            //***********       
            else if (auto is_pow1 = try_consume_value("^")){
                if(auto exp1 = check_expr()){
                    if(auto is_pow2 = try_consume_value("^")){
                        if(auto exp2 = check_expr()){
                            cout <<"expr_pow" <<endl;
                            auto my_pow = parse_tree.emplace<expr_pow>("expr_pow" , exp1.value() , exp2.value());
                            auto cool_pow = parse_tree.emplace<expr>("expr" ,my_pow);
                            return cool_pow;
                        }
                    }
                }
            }
            
            //else if ()
            return {};          
        }

        //*****************
        //check_formal
        //*****************

        inline optional<formal*> check_formal (){
            cout <<"FORMAL" <<endl;
            //%ID : TYPE
            if (auto is_id = try_consume_value("%")){
                if (auto is_id = try_consume_type(token_type :: Type_identifiers)){
                    if(auto is_col = try_consume_value(":")){
                        if(auto is_type = try_consume_type(token_type :: Type)){
                            auto fo = parse_tree.emplace<formal> ("formal" , is_id.value() , is_type.value());
                            return (fo);
                        }
                    }
                }
            }
            return {};
        }

        //*****************
        //check_feature
        //*****************
        inline optional<feature*> check_feature (){
            cout <<"Feature" <<endl;
            //$ID : TYPE [ <- expr ]
            if (auto is_dol = try_consume_value("$")){
                if (auto is_id = try_consume_type(token_type :: Type_identifiers)){
                    if(auto is_col = try_consume_value(":")){
                        if(auto is_type = try_consume_type(token_type :: Type)){
                            if(auto is_less = try_consume_value("<")){
                                if(auto is_d = try_consume_value("-")){
                                    if (auto my_expr = check_expr()){
                                        cout <<"formal_ $ID : TYPE [ <- expr ]" <<endl;
                                        auto temp_node = parse_tree.emplace<feature_d>("feature_d" , is_id.value() , is_type.value() , my_expr.value());
                                        auto node = parse_tree.emplace<feature>("feature" ,temp_node);
                                        return node;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //!ID( [ formal [[, formal]]∗] ) : TYPE { expr }
            else if (auto is_wow = try_consume_value("!")){
                auto temp_node = parse_tree.emplace<feature_e>("feature_e");
                if (auto is_id = try_consume_type(token_type :: Type_identifiers)){
                    temp_node -> id = is_id.value();
                    if (auto is_arr = try_consume_value("(")){
                        if (auto my_formal = check_formal()){
                            temp_node -> formal1 = my_formal.value();                           
                        }
                        while(auto is_comma = try_consume_value(",")){
                            auto my_formals = check_formal();
                            if(!my_formals.has_value()){
                                return{};
                            }
                            else{
                                temp_node ->formals.push_back(my_formals.value());
                            }
                        }
                        if (auto is_arr_close = try_consume_value(")")){
                            if(auto is_col = try_consume_value(":")){
                                if(auto is_type = try_consume_type(token_type :: Type)){
                                    temp_node ->type = is_type.value();
                                    if(auto is_curl = try_consume_value("{")){
                                        if (auto my_expr = check_expr()){
                                            temp_node ->expr1 = my_expr.value();
                                            if(auto is_curl_closed = try_consume_value("}")){
                                                cout << "!ID( [ formal [[, formal]]∗] ) : TYPE { expr }" <<endl;
                                                auto node = parse_tree.emplace <feature> ("feature" , temp_node);
                                                return (node);
                                            }
                                        }                                   
                                    }
                                }
                            }
                        } 
                    }
                }
            }
            return {};
        }

        //*****************
        //check_class
        //*****************
        inline optional<cool_class*> check_class (){
            cout << "CLASS" <<endl;
            //class TYPE [inherits TYPE] { [[feature; ]]∗}
            auto temp_node = parse_tree.emplace<cool_class>("cool_class");
            if (auto is_class = try_consume_value("class")){
                if (auto is_type = try_consume_type(token_type :: Type)){
                    temp_node -> type1 = is_type.value();
                    if (auto is_inheret = try_consume_value("inherits")){
                        if (auto is_type2 = try_consume_type(token_type :: Type)){
                            temp_node -> type2 = is_type2.value();
                            if (auto open_curl = try_consume_value("{")){
                                while (auto fea = check_feature()){
                                    temp_node ->features.push_back(fea.value());
                                    auto is_semicolon = try_consume_value(";");
                                    if (! is_semicolon.has_value()){
                                        return{};
                                    }
                                }
                                if(auto close_curl = try_consume_value("}")){
                                    return (temp_node);
                                }                               
                            }                           
                        }
                    }
                }
            }
        }

        //*****************
        //check_program
        //*****************
        inline optional<program*> check_program (){
            cout << "PROGRAM" <<endl;
            //[[class; ]]*
            auto my_program = parse_tree.emplace<program>("program");
            while (auto is_class = check_class()){
                my_program -> classes.push_back(is_class.value());
                auto is_semi_colon = try_consume_value(";");
                if(! is_semi_colon.has_value()){
                    return{};
                }
                if (try_consume_value("\n")){
                    continue;
                }
            }
            if (peak().has_value()){
                return{};
            }
            else{
                return (my_program);
            }
        }


    private:
        ArenaAllocator parse_tree;
        vector<token> my_tokens;
        int my_idx = 0;

        //********************
        // a group of methods used alot
        //********************

        [[nodiscard]] inline optional <token> peak () const{
            if (my_idx>= my_tokens.size()){
                return {};
            }
            else{
                return my_tokens.at(my_idx);
            }
        }

        inline token consume (){
            return my_tokens.at(my_idx ++);
        }

        inline optional <token> try_consume_type (const token_type ch){
            if (peak().has_value() && peak().value().type == ch){
                return consume();
            }
            return {};
        }

        inline optional <token> try_consume_value (string ch){
            if (peak().has_value() && peak().value().value == ch){
                return consume();
            }
            return {};
        }

};