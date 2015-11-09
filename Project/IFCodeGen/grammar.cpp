/* 
 * My recursive descent parser
 */
#include "grammar.h"
#include "token.h"

#include <string>
#include <iostream>

using namespace::std;

void Grammar::LoadParser(Parser *newparser) {
    parser = newparser;
}

/**
 * <program> --> <typename> ID <program prime> | empty
*/
bool Grammar::Program() {
    //cout<<"Program, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Type_Name()) {
        if (parser->GetCurrTokenType() == TOKEN_ID) {
            parser->Consume(TOKEN_ID);
            if (Program_Prime()) {
                return true;
            }
        }
    }
    return false;
}
/**
 * <program prime> --> <data decls prime> | <func list prime>
*/
bool Grammar::Program_Prime() {
    //cout<<"Program Prime, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "," || parser->GetCurrTokenValue() == "[" || parser->GetCurrTokenValue() == ";") {
        parser->Consume(TOKEN_SYMBOL);
        if (Data_Decls_Prime()) {
            return true;
        }
    } else {
        if (Func_List_Prime()) {
            return true;
        }
    }
    return false;
}

/**
 * Production:	<func list prime> --> <func decl prime> <func tail> <func list>
 */
bool Grammar::Func_List_Prime() {
    if (Func_Decl_Prime()) {
        if (Func_Tail()) {
            if (Func_List()) {
                return true;
            }
        }
    }
    return false;
}

/**
* Production:	<data decls prime> -> <id tail> <id list prime> semicolon <data decls prime tail> | empty
*/
bool Grammar::Data_Decls_Prime() {
    parser->IncNumVariables();
    if (Id_Tail()) {
        if (Id_List_Prime()) {
            if (parser->GetCurrTokenValue() == ";") {
                parser->Consume(TOKEN_SYMBOL);
                if (Data_Decls_Prime_Tail()) {
                    return true;
                }
            }
        }
    } else {
        return true;
    }
}

/**
* Production:	<data decls prime tail> --> <typename> ID <dataFuncDecl> | empty
*/
bool Grammar::Data_Decls_Prime_Tail() {
    if (Type_Name()) {
        if (parser->GetCurrTokenType() == TOKEN_ID) {
            parser->Consume(TOKEN_ID);
            if (Data_Func_Decl()) {
                return true;
            }
        }
    } else {
        return true;
    }
}

/**
* Production:	<dataFuncDecl> --> <data decls prime> | <func list prime>
*/
bool Grammar::Data_Func_Decl() {
    if (Data_Decls_Prime()) {
        return true;
    } else if (Func_List_Prime()) {
        return true;
    }
    return false;
}

/**
 * Production:	<func list> --> <func> <func list>
 *								| empty
 */
bool Grammar::Func_List() {
    //cout<<"Func_List, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Func()) {
        if (Func_List()) {
            return true;
        }
    } else {
        return true;
    }
}

/**
 * Production:	<func> --> <func decl> <func tail>
*/
bool Grammar::Func() {
    //cout<<"Func, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Func_Decl()) {
        if (Func_Tail()) {
            return true;
        }
    }
    return false;
}

/**
 * Production:	<func tail> -->semicolon 
 *								| left_brace <data decls> <statements> right_brace 
 */
