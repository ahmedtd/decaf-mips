/* File: parser.y
 * --------------
 * Yacc input file to generate the parser for the compiler.
 *
 * pp2: your job is to write a parser that will construct the parse tree
 *      and if no parse errors were found, print it.  The parser should 
 *      accept the language as described in specification, and as augmented 
 *      in the pp2 handout.
 */

%{

/* Just like lex, the text within this first region delimited by %{ and %}
 * is assumed to be C/C++ code and will be copied verbatim to the y.tab.c
 * file ahead of the definitions of the yyparse() function. Add other header
 * file inclusions or C++ variable declarations/prototypes that are needed
 * by your code here.
 */
#include "scanner.h" // for yylex
#include "parser.h"
#include "errors.h"

#include "ast.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "ast_decl.h"

void yyerror(const char *msg); // standard error-handling routine

%}

/* The section before the first %% is the Definitions section of the yacc
 * input file. Here is where you declare tokens and types, add precedence
 * and associativity options, and so on.
 */
 
/* yylval 
 * ------
 * Here we define the type of the yylval global variable that is used by
 * the scanner to store attibute information about the token just scanned
 * and thus communicate that information to the parser. 
 *
 * pp2: You will need to add new fields to this union as you add different 
 *      attributes to your non-terminal symbols.
 */
%union {
    int integerConstant;
    bool boolConstant;
    char *stringConstant;
    double doubleConstant;
    //                  char identifier[MaxIdentLen+1]; // +1 for terminating null
    
    Type *type;

    Identifier *ident;
    List<Identifier*> *ident_list;

    Decl *decl;
    List<Decl*> *decl_list;
    
    VarDecl *vardecl;
    List<VarDecl*> *vardecl_list;

    Stmt *stmt;
    List<Stmt*> *stmt_list;

    FnDecl *fndecl;
    List<FnDecl*> *fndecl_list;

    InterfaceDecl *ifacedecl;

    ClassDecl *classdecl;

    NamedType *namedtype;
    List<NamedType*> *namedtype_list;

    Expr *expr;
    List<Expr*> *expr_list;

    Call *call;

    LValue *lvalue;

    Operator *op;

    StmtBlock *stmtblock;

    PrintStmt *printstmt;
    BreakStmt *breakstmt;
    ReturnStmt *returnstmt;
    IfStmt     *ifstmt;
    ForStmt    *forstmt;
    WhileStmt  *whilestmt;

    CaseFragment *casefrag;
    List<CaseFragment*> *casefrag_list;
    
    SwitchStmt *switchstmt;
}


/* Tokens
 * ------
 * Here we tell yacc about all the token types that we are using.
 * Yacc will assign unique numbers to these and export the #define
 * in the generated y.tab.h header file.
 */
%token T_Void T_Bool T_Int T_Double T_String T_Class 
%token T_Dims
%token T_Null T_Extends T_This T_Interface T_Implements
%token T_While T_For T_If T_Else T_Return T_Break
%token T_New T_NewArray T_Print T_ReadInteger T_ReadLine
%token T_Switch T_Case T_Default

%token <ident>           T_Identifier
%token <stringConstant>  T_StringConstant 
%token <integerConstant> T_IntConstant
%token <doubleConstant>  T_DoubleConstant
%token <boolConstant>    T_BoolConstant

/* Non-terminal types
 * ------------------
 * In order for yacc to assign/access the correct field of $$, $1, we
 * must to declare which field is appropriate for the non-terminal.
 * As an example, this first type declaration establishes that the DeclList
 * non-terminal uses the field named "declList" in the yylval union. This
 * means that when we are setting $$ for a reduction for DeclList ore reading
 * $n which corresponds to a DeclList nonterminal we are accessing the field
 * of the union named "declList" which is of type List<Decl*>.
 * pp2: You'll need to add many of these of your own.
 */
%type <decl_list>  DeclList 
%type <decl>      Decl


%type <vardecl> VariableDecl
%type <vardecl> Variable
%type <vardecl_list> VariableDeclList

%type <fndecl> FunctionDecl

%type <fndecl> Prototype
%type <decl_list> PrototypeList

%type <decl> ClassDecl

%type <decl> InterfaceDecl

%type <type> Type

%type <vardecl_list> Formals

%type <stmt> Stmt
%type <stmt_list> StmtList

%type <decl> Field
%type <decl_list> FieldList

