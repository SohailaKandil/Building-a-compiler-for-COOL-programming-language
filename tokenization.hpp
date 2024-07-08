#pragma once
#include <string>
#include <vector>
#include <optional>
#include <iostream>
using namespace std;


enum class token_type{
    Keywords,  
    Type_identifiers,
    object_identifiers,
    Integers,
    Strings,
    Comments,
    White_Space,
    special_char,
    Type            
};

struct token {
    token_type type;
    optional <string> value;
};

class tokenizer{
    public:
        inline explicit tokenizer (const string & src):my_src(move(src)){
            
        }

        inline vector <token> tokenize (){
            string buf;
            vector <token> tokens;
            while (peak().has_value()){
                if (isalpha(peak().value())){
                    buf.push_back(consume());
                    while(peak().has_value() && isalnum(peak().value())){
                            buf.push_back(consume());
                    }

                    //***********************
                    //for keywords
                    //***********************
                    if (buf == "class" || buf == "else" || buf == "False" || buf == "fi" || buf == "if" || buf == "in" || buf == "inherits" || buf == "isvoid" || buf == "let" || buf == "loop" || buf == "pool" || buf == "then" || buf == "while" || buf == "case" || buf == "esac" || buf == "new" || buf == "of" || buf == "not" || buf == "True"){
                        tokens.push_back({.type = token_type::Keywords , .value = buf});
                        buf.clear();
                        continue;
                    }
                    //***********************
                    //for types
                    //***********************
                    if (buf == "Int" || buf == "String" || buf == "Bool"){
                        tokens.push_back({.type = token_type::Type , .value = buf});
                        buf.clear();
                        continue;
                    }

                    //***********************
                    //for Identifiers
                    //***********************
                    else{
                        char first_letter = buf[0];
                        if(isupper(first_letter)){
                            tokens.push_back({.type = token_type::Type_identifiers , .value = buf});
                            buf.clear();
                            continue;
                        }
                        else{
                            tokens.push_back({.type = token_type::object_identifiers , .value = buf});
                            buf.clear();
                            continue;
                        }                       
                    }
                }

                //***********************
                //for integers
                //***********************
                else if (isdigit(peak().value())){
                    buf.push_back(consume());
                    while(peak().has_value() && isdigit(peak().value())){
                        buf.push_back(consume());
                    }
                    if(peak().has_value() && !isspace(peak().value())){
                        cerr << "syntax error identifyer name can't start with a number :(" << endl;
                        exit(EXIT_FAILURE);
                    }
                    tokens.push_back({.type = token_type::Integers , .value = buf});
                    buf.clear();
                    continue;
                }

                //***********************
                //for comments
                //***********************

                else if (peak().value() == '#'){
                    consume();
                    while (peak().value() && peak().value() !='#'){
                        buf.push_back(consume());
                        if (!peak().has_value()){
                            cerr << "Syntax error: You didn't close the comment section :(" << endl;
                            exit(EXIT_FAILURE);
                        }
                    }
                    if (peak().has_value()) {
                        // Consume the closing '*' character
                        consume();
                    } else {
                        cerr << "Syntax error: You didn't close the comment section :(" << endl;
                        exit(EXIT_FAILURE);
                    }
                
                    tokens.push_back({.type = token_type::Comments , .value = buf});
                    buf.clear();
                    continue;
                }

                //***********************
                //for strings
                //***********************

                else if (peak().value() == '"') {
                    consume();

                    while (peak().has_value() && peak().value() != '"') {

                        if (peak().value() == '\\') { // Check for escape sequence
                            consume();
                            if (!peak().has_value()) {
                                cerr << "Syntax error: Unterminated escape sequence in string" << endl;
                                exit(EXIT_FAILURE);
                            }
                            char escaped_char = peak().value();
                            consume();
                            switch (escaped_char) {
                                case 'b':
                                    buf.push_back('\b'); // Backspace
                                    break;
                                case 't':
                                    buf.push_back('\t'); // Tab
                                    break;
                                case 'n':
                                    buf.push_back('\n'); // Newline
                                    break;
                                case 'f':
                                    buf.push_back('\f'); // Formfeed
                                    break;
                                case '\\':
                                    buf.push_back('\\'); // Backslash
                                    break;
                                case '"':
                                    buf.push_back('"'); // Double quote
                                    break;
                                default:
                                    cerr << "Syntax error: Invalid escape sequence \\" << escaped_char << endl;
                                    exit(EXIT_FAILURE);
                            }
                        }else{
                            buf.push_back(consume());                           
                        }

                        if (!peak().has_value()) {
                            cerr << "Syntax error: Unterminated string literal" << endl;
                            exit(EXIT_FAILURE);
                        }                        

                    }

                    if (peak().has_value()) {
                        // Consume the closing '"' character
                        consume();
                    } else {
                        cerr << "Syntax error: You didn't close the comment section :(" << endl;
                        exit(EXIT_FAILURE);
                    }

                    tokens.push_back({.type = token_type::Strings, .value = buf});
                    buf.clear();
                    continue;
                }

                 //***********************
                //for special_characters
                //***********************
                else if (peak().value() == '+' || peak().value() == '-' || peak().value() == '*' || peak().value() == '/' || peak().value() == '~' || peak().value() == '<' || peak().value() == '=' ||peak().value() == '(' ||peak().value() == ')' || peak().value() == ';' || peak().value() == '>' || peak().value() == ':' || peak().value() == '@' || peak().value() == '&' || peak().value() == '{' || peak().value() == '}' || peak().value() == '^' || peak().value() == '[' || peak().value() == ']' || peak().value() == ',' || peak().value() == '%' || peak().value() == '$' || peak().value() == '!'){
                    if (peak().value() == '<'){
                        buf.push_back(consume());
                        if (peak().has_value() && peak().value() == '='){
                            buf.push_back(consume());
                        }
                        tokens.push_back({.type = token_type::special_char , .value = buf}); 
                        buf.clear();
                        continue;
                    }
                    else{
                        buf.push_back(consume());
                        tokens.push_back({.type = token_type::special_char , .value = buf});
                        buf.clear();
                        continue;
                    }
                    
                }

                //***********************
                //for white spaces
                //***********************

                else if (peak().value() == '\n' || peak().value() == '\f' || peak().value() == '\r' || peak().value() == '\t' || peak().value() == '\v'){
                    buf.push_back(consume());
                    tokens.push_back({.type = token_type::White_Space , .value = buf});
                    buf.clear();
                    continue;
                }
                //************************************************ something we don't know
                else if (peak().value() == ' '){
                    consume();
                    continue;
                }

                else{
                    cerr << "syntax error :( unknown character: " << peak().value() << endl;
                    exit(EXIT_FAILURE);
                }
                my_idx = 0;
                return tokens;  
                    
            }
        }

        
        
    
    private:
        [[nodiscard]] inline optional <char> peak () const{
            if (my_idx >= my_src.length()){
                return {};
            }
            else{
                return my_src.at(my_idx);
            }
        }

        char inline consume (){
            return (my_src.at(my_idx ++));
        }

        const string my_src;
        int my_idx = 0;

};

void print_tokens(const vector<token>& tokens) {
    for (const auto& tok : tokens) {
        cout << "Token: ";
        switch (tok.type) {
            case token_type::Keywords:
                cout << "Keyword";
                break;
            case token_type::Type_identifiers:
                cout << "Type Identifier";
                break;
            case token_type::object_identifiers:
                cout << "Object Identifier";
                break;
            case token_type::Integers:
                cout << "Integer";
                break;
            case token_type::Strings:
                cout << "String";
                break;
            case token_type::Comments:
                cout << "Comment";
                break;
            case token_type::White_Space:
                cout << "White_Space";
                break;
            case token_type::special_char:
                cout << "special_char";
                break;
            case token_type::Type:
                cout << "Type";
                break;
        }

        cout << ", Token name: ";
        if (tok.value.has_value()){
            cout << tok.value.value();
        cout << endl;
        }
    }   
}