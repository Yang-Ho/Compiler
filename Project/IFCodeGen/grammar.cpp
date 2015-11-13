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
 * <program> --> <typename> ID <program prime> 
 *              | empty
 *
 *              $$ = ASTNode(PROGRAM, value=ID, left=typename, right=program prime)
 *              $$ = ASTNode()
*/
ASTNode * Grammar::Program() {
    //cout<<"Program, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * type_name = Type_Name();
    if (type_name) {
        if (parser->GetCurrTokenType() == TOKEN_ID) {
            string id = parser->GetCurrTokenValue();
            parser->Consume(TOKEN_ID);
            ASTNode * program_prime = Program_Prime();
            if (program_prime) {
                return new ASTNode(AST_PROGRAM, id, NULL, type_name, program_prime);
            }
            delete program_prime;
        }
    } else {
        return new ASTNode();
    }
    delete type_name;
    return NULL;
}
/**
 * <program prime> --> <data decls prime>
 *                    | <func list prime>
 *                    
 *                    $$ = data decls prime
 *                    $$ = func list prime
*/
ASTNode * Grammar::Program_Prime() {
    //cout<<"Program Prime, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "," || parser->GetCurrTokenValue() == "[" || parser->GetCurrTokenValue() == ";") {
        return Data_Decls_Prime();
    } else {
        return Func_List_Prime();
    }
    return NULL;
}

/**
 * Production:	<func list prime> --> <func decl prime> <func tail> <func list>
 *
 *                                  $$ = ASTNode(FUNC LIST PRIME, child=func tail, left=func decl prime, right= func list)
 */
ASTNode * Grammar::Func_List_Prime() {
    //cout<<"Func List Prime, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * func_decl_prime = Func_Decl_Prime();
    if (func_decl_prime) {
        ASTNode * func_tail = Func_Tail();
        if (func_tail) {
            ASTNode * func_list = Func_List();
            if (func_list) {
                return new ASTNode(AST_FUNC_LIST, "", func_tail, func_decl_prime, func_list);
            }
            delete func_list;
        }
        delete func_tail;
    }
    delete func_decl_prime;
    return NULL;
}

/**
* Production:	<data decls prime> -> <id tail> <id list prime> semicolon <data decls prime tail> 
*                                   | empty
*
*                                   $$ = ASTNode(DATA DECLS PRIME, child=id tail, child=id list prime, right=data decls prime tail)
*                                   $$ = ASTNode()
*/
ASTNode * Grammar::Data_Decls_Prime() {
    //cout<<"Data Decls Prime, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode* id_tail = Id_Tail();
    if (id_tail) {
        parser->IncNumVariables();
        ASTNode * id_list_prime = Id_List_Prime();
        if (id_list_prime) {
            if (parser->GetCurrTokenValue() == ";") {
                parser->Consume(TOKEN_SYMBOL);
                ASTNode* data_decls_prime_tail = Data_Decls_Prime_Tail();
                if (data_decls_prime_tail) {
                    return new ASTNode(AST_DATA_DECLS_PRIME, "", id_tail, id_list_prime, data_decls_prime_tail);
                }
                delete data_decls_prime_tail;
            }
        }
        delete id_list_prime;
    } else {
        return new ASTNode();
    }
    delete id_tail;
    return NULL;
}

/**
* Production:	<data decls prime tail> --> <typename> ID <data Func Decl> 
*                                           | empty
*
*                                           $$ = ASTNode(DATA DECLS PRIME TAIL, value=ID, left=typename, right=data func decl)
*                                           $$ = ASTNode()
*/
ASTNode * Grammar::Data_Decls_Prime_Tail() {
    //cout<<"Data Decls Tail, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * type_name = Type_Name();
    if (type_name) {
        if (parser->GetCurrTokenType() == TOKEN_ID) {
            string id = parser->GetCurrTokenValue();
            parser->Consume(TOKEN_ID);
            ASTNode * data_func_decl = Data_Func_Decl();
            if (data_func_decl) {
                return new ASTNode(AST_DATA_DECLS_PRIME_TAIL, id, NULL, type_name, data_func_decl);
            }
            delete data_func_decl;
        }
    } else {
        return new ASTNode();
    }
    delete type_name;
    return NULL;
}

/**
* Production:	<dataFuncDecl> --> <data decls prime> 
*                               | <func list prime>
*
*                               $$ = data decls prime
*                               $$ = func list prime
*/
ASTNode * Grammar::Data_Func_Decl() {
    //cout<<"Data Func Decl, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * data_decls_prime = Data_Decls_Prime();
    if (data_decls_prime) {
        return data_decls_prime;
    } else {
        ASTNode * func_list_prime = Func_List_Prime();
        if (func_list_prime) {
            return func_list_prime;
        }
        delete func_list_prime;
    }
    delete data_decls_prime;
    return NULL;
}

/**
 * Production:	<func list> --> <func> <func list>
 *								| empty
 *
 *								$$ = ASTNode(FUNC LIST, left=func, right=func list)
 *								$$ = ASTNode()
 */
