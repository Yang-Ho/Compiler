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
* Production:	<type name> --> int 
*								| void 
*								| binary 
*								| decimal 
 *
 *              $$ = ASTNode(PROGRAM, value=ID, left=typename, right=program prime)
 *              $$ = ASTNode()
*/
StmtNode* Grammar::Program() {
    //cout<<"Program, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "void" ||
            parser->GetCurrTokenValue() == "int" ||
            parser->GetCurrTokenValue() == "binary" ||
            parser->GetCurrTokenValue() == "decimal") {
        string type= parser->GetCurrTokenValue();
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenType() == TOKEN_ID) {
            string id = parser->GetCurrTokenValue();
            parser->Consume(TOKEN_ID);
            return Program_Prime(type, id);
        }
    } else {
        return new ProgramStmt(NULL, NULL);
    }
    return NULL;
}
/**
 * <program prime> --> <data decls prime>
 *                    | <func list prime>
 *                    
 *                    $$ = data decls prime
 *                    $$ = func list prime
*/
StmtNode * Grammar::Program_Prime(string type, string id) {
    //cout<<"Program Prime, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "," || parser->GetCurrTokenValue() == "[" || parser->GetCurrTokenValue() == ";") {
        return Data_Decls_Prime(type, id);
    } else {
        return new ProgramStmt(NULL, Func_List_Prime(type, id));
    }
    return NULL;
}

/**
 * Production:	<func list prime> --> <func decl prime> <func tail> <func list>
 * Production:	<func decl prime> --> left_parenthesis <parameter list> right_parenthesis 
 * Production:	<func tail> -->semicolon 
 *								| left_brace <data decls> <statements> right_brace 
 *
 *                                  $$ = ASTNode(FUNC LIST PRIME, child=func tail, left=func decl prime, right= func list)
 */
vector<StmtNode*> * Grammar::Func_List_Prime(string type, string id) {
    //cout<<"Func List Prime, "<<parser->GetCurrTokenValue()<<endl;
    StmtNode* func_decl;
    if (parser->GetCurrTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        ExprNode* param_list = Parameter_List();
        if (parser->GetCurrTokenValue() == ")") {
            parser->Consume(TOKEN_SYMBOL);
            if (parser->GetCurrTokenValue() == ";") {
                parser->Consume(TOKEN_SYMBOL);
                func_decl = new FuncDeclStmt(type, id, param_list, NULL, NULL);
            } else { 
                if (parser->GetCurrTokenValue() == "{") {
                    parser->Consume(TOKEN_SYMBOL);
                    vector<StmtNode*>* data_decls = Data_Decls(); 
                    if (data_decls) {
                        vector<StmtNode*>* statements = Statements();
                        if (statements) {
                            if (parser->GetCurrTokenValue() == "}") {
                                parser->Consume(TOKEN_SYMBOL);
                                func_decl = new FuncDeclStmt(type, id, param_list, data_decls, statements);
                            }
                        }
                    }
                }
            }
        }
    }
    vector<StmtNode*> * func_list = Func_List();
    func_list->insert(func_list->begin(), func_decl);
    return func_list;
}