bool Grammar::Func_Tail() {
    //cout<<"Func Tail, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == ";") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "{") {
        parser->Consume(TOKEN_SYMBOL);
        if (Data_Decls()) {
            if (Statements()) {
                if (parser->GetCurrToken()->GetTokenValue() == "}") {
                    parser->Consume(TOKEN_SYMBOL);
                    parser->IncNumFunctions();
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Production:	<func decl> --> <typename> ID <func decl prime>
 */
bool Grammar::Func_Decl() {
    //cout<<"Func_Decl, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Type_Name()) {
        //cout<<"Func_Decl, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
        if (parser->GetCurrToken()->GetTokenType() == TOKEN_ID) {
            parser->Consume(TOKEN_ID);
            if (Func_Decl_Prime()) {
                return true;
            }
        }
    }
    return false;
}

/**
 * Production:	<func decl prime> --> left_parenthesis <parameter list> right_parenthesis 
 */
bool Grammar::Func_Decl_Prime() {
    if (parser->GetCurrToken()->GetTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        if (Parameter_List()) {
            if (parser->GetCurrToken()->GetTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    }
    return false;
}

/**
* Production:	<type name> --> int 
*								| void 
*								| binary 
*								| decimal 
*/
bool Grammar::Type_Name() {
    //cout<<"Type_Name, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "int") {
        parser->Consume(TOKEN_RESERVED);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "void") {
        parser->Consume(TOKEN_RESERVED);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "binary") {
        parser->Consume(TOKEN_RESERVED);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "decimal") {
        parser->Consume(TOKEN_RESERVED);
        return true;
    }
    return false;
}

/**
* Production:	<type name prime> --> int
*									| binary
*									| decimal
*/
bool Grammar::Type_Name_Prime() {
    //cout<<"Type_Name, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "int") {
        parser->Consume(TOKEN_RESERVED);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "binary") {
        parser->Consume(TOKEN_RESERVED);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "decimal") {
        parser->Consume(TOKEN_RESERVED);
        return true;
    }
    return false;
}

/**
* Production:	<parameter list> --> void <parameter list tail>
*									| <typename prime> ID <non-empty list prime> 
*									| empty
*/
bool Grammar::Parameter_List() {
    //cout<<"Parameter_List, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "void") {
        parser->Consume(TOKEN_RESERVED);
        if (Parameter_List_Tail()) {
            return true;
        }
    } else if (Type_Name_Prime()) {
        if (parser->GetCurrToken()->GetTokenType() == TOKEN_ID) {
            parser->Consume(TOKEN_ID);
            if (Non_Empty_List_Prime()) {
                return true;
            }
        }
    } else {
        return true;
    }
}

/**
* Production:	<parameter list tail> --> ID <non-empty list prime> 
*											| empty
*/
bool Grammar::Parameter_List_Tail() {
    if (parser->GetCurrToken()->GetTokenType() == TOKEN_ID) {
        parser->Consume(TOKEN_ID);
        if (Non_Empty_List_Prime()) {
            return true;
        }
    } else {
        return true;
    }
}

/**
* Production:	<non-empty list> --> <type name> ID <non-empty list prime>
*/
bool Grammar::Non_Empty_List() {
    //cout<<"Non_Empty_List, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Type_Name()) {
        if (parser->GetCurrToken()->GetTokenType() == TOKEN_ID) {
            parser->Consume(TOKEN_ID);
            return Non_Empty_List_Prime();
        }
    }
    return false;
}

/**
* Production:	<non-empty list prime> --> comma <type name> ID <non-empty list prime>
*  											| empty
*/
bool Grammar::Non_Empty_List_Prime() {
    //cout<<"Non_Empty_List_Prime, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == ",") {
        parser->Consume(TOKEN_SYMBOL);
        if (Type_Name()) {
            if (parser->GetCurrToken()->GetTokenType() == TOKEN_ID) {
                parser->Consume(TOKEN_ID);
                return Non_Empty_List_Prime();
            }
        }
    } else {
        return true;
    }
}

/**
* Production:	<data decls> --> empty 
*								| <type name> <id list> semicolon <data decls>
*/
bool Grammar::Data_Decls() {
    //cout<<"Data_Decls, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Type_Name()) {
        if (Id_List()) {
            if (parser->GetCurrToken()->GetTokenValue() == ";") {
                parser->Consume(TOKEN_SYMBOL);
                if (Data_Decls()) {
                    return true;
                }
            }
        }
    } else {
        return true;
    }
}

/**
* Production:	<id list> --> <id> <id list prime>
*/
bool Grammar::Id_List() {
    //cout<<"Id_List, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Id()) {
        if (Id_List_Prime()) {
            return true;
        }
    }
    return false;
}