ASTNode * Grammar::Func_List() {
    //cout<<"Func_List, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "int" || 
            parser->GetCurrTokenValue() == "void" ||
            parser->GetCurrTokenValue() == "binary" ||
            parser->GetCurrTokenValue() == "decimal") {
        ASTNode * func = Func();
        if (func) {
            ASTNode * func_list = Func_List();
            if (func_list) {
                return new ASTNode(AST_FUNC_LIST, "", NULL, func, func_list);
            }
            delete func_list;
        }
        delete func;
    } else {
        return new ASTNode();
    }
    return NULL;
}

/**
 * Production:	<func> --> <func decl> <func tail>
 *      
 *                      $$ = ASTNode(FUNC, left=func decl, right=func tail)
*/
ASTNode * Grammar::Func() {
    //cout<<"Func, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode* func_decl = Func_Decl();
    if (func_decl) {
        ASTNode * func_tail = Func_Tail();
        if (func_tail) {
            return new ASTNode(AST_FUNC, "", NULL, func_decl, func_tail);
        }
        delete func_tail;
    }
    delete func_decl;
    return NULL;
}

/**
 * Production:	<func tail> -->semicolon 
 *								| left_brace <data decls> <statements> right_brace 
 *
 *								$$ = ASTNode(FUNC TAIL, value=;)
 *								$$ = ASTNode(FUNC TAIL, left=data decls, right=statements)
 */
ASTNode * Grammar::Func_Tail() {
    //cout<<"Func Tail, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == ";") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_FUNC_TAIL, ";", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "{") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode * data_decls = Data_Decls();
        if (data_decls) {
            ASTNode * statements = Statements();
            if (statements) {
                if (parser->GetCurrTokenValue() == "}") {
                    parser->Consume(TOKEN_SYMBOL);
                    parser->IncNumFunctions();
                    return new ASTNode(AST_FUNC_TAIL, "", NULL, data_decls, statements);
                }
            }
            delete statements;
        }
        delete data_decls;
    }
    return NULL;
}

/**
 * Production:	<func decl> --> <typename> ID <func decl prime>
 *
 *                          $$ = ASTNode(FUNC DECL, value=ID, left=typename, right=func decl prime)
 */
ASTNode * Grammar::Func_Decl() {
    //cout<<"Func_Decl, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * type_name = Type_Name();
    if (type_name) {
        ////cout<<"Func_Decl, "<<parser->GetCurrTokenValue()<<endl;
        if (parser->GetCurrTokenType() == TOKEN_ID) {
            string id = parser->GetCurrTokenValue();
            parser->Consume(TOKEN_ID);
            ASTNode * func_decl_prime = Func_Decl_Prime();
            if (func_decl_prime) {
                return new ASTNode(AST_FUNC_DECL, id, NULL, type_name, func_decl_prime);
            }
            delete func_decl_prime;
        }
    }
    delete type_name;
    return NULL;
}

/**
 * Production:	<func decl prime> --> left_parenthesis <parameter list> right_parenthesis 
 *
 *                                  $$ = parameter list 
 */
ASTNode * Grammar::Func_Decl_Prime() {
    //cout<<"Func_Decl prime, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode * parameter_list = Parameter_List();
        if (parameter_list) {
            if (parser->GetCurrTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                return parameter_list;
            }
        }
        delete parameter_list;
    }
    return NULL;
}

