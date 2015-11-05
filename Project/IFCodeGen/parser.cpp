/* 
 * My recursive descent parser
 */
#include "compiler.h"
#include "parser.h"
#include "token.h"

#include <string>

using namespace::std;

Token currToken;
int num_variables = 0;
int num_functions = 0;
int num_statements = 0;

bool Parse(string fileName) {
    Compiler compiler(fileName);
    currToken = compiler.GetNextToken();
}

void Consume(TokenType t) {
    if (currToken.type == t) {
        currToken = compiler.GetNextToken();
    } else {
        cout<<"Error: Expected token type of "<<t<< ", but got "<<currToken.type<<"\n";
        throw exception();
    }
}

bool Program() {
    //cout<<"Program, "<<currToken.value<<endl;
    if (Type_Name()) {
        if (currToken.type == TOKEN_ID) {
            Consume(TOKEN_ID);
            if (Program_Prime()) {
                return true;
            }
        }
    }
    return false;
}

bool Program_Prime() {
    //cout<<"Program Prime, "<<currToken.value<<endl;
    if (currToken.value == "(") {
        Consume(TOKEN_SYMBOL);
        if (Parameter_List()) {
            if (currToken.value == ")") {
                Consume(TOKEN_SYMBOL);
                if (Func_Tail()) {
                    if (Func_List()) {
                        return true;
                    }
                }
            }
        }
    } else if (Id_Tail()) {
        if (Id_List_Prime()) {
            if (currToken.value == ";") {
                Consume(TOKEN_SYMBOL);
                if (Program()) {
                    return true;
                }
            }
        }
    } else {
        return true;
    }
    return false;
}

