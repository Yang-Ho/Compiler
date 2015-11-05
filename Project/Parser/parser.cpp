/* 
 * My recursive descent parser
 * uses a vector of all tokens and an index tIndex as a reference to the 
 * "top of stack" token
 */
#include "scanner.h"
#include "parser.h"
#include "token.h"

using namespace::std;

vector< Token > tokens;

int tIndex = 0;
int num_variables = 0;
int num_functions = 0;
int num_statements = 0;

bool Program() {
    //cout<<"Program, "<<tokens[tIndex].value<<endl;
    if (Type_Name()) {
        if (tokens[tIndex].type == ID) {
            tIndex++;
            if (Program_Prime()) {
                return true;
            }
        }
    }
    return false;
}

bool Program_Prime() {
    //cout<<"Program Prime, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "(") {
        tIndex++;
        if (Parameter_List()) {
            if (tokens[tIndex].value == ")") {
                tIndex++;
                if (Func_Tail()) {
                    if (Func_List()) {
                        return true;
                    }
                }
            }
        }
    } else if (Id_Tail()) {
        if (Id_List_Prime()) {
            if (tokens[tIndex].value == ";") {
                tIndex++;
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
    //cout<<"Func_List, "<<tokens[tIndex].value<<endl;
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
    //cout<<"Func, "<<tokens[tIndex].value<<endl;
    if (Func_Decl()) {
        if (Func_Tail()) {
            return true;
        }
    }
    return false;
}

bool Func_Tail() {
    //cout<<"Func Tail, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == ";") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "{") {
        tIndex++;
        if (Data_Decls()) {
            if (Statements()) {
                if (tokens[tIndex].value == "}") {
                    tIndex++;
                    num_functions++;
                    return true;
                }
            }
        }
    }
    return false;
}

bool Func_Decl() {
    //cout<<"Func_Decl, "<<tokens[tIndex].value<<endl;
    if (Type_Name()) {
        //cout<<"Func_Decl, "<<tokens[tIndex].value<<endl;
        if (tokens[tIndex].type == ID) {
            tIndex++;
            if (tokens[tIndex].value == "(") {
                tIndex++;
                if (Parameter_List()) {
                    if (tokens[tIndex].value == ")") {
                        tIndex++;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Type_Name() {
    //cout<<"Type_Name, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "int") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "void") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "binary") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "decimal") {
        tIndex++;
        return true;
    }
    return false;
}

bool Parameter_List() {
    //cout<<"Parameter_List, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "void") {
        tIndex++;
        return true;
    } else if (Non_Empty_List()) {
        return true;
    } else {
        return true;
    }
}

bool Non_Empty_List() {
    //cout<<"Non_Empty_List, "<<tokens[tIndex].value<<endl;
    if (Type_Name()) {
        if (tokens[tIndex].type == ID) {
            tIndex++;
            return Non_Empty_List_Prime();
        }
    } else {
        return true;
    }
    return false;
}

bool Non_Empty_List_Prime() {
    //cout<<"Non_Empty_List_Prime, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == ",") {
        tIndex++; 
        if (Type_Name()) {
            if (tokens[tIndex].type == ID) {
                tIndex++;
                return Non_Empty_List_Prime();
            }
        }
    } else {
        return true;
    }
    return false;
}

bool Data_Decls() {
    //cout<<"Data_Decls, "<<tokens[tIndex].value<<endl;
    if (Type_Name()) {
        if (Id_List()) {
            if (tokens[tIndex].value == ";") {
                tIndex++;
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
    //cout<<"Id_List, "<<tokens[tIndex].value<<endl;
    if (Id()) {
        if (Id_List_Prime()) {
            return true;
        }
    }
    return false;
}

bool Id_List_Prime() {
    //cout<<"Id_List_Prime, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == ",") {
        tIndex++;
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
    //cout<<"Id, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].type == ID) {
        tIndex++;
        if (Id_Tail()) {
            return true;
        }
    }
    return false;
}

bool Id_Tail() {
    //cout<<"Id Tail, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "[") {
        tIndex++;
        if (Expression()) {
            if (tokens[tIndex].value == "]") {
                tIndex++;
                return true;
            }
        }
    } else if (tokens[tIndex].value != "(") {
        return true;
    }
    return false;
}

bool Block_Statements() {
    //cout<<"Block_Statements, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "{") {
        tIndex++;
        if (Statements()) {
            if (tokens[tIndex].value == "}") {
                tIndex++;
                return true;
            }
        }
    }
    return false;
}

bool Statements() {
    //cout<<"Statements, "<<tokens[tIndex].value<<endl;
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
    //cout<<"Statement, "<<tokens[tIndex].value<<endl;
    num_statements++;
    if (tokens[tIndex].type == ID) {
        tIndex++;
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
    } else if (tokens[tIndex].value == "read") {
        tIndex++;
        if (tokens[tIndex].value == "(") {
            tIndex++;
            if (tokens[tIndex].type == ID) {
                tIndex++;
                if (tokens[tIndex].value == ")") {
                    tIndex++;
                    if (tokens[tIndex].value == ";") {
                        tIndex++;
                        return true;
                    }
                }
            }
        }
    } else if (tokens[tIndex].value == "write") {
        tIndex++;
        if (tokens[tIndex].value == "(") {
            tIndex++;
            if (Expression()) {
                if (tokens[tIndex].value == ")") {
                    tIndex++;
                    if (tokens[tIndex].value == ";") {
                        tIndex++;
                        return true;
                    }
                }
            }
        }
    } else if (tokens[tIndex].value == "print") {
        tIndex++;
        if (tokens[tIndex].value == "(") {
            tIndex++;
            if (tokens[tIndex].type == STRING) {
                tIndex++;
                if (tokens[tIndex].value == ")") {
                    tIndex++;
                    if (tokens[tIndex].value == ";") {
                        tIndex++;
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
    } else if (tokens[tIndex].value == "(") {
        tIndex++;
        if (Expr_List()) {
            if (tokens[tIndex].value == ")") {
                tIndex++;
                if (tokens[tIndex].value == ";") {
                    tIndex++;
                    return true;
                }
            }
        }
    }
    return false;
}

bool Assignment_Tail() {
    if (tokens[tIndex].value == "=") {
        tIndex++;
        if (Expression()) {
            if (tokens[tIndex].value == ";") {
                tIndex++;
                return true;
            }
        }
    } else if (tokens[tIndex].value == "[") {
        tIndex++;
        if (Expression()) {
            if (tokens[tIndex].value == "]") {
                tIndex++;
                if (tokens[tIndex].value == "=") {
                    tIndex++;
                    if (Expression()) {
                        if (tokens[tIndex].value == ";") {
                            tIndex++;
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
    //cout<<"Func_Call, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].type == ID) {
        tIndex++;
        if (tokens[tIndex].value == "(") {
            tIndex++;
            if (Expr_List()) {
                if (tokens[tIndex].value == ")") {
                    tIndex++;
                    if (tokens[tIndex].value == ";") {
                        tIndex++;
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
    //cout<<"Expr_List, "<<tokens[tIndex].value<<endl;
    if (Non_Empty_Expr_List()) {
        return true;
    } else {
        return true;
    }
}

bool Non_Empty_Expr_List() {
    //cout<<"Non_Empty_Expr_List, "<<tokens[tIndex].value<<endl;
    if (Expression()) {
        if (Non_Empty_Expr_List_Prime()) {
            return true;
        }
    }
    return false;
}

bool Non_Empty_Expr_List_Prime() {
    //cout<<"Non_Empty_Expr_List_Prime, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == ",") {
        tIndex++;
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
    //cout<<"If_Statement, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "if") {
        tIndex++;
        if (tokens[tIndex].value == "(") {
            tIndex++;
            if (Condition_Expression()) {
                if (tokens[tIndex].value == ")") {
                    tIndex++;
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
    //cout<<"Condition_Expression, "<<tokens[tIndex].value<<endl;
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
    //cout<<"Condition_Op, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "&&") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "||") {
        tIndex++;
        return true;
    }
    return false;
} 

bool Condition() {
    //cout<<"Condition, "<<tokens[tIndex].value<<endl;
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
    //cout<<"Comparison_Op, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "==") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "!=") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == ">") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == ">=") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "<") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "<=") {
        tIndex++;
        return true;
    }
    return false;
}

bool While_Statement() {
    //cout<<"While_Statement, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "while") {
        tIndex++;
        if (tokens[tIndex].value == "(") {
            tIndex++;
            if (Condition_Expression()) {
                if (tokens[tIndex].value == ")") {
                    tIndex++;
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
    //cout<<"Return_Statement, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "return") {
        tIndex++;
        if (Expression()) {
            if (tokens[tIndex].value == ";") {
                tIndex++;
                return true;
            }
        }
    }
    return false;
}

bool Break_Statement() {
    //cout<<"Break_Statement, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "break") {
        tIndex++;
        if (tokens[tIndex].value == ";") {
            tIndex++;
            return true;
        }
    }
    return false;
}

bool Continue_Statement() {
    //cout<<"Continue_Statement, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "continue") {
        tIndex++;
        if (tokens[tIndex].value == ";") {
            tIndex++;
            return true;
        }
    }
    return false;
}

bool Expression() {
    //cout<<"Expression, "<<tokens[tIndex].value<<endl;
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
    //cout<<"Expression_Prime, "<<tokens[tIndex].value<<endl;
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
    //cout<<"Addop, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "+") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "-") {
        tIndex++;
        return true;
    }
    return false;
}

bool Term() {
    //cout<<"Term, "<<tokens[tIndex].value<<endl;
    if (Factor()) {
        if (Term_Prime()) {
            return true;
        }
    }
    return false;
}

bool Term_Prime() {
    //cout<<"Term_Prime, "<<tokens[tIndex].value<<endl;
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
    //cout<<"Mulop, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].value == "*") {
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "/") {
        tIndex++;
        return true;
    }
    return false;
}

bool Factor() {
    //cout<<"Factor, "<<tokens[tIndex].value<<endl;
    if (tokens[tIndex].type == ID) {
        tIndex++;
        if (tokens[tIndex].value == "[") {
            tIndex++;
            if (Expression()) {
                if (tokens[tIndex].value == "]") {
                    tIndex++;
                    return true;
                }
            }
        } else if (tokens[tIndex].value == "(") {
            tIndex++;
            if (Expr_List()) {
                if (tokens[tIndex].value == ")") {
                    tIndex++;
                    return true;
                }
            }
        } else {
            return true;
        }
    } else if (tokens[tIndex].type == NUMBER) {
        //cout<<"Number\n";
        tIndex++;
        return true;
    } else if (tokens[tIndex].value == "-") {
        tIndex++;
        if (tokens[tIndex].type == NUMBER) {
            tIndex++;
            return true;
        }
    } else if (tokens[tIndex].value == "(") {
        tIndex++;
        if (Expression()) {
            if (tokens[tIndex].value == ")") {
                tIndex++;
                return true;
            }
        }
    }
    return false;
}

bool Factor_Tail() {
    if (tokens[tIndex].value == "[") {
        tIndex++;
        if (Expression()) {
            if (tokens[tIndex].value == "]") {
                tIndex++;
                return true;
            }
        }
    } else if (tokens[tIndex].value == "(") {
        tIndex++;
        if (Expr_List()) {
            if (tokens[tIndex].value == ")") {
                tIndex++;
                return true;
            }
        }
    } else {
        return true;
    }
    return false;
}