/**
* Production:	<type name> --> int 
*								| void 
*								| binary 
*								| decimal 
*
*									$$ = ASTNode(RESERVED)
*									$$ = ASTNode(RESERVED)
*									$$ = ASTNode(RESERVED)
*									$$ = ASTNode(RESERVED)
*/
ASTNode * Grammar::Type_Name() {
    //cout<<"Type_Name, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "int") {
        parser->Consume(TOKEN_RESERVED);
        return new ASTNode(AST_RESERVED, "int", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "void") {
        parser->Consume(TOKEN_RESERVED);
        return new ASTNode(AST_RESERVED, "void", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "binary") {
        parser->Consume(TOKEN_RESERVED);
        return new ASTNode(AST_RESERVED, "binary", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "decimal") {
        parser->Consume(TOKEN_RESERVED);
        return new ASTNode(AST_RESERVED, "decimal", NULL, NULL, NULL);
    }
    return NULL;
}

/**
* Production:	<type name prime> --> int
*									| binary
*									| decimal
*
*									$$ = ASTNode(RESERVED)
*									$$ = ASTNode(RESERVED)
*									$$ = ASTNode(RESERVED)
*/
ASTNode * Grammar::Type_Name_Prime() {
    //cout<<"Type_Name, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "int") {
        parser->Consume(TOKEN_RESERVED);
        return new ASTNode(AST_RESERVED, "int", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "binary") {
        parser->Consume(TOKEN_RESERVED);
        return new ASTNode(AST_RESERVED, "binary", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "decimal") {
        parser->Consume(TOKEN_RESERVED);
        return new ASTNode(AST_RESERVED, "decimal", NULL, NULL, NULL);
    }
    return NULL;
}

/**
* Production:	<parameter list> --> void <parameter list tail>
*									| <typename prime> ID <non-empty list prime> 
*									| empty
*
*									$$ = ASTNode(PARAM LIST, value=void, child=parameter list tail)
*									$$ = ASTNode(PARAM LIST, value=ID, left=typename prime, right=non empty list prime)
*									$$ = ASTNode()
*/
ASTNode * Grammar::Parameter_List() {
    //cout<<"Parameter_List, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "void") {
        parser->Consume(TOKEN_RESERVED);
        ASTNode * parameter_list_tail = Parameter_List_Tail();
        if (parameter_list_tail) {
            return new ASTNode(AST_PARAM_LIST, "void", parameter_list_tail, NULL, NULL);
        }
        delete parameter_list_tail;
    } else {
        ASTNode * type_name_prime = Type_Name_Prime();
        if (type_name_prime) {
            if (parser->GetCurrTokenType() == TOKEN_ID) {
                string id = parser->GetCurrTokenValue();
                parser->Consume(TOKEN_ID);
                ASTNode * non_empty_list_prime = Non_Empty_List_Prime();
                if (non_empty_list_prime) {
                    return new ASTNode(AST_PARAM_LIST, id, NULL, type_name_prime, non_empty_list_prime);
                }
                delete non_empty_list_prime;
            }
        } else {
            return new ASTNode();
        }
        delete type_name_prime;
    }
    return NULL;
}

/**
* Production:	<parameter list tail> --> ID <non-empty list prime> 
*											| empty
*
*											$$ = ASTNode(PARAM LIST TAIL, value=ID, child=non empty list prime)
*											$$ = ASTNode()
*/
ASTNode * Grammar::Parameter_List_Tail() {
    //cout<<"Parameter_List tail, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenType() == TOKEN_ID) {
        string id = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_ID);
        ASTNode * non_empty_list_prime = Non_Empty_List_Prime();
        if (non_empty_list_prime) {
            return new ASTNode(AST_PARAM_LIST, id, non_empty_list_prime, NULL, NULL);
        }
        delete non_empty_list_prime;
    } else {
        return new ASTNode();
    }
    return NULL;
}

/**
* Production:	<non-empty list> --> <type name> ID <non-empty list prime>
*
*                                   $$ = ASTNode(NON EMPTY LIST, value=ID, left=type name, right=non empty list prime)
*/
ASTNode * Grammar::Non_Empty_List() {
    //cout<<"Non_Empty_List, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * type_name = Type_Name();
    if (type_name) {
        if (parser->GetCurrTokenType() == TOKEN_ID) {
            string id = parser->GetCurrTokenValue();
            parser->Consume(TOKEN_ID);
            ASTNode * non_empty_list_prime = Non_Empty_List_Prime();
            if (non_empty_list_prime) {
                return new ASTNode(AST_NON_EMPTY_LIST, id, NULL, type_name, non_empty_list_prime);
            }
            delete non_empty_list_prime;
        }
    }
    delete type_name;
    return NULL;
}

/**
* Production:	<non-empty list prime> --> comma <type name> ID <non-empty list prime>
*  											| empty
*
*  											$$ = ASTNode(NON EMPTY LIST PRIME, value=ID, left=type name, right=non-empty list prime)
*  											$$ = ASTNode()
*/
ASTNode * Grammar::Non_Empty_List_Prime() {
    //cout<<"Non_Empty_List_Prime, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == ",") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode * type_name = Type_Name();
        if (type_name) {
            if (parser->GetCurrTokenType() == TOKEN_ID) {
                string id = parser->GetCurrTokenValue();
                parser->Consume(TOKEN_ID);
                ASTNode * non_empty_list_prime = Non_Empty_List_Prime();
                if (non_empty_list_prime) {
                    return new ASTNode(AST_NON_EMPTY_LIST_PRIME, id, NULL, type_name, non_empty_list_prime);
                }
                delete non_empty_list_prime;
            }
        }
        delete type_name;
    } else {
        return new ASTNode();
    }
    return NULL;
}

/**
* Production:	<data decls> --> empty 
*								| <type name> <id list> semicolon <data decls>
*
*								$$ = ASTNode()
*								$$ = ASTNode(data decls, child=id list, left=type name, right=data decls) 
*/
ASTNode * Grammar::Data_Decls() {
    //cout<<"Data_Decls, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * type_name = Type_Name();
    if (type_name) {
        ASTNode * id_list = Id_List();
        if (id_list) {
            if (parser->GetCurrTokenValue() == ";") {
                parser->Consume(TOKEN_SYMBOL);
                ASTNode * data_decls = Data_Decls();
                if (data_decls) {
                    return new ASTNode(AST_DATA_DECLS, "", id_list, type_name, data_decls);
                }
                delete data_decls;
            }
        }
        delete id_list;
    } else {
        delete type_name;
        return new ASTNode();
    }
    delete type_name;
    return NULL;
}