bool Func_List() {
    //cout<<"Func_List, "<<currToken.value<<endl;
    if (Func()) {
        if (Func_List()) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

bool Func() {
    //cout<<"Func, "<<currToken.value<<endl;
    if (Func_Decl()) {
        if (Func_Tail()) {
            return true;
        }
    }
    return false;
}

bool Func_Tail() {
    //cout<<"Func Tail, "<<currToken.value<<endl;
    if (currToken.value == ";") {
        Consume(TOKEN_SYMBOL);
        return true;
    } else if (currToken.value == "{") {
        Consume(TOKEN_SYMBOL);
        if (Data_Decls()) {
            if (Statements()) {
                if (currToken.value == "}") {
                    Consume(TOKEN_SYMBOL);
                    num_functions++;
                    return true;
                }
            }
        }
    }
    return false;
}

bool Func_Decl() {
    //cout<<"Func_Decl, "<<currToken.value<<endl;
    if (Type_Name()) {
        //cout<<"Func_Decl, "<<currToken.value<<endl;
        if (currToken.type == TOKEN_ID) {
            Consume(TOKEN_ID);
            if (currToken.value == "(") {
                Consume(TOKEN_SYMBOL);
                if (Parameter_List()) {
                    if (currToken.value == ")") {
                        Consume(TOKEN_SYMBOL);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Type_Name() {
    //cout<<"Type_Name, "<<currToken.value<<endl;
    if (currToken.value == "int") {
        Consume(TOKEN_RESERVED);
        return true;
    } else if (currToken.value == "void") {
        Consume(TOKEN_RESERVED);
        return true;
    } else if (currToken.value == "binary") {
        Consume(TOKEN_RESERVED);
        return true;
    } else if (currToken.value == "decimal") {
        Consume(TOKEN_RESERVED);
        return true;
    }
    return false;
}

bool Parameter_List() {
    //cout<<"Parameter_List, "<<currToken.value<<endl;
    if (currToken.value == "void") {
        Consume(TOKEN_RESERVED);
        return true;
    } else if (Non_Empty_List()) {
        return true;
    } else {
        return true;
    }
}

bool Non_Empty_List() {
    //cout<<"Non_Empty_List, "<<currToken.value<<endl;
    if (Type_Name()) {
        if (currToken.type == TOKEN_ID) {
            Consume(TOKEN_ID);
            return Non_Empty_List_Prime();
        }
    } else {
        return true;
    }
    return false;
}

bool Non_Empty_List_Prime() {
    //cout<<"Non_Empty_List_Prime, "<<currToken.value<<endl;
    if (currToken.value == ",") {
        Consume(TOKEN_SYMBOL);
        if (Type_Name()) {
            if (currToken.type == TOKEN_ID) {
                Consume(TOKEN_ID);
                return Non_Empty_List_Prime();
            }
        }
    } else {
        return true;
    }
    return false;
}

bool Data_Decls() {
    //cout<<"Data_Decls, "<<currToken.value<<endl;
    if (Type_Name()) {
        if (Id_List()) {
            if (currToken.value == ";") {
                Consume(TOKEN_SYMBOL);
                if (Data_Decls()) {
                    return true;
                }
            }
        }
    } else {
        return true;
    }
    return false;
}

bool Id_List() {
    //cout<<"Id_List, "<<currToken.value<<endl;
    if (Id()) {
        if (Id_List_Prime()) {
            return true;
        }
    }
    return false;
}

bool Id_List_Prime() {
    //cout<<"Id_List_Prime, "<<currToken.value<<endl;
    if (currToken.value == ",") {
        Consume(TOKEN_SYMBOL);
        num_variables++;
        if (Id()) {
            if (Id_List_Prime()) {
                return true;
            }
        }
    } else {
        num_variables++;
        return true;
    }
    return false;
}

bool Id() {
    //cout<<"Id, "<<currToken.value<<endl;
    if (currToken.type == TOKEN_ID) {
        Consume(TOKEN_ID);
        if (Id_Tail()) {
            return true;
        }
    }
    return false;
}

bool Id_Tail() {
    //cout<<"Id Tail, "<<currToken.value<<endl;
    if (currToken.value == "[") {
        Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (currToken.value == "]") {
                Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    } else if (currToken.value != "(") {
        return true;
    }
    return false;
}

bool Block_Statements() {
    //cout<<"Block_Statements, "<<currToken.value<<endl;
    if (currToken.value == "{") {
        Consume(TOKEN_SYMBOL);
        if (Statements()) {
            if (currToken.value == "}") {
                Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    }
    return false;
}

bool Statements() {
    //cout<<"Statements, "<<currToken.value<<endl;
    if (Statement()) {
        if (Statements()) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

bool Statement() {
    //cout<<"Statement, "<<currToken.value<<endl;
    num_statements++;
    if (currToken.type == TOKEN_ID) {
        Consume(TOKEN_ID);
        if (Statement_Tail()) {
            return true;
        }
    } else if (If_Statement()) {
        return true;
    } else if (While_Statement()) {
        return true;
    } else if (Return_Statement()) {
        return true;
    } else if (Break_Statement()) {
        return true;
    } else if (Continue_Statement()) {
        return true;
    } else if (currToken.value == "read") {
        Consume(TOKEN_RESERVED);
        if (currToken.value == "(") {
            Consume(TOKEN_SYMBOL);
            if (currToken.type == TOKEN_ID) {
                Consume(TOKEN_ID);
                if (currToken.value == ")") {
                    Consume(TOKEN_SYMBOL);
                    if (currToken.value == ";") {
                        Consume(TOKEN_SYMBOL);
                        return true;
                    }
                }
            }
        }
    } else if (currToken.value == "write") {
        Consume(TOKEN_RESERVED);
        if (currToken.value == "(") {
            Consume(TOKEN_SYMBOL);
            if (Expression()) {
                if (currToken.value == ")") {
                    Consume(TOKEN_SYMBOL);
                    if (currToken.value == ";") {
                        Consume(TOKEN_SYMBOL);
                        return true;
                    }
                }
            }
        }
    } else if (currToken.value == "print") {
        Consume(TOKEN_RESERVED);
        if (currToken.value == "(") {
            Consume(TOKEN_SYMBOL);
            if (currToken.type == TOKEN_STRING) {
                Consume(TOKEN_STRING);
                if (currToken.value == ")") {
                    Consume(TOKEN_SYMBOL);
                    if (currToken.value == ";") {
                        Consume(TOKEN_SYMBOL);
                        return true;
                    }
                }
            }
        }
    }
    num_statements--;
    return false;
}

bool Statement_Tail() {
    if (Assignment_Tail()) {
        return true;
    } else if (currToken.value == "(") {
        Consume(TOKEN_SYMBOL);
        if (Expr_List()) {
            if (currToken.value == ")") {
                Consume(TOKEN_SYMBOL);
                if (currToken.value == ";") {
                    Consume(TOKEN_SYMBOL);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Assignment_Tail() {
    if (currToken.value == "=") {
        Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (currToken.value == ";") {
                Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    } else if (currToken.value == "[") {
        Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (currToken.value == "]") {
                Consume(TOKEN_SYMBOL);
                if (currToken.value == "=") {
                    Consume(TOKEN_SYMBOL);
                    if (Expression()) {
                        if (currToken.value == ";") {
                            Consume(TOKEN_SYMBOL);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Func_Call() {
    //cout<<"Func_Call, "<<currToken.value<<endl;
    if (currToken.type == TOKEN_ID) {
        Consume(TOKEN_ID);
        if (currToken.value == "(") {
            Consume(TOKEN_SYMBOL);
            if (Expr_List()) {
                if (currToken.value == ")") {
                    Consume(TOKEN_SYMBOL);
                    if (currToken.value == ";") {
                        Consume(TOKEN_SYMBOL);
                        num_functions++;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Expr_List() {
    //cout<<"Expr_List, "<<currToken.value<<endl;
    if (Non_Empty_Expr_List()) {
        return true;
    } else {
        return true;
    }
}

bool Non_Empty_Expr_List() {
    //cout<<"Non_Empty_Expr_List, "<<currToken.value<<endl;
    if (Expression()) {
        if (Non_Empty_Expr_List_Prime()) {
            return true;
        }
    }
    return false;
}

bool Non_Empty_Expr_List_Prime() {
    //cout<<"Non_Empty_Expr_List_Prime, "<<currToken.value<<endl;
    if (currToken.value == ",") {
        Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (Non_Empty_Expr_List_Prime()) {
                return true;
            }
        }
    } else {
        return true;
    }
    return false;
}

bool If_Statement() {
    //cout<<"If_Statement, "<<currToken.value<<endl;
    if (currToken.value == "if") {
        Consume(TOKEN_RESERVED);
        if (currToken.value == "(") {
            Consume(TOKEN_SYMBOL);
            if (Condition_Expression()) {
                if (currToken.value == ")") {
                    Consume(TOKEN_SYMBOL);
                    if (Block_Statements()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Condition_Expression() {
    //cout<<"Condition_Expression, "<<currToken.value<<endl;
    if (Condition()) {
        if (Condition_Op()) {
            if (Condition()) {
                return true;
            }
        } else {
            return true;
        }
    }
    return false;
}
       
bool Condition_Expression_Tail() {
        if (Condition_Op()) {
            if (Condition()) {
                return true;
            }
        } else {
            return true;
        }
}

bool Condition_Op() {
    //cout<<"Condition_Op, "<<currToken.value<<endl;
    if (currToken.value == "&&") {
        Consume(TOKEN_SYMBOL);
        return true;
    } else if (currToken.value == "||") {
        Consume(TOKEN_SYMBOL);
        return true;
    }
    return false;
} 

bool Condition() {
    //cout<<"Condition, "<<currToken.value<<endl;
    if (Expression()) {
        if (Comparison_Op()) {
            if (Expression()) {
                return true;
            }
        }
    }
    return false;
}

bool Comparison_Op() {
    //cout<<"Comparison_Op, "<<currToken.value<<endl;
    if (currToken.value == "==") {
        Consume(TOKEN_SYMBOL);
        return true;
    } else if (currToken.value == "!=") {
        Consume(TOKEN_SYMBOL);
        return true;
    } else if (currToken.value == ">") {
        Consume(TOKEN_SYMBOL);
        return true;
    } else if (currToken.value == ">=") {
        Consume(TOKEN_SYMBOL);
        return true;
    } else if (currToken.value == "<") {
        Consume(TOKEN_SYMBOL);
        return true;
    } else if (currToken.value == "<=") {
        Consume(TOKEN_SYMBOL);
        return true;
    }
    return false;
}

bool While_Statement() {
    //cout<<"While_Statement, "<<currToken.value<<endl;
    if (currToken.value == "while") {
        Consume(TOKEN_RESERVED);
        if (currToken.value == "(") {
            Consume(TOKEN_SYMBOL);
            if (Condition_Expression()) {
                if (currToken.value == ")") {
                    Consume(TOKEN_SYMBOL);
                    if (Block_Statements()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Return_Statement() {
    //cout<<"Return_Statement, "<<currToken.value<<endl;
    if (currToken.value == "return") {
        Consume(TOKEN_RESERVED);
        if (Expression()) {
            if (currToken.value == ";") {
                Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    }
    return false;
}

bool Break_Statement() {
    //cout<<"Break_Statement, "<<currToken.value<<endl;
    if (currToken.value == "break") {
        Consume(TOKEN_RESERVED);
        if (currToken.value == ";") {
            Consume(TOKEN_SYMBOL);
            return true;
        }
    }
    return false;
}

bool Continue_Statement() {
    //cout<<"Continue_Statement, "<<currToken.value<<endl;
    if (currToken.value == "continue") {
        Consume(TOKEN_RESERVED);
        if (currToken.value == ";") {
            Consume(TOKEN_SYMBOL);
            return true;
        }
    }
    return false;
}

bool Expression() {
    //cout<<"Expression, "<<currToken.value<<endl;
    if (Term()) {
        if (Expression_Prime()) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

bool Expression_Prime() {
    //cout<<"Expression_Prime, "<<currToken.value<<endl;
    if (Addop()) {
        if (Term()) {
            if (Expression_Prime()) {
                return true;
            }
        }
    } else {
        return true;
    }
    return false;
}

bool Addop() {
    //cout<<"Addop, "<<currToken.value<<endl;
    if (currToken.value == "+") {
        Consume(TOKEN_SYMBOL);
        return true;
    } else if (currToken.value == "-") {
        Consume(TOKEN_SYMBOL);
        return true;
    }
    return false;
}

bool Term() {
    //cout<<"Term, "<<currToken.value<<endl;
    if (Factor()) {
        if (Term_Prime()) {
            return true;
        }
    }
    return false;
}

bool Term_Prime() {
    //cout<<"Term_Prime, "<<currToken.value<<endl;
    if (Mulop()) {
        if (Factor()) {
            if (Term_Prime()) {
                return true;
            }
        }
    } else {
        return true;
    }
    return false;
}

bool Mulop() {
    //cout<<"Mulop, "<<currToken.value<<endl;
    if (currToken.value == "*") {
        Consume(TOKEN_SYMBOL);
        return true;
    } else if (currToken.value == "/") {
        Consume(TOKEN_SYMBOL);
        return true;
    }
    return false;
}

bool Factor() {
    //cout<<"Factor, "<<currToken.value<<endl;
    if (currToken.type == TOKEN_ID) {
        Consume(TOKEN_ID);
        if (currToken.value == "[") {
            Consume(TOKEN_SYMBOL);
            if (Expression()) {
                if (currToken.value == "]") {
                    Consume(TOKEN_SYMBOL);
                    return true;
                }
            }
        } else if (currToken.value == "(") {
            Consume(TOKEN_SYMBOL);
            if (Expr_List()) {
                if (currToken.value == ")") {
                    Consume(TOKEN_SYMBOL);
                    return true;
                }
            }
        } else {
            return true;
        }
    } else if (currToken.type == TOKEN_NUMBER) {
        //cout<<"Number\n";
        Consume(TOKEN_NUMBER);
        return true;
    } else if (currToken.value == "-") {
        Consume(TOKEN_SYMBOL);
        if (currToken.type == TOKEN_NUMBER) {
            Consume(TOKEN_NUMBER);
            return true;
        }
    } else if (currToken.value == "(") {
        Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (currToken.value == ")") {
                Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    }
    return false;
}

bool Factor_Tail() {
    if (currToken.value == "[") {
        Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (currToken.value == "]") {
                Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    } else if (currToken.value == "(") {
        Consume(TOKEN_SYMBOL);
        if (Expr_List()) {
            if (currToken.value == ")") {
                Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    } else {
        return true;
    }
    return false;
}
