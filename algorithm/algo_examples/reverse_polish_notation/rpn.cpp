#include <cstdlib>
#include <stack>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <fstream>
#include <algorithm>

using namespace std;

void Tokenize(const string& str,
    vector<string>& tokens,
    const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


struct rpn{

    vector<int> _stack;
    vector<string>& _tokens;
    rpn(vector<string>& t):_tokens(t){
        vector<string>::iterator it = _tokens.begin();
        while( it != _tokens.end() ){
            push(*it);
            ++it;
        }
    }

    //============================================================== pop
    // This utility function checks stack for underflow
    // and pops (removes and returns) last element.
    int pop(vector<int>& stk) {
        if (!stk.empty()) {
            int result = stk.back();
            stk.pop_back();
            return result;
        }
        else{
            exit(0);
        }
    }

    void push(const string& token){
        
        if (isdigit(token[0])) { // if first is digit, it's number.
            _stack.push_back(atoi(token.c_str())); // convert, push

        }
        else { // If it's not a number, assume it's an operator
            int left, right; //  used by some operators as temps

            switch (token[0]) {  // assume operators are one char
            case '+': _stack.push_back(pop(_stack) + pop(_stack));
                break;
            case '-': right = pop(_stack); // get right operand
                left  = pop(_stack); // get left operand
                _stack.push_back(left - right);
                break;
            case '*': _stack.push_back(pop(_stack) * pop(_stack));
                break;
            default:
                break;
            }
        }
        //return _stack.back();
    }
    int result(){
        if(!_stack.empty()) 
            return _stack.back();
        else
            exit(0);

    }

};


void read_file(string& contents){
    fstream from_file("input.txt");
    std::stringstream buffer;
    buffer << from_file.rdbuf();
    contents.assign(buffer.str());    
}

void write_file(int r){
    ofstream o("output.txt");
    o << r;
}

int main()
{
    vector<string> tokens;
    string str;
    read_file(str);
    Tokenize(str, tokens);
    rpn r(tokens);
    int res = r.result();
    write_file(res);
}


//int main(){
//    read_file();
//    return 0;
//}