/**
* Production:	<id list> --> <id> <id list prime>
*                           
*                           $$ = ASTNode(ID LIST, left=id, right=id list prime)
*/
ASTNode * Grammar::Id_List() {
    //cout<<"Id_List, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode* id = Id();
    if (id) {
        ASTNode * id_list_prime = Id_List_Prime();
        if (id_list_prime) {
            return new ASTNode(AST_ID_LIST, "", NULL, id, id_list_prime);
        }
        delete id_list_prime;
    }
    delete id;
    return NULL;
}

/**
* Production:	<id list prime> --> comma <id list> 
*									| empty
*
*									$$ = ASTNode(ID LIST, value="," child=id list)
*									$$ = ASTNode()
*/
ASTNode * Grammar::Id_List_Prime() {
    //cout<<"Id_List_Prime, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == ",") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode* id_list = Id_List();
        if (id_list) {
            return new ASTNode(AST_ID_LIST, ",", id_list, NULL, NULL);
        }
        delete id_list;
    } else {
        return new ASTNode();
    }
    return NULL;
}

/**
* Production:	<id> --> ID <id tail>
*                        
*                        $$ = ASTNode(ID, value=ID, child=id tail)
*/
ASTNode * Grammar::Id() {
    //cout<<"Id, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenType() == TOKEN_ID) {
        string id = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_ID);
        ASTNode* id_tail = Id_Tail();
        if (id_tail) {
            parser->IncNumVariables();
            return new ASTNode(AST_ID, id, id_tail, NULL, NULL);
        }
        delete id_tail;
    }
    return NULL;
}

/**
* Production:	<id tail> --> left_bracket <expression> right_bracket 
*								| empty
*
*								$$ = ASTNode(ARRAY ELEMENT, child=expression)
*								$$ = ASTNode()
*/
ASTNode * Grammar::Id_Tail() {
    //cout<<"Id Tail, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "[") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode* expression = Expression();
        if (expression) {
            if (parser->GetCurrTokenValue() == "]") {
                parser->Consume(TOKEN_SYMBOL);
                return new ASTNode(AST_ARRAY_ELEMENT, "", expression, NULL, NULL);
            }
        }
        delete expression;
    } else {
        return new ASTNode();
    }
    return NULL;
}

/**
* Production:	<block statements> --> left_brace <statements> right_brace 
*                                   
*                                   $$ = ASTNode(BLOCK, child=statements)
*/
ASTNode * Grammar::Block_Statements() {
    //cout<<"Block_Statements, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "{") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode * statements = Statements();
        if (statements) {
            //cout<<"In block statements\n";
            if (parser->GetCurrTokenValue() == "}") {
                parser->Consume(TOKEN_SYMBOL);
                return new ASTNode(AST_BLOCK, "", statements, NULL, NULL);
            }
        }
        delete statements;
    }
    return NULL;
}