/**
* Production:	<id list prime> --> comma <id list> 
*									| empty
*/
bool Grammar::Id_List_Prime() {
    //cout<<"Id_List_Prime, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == ",") {
        parser->Consume(TOKEN_SYMBOL);
        if (Id_List()) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

/**
* Production:	<id> --> ID <id tail>
*/
bool Grammar::Id() {
    //cout<<"Id, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenType() == TOKEN_ID) {
        parser->Consume(TOKEN_ID);
        if (Id_Tail()) {
            parser->IncNumVariables();
            return true;
        }
    }
    return false;
}

/**
* Production:	<id tail> --> left_bracket <expression> right_bracket 
*								| empty
*/
bool Grammar::Id_Tail() {
    //cout<<"Id Tail, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "[") {
        parser->Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (parser->GetCurrToken()->GetTokenValue() == "]") {
                parser->Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    } else {
        return true;
    }
}

/**
* Production:	<block statements> --> left_brace <statements> right_brace 
*/
bool Grammar::Block_Statements() {
    //cout<<"Block_Statements, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "{") {
        parser->Consume(TOKEN_SYMBOL);
        if (Statements()) {
            if (parser->GetCurrToken()->GetTokenValue() == "}") {
                parser->Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    }
    return false;
}

/**
* Production:	<statements> --> <statement> <statements> 
*								| empty
*/
bool Grammar::Statements() {
    //cout<<"Statements, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Statement()) {
        parser->IncNumStatements();
        if (Statements()) {
            return true;
        }
    } else {
        return true;
    }
}

/**
* Production:	<statement> --> ID <statement tail> 
*								| <if statement> 
*								| <while statement> 
*								| <return statement> 
*								| <break statement> 
*								| <continue statement> 
*								| read left_parenthesis  ID right_parenthesis semicolon 
*								| write left_parenthesis <expression> right_parenthesis semicolon 
*								| print left_parenthesis  STRING right_parenthesis semicolon 
*/
bool Grammar::Statement() {
    //cout<<"Statement, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenType() == TOKEN_ID) {
        parser->Consume(TOKEN_ID);
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
    } else if (parser->GetCurrToken()->GetTokenValue() == "read") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrToken()->GetTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            if (parser->GetCurrToken()->GetTokenType() == TOKEN_ID) {
                parser->Consume(TOKEN_ID);
                if (parser->GetCurrToken()->GetTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    if (parser->GetCurrToken()->GetTokenValue() == ";") {
                        parser->Consume(TOKEN_SYMBOL);
                        return true;
                    }
                }
            }
        }
    } else if (parser->GetCurrToken()->GetTokenValue() == "write") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrToken()->GetTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            if (Expression()) {
                if (parser->GetCurrToken()->GetTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    if (parser->GetCurrToken()->GetTokenValue() == ";") {
                        parser->Consume(TOKEN_SYMBOL);
                        return true;
                    }
                }
            }
        }
    } else if (parser->GetCurrToken()->GetTokenValue() == "print") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrToken()->GetTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            if (parser->GetCurrToken()->GetTokenType() == TOKEN_STRING) {
                parser->Consume(TOKEN_STRING);
                if (parser->GetCurrToken()->GetTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    if (parser->GetCurrToken()->GetTokenValue() == ";") {
                        parser->Consume(TOKEN_SYMBOL);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
* Production:	<statement tail> --> <assignment tail> 
*									| <func call tail>
*/
bool Grammar::Statement_Tail() {
    if (Assignment_Tail()) {
        return true;
    } else if (Func_Call_Tail()) {
        return true;
    }
    return false;
}

/**
* Production:	<assignment tail> --> <id tail> equal_sign <expression> semicolon 
*/
bool Grammar::Assignment_Tail() {
    if (Id_Tail()) {
        if (parser->GetCurrToken()->GetTokenValue() == "=") {
            parser->Consume(TOKEN_SYMBOL);
            if (Expression()) {
                if (parser->GetCurrToken()->GetTokenValue() == ";") {
                    parser->Consume(TOKEN_SYMBOL);
                    return true;
                }
            }
        }
    }
    return false;
}

/**
* Production:	<func call tail> --> left_parenthesis <expr list> right_parenthesis semicolon 
*/
bool Grammar::Func_Call_Tail() {
    //cout<<"Func_Call, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        if (Expr_List()) {
            if (parser->GetCurrToken()->GetTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                if (parser->GetCurrToken()->GetTokenValue() == ";") {
                    parser->Consume(TOKEN_SYMBOL);
                    return true;
                }
            }
        }
    }
    return false;
}

/**
* Production:	<expr list> --> <non-empty expr list> 
*  								| empty
*/
bool Grammar::Expr_List() {
    //cout<<"Expr_List, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Non_Empty_Expr_List()) {
        return true;
    } else {
        return true;
    }
}