/**
* Production:	<data decls prime> -> <id tail> {, <id>} semicolon <data decls prime tail> 
*                                   | empty
* Production:	<id tail> --> left_bracket <expression> right_bracket 
*								| empty
* Production:	<data decls prime tail> --> <typename> ID <data Func Decl> 
*                                           | empty
* Production:	<dataFuncDecl> --> <data decls prime> 
*                               | <func list prime>
* Production:	<type name> --> int 
*								| void 
*								| binary 
*								| decimal 
*
*                                   $$ = ASTNode(DATA DECLS PRIME, child=id tail, child=id list prime, right=data decls prime tail)
*                                   $$ = ASTNode()
*/
StmtNode* Grammar::Data_Decls_Prime(string type, string id) {
    //cout<<"Data Decls Prime, "<<parser->GetCurrTokenValue()<<endl;
    vector<StmtNode*>* data_decls = new vector<StmtNode*>();
    vector<StmtNode*>* func_list = new vector<StmtNode*>();
    ExprNode* id1;
    string type_name = type;
    string id_x = id;
    while (type_name == "void" ||
            type_name == "int" ||
            type_name == "binary" ||
            type_name == "decimal") {
        if (parser->GetCurrTokenValue() == "[") {
            parser->Consume(TOKEN_SYMBOL);
            ExprNode * expression = Expression();
            if (expression) {
                if (parser->GetCurrTokenValue() == "]") {
                    parser->Consume(TOKEN_SYMBOL);
                    id1 = new ArrayRefExpr(id_x, expression);
                }
            }
        } else {
            id1 = new VariableExpr(id_x);
        }
        vector<ExprNode*> * idList = new vector<ExprNode*>();
        idList->push_back(id1);
        parser->IncNumVariables();
        while(parser->GetCurrTokenValue() == ",") {
            parser->Consume(TOKEN_SYMBOL);
            idList->push_back(Id());
        }
        if (parser->GetCurrTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            data_decls->push_back(new DataDeclsStmt(type_name, idList));
            type_name= parser->GetCurrTokenValue();
            if (type_name == "void" ||
                    type_name == "int" ||
                    type_name == "binary" ||
                    type_name == "decimal") {
                parser->Consume(TOKEN_RESERVED);
                if (parser->GetCurrTokenType() == TOKEN_ID) {
                    id_x = parser->GetCurrTokenValue();
                    parser->Consume(TOKEN_ID);
                    if (parser->GetCurrTokenValue() == "(") {
                        func_list = Func_List_Prime(type_name, id_x);
                        type_name = "";
                    }
                }
            }
        }
    }
    return new ProgramStmt(data_decls, func_list);
}

/**
 * Production:	<func list> --> <func> <func list>
 *								| empty
 *
 *								$$ = ASTNode(FUNC LIST, left=func, right=func list)
 *								$$ = ASTNode()
 */
vector<StmtNode *>* Grammar::Func_List() {
    //cout<<"Func_List, "<<parser->GetCurrTokenValue()<<endl;
    vector<StmtNode*>* func_list = new vector<StmtNode*>();
    StmtNode * func = Func();
    while (func) {
        func_list->push_back(func);
        func = Func();
    }
    if (func_list->empty()) {
        return func_list;
    }
    return func_list;
}