/**
* Production:	<statements> --> <statement> <statements> 
*								| empty
*
*								$$ = ASTNode(STATEMENT, left=statement, right=statements)
*/
ASTNode * Grammar::Statements() {
    //cout<<"Statements, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * statement = Statement();
    if (statement) {
        parser->IncNumStatements();
        //cout<<"Back in statements from statement"<<endl;
        ASTNode * statements = Statements();
        if (statements) {
            return new ASTNode(AST_STATEMENT, "", NULL, statement, statements);
        }
    } else {
        return new ASTNode();
    }
    delete statement;
    return NULL;
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
*
*								$$ = ASTNode(ID_STATEMENT, value=id, child=statement tail)
*								$$ = if statement
*								$$ = while statement
*								$$ = return statement
*								$$ = breakstatement
*								$$ = continue statement
*								$$ = ASTNode(READ, value=id)
*								$$ = ASTNode(WRITE, child=expression)
*								$$ = ASTNode(print, value=string)
*/
ASTNode * Grammar::Statement() {
    //cout<<"Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenType() == TOKEN_ID) {
        string id = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_ID);
        ASTNode * statement_tail = Statement_Tail();
        if (statement_tail) {
            return new ASTNode(AST_ID_STATEMENT, id, statement_tail, NULL, NULL);
        }
        delete statement_tail;
    } else {
        ASTNode * if_statement = If_Statement();
        if (if_statement) {
            return if_statement;
        } else {
            ASTNode * while_statement = While_Statement();
            if (while_statement) {
                return while_statement;
            } else {
                ASTNode * return_statement = Return_Statement();
                if (return_statement) {
                    return return_statement;
                } else {
                    ASTNode * break_statement = Break_Statement();
                    if (break_statement) {
                        return break_statement;
                    } else {
                        ASTNode * continue_statement = Continue_Statement();
                        if (continue_statement) {
                            return continue_statement;
                        } else if (parser->GetCurrTokenValue() == "read") {
                            //cout<<"read\n";
                            parser->Consume(TOKEN_RESERVED);
                            if (parser->GetCurrTokenValue() == "(") {
                                parser->Consume(TOKEN_SYMBOL);
                                if (parser->GetCurrTokenType() == TOKEN_ID) {
                                    string id = parser->GetCurrTokenValue();
                                    parser->Consume(TOKEN_ID);
                                    if (parser->GetCurrTokenValue() == ")") {
                                        parser->Consume(TOKEN_SYMBOL);
                                        if (parser->GetCurrTokenValue() == ";") {
                                            parser->Consume(TOKEN_SYMBOL);
                                            return new ASTNode(AST_READ, id, NULL, NULL, NULL);
                                        }
                                    }
                                }
                            }
                        } else if (parser->GetCurrTokenValue() == "write") {
                            //cout<<"write\n";
                            parser->Consume(TOKEN_RESERVED);
                            if (parser->GetCurrTokenValue() == "(") {
                                parser->Consume(TOKEN_SYMBOL);
                                ASTNode * expression = Expression();
                                if (expression) {
                                    if (parser->GetCurrTokenValue() == ")") {
                                        parser->Consume(TOKEN_SYMBOL);
                                        if (parser->GetCurrTokenValue() == ";") {
                                            parser->Consume(TOKEN_SYMBOL);
                                            return new ASTNode(AST_WRITE, "", expression, NULL, NULL);
                                        }
                                    }
                                }
                                delete expression;
                            }
                        } else if (parser->GetCurrTokenValue() == "print") {
                            //cout<<"print\n";
                            parser->Consume(TOKEN_RESERVED);
                            if (parser->GetCurrTokenValue() == "(") {
                                parser->Consume(TOKEN_SYMBOL);
                                if (parser->GetCurrTokenType() == TOKEN_STRING) {
                                    string val = parser->GetCurrTokenValue();
                                    parser->Consume(TOKEN_STRING);
                                    if (parser->GetCurrTokenValue() == ")") {
                                        parser->Consume(TOKEN_SYMBOL);
                                        if (parser->GetCurrTokenValue() == ";") {
                                            parser->Consume(TOKEN_SYMBOL);
                                            return new ASTNode(AST_PRINT, val, NULL, NULL, NULL);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    delete break_statement;
                }
                delete return_statement;
            }
            delete while_statement;
        }
        delete if_statement;
    }
    return NULL;
}

/**
* Production:	<statement tail> --> <assignment tail> 
*									| <func call tail>
*
*									$$ = assignment
*									$$ = func call 
*/
ASTNode * Grammar::Statement_Tail() {
    //cout<<"Statement tail, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode* assignment_tail = Assignment_Tail();
    if (assignment_tail) {
        return assignment_tail;
    } else {
        ASTNode* func_call_tail = Func_Call_Tail();
        if (func_call_tail) {
            return func_call_tail;
        }
        delete func_call_tail;
    }
    delete assignment_tail; 
    return NULL;
}

/**
* Production:	<assignment tail> --> <id tail> equal_sign <expression> semicolon 
*                   
*                                   $$ = ASTNode(ASSIGNMENT, left=id tail, right=expression)
*/
ASTNode * Grammar::Assignment_Tail() {
    //cout<<"assignment tail, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode* id_tail = Id_Tail();
    if (id_tail) {
        if (parser->GetCurrTokenValue() == "=") {
            parser->Consume(TOKEN_SYMBOL);
            ASTNode * expression = Expression();
            if (expression) {
                if (parser->GetCurrTokenValue() == ";") {
                    parser->Consume(TOKEN_SYMBOL);
                    return new ASTNode(AST_ASSIGNMENT, "", NULL, id_tail, expression);
                }
            }
            delete expression;
        }
    }
    delete id_tail;
    return NULL;
}

/**
* Production:	<func call tail> --> left_parenthesis <expr list> right_parenthesis semicolon 
*
*                                   $$ = ASTNode(FUNC CALL, child=expr list, value=;)
*/
ASTNode * Grammar::Func_Call_Tail() {
    //cout<<"Func_Call tail, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode* expr_list = Expr_List();
        if (expr_list) {
            if (parser->GetCurrTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                if (parser->GetCurrTokenValue() == ";") {
                    parser->Consume(TOKEN_SYMBOL);
                    return new ASTNode(AST_FUNC_CALL, ";", expr_list, NULL, NULL);
                }
            }
        }
        delete expr_list;
    }
    return NULL;
}

/**
* Production:	<expr list> --> <non-empty expr list> 
*  								| empty
*
*  								$$ = ASTNode(EXPR LIST, child=non-empty expr list)
*  								$$ = ASTNode()
*/
ASTNode * Grammar::Expr_List() {
    //cout<<"Expr_List, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * non_empty_expr_list = Non_Empty_Expr_List();
    if (non_empty_expr_list) {
        return new ASTNode(AST_EXPR_LIST, "", non_empty_expr_list, NULL, NULL);
    } else {
        delete non_empty_expr_list;
        return new ASTNode();
    }
    delete non_empty_expr_list;
    return NULL;
}

/**
* Production:	<non-empty expr list> --> <expression> <non-empty expr list prime> 
*
*                                       $$ = ASTNode(NON EMPTY EXPR LIST, left=expression, right=non-empty expr list prime)
*/
ASTNode * Grammar::Non_Empty_Expr_List() {
    //cout<<"Non_Empty_Expr_List, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode* expression = Expression();
    if (expression) {
        ASTNode* non_empty_expr_list_prime = Non_Empty_Expr_List_Prime();
        if (non_empty_expr_list_prime) {
            return new ASTNode(AST_NON_EMPTY_EXPR_LIST, "", NULL, expression, non_empty_expr_list_prime);
        }
        delete non_empty_expr_list_prime;
    }
    delete expression;
    return NULL;
}

/**
* Production:	<non-empty expr list prime> --> comma <expression> <non-empty expr list prime>
*												| empty
*
*												$$ = ASTNode(NONEMPTYEXPR LIST PRIME, value=, left=expression, right=non-empty expr list prime)
*												$$ = ASTNode()
*/
ASTNode * Grammar::Non_Empty_Expr_List_Prime() {
    //cout<<"Non_Empty_Expr_List_Prime, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == ",") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode* expression = Expression();
        if (expression) {
            ASTNode* non_empty_expr_list_prime = Non_Empty_Expr_List_Prime();
            if (non_empty_expr_list_prime) {
                return new ASTNode(AST_NON_EMPTY_EXPR_LIST, ",", NULL, expression, non_empty_expr_list_prime);
            }
            delete non_empty_expr_list_prime;
        }
        delete expression;
    } else {
        return new ASTNode();
    }
    return NULL;
}

/**
* Production:	<if statement> --> if left_parenthesis <condition expression> right_parenthesis <block statements> 
*           
*                               $$ = ASTNode(IF_STATMENT, left=condition expression, right=block statements)
*/
ASTNode * Grammar::If_Statement() {
    //cout<<"If_Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "if") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            ASTNode* condition_expression = Condition_Expression();
            if (condition_expression) {
                if (parser->GetCurrTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    ASTNode* block_statements = Block_Statements();
                    if (block_statements) {
                        return new ASTNode(AST_IF, "", NULL, condition_expression, block_statements);
                    }
                    delete block_statements;
                }
            }
            delete condition_expression;
        }
    }
    return NULL;
}

/**
* Production:	<condition expression> -->  <condition> <condition expression tail>
*
*                                       $$ = ASTNode(CONDTION EXPR, left=condition, right=condition expression tail)
*/
ASTNode * Grammar::Condition_Expression() {
    //cout<<"Condition_Expression, "<<parser->GetCurrTokenValue()<<endl;

    ASTNode* condition = Condition();
    if (condition) {
        ASTNode* condition_expression_tail = Condition_Expression_Tail();
        if (condition_expression_tail) {
            return new ASTNode(AST_CONDITION_EXPR, "", NULL, condition, condition_expression_tail);
        }
        delete condition_expression_tail;
    }
    delete condition;
    return NULL;
}

/**
* Production:	<condition expression tail> -->  <condition op> <condition> 
*												| empty
*
*												$$ = ASTNode(CONDTION EXPR TAIL, value=condtion op, child=condition)
*												$$ = ASTNode()
*/
ASTNode * Grammar::Condition_Expression_Tail() {
    //cout<<"Condition_Expression tail, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode* condition_op = Condition_Op();
    if (condition_op) {
        ASTNode* condition = Condition();
        if (condition) {
            string val = condition_op->GetValue();
            delete condition_op;
            return new ASTNode(AST_CONDITION_EXPR, val, condition, NULL, NULL);
        }
        delete condition;
    } else {
        delete condition_op;
        return new ASTNode();
    }
    delete condition_op;
    return NULL;
}
       
/**
* Production:	<condition op> --> double_and_sign 
*								| double_or_sign
*
*								$$ = ASTNode(CONDTION OP)
*								$$ = ASTNode(CONDTION OP)
*/
ASTNode * Grammar::Condition_Op() {
    //cout<<"Condition_Op, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "&&") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_CONDITION_OP, "&&", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "||") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_CONDITION_OP, "||", NULL, NULL, NULL);
    }
    return NULL;
} 

