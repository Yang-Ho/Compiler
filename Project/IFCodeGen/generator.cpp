#include "generator.h"

#include <fstream>
#include <iostream>

using namespace::std;

Generator::Generator(string fileName) {
    label_count = 0;

    output_file = fileName.substr(0, fileName.rfind('.'));
    output_file += "_gen.c";
    output.open(output_file.c_str());
}

Generator::~Generator() {
    output.close();
}

void Generator::WriteToOutput(string line) {
    output<<line;
}

void Generator::Evaluate(ASTNode *node) {
    switch (node->GetType()) {
        case AST_EMPTY:
            break;
        case AST_FUNC_CALL:
            Evaluate(node->GetChild());      // expr list
            output<<"(";
            output<<node->GetChild()->GetValue();
            output<<")";
            output<<node->GetValue();       // ; OR nothing
            break;
        case AST_ARRAY_ELEMENT:
            Evaluate(node->GetChild());      // expression
            output<<"[";
            output<<node->GetChild()->GetValue();
            output<<"]";
            break;
        case AST_FACTOR:
            if (node->GetValue() == "") {
                Evaluate(node->GetChild());      // expression
                output<<"(";
                output<<node->GetChild()->GetValue();
                output<<")";
            } else {
                if (node->GetChild()) {
                    Evaluate(node->GetChild());  // factor tail
                    output<<node->GetValue()<<node->GetChild()->GetValue();   // ID+facotr tail
                } else {
                    output<<node->GetValue();   // NUMBER, -NUMBER 
                }
            }
            break;
        case AST_OP:
            output<<node->GetValue();   // mulop or addop
            break;
        case AST_TERM:
            Evaluate(node->GetLeft());      // factor
            Evaluate(node->GetRight());     // term prime
            output<<node->GetLeft()->GetValue();   // mulop OR nothing
            output<<node->GetValue();   // mulop OR nothing
            output<<node->GetRight()->GetValue();   // mulop OR nothing
            break;
        case AST_EXPRESSION:
            Evaluate(node->GetLeft());      // term
            Evaluate(node->GetRight());     // expression prime
            output<<node->GetLeft()->GetValue();   // mulop OR nothing
            output<<node->GetValue();   // mulop OR nothing
            output<<node->GetRight()->GetValue();   // mulop OR nothing
            break;
        case AST_CONTINUE:
            output<<"goto LABEL_"<<GetWhileLabel()<<":;\n";
            break;
        case AST_BREAK:
            output<<"goto LABEL_"<<GetCurrLabel() - 1<<":;\n";
            break;
        case AST_RETURN:
            if (node->GetChild()) {
                Evaluate(node->GetChild());     // expression
            }
            output<<";\n";
            break;
        case AST_WHILE:
            {
                Evaluate(node->GetLeft());              // condition expression
                int label1 = GetCurrLabel();
                IncLabel();
                int label2 = GetCurrLabel();
                IncLabel();
                int label3 = GetCurrLabel();
                IncLabel();
                output<<"LABEL_"<<label1<<":;\n";
                output<<"if ("<<node->GetValue()<<") goto LABEL_"<<label2<<";\n";
                output<<"goto LABEL_"<<label3<<";\n";
                output<<"LABEL_"<<label2<<":;\n";
                Evaluate(node->GetRight());             // block statements
                output<<"goto LABEL_"<<label1<<";\n";
                output<<"LABEL_"<<label3<<":;\n";
                break;
            }
        case AST_COMPARISON_OP:
            output<<node->GetValue();   // ==, !=, >, >=, <, <=
            break;
        case AST_CONDITION:
            Evaluate(node->GetLeft());              //expression 1
            Evaluate(node->GetRight());                 //expression 2
            output<<node->GetLeft()->GetValue();
            output<<node->GetValue();               //condition op
            output<<node->GetRight()->GetValue();
            break;
        case AST_CONDITION_OP:
            output<<node->GetValue();   // &&, || 
            break;
        case AST_CONDITION_EXPR:
            if (node->GetChild()) {
                Evaluate(node->GetChild());
                output<<node->GetValue();               // condition op
                output<<node->GetChild()->GetValue();   // condition
            } else {
                Evaluate(node->GetLeft());                  // condition
                Evaluate(node->GetRight());                 // condition expression tail
            }
            break;
        case AST_IF:
            {
                Evaluate(node->GetLeft());
                int label1 = GetCurrLabel();
                IncLabel();
                int label2 = GetCurrLabel();
                IncLabel();
                output<<"if ("<<node->GetLeft()->GetValue()<<") goto LABEL_"<<label1<<";\n"; // Condition expression
                output<<"goto LABEL_"<<label2<<";\n"; // Condition expression
                output<<"LABEL_"<<label1<<":;\n";
                Evaluate(node->GetRight()); // block statement
                output<<"LABEL_"<<label2<<":;\n";
                break;
            }
        case AST_NON_EMPTY_EXPR_LIST:
            Evaluate(node->GetLeft());      //expressoin
            output<<node->GetValue(); // , or nothing
            output<<node->GetLeft()->GetValue(); // expression result
            Evaluate(node->GetRight()); // non-empty expr list prime
            break;
        case AST_EXPR_LIST:
            Evaluate(node->GetChild());
            break;
        case AST_ASSIGNMENT:
            Evaluate(node->GetLeft());          //id tail
            Evaluate(node->GetRight());         // expression
            output<<node->GetLeft()->GetValue();
            output<<"=";
            output<<node->GetRight()->GetValue();
            output<<";\n";
            break;
        case AST_ID_STATEMENT:
            output<<node->GetValue();
            Evaluate(node->GetChild());
            break;
        case AST_READ:
            output<<"read("<<node->GetValue()<<");\n";
            break;
        case AST_WRITE:
            Evaluate(node->GetChild());         // expression
            output<<"write("<<node->GetChild()->GetValue()<<");\n";
            break;
        case AST_PRINT:
            output<<"print("<<node->GetValue()<<");\n";
            break;
        case AST_STATEMENT:
            Evaluate(node->GetLeft());
            Evaluate(node->GetRight());
            break;
        case AST_BLOCK:
            // @ ENTER SCOPE
            output<<"{\n";
            Evaluate(node->GetChild()); // statements
            // @ EXIT SCOPE
            output<<"}\n";
            break;
        case AST_ID:
            Evaluate(node->GetChild()); // id tail
            output<<node->GetValue();
            output<<node->GetChild()->GetValue();
            break;
        case AST_ID_LIST:
            if (node->GetLeft()) {
                Evaluate(node->GetLeft());
                Evaluate(node->GetRight());
            } else {
                Evaluate(node->GetChild());
            }
            output<<node->GetValue();
            break;
        case AST_DATA_DECLS: 
            Evaluate(node->GetLeft()); //type name
            Evaluate(node->GetChild());// id list
            output<<";\n";
            Evaluate(node->GetRight()); // data decls
            break;
        case AST_NON_EMPTY_LIST:
            Evaluate(node->GetLeft()); // type name
            output<<node->GetValue(); // ID
            Evaluate(node->GetRight()); //non empty list prime
            break;
        case AST_NON_EMPTY_LIST_PRIME:
            output<<", ";
            Evaluate(node->GetLeft()); // type name
            output<<node->GetValue(); // ID
            Evaluate(node->GetRight()); // non empty list prime
            break;
        case AST_PARAM_LIST:
            if (node->GetValue() == "void") {
                output<<"void ";
                Evaluate(node->GetChild());         // Parameter list tail
            } else if (node->GetLeft()) {
                Evaluate(node->GetLeft());          // Type name prime
                output<<node->GetValue();       // ID
                Evaluate(node->GetRight());         // non empty list prime
            } else {
                output<<node->GetValue();       //ID
                Evaluate(node->GetChild());         // non empty list prime
            }
            break;
        case AST_RESERVED:
            output<<node->GetValue();   // int, void, binary, decimal,
            break;
        case AST_FUNC_DECL:
            Evaluate(node->GetLeft());      // typename
            output<<node->GetValue();   // ID
            Evaluate(node->GetRight());     // func decl prime
            break;
        case AST_FUNC_TAIL:
            if (node->GetLeft()) {
                output<<"{\n";
                // @ ENTER SCOPE
                Evaluate(node->GetLeft());      // Data decls
                Evaluate(node->GetRight());     // statements
                output<<"}\n";
                // @Exit Scope
            } else {
                output<<";\n";
            }
            break;
        case AST_FUNC:
            Evaluate(node->GetLeft());          // func decl
            Evaluate(node->GetRight());         // Func Tail
            break;
        case AST_FUNC_LIST:
            if (node->GetChild()) {
                Evaluate(node->GetLeft());      // func decl prime
                Evaluate(node->GetChild());     // func tail
                Evaluate(node->GetRight());     // func list
            } else {
                Evaluate(node->GetLeft());      // func
                Evaluate(node->GetRight());     // func list
            }
            break;
        case AST_DATA_DECLS_PRIME_TAIL:
            Evaluate(node->GetLeft());          // typename
            output<<node->GetValue();       // ID
            Evaluate(node->GetRight());         // data func decl
            break;
        case AST_DATA_DECLS_PRIME:
            output<<node->GetChild()->GetValue();   // id tail
            Evaluate(node->GetLeft());      // id list prime    
            output<<";\n";
            Evaluate(node->GetRight());     // data decls prime tail
            break;
        case AST_PROGRAM:
            Evaluate(node->GetLeft());      // typename
            output<<node->GetValue();   // ID
            Evaluate(node->GetRight());     // program prime
            break;
        default:
            break;
    }
}

int Generator::GetCurrLabel() {
    return label_count;
}

int Generator::GetWhileLabel() {
    return label_count - 2;
}

void Generator::IncLabel() {
    label_count += 1;
}

string Generator::GetOutputFileName() {
    return output_file;
}