/**
* Production:	<non-empty expr list> --> <expression> <non-empty expr list prime> 
*/
bool Grammar::Non_Empty_Expr_List() {
    //cout<<"Non_Empty_Expr_List, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Expression()) {
        if (Non_Empty_Expr_List_Prime()) {
            return true;
        }
    }
    return false;
}

/**
* Production:	<non-empty expr list prime> --> comma <expression> <non-empty expr list prime>
*												| empty
*/
bool Grammar::Non_Empty_Expr_List_Prime() {
    //cout<<"Non_Empty_Expr_List_Prime, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == ",") {
        parser->Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (Non_Empty_Expr_List_Prime()) {
                return true;
            }
        }
    } else {
        return true;
    }
}

/**
* Production:	<if statement> --> if left_parenthesis <condition expression> right_parenthesis <block statements> 
*/
bool Grammar::If_Statement() {
    //cout<<"If_Statement, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "if") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrToken()->GetTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            if (Condition_Expression()) {
                if (parser->GetCurrToken()->GetTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    if (Block_Statements()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
* Production:	<condition expression> -->  <condition> <condition expression tail>
*/
bool Grammar::Condition_Expression() {
    //cout<<"Condition_Expression, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Condition()) {
        if (Condition_Expression_Tail()) {
            return true;
        }
    }
    return false;
}

/**
* Production:	<condition expression tail> -->  <condition op> <condition> 
*												| empty
*/
bool Grammar::Condition_Expression_Tail() {
    if (Condition_Op()) {
        if (Condition()) {
            return true;
        }
    } else {
        return true;
    }
}
       
/**
* Production:	<condition op> --> double_and_sign 
*								| double_or_sign
*/
bool Grammar::Condition_Op() {
    //cout<<"Condition_Op, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "&&") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "||") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    }
    return false;
} 

/**
* Production:	<condition> --> <expression> <comparison op> <expression> 
*/
bool Grammar::Condition() {
    //cout<<"Condition, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Expression()) {
        if (Comparison_Op()) {
            if (Expression()) {
                return true;
            }
        }
    }
    return false;
}

/**
* Production:	<comparison op> --> == 
*									| != 
*									| > 
*									| >= 
*									| < 
*									| <=
*/
bool Grammar::Comparison_Op() {
    //cout<<"Comparison_Op, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "==") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "!=") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == ">") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == ">=") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "<") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "<=") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    }
    return false;
}

/**
* Production:	<while statement> --> while left_parenthesis <condition expression> right_parenthesis <block statements> 
*/
bool Grammar::While_Statement() {
    //cout<<"While_Statement, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "while") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrToken()->GetTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            if (Condition_Expression()) {
                if (parser->GetCurrToken()->GetTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    if (Block_Statements()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
* Production:	<return statement> --> return <return statement prime>
*/
bool Grammar::Return_Statement() {
    //cout<<"Return_Statement, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "return") {
        parser->Consume(TOKEN_RESERVED);
        if (Return_Statement_Tail()) {
            return true;
        }
    }
    return false;
}

/**
* Production:	<return statement tail> --> <expression> semicolon 
*											| semicolon 
*/
bool Grammar::Return_Statement_Tail() {
    if (Expression()) {
        if (parser->GetCurrToken()->GetTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            return true;
        }
    } else if (parser->GetCurrToken()->GetTokenValue() == ";") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    }
    return false;
}

/**
* Production:	<break statement> ---> break semicolon 
*/
bool Grammar::Break_Statement() {
    //cout<<"Break_Statement, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "break") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrToken()->GetTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            return true;
        }
    }
    return false;
}