/**
* Production:	<condition> --> <expression> <comparison op> <expression> 
*           
*                           $$ = ASTNode(CONDITION, value=comparison op, left=expression1, right=expression2)
*/
ASTNode * Grammar::Condition() {
    //cout<<"Condition, "<<parser->GetCurrTokenValue()<<endl;
    ASTNode * expression1 = Expression();
    if (expression1) {
        ASTNode* comparison_op = Comparison_Op();
        if (comparison_op) {
            ASTNode* expression2 = Expression();
            if (expression2) {
                return new ASTNode(AST_CONDITION, comparison_op->GetValue(), NULL, expression1, expression2);
            }
            delete expression2;
        }
        delete comparison_op;
    }
    delete expression1;
    return NULL;
}

/**
* Production:	<comparison op> --> == 
*									| != 
*									| > 
*									| >= 
*									| < 
*									| <=
*
*									$$ = ASTNode(COMPARISION OP)
*									$$ = ASTNode(COMPARISION OP)
*									$$ = ASTNode(COMPARISION OP)
*									$$ = ASTNode(COMPARISION OP)
*									$$ = ASTNode(COMPARISION OP)
*/
ASTNode * Grammar::Comparison_Op() {
    //cout<<"Comparison_Op, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "==") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_COMPARISON_OP, "==", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "!=") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_COMPARISON_OP, "!=", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == ">") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_COMPARISON_OP, ">", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == ">=") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_COMPARISON_OP, ">=", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "<") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_COMPARISON_OP, "<", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "<=") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_COMPARISON_OP, "<=", NULL, NULL, NULL);
    }
    return NULL;
}