%type <namedtype> NamedType
%type <namedtype_list> NamedTypeList

%type <expr> Constant

%type <expr_list> Actuals

%type <call> Call

%type <lvalue> LValue

%type <expr> Expr

%type <expr_list> ExprList

%type <ifstmt>     IfStmt
%type <whilestmt>  WhileStmt
%type <forstmt>    ForStmt
%type <breakstmt>  BreakStmt
%type <returnstmt> ReturnStmt
%type <printstmt>  PrintStmt
%type <stmtblock>  StmtBlock

%type <casefrag> CaseFragment
%type <casefrag_list> CaseList
%type <switchstmt> SwitchStmt
%type <expr> CaseLabel

%left <op> '='
%left <op> T_Or
%left <op> T_And
%left <op> T_Equal T_NotEqual
%left <op> '<' T_LessEqual '>' T_GreaterEqual
%left <op> '+' '-'
%left <op> '*' '/' '%'
%left <op> '!' DUMMY_NEG T_Incr T_Decr

%%
/* Rules
 * -----
 * All productions and actions should be placed between the start and stop
 * %% markers which delimit the Rules section.
	 
 */
Program:        DeclList            { 
                                      @1; 
                                      /* pp2: The @1 is needed to convince 
                                       * yacc to set up yylloc. You can remove 
                                       * it once you have other uses of @n*/
                                      Program *program = new Program($1);
                                      // if no errors, advance to next phase
                                      if (ReportError::NumErrors() == 0) 
                                          program->Print(0);
                                    }
                ;

DeclList:       Decl                 { ($$ = new List<Decl*>)->Append($1); }
        |       DeclList Decl        { ($$=$1)->Append($2); }
                ;

Decl:           FunctionDecl  { $$ = $1; }
        |       VariableDecl  { $$ = $1; }
        |       ClassDecl     { $$ = $1; }
        |       InterfaceDecl { $$ = $1; }
                ;  
          

VariableDecl:   Variable ';' { $$ = $1; }
                ;

Variable:       Type T_Identifier { $$ = new VarDecl($2, $1); }
                ;

Type:           T_Int         { $$ = Type::intType; }
        |       T_Double      { $$ = Type::doubleType; }
        |       T_Bool        { $$ = Type::boolType; }
        |       T_String      { $$ = Type::stringType; }
        |       T_Identifier  { $$ = new NamedType($1); }
        |       Type T_Dims   { $$ = new ArrayType(@$, $1); }
                ;

FunctionDecl:   Type T_Identifier '(' Formals ')' StmtBlock
                { 
                  auto retval = new FnDecl($2, $1, $4);
                  retval->SetFunctionBody($6);
                  $$ = retval;
                }
        |       T_Void T_Identifier '(' Formals ')' StmtBlock
                {
                    auto retval = new FnDecl($2, Type::voidType, $4);
                    retval->SetFunctionBody($6);
                    $$ = retval;
                }
                ;

Formals:        /*epsilon*/          { ($$ = new List<VarDecl*>); }
        |       Variable             { ($$ = new List<VarDecl*>)->Append($1); }
        |       Formals ',' Variable { ($$ = $1)->Append($3); }
                ;

StmtBlock:      '{' '}'
                { $$ = new StmtBlock(new List<VarDecl*>(), new List<Stmt*>()); }

        |       '{'   StmtList '}'
                { $$ = new StmtBlock(new List<VarDecl*>(), $2); }

        |       '{' VariableDeclList '}'
                { $$ = new StmtBlock($2, new List<Stmt*>()); }

        |       '{' VariableDeclList StmtList '}'
                { $$ = new StmtBlock($2, $3); }
        ;

VariableDeclList:
                VariableDecl { ($$ = new List<VarDecl*>())->Append($1); }
        |       VariableDeclList VariableDecl { ($$ = $1)->Append($2); }
        ;

StmtList:       Stmt { ($$ = new List<Stmt*>())->Append($1); }
        |       StmtList Stmt { ($$ = $1)->Append($2); }
        ;

Stmt:           ';'        { $$ = new EmptyExpr(); }    
        |       Expr ';'   { $$ = $1; }
        |       IfStmt     { $$ = $1; }
        |       WhileStmt  { $$ = $1; }
        |       ForStmt    { $$ = $1; }
        |       BreakStmt  { $$ = $1; }
        |       ReturnStmt { $$ = $1; }
        |       PrintStmt  { $$ = $1; }
        |       StmtBlock  { $$ = $1; }
        |       SwitchStmt { $$ = $1; }
        ;