/**
 * Production:	<func> --> <func decl> <func tail>
 * Production:	<func decl> --> <typename> ID <func decl prime>
 * Production:	<func decl prime> --> left_parenthesis <parameter list> right_parenthesis 
 * Production:	<func tail> -->semicolon 
 *								| left_brace <data decls> <statements> right_brace 
* Production:	<type name> --> int 
*								| void 
*								| binary 
*								| decimal 
 *      
 *                      $$ = ASTNode(FUNC, left=func decl, right=func tail)
*/
StmtNode * Grammar::Func() {
    //cout<<"Func, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "void" ||
            parser->GetCurrTokenValue() == "int" ||
            parser->GetCurrTokenValue() == "binary" ||
            parser->GetCurrTokenValue() == "decimal") {
        string type= parser->GetCurrTokenValue();
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenType() == TOKEN_ID) {
            string id = parser->GetCurrTokenValue();
            parser->Consume(TOKEN_ID);
            if (parser->GetCurrTokenValue() == "(") {
                parser->Consume(TOKEN_SYMBOL);
                ExprNode* param_list = Parameter_List();
                if (parser->GetCurrTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    if (parser->GetCurrTokenValue() == ";") {
                        parser->Consume(TOKEN_SYMBOL);
                        return new FuncDeclStmt(type, id, param_list, NULL, NULL);
                    } else { 
                        if (parser->GetCurrTokenValue() == "{") {
                            parser->Consume(TOKEN_SYMBOL);
                            vector<StmtNode*>* data_decls = Data_Decls(); 
                            if (data_decls) {
                                vector<StmtNode*>* statements = Statements();
                                if (statements) {
                                    if (parser->GetCurrTokenValue() == "}") {
                                        parser->Consume(TOKEN_SYMBOL);
                                        return new FuncDeclStmt(type, id, param_list, data_decls, statements);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

/**
*/

/**
* Production:	<parameter list> --> void <parameter list tail>
*									| <typename prime> ID <non-empty list prime> 
*									| empty
* Production:	<parameter list tail> --> ID <non-empty list prime> 
*											| empty
* Production:	<non-empty list prime> --> comma <type name> ID <non-empty list prime>
*  											| empty
*
*									$$ = ASTNode(PARAM LIST, value=void, child=parameter list tail)
*									$$ = ASTNode(PARAM LIST, value=ID, left=typename prime, right=non empty list prime)
*									$$ = ASTNode()
*/
ExprNode* Grammar::Parameter_List() {
    //cout<<"Parameter_List, "<<parser->GetCurrTokenValue()<<endl;
    vector<ExprNode *>* param_list = new vector<ExprNode*>();
    if (parser->GetCurrTokenValue() == "void") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenType() == TOKEN_ID) {
            string id = parser->GetCurrTokenValue();
            parser->Consume(TOKEN_ID);
            param_list->push_back(new ParamExpr("void", id));
            if (parser->GetCurrTokenValue() == ",") {
                parser->Consume(TOKEN_SYMBOL);
                while (parser->GetCurrTokenValue() == "void" ||
                        parser->GetCurrTokenValue() == "int" ||
                        parser->GetCurrTokenValue() == "binary" ||
                        parser->GetCurrTokenValue() == "decimal") {
                    string type= parser->GetCurrTokenValue();
                    parser->Consume(TOKEN_RESERVED);
                    if (parser->GetCurrTokenType() == TOKEN_ID) {
                        id = parser->GetCurrTokenValue();
                        parser->Consume(TOKEN_ID);
                        param_list->push_back(new ParamExpr(type, id));
                        if (parser->GetCurrTokenValue() == ",") {
                            parser->Consume(TOKEN_SYMBOL);
                        }
                    } else {
                        return NULL;
                    }
                }
            }
        }
    } else {
        string id = "";
        while (parser->GetCurrTokenValue() == "void" ||
                parser->GetCurrTokenValue() == "int" ||
                parser->GetCurrTokenValue() == "binary" ||
                parser->GetCurrTokenValue() == "decimal") {
                    string type= parser->GetCurrTokenValue();
            parser->Consume(TOKEN_RESERVED);
            if (parser->GetCurrTokenType() == TOKEN_ID) {
                id = parser->GetCurrTokenValue();
                parser->Consume(TOKEN_ID);
                param_list->push_back(new ParamExpr(type, id));
                if (parser->GetCurrTokenValue() == ",") {
                    parser->Consume(TOKEN_SYMBOL);
                }
            } else {
                return NULL;
            }
        }
    }
    if (param_list->empty()) {
        return NULL;
    }
    return new ParamListExpr(param_list);
}

/**
* Production:	<data decls> --> empty 
*								| <type name> <id list> semicolon <data decls>
*
*								$$ = ASTNode()
*								$$ = ASTNode(data decls, child=id list, left=type name, right=data decls) 
*/
vector<StmtNode*>* Grammar::Data_Decls() {
    //cout<<"Data_Decls, "<<parser->GetCurrTokenValue()<<endl;
    vector<StmtNode*> * stmts = new vector<StmtNode*>();
    while (parser->GetCurrTokenValue() == "void" ||
            parser->GetCurrTokenValue() == "int" ||
            parser->GetCurrTokenValue() == "binary" ||
            parser->GetCurrTokenValue() == "decimal") {
        string type = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_RESERVED);
        vector<ExprNode*> * id_list = Id_List();
        if (id_list) {
            if (parser->GetCurrTokenValue() == ";") {
                parser->Consume(TOKEN_SYMBOL);
                stmts->push_back(new DataDeclsStmt(type, id_list));
            }
        }
    }
    if (stmts->empty()) {
        return stmts;
    }
    return stmts;
}

/**
* Production:	<id list> --> <id> {, <id>} 
*                           
*                           $$ = ASTNode(ID LIST, left=id, right=id list prime)
*/
vector<ExprNode*> * Grammar::Id_List() {
    //cout<<"Id_List, "<<parser->GetCurrTokenValue()<<endl;
    vector<ExprNode*> * idList = new vector<ExprNode*>();
    idList->push_back(Id());
    while(parser->GetCurrTokenValue() == ",") {
        parser->Consume(TOKEN_SYMBOL);
        idList->push_back(Id());
    }
    return idList;
}

/**
* Production:	<id> --> ID <id tail>
* Production:	<id tail> --> left_bracket <expression> right_bracket 
*								| empty
*                        
*                        $$ = ASTNode(ID, value=ID, child=id tail)
*/
ExprNode* Grammar::Id() {
    //cout<<"Id, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenType() == TOKEN_ID) {
        string id = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_ID);
        if (parser->GetCurrTokenValue() == "[") {
            parser->Consume(TOKEN_SYMBOL);
            ExprNode * expression = Expression();
            if (expression) {
                if (parser->GetCurrTokenValue() == "]") {
                    parser->Consume(TOKEN_SYMBOL);
                    return new ArrayRefExpr(id, expression);
                }
            }
        } else {
            return new VariableExpr(id);
        }
    }
    return NULL;
}

/**
* Production:	<block statements> --> left_brace <statements> right_brace 
*                                   
*                                   $$ = ASTNode(BLOCK, child=statements)
*/
StmtNode * Grammar::Block_Statements() {
    //cout<<"Block_Statements, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "{") {
        parser->Consume(TOKEN_SYMBOL);
        vector<StmtNode*> * statements = Statements();
        if (statements) {
            //cout<<"In block statements\n";
            if (parser->GetCurrTokenValue() == "}") {
                parser->Consume(TOKEN_SYMBOL);
                return new BlockStmt(statements);
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
vector<StmtNode*>* Grammar::Statements() {
    //cout<<"Statements, "<<parser->GetCurrTokenValue()<<endl;
    vector<StmtNode*> * statements = new vector<StmtNode*>();
    StmtNode * statement = Statement();
    while(statement) {
        statements->push_back(statement);
        statement = Statement();
    }
    if (statements->empty()) {
        return NULL;
    }
    return statements;
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
* Production:	<statement tail> --> <assignment tail> 
*									| <func call tail>
* Production:	<assignment tail> --> <id tail> equal_sign <expression> semicolon 
* Production:	<func call tail> --> left_parenthesis <expr list> right_parenthesis semicolon 
* Production:	<id tail> --> left_bracket <expression> right_bracket 
*								| empty
*/
StmtNode* Grammar::Statement() {
    //cout<<"Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenType() == TOKEN_ID) {
        string id = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_ID);
        if (parser->GetCurrTokenValue() == "(") {
            //cout<<"Statement, function call\n";
            parser->Consume(TOKEN_SYMBOL);
            vector<ExprNode*>* expr_list = Expr_List();
            if (parser->GetCurrTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                if (parser->GetCurrTokenValue() == ";") {
                    parser->Consume(TOKEN_SYMBOL);
                    return new FuncCallStmt(id, expr_list);
                }
            }
        } else {
            //cout<<"Statement, assignment\n";
            if (parser->GetCurrTokenValue() == "[") {
                parser->Consume(TOKEN_SYMBOL);
                ExprNode * index = Expression();
                if (parser->GetCurrTokenValue() == "]") {
                    parser->Consume(TOKEN_SYMBOL);
                    if (parser->GetCurrTokenValue() == "=") {
                        parser->Consume(TOKEN_SYMBOL);
                        ExprNode * expression = Expression();
                        if (parser->GetCurrTokenValue() == ";") {
                            parser->Consume(TOKEN_SYMBOL);
                            ExprNode * ary = new ArrayRefExpr(id, index); 
                            return new AssignmentStmt(ary, expression);
                        }
                    }
                }
            } else {
                if (parser->GetCurrTokenValue() == "=") {
                    parser->Consume(TOKEN_SYMBOL);
                    ExprNode * expression = Expression();
                    if (parser->GetCurrTokenValue() == ";") {
                        parser->Consume(TOKEN_SYMBOL);
                        ExprNode * lhs = new VariableExpr(id); 
                        return new AssignmentStmt(lhs, expression);
                    }
                }
            }
        }
    } else {
        StmtNode * if_statement = If_Statement();
        if (if_statement) {
            return if_statement;
        } else {
            StmtNode * while_statement = While_Statement();
            if (while_statement) {
                return while_statement;
            } else {
                StmtNode* return_statement = Return_Statement();
                if (return_statement) {
                    return return_statement;
                } else {
                    StmtNode * break_statement = Break_Statement();
                    if (break_statement) {
                        return break_statement;
                    } else {
                        StmtNode * continue_statement = Continue_Statement();
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
                                            return new ReadStmt(id);
                                        }
                                    }
                                }
                            }
                        } else if (parser->GetCurrTokenValue() == "write") {
                            //cout<<"write\n";
                            parser->Consume(TOKEN_RESERVED);
                            if (parser->GetCurrTokenValue() == "(") {
                                parser->Consume(TOKEN_SYMBOL);
                                ExprNode * expression = Expression();
                                if (expression) {
                                    if (parser->GetCurrTokenValue() == ")") {
                                        parser->Consume(TOKEN_SYMBOL);
                                        if (parser->GetCurrTokenValue() == ";") {
                                            parser->Consume(TOKEN_SYMBOL);
                                            return new WriteStmt(expression);
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
                                            return new PrintStmt(val);
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
* Production:	<expr list> --> <non-empty expr list> 
*  								| empty
*
*  								$$ = ASTNode(EXPR LIST, child=non-empty expr list)
*  								$$ = ASTNode()
*/
vector<ExprNode*> * Grammar::Expr_List() {
    //cout<<"Expr_List, "<<parser->GetCurrTokenValue()<<endl;
    vector<ExprNode *>* non_empty_expr_list = Non_Empty_Expr_List();
    if (!non_empty_expr_list->empty()) {
        return non_empty_expr_list;
    } else {
        //vector<ExprNode *>::iterator it;
        //for (it = non_empty_expr_list->begin(); it != non_empty_expr_list->end(); ++it) {
         //   delete (*it);
        //}
        delete non_empty_expr_list;
        return NULL;
    }
}

/**
* Production:	<non-empty expr list> --> <expression> {, <expression>}
*
*                                       $$ = ASTNode(NON EMPTY EXPR LIST, left=expression, right=non-empty expr list prime)
*/
vector<ExprNode *> * Grammar::Non_Empty_Expr_List() {
    //cout<<"Non_Empty_Expr_List, "<<parser->GetCurrTokenValue()<<endl;
    vector<ExprNode *> *expressions = new vector<ExprNode *>();
    expressions->push_back(Expression());
    while(parser->GetCurrTokenValue() == ",") {
        parser->Consume(TOKEN_SYMBOL);
        expressions->push_back(Expression());
    }
    return expressions;
}

/**
* Production:	<if statement> --> if left_parenthesis <condition expression> right_parenthesis <block statements> 
*           
*                               $$ = ASTNode(IF_STATMENT, left=condition expression, right=block statements)
*/
StmtNode * Grammar::If_Statement() {
    //cout<<"If_Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "if") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            ExprNode * condition_expression = Condition_Expression();
            if (condition_expression) {
                if (parser->GetCurrTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    StmtNode* block_statements = Block_Statements();
                    if (block_statements) {
                        return new IfStmt(condition_expression, block_statements);
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
* Production:	<condition expression> -->  <condition> {<condition op> <condition>} 
*               <condition op> --> double_and_sign 
*								| double_or_sign
*                                       $$ = ASTNode(CONDTION EXPR, left=condition, right=condition expression tail)
*/
ExprNode* Grammar::Condition_Expression() {
    //cout<<"Condition_Expression, "<<parser->GetCurrTokenValue()<<endl;

    ExprNode* condition = Condition();
    while (parser->GetCurrTokenValue() == "&&" || parser->GetCurrTokenValue() == "||") {
        string cond_op = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_SYMBOL);
        ExprNode * condition1 = Condition();
        condition = new ConditionExpr(cond_op, condition, condition1);
    }
    return condition;
}

/**
* Production:	<condition> --> <expression> <comparison op> <expression> 
* 	            <comparison op> --> == 
*									| != 
*									| > 
*									| >= 
*									| < 
*									| <=
*           
*                           $$ = ASTNode(CONDITION, value=comparison op, left=expression1, right=expression2)
*/
ExprNode * Grammar::Condition() {
    //cout<<"Condition, "<<parser->GetCurrTokenValue()<<endl;
    ExprNode* expression1 = Expression();
    if (expression1) {
        if (parser->GetCurrTokenValue() == "==" 
                || parser->GetCurrTokenValue() == "!="
                || parser->GetCurrTokenValue() == ">"
                || parser->GetCurrTokenValue() == ">="
                || parser->GetCurrTokenValue() == "<"
                || parser->GetCurrTokenValue() == "<=") {
            string comp_op = parser->GetCurrTokenValue();
            parser->Consume(TOKEN_SYMBOL);
            ExprNode * expression2 = Expression();
            if (expression2) {
                return new ConditionExpr(comp_op, expression1, expression2);
            }
            delete expression2;
        }
    }
    delete expression1;
    return NULL;
}

/**
* Production:	<while statement> --> while left_parenthesis <condition expression> right_parenthesis <block statements> 
*                                   
*                                   $$ = ASTNode(WHILE, left=condition expression, right=block statements)
*/
StmtNode * Grammar::While_Statement() {
    //cout<<"While_Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "while") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            ExprNode * condition_expression = Condition_Expression();
            if (condition_expression) {
                if (parser->GetCurrTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    StmtNode * block_statements = Block_Statements();
                    if (block_statements) {
                        return new WhileStmt(condition_expression, block_statements);
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
StmtNode* Grammar::Return_Statement() {
    //cout<<"Return_Statement, "<<parser->GetCurrTokenValue()<<endl;

    if (parser->GetCurrTokenValue() == "return") {
        parser->Consume(TOKEN_RESERVED);
        StmtNode * return_statement_tail = Return_Statement_Tail();
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
StmtNode * Grammar::Return_Statement_Tail() {
    //cout<<"Return_Statement tail, "<<parser->GetCurrTokenValue()<<endl;

    ExprNode * expression = Expression();
    if (expression) {
        if (parser->GetCurrTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            return new ReturnStmt(expression);
        }
    } else if (parser->GetCurrTokenValue() == ";") {
        parser->Consume(TOKEN_SYMBOL);
        return new ReturnStmt(expression);
    }
    delete expression;
    return NULL;
}

/**
* Production:	<break statement> ---> break semicolon 
*                                       $$ = ASTNode(break)
*/
StmtNode* Grammar::Break_Statement() {
    //cout<<"Break_Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "break") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            return new BreakStmt();
        }
    }
    return NULL;
}

/** 
* Production:	<continue statement> ---> continue semicolon
*                                       $$ = ASTNode(CONTINUE)
*/
StmtNode * Grammar::Continue_Statement() {
    //cout<<"Continue_Statement, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenValue() == "continue") {
        parser->Consume(TOKEN_RESERVED);
        if (parser->GetCurrTokenValue() == ";") {
            parser->Consume(TOKEN_SYMBOL);
            return new ContinueStmt();
        }
    }
    return NULL;
}

/**
* Production:	<expression> --> <term> {<addop> <term>}
*               <addop>      --> +
*                               | -
*                         $$ = ASTNode(EXPRESSION, left=term, right=expression prime)
*/
ExprNode * Grammar::Expression() {
    //cout<<"Expression, "<<parser->GetCurrTokenValue()<<endl;

    ExprNode* term = Term();
    string op = "";
    while(parser->GetCurrTokenValue() == "+" || parser->GetCurrTokenValue() == "-") {
        op = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_SYMBOL);
        ExprNode* term1= Term();
        term = new BinaryExpr(op, term, term1);
    }
    return term;
}

/**
* Production:	<term> --> <factor> {<mulop> <factor>}
*               <addop>      --> *
*                               | /
*                         $$ = ASTNode(TERM, left=factor, right=term prime)
*/
ExprNode * Grammar::Term() {
    //cout<<"Term, "<<parser->GetCurrTokenValue()<<endl;
    ExprNode * factor = Factor();
    string op = "";
    while(parser->GetCurrTokenValue() == "*" || parser->GetCurrTokenValue() == "/") {
        op = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_SYMBOL);
        ExprNode * factor1 = Factor();
        factor = new BinaryExpr(op, factor, factor1);
    }
    return factor;
}

/**
* Production:	<factor> --> ID <factor tail> 
*							| NUMBER 
*							| minus_sign NUMBER 
*							| left_parenthesis <expression> right_parenthesis
*               <factor tail> --> left_bracket <expression> right_bracket  
*			    			| left_parenthesis <expr list> right_parenthesis 
*							| empty 
*
*                           $$ = ASTNode(FACTOR, value=ID, child=factor_tail)
*                           $$ = ASTNode(FACTOR, value=NUMBER)
*                           $$ = ASTNode(FACTOR, value=-NUMBER)
*                           $$ = ASTNode(FACTOR, child=expression) 
*/
ExprNode * Grammar::Factor() {
    //cout<<"Factor, "<<parser->GetCurrTokenValue()<<endl;
    if (parser->GetCurrTokenType() == TOKEN_ID) {
        string val = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_ID);
        if (parser->GetCurrTokenValue() == "[") {
            parser->Consume(TOKEN_SYMBOL);
            ExprNode * expression = Expression();
            if (expression) {
                if (parser->GetCurrTokenValue() == "]") {
                    parser->Consume(TOKEN_SYMBOL);
                    return new ArrayRefExpr(val, expression);
                }
            }
            delete expression;
        } else if (parser->GetCurrTokenValue() == "(") {
            parser->Consume(TOKEN_SYMBOL);
            vector<ExprNode *> *expr_list = Expr_List();
            if (expr_list) {
                if (parser->GetCurrTokenValue() == ")") {
                    parser->Consume(TOKEN_SYMBOL);
                    return new FuncCallExpr(val, expr_list);
                }
            }
            delete expr_list;
        } else {
            return new VariableExpr(val);
        }
    } else if (parser->GetCurrTokenType() == TOKEN_NUMBER) {
        string val = parser->GetCurrTokenValue();
        parser->Consume(TOKEN_NUMBER);
        return new NumberExpr(val); 
    } else if (parser->GetCurrTokenValue() == "-") {
        parser->Consume(TOKEN_SYMBOL);
        if (parser->GetCurrTokenType() == TOKEN_NUMBER) {
            string val = "-"+parser->GetCurrTokenValue();
            parser->Consume(TOKEN_NUMBER);
            return new NumberExpr(val); 
        }
    } else if (parser->GetCurrTokenValue() == "(") {
        parser->Consume(TOKEN_SYMBOL);
        ExprNode * expression = Expression(); 
        if (expression) {
            if (parser->GetCurrTokenValue() == ")") {
                parser->Consume(TOKEN_SYMBOL);
                return new ParenExpr(expression);
            }
        }
        delete expression;
    }
    return NULL;
}