/**
* Production:	<while statement> --> while left_parenthesis <condition expression> right_parenthesis <block statements> 
*                                   
*                                   $$ = ASTNode(WHILE, left=condition expression, right=block statements)
*/
ASTNode * Grammar::While_Statement() {
    //cout<<"While_Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "while") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            ASTNode* condition_expression = Condition_Expression();
            if (condition_expression) {
                if (parser->GetCurrTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    ASTNode* block_statements = Block_Statements();
                    if (block_statements) {
                        return new ASTNode(AST_WHILE, "", NULL, condition_expression, block_statements);
                    }
                    delete block_statements;
                }
            }
            delete condition_expression;
        }
    }
    return NULL;
}

/**
* Production:	<return statement> --> return <return statement tail>
*                                       
*                                       $$ = ASTNode(RETURN, child=return tail)
*/
ASTNode * Grammar::Return_Statement() {
    //cout<<"Return_Statement, "<<parser->GetCurrTokenValue()<<endl;

    if (parser->GetCurrTokenValue() == "return") {
        parser->Consume(TOKEN_RESERVED);
        ASTNode * return_statement_tail = Return_Statement_Tail();
        if (return_statement_tail) {
            return return_statement_tail;
        }
        delete return_statement_tail;
    }
    return NULL;
}

/**
* Production:	<return statement tail> --> <expression> semicolon 
*											| semicolon 
*
*											$$ = ASTNode(RETURN, child=expression)
*											$$ = ASTNode(RETURN)
*/
ASTNode * Grammar::Return_Statement_Tail() {
    //cout<<"Return_Statement tail, "<<parser->GetCurrTokenValue()<<endl;

    ASTNode* expression = Expression();
    if (expression) {
        if (parser->GetCurrTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            return new ASTNode(AST_RETURN, "", expression, NULL, NULL);
        }
    } else if (parser->GetCurrTokenValue() == ";") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_RETURN, "", NULL, NULL, NULL);
    }
    delete expression;
    return NULL;
}

/**
* Production:	<break statement> ---> break semicolon 
*                                       $$ = ASTNode(break)
*/
ASTNode * Grammar::Break_Statement() {
    //cout<<"Break_Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "break") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            return new ASTNode(AST_BREAK, "", NULL, NULL, NULL);
        }
    }
    return NULL;
}

/** 
* Production:	<continue statement> ---> continue semicolon
*                                       $$ = ASTNode(CONTINUE)
*/
ASTNode * Grammar::Continue_Statement() {
    //cout<<"Continue_Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "continue") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            return new ASTNode(AST_CONTINUE, "", NULL, NULL, NULL);
        }
    }
    return NULL;
}

/**
* Production:	<expression> --> <term> <expression prime> 
*                         $$ = ASTNode(EXPRESSION, left=term, right=expression prime)
*/
ASTNode * Grammar::Expression() {
    //cout<<"Expression, "<<parser->GetCurrTokenValue()<<endl;

    ASTNode * term = Term();
    if (term) {
        ASTNode * expression_prime = Expression_Prime();
        if (expression_prime) {
            return new ASTNode(AST_EXPRESSION, "", NULL, term, expression_prime);
        }
        delete expression_prime;
    }
    delete term;
    return NULL;
}

/**
* Production:	<expression prime> --> <addop> <term> <expression prime>
*				| empty
*               $$ = ASTNode(EXPRESSION, value=addop, left=term, right=expression prime) 
*               $$ = ASTNode()
*/
ASTNode * Grammar::Expression_Prime() {
    //cout<<"Expression_Prime, "<<parser->GetCurrTokenValue()<<endl;

    ASTNode * addop = Addop();
    if (addop) {
        ASTNode * term = Term();
        if (term) {
            ASTNode * expression_prime = Expression_Prime();
            if (expression_prime) {
                ASTNode * result = new ASTNode(AST_EXPRESSION, addop->GetValue(), NULL, term, expression_prime);
                delete addop;
                return result;
            }
            delete expression_prime;
        }
        delete term;
    } else {
        delete addop;
        return new ASTNode();
    }
    delete addop;
    return NULL;
}