/**
* Production:	<continue statement> ---> continue semicolon
*/
bool Grammar::Continue_Statement() {
    //cout<<"Continue_Statement, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "continue") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrToken()->GetTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            return true;
        }
    }
    return false;
}

/**
* Production:	<expression> --> <term> <expression prime> 
*/
bool Grammar::Expression() {
    //cout<<"Expression, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Term()) {
        if (Expression_Prime()) {
            return true;
        }
    }
    return false;
}

/**
* Production:	<expression prime> --> <addop> <term> <expression prime>
*				| empty
*/
bool Grammar::Expression_Prime() {
    //cout<<"Expression_Prime, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Addop()) {
        if (Term()) {
            if (Expression_Prime()) {
                return true;
            }
        }
    } else {
        return true;
    }
}

/**
* Production:	<addop> --> plus_sign 
*							| minus_sign 
*/
bool Grammar::Addop() {
    //cout<<"Addop, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "+") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "-") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    }
    return false;
}

/**
* Production:	<term> --> <factor> <term prime> 
*/
bool Grammar::Term() {
    //cout<<"Term, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Factor()) {
        if (Term_Prime()) {
            return true;
        }
    }
    return false;
}

/**
* Production:	<term prime> --> <mulop> <factor> <term prime>
*                               | empty
*/
bool Grammar::Term_Prime() {
    //cout<<"Term_Prime, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (Mulop()) {
        if (Factor()) {
            if (Term_Prime()) {
                return true;
            }
        }
    } else {
        return true;
    }
}

/**
* Production:	<mulop> --> star_sign 
*							| forward_slash
*/
bool Grammar::Mulop() {
    //cout<<"Mulop, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenValue() == "*") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "/") {
        parser->Consume(TOKEN_SYMBOL);
        return true;
    }
    return false;
}

/**
* Production:	<factor> --> ID <factor tail> 
*							| NUMBER 
*							| minus_sign NUMBER 
*							| left_parenthesis <expression> right_parenthesis
*/
bool Grammar::Factor() {
    //cout<<"Factor, "<<parser->GetCurrToken()->GetTokenValue()<<endl;
    if (parser->GetCurrToken()->GetTokenType() == TOKEN_ID) {
        parser->Consume(TOKEN_ID);
        if (Factor_Tail()) {
            return true;
        }
    } else if (parser->GetCurrToken()->GetTokenType() == TOKEN_NUMBER) {
        parser->Consume(TOKEN_NUMBER);
        return true;
    } else if (parser->GetCurrToken()->GetTokenValue() == "-") {
        parser->Consume(TOKEN_SYMBOL);
        if (parser->GetCurrToken()->GetTokenType() == TOKEN_NUMBER) {
            parser->Consume(TOKEN_NUMBER);
            return true;
        }
    } else if (parser->GetCurrToken()->GetTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (parser->GetCurrToken()->GetTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    }
    return false;
}

/**
* Production:	<factor tail> --> left_bracket <expression> right_bracket 
*									| left_parenthesis <expr list> right_parenthesis 
*									| empty
*/
bool Grammar::Factor_Tail() {
    if (parser->GetCurrToken()->GetTokenValue() == "[") {
        parser->Consume(TOKEN_SYMBOL);
        if (Expression()) {
            if (parser->GetCurrToken()->GetTokenValue() == "]") {
                parser->Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    } else if (parser->GetCurrToken()->GetTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        if (Expr_List()) {
            if (parser->GetCurrToken()->GetTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                return true;
            }
        }
    } else {
        return true;
    }
    return false;
}