SwitchStmt:     T_Switch '(' Expr ')'
                '{' CaseList '}'
                {
                    $$ = new SwitchStmt($3, $6, new DefaultFragment(new List<Stmt*>()));
                }

        |       T_Switch '(' Expr ')'
                '{' CaseList T_Default ':' '}'
                {
                    $$ = new SwitchStmt($3, $6, new DefaultFragment(new List<Stmt*>()));
                }

        |       T_Switch '(' Expr ')'
                '{' CaseList T_Default ':' StmtList '}'
                {
                    $$ = new SwitchStmt($3, $6, new DefaultFragment($9));
                }
                ;

CaseList:       CaseFragment { ($$ = new List<CaseFragment*>())->Append($1); }
        |       CaseList CaseFragment { ($$=$1)->Append($2); }
        ;

CaseFragment:   CaseLabel { $$ = new CaseFragment($1, new List<Stmt*>()); }
        |       CaseLabel StmtList { $$ = new CaseFragment($1, $2); }
        ;

CaseLabel:      T_Case T_IntConstant ':' { $$ = new IntConstant(@$, $2); }
        ;

IfStmt:         T_If '(' Expr ')' Stmt T_Else Stmt
                { $$ = new IfStmt($3, $5, $7); }
        
        |       T_If '(' Expr ')' Stmt
                { $$ = new IfStmt($3, $5, nullptr); }
        ;

WhileStmt:      T_While '(' Expr ')' Stmt
                { $$ = new WhileStmt($3, $5); }
        ;

ForStmt:        T_For '(' Expr ';' Expr ';' Expr ')' Stmt
                { $$ = new ForStmt($3, $5, $7, $9); }

        |       T_For '(' ';' Expr ';' Expr ')' Stmt
                { $$ = new ForStmt(new EmptyExpr(), $4, $6, $8); }

        |       T_For '(' Expr ';' Expr ';' ')' Stmt
                { $$ = new ForStmt($3, $5, new EmptyExpr(), $8); }
        ;

ReturnStmt:     T_Return Expr ';' { $$ = new ReturnStmt(@$, $2); }
        |       T_Return ';' { $$ = new ReturnStmt(@$, new EmptyExpr()); }
        ;

BreakStmt:      T_Break ';' { $$ = new BreakStmt(@$); }
        ;

PrintStmt:      T_Print '(' ExprList ')' ';' { $$ = new PrintStmt($3); }
        ;

ExprList:       Expr              { ($$ = new List<Expr*>())->Append($1); }
        |       ExprList ',' Expr { ($$ = $1)->Append($3); }
        ;

Expr:           LValue '=' Expr
                { $$ = new AssignExpr($1, $2, $3); }

        |       Constant
                { $$ = $1; }

        |       LValue
                { $$ = $1; }
                
        |       T_This
                { $$ = new This(@$); }

        |       Call
                { $$ = $1; }

        |       '(' Expr ')'
                { $$ = $2; }

        |       Expr T_Incr
                { $$ = new PostfixExpr($1, $2); }

        |       Expr T_Decr
                { $$ = new PostfixExpr($1, $2); }

        |       Expr '+' Expr
                { $$ = new ArithmeticExpr($1, $2, $3); }

        |       Expr '-' Expr
                { $$ = new ArithmeticExpr($1, $2, $3); }
                
        |       Expr '*' Expr
                { $$ = new ArithmeticExpr($1, $2, $3); }
                
        |       Expr '/' Expr
                { $$ = new ArithmeticExpr($1, $2, $3); }

        |       Expr '%' Expr
                { $$ = new ArithmeticExpr($1, $2, $3); }

        |       '-' Expr %prec DUMMY_NEG
                { $$ = new ArithmeticExpr($1, $2); }

        |       Expr '<' Expr
                { $$ = new RelationalExpr($1, $2, $3); } 
                
        |       Expr T_LessEqual Expr
                { $$ = new RelationalExpr($1, $2, $3); }
                
        |       Expr '>' Expr
                { $$ = new RelationalExpr($1, $2, $3); }

        |       Expr T_GreaterEqual Expr
                { $$ = new RelationalExpr($1, $2, $3); }
                
        |       Expr T_Equal Expr
                { $$ = new EqualityExpr($1, $2, $3); }                

        |       Expr T_NotEqual Expr
                { $$ = new EqualityExpr($1, $2, $3); }       

        |       Expr T_And Expr
                { $$ = new LogicalExpr($1, $2, $3); }

        |       Expr T_Or Expr
                { $$ = new LogicalExpr($1, $2, $3); }

        |       '!' Expr
                { $$ = new LogicalExpr($1, $2); }

        |       T_ReadInteger '(' ')'
                { $$ = new ReadIntegerExpr(@$); }

        |       T_ReadLine '(' ')'
                { $$ = new ReadLineExpr(@$); }

        |       T_New '(' NamedType ')'
                { $$ = new NewExpr(@$, $3); }

        |       T_NewArray '(' Expr ',' Type ')'
                { $$ = new NewArrayExpr(@$, $3, $5); }
        ;