/**
* Production:	<addop> --> plus_sign 
*							| minus_sign 
*
*							$$ = AST(OP)
*							$$ = AST(OP)
*/
ASTNode * Grammar::Addop() {
    //cout<<"Addop, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "+") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_OP, "+", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "-") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_OP, "-", NULL, NULL, NULL);
    }
    return NULL;
}

/**
* Production:	<term> --> <factor> <term prime> 
*                        
*                         $$ = ASTNode(TERM, left=factor, right=term prime)
*/
ASTNode * Grammar::Term() {
    //cout<<"Term, "<<parser->GetCurrTokenValue()<<endl;

    ASTNode * factor = Factor();
    if (factor) {
        ASTNode * term_prime = Term_Prime();
        if (term_prime) {
            return new ASTNode(AST_TERM, "", NULL, factor, term_prime);
        }
        delete term_prime;
    }
    delete factor;
    return NULL;
}

/**
* Production:	<term prime> --> <mulop> <factor> <term prime>
*                               | empty
*
*                               $$ = ASTNode(TERM_PRIME, value=mulop, left=factor, right=term prime) 
*                               $$ = ASTNode()
*/
ASTNode * Grammar::Term_Prime() {
    //cout<<"Term_Prime, "<<parser->GetCurrTokenValue()<<endl;

    ASTNode * mulop = Mulop();

    if (mulop) {
        ASTNode * factor = Factor();
        if (factor) {
            ASTNode * term_prime = Term_Prime();
            if (term_prime) {
                ASTNode * term_prime1 = new ASTNode(AST_TERM, mulop->GetValue(), NULL, factor, term_prime);
                delete mulop;
                return term_prime1;
            }
            delete term_prime;
        }
        delete factor;
    } else {
        delete mulop;
        return new ASTNode();
    }
    delete mulop;
    return NULL;
}

/**
* Production:	<mulop> --> star_sign 
*							| forward_slash
*
*							$$ = ASTNode(OP)
*							$$ = ASTNode(OP)
*/
ASTNode * Grammar::Mulop() {
    //cout<<"Mulop, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "*") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_OP, "*", NULL, NULL, NULL);
    } else if (parser->GetCurrTokenValue() == "/") {
        parser->Consume(TOKEN_SYMBOL);
        return new ASTNode(AST_OP, "/", NULL, NULL, NULL);
    }
    return NULL;
}

/**
* Production:	<factor> --> ID <factor tail> 
*							| NUMBER 
*							| minus_sign NUMBER 
*							| left_parenthesis <expression> right_parenthesis
*
*                           $$ = ASTNode(FACTOR, value=ID, child=factor_tail)
*                           $$ = ASTNode(FACTOR, value=NUMBER)
*                           $$ = ASTNode(FACTOR, value=-NUMBER)
*                           $$ = ASTNode(FACTOR, child=expression) 
*/
ASTNode * Grammar::Factor() {
    //cout<<"Factor, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenType() == TOKEN_ID) {
        string val = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_ID);
        ASTNode* factor_tail = Factor_Tail();
        return new ASTNode(AST_FACTOR, val, factor_tail, NULL, NULL); 
    } else if (parser->GetCurrTokenType() == TOKEN_NUMBER) {
        string val = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_NUMBER);
        return new ASTNode(AST_FACTOR, val, NULL, NULL, NULL); 
    } else if (parser->GetCurrTokenValue() == "-") {
        parser->Consume(TOKEN_SYMBOL);
        if (parser->GetCurrTokenType() == TOKEN_NUMBER) {
            string val = "-"+parser->GetCurrTokenValue();
            parser->Consume(TOKEN_NUMBER);
            return new ASTNode(AST_FACTOR, val, NULL, NULL, NULL); 
        }
    } else if (parser->GetCurrTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode * expression = Expression(); 
        if (expression) {
            if (parser->GetCurrTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                return new ASTNode(AST_FACTOR, "", expression, NULL, NULL);
            }
        }
        delete expression;
    }
    return NULL;
}

/**
* Production:	<factor tail> --> left_bracket <expression> right_bracket  
*									| left_parenthesis <expr list> right_parenthesis 
*									| empty 
*									$$ = ASTNode(ARY_ELEMENT, child=expression)
*									$$ = ASTNode(FUNC_CALL, child=expr list)
*									$$ = ASTNode()
*/
ASTNode * Grammar::Factor_Tail() {
    //cout<<"Factor tail, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "[") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode* expression = Expression();
        if (expression) {
            if (parser->GetCurrTokenValue() == "]") {
                parser->Consume(TOKEN_SYMBOL);
                return new ASTNode(AST_ARRAY_ELEMENT, "", expression, NULL, NULL);
            }
        }
        delete expression;
    } else if (parser->GetCurrTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        ASTNode* expr_list = Expr_List();
        if (expr_list) {
            if (parser->GetCurrTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                return new ASTNode(AST_FUNC_CALL, "", expr_list, NULL, NULL);
            }
        }
        delete expr_list;
    } else {
        return new ASTNode();
    }
    return NULL;
}
