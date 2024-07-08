#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include "parser.hpp"
#include "tokenization.hpp"

using namespace std;


int main(int argc , char* argv[]){
    //the first argument is the directory of the file 
    //the other arguments in argv are the arguemts to the function
    
    if (argc != 2){
        cerr << "this is incorrect the correct way is:" <<endl;
        cerr << "cool <input.cl>" <<endl;
        return EXIT_FAILURE;

    }
    
    string content;
    {
        stringstream content_stream;
        fstream input (argv[1] , ios::in);
        content_stream << input.rdbuf();
        content = content_stream.str();
    }

    tokenizer my_tokenizer (move(content));
    

    vector<token> tokens = my_tokenizer.tokenize();
    parser my_parser (tokens);
    cout << "The parse tree: " <<endl;
    auto tree = my_parser.check_program();
    cout << "End of the parse tree: " <<endl;

    if (tree.has_value()){
        cout << "*******************" <<endl;
        cout << "right syntax :)" << endl;
        //my_parser.visualize_parse_tree(tree.value(), "wonderful_parse_tree.dot");
        cout << "*******************" <<endl;
    }
    else{
        cout << "*******************" <<endl;
        cout << "wrong syntax :(" << endl;
        cout << "*******************" <<endl;
    }

    //my_parser.visualize_parse_tree();
    print_tokens(tokens);
    return EXIT_SUCCESS;
    
}