LValue:         T_Identifier { $$ = new FieldAccess(nullptr, $1); }
        |       Expr '.' T_Identifier { $$ = new FieldAccess($1, $3); }
        |       Expr '[' Expr ']' { $$ = new ArrayAccess(@$, $1, $3); }
        ;

Call:           T_Identifier '(' Actuals ')'
                { $$ = new Call(@$, nullptr, $1, $3); }
        
        |       Expr '.' T_Identifier '(' Actuals ')'
                { $$ = new Call(@$, $1, $3, $5); }
        ;

Actuals:        /* epsilon */ { $$ = new List<Expr*>(); }
        |       ExprList      { $$ = $1; }
        ;

Constant:       T_IntConstant    { $$ = new IntConstant(@$, $1);    }
        |       T_DoubleConstant { $$ = new DoubleConstant(@$, $1); }
        |       T_BoolConstant   { $$ = new BoolConstant(@$, $1);   }
        |       T_StringConstant { $$ = new StringConstant(@$, $1); }
        |       T_Null           { $$ = new NullConstant(@$);       }
        ;

ClassDecl:      T_Class T_Identifier
                T_Extends NamedType
                T_Implements NamedTypeList
                '{' FieldList '}'
                {
                    $$ = new ClassDecl($2, $4, $6, $8);
                }
        
        |       T_Class T_Identifier
                T_Implements NamedTypeList
                '{' FieldList '}'
                {
                    $$ = new ClassDecl($2, nullptr, $4, $6);
                }

        |       T_Class T_Identifier
                T_Extends NamedType
                '{' FieldList '}'
                {
                    $$ = new ClassDecl($2, $4, new List<NamedType*>(), $6);
                }

        |       T_Class T_Identifier
                '{' FieldList '}'
                {
                    $$ = new ClassDecl($2, nullptr, new List<NamedType*>(), $4);
                }
        ;

NamedType:      T_Identifier { $$ = new NamedType($1); };
        ;

NamedTypeList:  NamedType { ($$ = new List<NamedType*>())->Append($1); }
        |       NamedTypeList ',' NamedType { ($$ = $1)->Append($3); }
        ;



FieldList:      /*epsilon */   { ($$ = new List<Decl*>()); }
        |       Field           { ($$ = new List<Decl*>())->Append($1); }
        |       FieldList Field { ($$ = $1)->Append($2); }
        ;

Field:          VariableDecl { $$ = $1; }
        |       FunctionDecl { $$ = $1; }
        ;

InterfaceDecl:  T_Interface T_Identifier '{' PrototypeList '}'
                {
                  $$ = new InterfaceDecl($2, $4);
                }
        ;

PrototypeList:  /* epsilon */ { $$ = new List<Decl*>(); }
        |       Prototype     { ($$ = new List<Decl*>())->Append($1); }
        |       PrototypeList Prototype { ($$ = $1)->Append($2); }
        ;

Prototype:      Type T_Identifier '(' Formals ')' ';'
                {
                    $$ = new FnDecl($2, $1, $4);
                }
        |       T_Void T_Identifier '(' Formals ')' ';'
                {
                    $$ = new FnDecl($2, Type::voidType, $4);
                }
        ;

%%

/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */

/* Function: InitParser
 * --------------------
 * This function will be called before any calls to yyparse().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the parser (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yydebug that controls whether yacc prints debugging information about
 * parser actions (shift/reduce) and contents of state stack during parser.
 * If set to false, no information is printed. Setting it to true will give
 * you a running trail that might be helpful when debugging your parser.
 * Please be sure the variable is set to false when submitting your final
 * version.
 */
void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}
