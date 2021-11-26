%{
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "wrdcNode.hpp"
using namespace std;

int yylex();
File* parsed = 0;
extern int yylineno;
extern char* yytext;
Node::Color Node::over = Node::NONE;
namespace wrd {
    const char* getName(int type);
}

void yyerror(const char* s)
{
    cout << Node::clr(Node::RED) << "error " << yylineno << Node::clr(Node::WHITE) << ": " << s << ", " << yytext << "\n";
    //asm("int3");
}
%}

%glr-parser
%union {
    int typeId;
    int intVal;
    float floatVal;
    bool boolVal;
    char charVal;
    const char* strVal;
    Node* node;
    void* nodes;
}

%verbose
%start tfile

%token tpack tswitch tas tfor tdef twith tret treturn tif telse telif tfrom tnext tprop timport taka tthis tnode tin tindent tdedent tnull

%token <intVal> teof
%token <floatVal> tnum
%token <boolVal> tbool
%token <charVal> tchar
%token <strVal> tstr tfctor tfdtor tfget tfset tfres

%token <strVal> tnormalId taccessedId tconName
%type <strVal> tid

%token teol topDefAssign topMinusAssign topSquareAssign topDivideAssign topModAssign topPowAssign topLessEqual topMoreEqual topEqual topRefEqual topNotEqual topNotRefEqual topPlusAssign topSeq topSafeNavi topUplus topUminus

%type <node> tblock tindentBlock
%type <node> texpr tcast tfile tfuncCall telseBlock telifBlock tbranch termIf termFor tseq tarray ttype tmap taccess tconAccess treturnexpr ttuple ttuples tparam tparams tsafeAccess tconNames


%type <node> tfuncRhsList trhsIdExpr tlhsId trhsIds trhslist

%type <node> timportExpr tpackExpr tfileExpr tpackAccess

%type <node> tfunc tctorfunc tdtorfunc tfunclist tfuncBody

%type <node> tdefOrigin tdefIndentBlock tdefexpr tdefBlock tdefBlockExpr

%type <nodes> telifBlocks

%type <node> takaStmt tcaseIndentBlock tswitchExpr tcaseExpr tcaseStmt

%type <node> tgetsetterStmt tgetsetterExpr tpropexpr tpropIndentBlock tpropBlock tgetsetList tgetsetFuncName

// 우선순위: 밑으로 갈수록 높음.
//  결합 순서 정의:
//      정해주지 않으면 모호성 오류 나온다.
//      left: 왼쪽 결합.
//      right: 오른쪽 결합.
%right '=' topPlusAssign topDivideAssign topModAssign topPowAssign topSquareAssign topMinusAssign topDefAssign
%left '&' '|'
%left '<' '>' topMoreEqual topLessEqual topEqual topRefEqual topNotEqual topNotRefEqual
%left '%' '^'
%left '+' '-'
%left '*' '/'
%left '.'

%%

tid         : tnormalId { $$ = $1; }
            | taccessedId { $$ = $1; }
            ;

telseBlock  : teol telse tindentBlock {
                $$ = $3;
            }

telifBlock  : teol telif tbranch {
                $$ = $3;
            }
telifBlocks : telifBlock {
                vector<Branch*>* ret = new vector<Branch*>;
                ret->push_back((Branch*) $1);
                $$ = ret;
            }
            | telifBlocks telifBlock {
                vector<Branch*>* blk = (vector<Branch*>*) $1;
                blk->push_back((Branch*) $2);
                $$ = $1;
            }

tbranch     : trhsIdExpr tindentBlock {
                $$ = new Branch($1, (Container*) $2);
            }

termFor     : tfor tid tin trhsIdExpr tindentBlock {
                $$ = new For(new Id($2), $4, (Container*) $5);
            }
            | tfor trhsIdExpr tindentBlock {
                $$ = new For($2, (Container*) $3);
            }
            ;

tcaseExpr   : trhsIdExpr {
                Block* ret = new Block();
                if ($1)
                    ret->add($1);
                $$ = ret;
            }
            | tcaseExpr ',' trhsIdExpr {
                Block* ret = (Block*) $$;
                if ($3)
                    ret->add($3);
                $$ = ret;
            }
            ;

tcaseStmt   : tcaseExpr tindentBlock { $$ = new CaseBlock($1, $2); }
            | telse tindentBlock { $$ = new CaseBlock(new Str("else"), $2); }
            ;

tcaseIndentBlock : tcaseStmt {
                Block* ret = new Block();
                if ($1)
                    ret->add($1);
                $$ = ret;
            }
            | tcaseIndentBlock tcaseStmt {
                Block* ret = (Block*) $1;
                if ($2)
                    ret->add($2);
                $$ = ret;
            }
            ;

tswitchExpr : tswitch tlhsId teol tindent tcaseIndentBlock teol tdedent {
                $$ = new SwitchExpr($2, $5);
            }
            ;

termIf      : tif tbranch telifBlocks telseBlock {
                $$ = new If((Branch*) $2, (vector<Branch*>*) $3, (Block*) $4);
            }
            | tif tbranch telifBlocks {
                $$ = new If((Branch*) $2, (vector<Branch*>*) $3, 0);
            }
            | tif tbranch telseBlock {
                $$ = new If((Branch*) $2, (Block*) $3);
            }
            | tif tbranch {
                $$ = new If((Branch*) $2);
            }
            ;

// trhsexpr과 tlhsexpr:
//  tlhsexpr은 할당이 가능한 변수. lvalue.
//  trhsexpr은 값을 나타내는 모든 표현식.
//  따라서 범주상으로 보았을때 trhsexpr 은 tlhsexpr을 포함한다. 더 크다는 얘기다.
trhsIdExpr  : tbool { $$ = new Bool($1); }
            | tnum { $$ = new Float($1); }
            | tstr { $$ = new Str($1); }
            | tchar { $$ = new Char($1); }
            | tswitchExpr { $$ = $1; }
            | tarray { $$ = $1; }
            | tnormalId { $$ = new Str($1); }
            | tmap { $$ = $1; }
            | tdefexpr { $$ = $1; }
            | tcast %dprec 1 { $$ = $1; }
            | tsafeAccess %dprec 1 { $$ = $1; }
            | taccess %dprec 1 { $$ = $1; }
            | '(' trhsIdExpr ')' { $$ = $2; }

            | topUplus trhsIdExpr %dprec 1 { $$ = new UPre(new Id("++"), $2); }
            | trhsIdExpr topUplus %dprec 1 { $$ = new UPost(new Id("++"), $1); }
            | topUminus trhsIdExpr %dprec 1 { $$ = new UPre(new Id("--"), $2); }
            | trhsIdExpr topUminus %dprec 1 { $$ = new UPost(new Id("--"), $1); }

            | tconAccess %dprec 1 { $$ = $1; }

            | trhsIdExpr '+' trhsIdExpr %dprec 2 { $$ = new Plus($1, $3); }
            | trhsIdExpr '-' trhsIdExpr %dprec 2 { $$ = new Minus($1, $3); }
            | trhsIdExpr '*' trhsIdExpr %dprec 2 { $$ = new Square($1, $3); }
            | trhsIdExpr '/' trhsIdExpr %dprec 2 { $$ = new Divide($1, $3); }
            | trhsIdExpr '%' trhsIdExpr %dprec 2 { $$ = new Moduler($1, $3); }
            | trhsIdExpr '^' trhsIdExpr %dprec 2 { $$ = new Power($1, $3); }
            | trhsIdExpr '<' trhsIdExpr %dprec 2 { $$ = new Less($1, $3); }
            | trhsIdExpr topLessEqual trhsIdExpr %dprec 2 { $$ = new LessEqual($1, $3); }
            | trhsIdExpr '>' trhsIdExpr %dprec 2 { $$ = new More($1, $3); }
            | trhsIdExpr topMoreEqual trhsIdExpr %dprec 2 { $$ = new MoreEqual($1, $3); }
            | trhsIdExpr topEqual trhsIdExpr %dprec 2 { $$ = new Equal($1, $3); }
            | trhsIdExpr topRefEqual trhsIdExpr %dprec 2 { $$ = new RefEqual($1, $3); }
            | trhsIdExpr topNotEqual trhsIdExpr %dprec 2 { $$ = new NotEqual($1, $3); }
            | trhsIdExpr topNotRefEqual trhsIdExpr %dprec 2 { $$ = new NotRefEqual($1, $3); }
            | trhsIdExpr '&' trhsIdExpr %dprec 3 { $$ = new And($1, $3); }
            | trhsIdExpr '|' trhsIdExpr %dprec 3 { $$ = new Or($1, $3); }

            | termFor %dprec 4 { $$ = $1; }
            | tseq %dprec 4 { $$ = $1; }

            | twith tnormalId tindentBlock {
                $$ = new With(new Id($2), $3);
            }
            | twith tindentBlock {
                $$ = new With(0, $2);
            }
            ;

            | termIf { $$ = $1; }
            | tfuncCall { $$ = $1; }

            | tnormalId topPlusAssign trhsIdExpr %dprec 5 { $$ = new PlusAssign(new Id($1), $3); }
            | tnormalId topMinusAssign trhsIdExpr %dprec 5 { $$ = new MinusAssign(new Id($1), $3); }
            | tnormalId topSquareAssign trhsIdExpr %dprec 5 { $$ = new SquareAssign(new Id($1), $3); }
            | tnormalId topDivideAssign trhsIdExpr %dprec 5 { $$ = new DivideAssign(new Id($1), $3); }
            | tnormalId topModAssign trhsIdExpr %dprec 5 { $$ = new ModulerAssign(new Id($1), $3); }
            | tnormalId topPowAssign trhsIdExpr %dprec 5 { $$ = new PowAssign(new Id($1), $3); }
            | tnormalId '=' trhsIdExpr %dprec 5 { $$ = new Assign(new Id($1), $3); }
            ;

tdefBlockExpr:  tfunc { $$ = $1; }
                | tdefOrigin { $$ = $1; }
                | tpropexpr { $$ = $1; }
                | tctorfunc { $$ = $1; }
                | tdtorfunc { $$ = $1; }
                ;

tdefexpr    : tid topDefAssign trhsIdExpr { $$ = new DefAssign(new Id($1), $3); }
            | tid ttype topDefAssign trhsIdExpr { $$ = new DefAssign(new Param($2, new Id($1)), $4); }
            | tid ttype topDefAssign '{' '}' { $$ = new DefAssign(new Param($2, new Id($1)), new Array()); }
            | tparam { $$ = $1; }
            | takaStmt { $$ = $1; }
            | tdefBlockExpr { $$ = $1; }
            ;

tconNames   : tconName '{' '}' {
                $$ = new Origin(new Id(string($1) + "{}"));
            }
            | tconNames '{' '}' {
                $$ = new Origin(new Id($1->print() + "{}"));
            }
            | tconName '{' ttype '}' {
                $$ = new MapOrigin(new Id($1), $3);
            }
            | tconNames '{' ttype '}' {
                $$ = new MapOrigin($1, $3);
            }
            ;

ttype       : tlhsId { $$ = $1; }
            | tconNames { $$ = $1; }
            ;

tconAccess  : tlhsId '[' trhsIdExpr ']' {
                $$ = new ContainerAccess($1, $3);
            }
            ;

takaStmt    : tlhsId taka tnormalId {
                $$ = new AkaStmt($1, new Id($3));
            }
            | tfunc taka tnormalId {
                $$ = new AkaStmt($1, new Id($3));
            }
            ;

tcast       : trhsIdExpr tas ttype {
                $$ = new Cast($3, $1);
            }
            ;

treturnexpr : tret trhsIdExpr { $$ = new Return("ret", $2); }
            | treturn trhsIdExpr { $$ = new Return("return", $2); }
            ;


trhsIds     : trhsIdExpr ',' trhsIdExpr {
                Args* ret = new Args();
                ret->add($1);
                ret->add($3);
                $$ = ret;
            }
            | trhsIds ',' trhsIdExpr {
                Args* ret = (Args*) $1;
                ret->add($3);
                $$ = ret;
            }
            ;
trhslist    : '(' trhsIds ')' { //  " "를 쓰면 안된다.
                $$ = new List((Args*) $2);
            }
            ;

tlhsId      : tnormalId { $$ = new Id($1); }
            | tpackAccess { $$ = $1; }
            ;

ttuple      : trhsIdExpr ';' trhsIdExpr {
                $$ = new Tuple($1, $3);
            }

ttuples     : ttuple {
                Args* ret = new Args();
                ret->add($1);
                $$ = ret;
            }
            | ttuples ',' ttuple {
                Args* ret = (Args*) $1;
                ret->add($3);
                $$ = ret;
            }
            ;

tmap        : '{' ttuples '}' {
                $$ = new Array($2);
            }
            ;

tarray      : '{' trhsIds '}' {
                $$ = new Array($2);
            }
            ;

tseq        : trhsIdExpr topSeq trhsIdExpr {
                $$ = new Sequence($1, $3);
            }

taccess     : trhsIdExpr '.' tnormalId { $$ = new Access($1, new Id($3)); }
            | trhsIdExpr '.' tfuncCall { $$ = new Access($1, $3); }
            ;

tpackAccess : tlhsId '.' tnormalId { $$ = new Access($1, new Id($3)); }
            | tlhsId '.' tfuncCall { $$ = new Access($1, $3); }
            ;

tsafeAccess : trhsIdExpr topSafeNavi tnormalId { $$ = new SafeAccess($1, new Id($3)); }
            | trhsIdExpr topSafeNavi tfuncCall { $$ = new SafeAccess($1, $3); }
            ;

tparam      : tid ttype {
                $$ = new Param($2, new Id($1));
            }
            | '@' tid {
                $$ = new Id($2);
            }
            ;

tparams     : tparam {
                Args* ret = new Args();
                ret->add($1);
                $$ = ret;
            }
            | tparams ',' tparam {
                Args* ret = (Args*) $1;
                ret->add($3);
                $$ = ret;
            }
            ;

tfuncRhsList: trhslist { $$ = $1; }
            | '(' ')' { $$ = new List(); }
            | '(' trhsIdExpr ')' {
                List* ret = new List();
                Args* args = new Args();
                args->add($2);
                ret->add(args);
                $$ = ret;
            }
            ;


tpropexpr   : tprop tid tfrom trhsIdExpr tpropIndentBlock {
                $$ = new Prop(new Id($2), 0, $4, $5);
            }
            | tprop tid tfuncRhsList tfrom trhsIdExpr tpropIndentBlock {
                $$ = new Prop(new Id($2), $3, $5, $6);
            }
            | tprop tfrom trhsIdExpr tpropIndentBlock {
                $$ = new Prop(0, 0, $3, $4);
            }
            | tprop tfuncRhsList tfrom trhsIdExpr tpropIndentBlock {
                $$ = new Prop(0, $2, $4, $5);
            }
            ;
tpropIndentBlock: teol tindent tpropBlock tdedent { $$ = $3; }
            | ':' tgetsetterExpr { $$ = new InlineStmt($2); }
            | { $$ = 0; }
            ;
tpropBlock  : tgetsetterStmt {
                Block* ret = new Block();
                ret->add($1);
                $$ = ret;
            }
            | tpropBlock tgetsetterStmt {
                Block* ret = (Block*) $1;
                ret->add($2);
                $$ = ret;
            }
            ;

tgetsetFuncName : tfget { $$ = new Id($1); }
            | tfset { $$ = new Id($1); }
            ;
tgetsetList : tfunclist { $$ = $1; }
            | { $$ = 0; }
            ;

tgetsetterExpr: tgetsetFuncName tgetsetList tindentBlock {
                $$ = new Func(0, 0, $1, $2, 0, $3);
            }
            ;
tgetsetterStmt: tgetsetterExpr teol { $$ = new Stmt($1); }
            ;

tdefOrigin  : tdef tid tdefIndentBlock {
                $$ = new Def(new Id($2), 0, 0, $3);
            }
            | tdef tid tfuncRhsList tdefIndentBlock {
                $$ = new Def(new Id($2), $3, 0, $4);
            }
            | tdef tid tfuncRhsList tfrom trhsIdExpr tdefIndentBlock {
                $$ = new Def(new Id($2), $3, $5, $6);
            }
            | tdef tid tfrom trhsIdExpr tdefIndentBlock {
                $$ = new Def(new Id($2), 0, $4, $5);
            }
            ;

tfunclist   : '(' ')' { $$ = 0; }
            | '(' tparams ')' { $$ = $2; }
            ;

tfunc       : tid tfunclist ttype tfuncBody {
                $$ = new Func(0, $3, new Id($1), $2, 0, $4);
            }
            | tas tfunclist ttype tfuncBody {
                $$ = new Func(0, $3, new Id("as"), $2, 0, $4);
            }
            | tid tfunclist tfuncBody {
                $$ = new Func(0, 0, new Id($1), $2, 0, $3);
            }
            | tfunclist ttype tfuncBody {
                $$ = new Func(0, $2, new Id(""), $1, 0, $3);
            }
            | tfunclist tfuncBody {
                $$ = new Func(0, 0, new Id(""), $1, 0, $2);
            }
            ;

tfuncBody   : tindentBlock { $$ = $1; }
            | '=' tnull { $$ = new Str(" = null"); }
            ;

tctorfunc   : tfctor tfunclist tindentBlock {
                $$ = new Func(0, 0, new Id($1), $2, 0, $3);
            }
            ;

tdtorfunc   : tfdtor tfunclist tindentBlock {
                $$ = new Func(0, 0, new Id($1), $2, 0, $3);
            }
            ;

tfuncCall   : tid tfuncRhsList {
                $$ = new FuncCall(new Id($1), (List*) $2);
            }
            ;

texpr       : trhsIdExpr { $$ = $1; }
            | treturnexpr { $$ = $1; }
            | tnext { $$ = new Next(); }
            ;

tblock      : texpr {
                Block* ret = new Block();
                if ($1)
                    ret->add($1);
                $$ = ret;
            }
            | tblock teol texpr {
                Block* ret = (Block*) $1;
                if ($3)
                    ret->add($3);
                $$ = ret;
            }
            | tblock teof {
                $$ = $1;
            }
            ;

tdefBlock   : tdefexpr {
                Block* ret = new Block();
                if ($1)
                    ret->add(new Stmt($1));
                $$ = ret;
            }
            | tdefBlock teol tdefexpr {
                Block* ret = (Block*) $1;
                if ($3)
                    ret->add(new Stmt($3));
                $$ = ret;
            }
            ;

tpackExpr   : tpack tlhsId {
                $$ = new PackStmt($2);
            }
            ;

timportExpr : timport tlhsId {
                $$ = new ImportStmt($2);
            }
            | timport tlhsId taka tnormalId {
                $$ = new ImportStmt(new AkaStmt($2, new Id($4)));
            }
            ;

tfileExpr : tpackExpr { $$ = $1; }
            | timportExpr { $$ = $1; }
            | tdefexpr { $$ = $1; }
            ;

tdefIndentBlock: teol tindent tdefBlock tdedent { $$ = $3; }
            | ':' tdefexpr { $$ = new InlineStmt($2); }
            | { $$ = 0; }
            ;

tindentBlock: teol tindent tblock teol tdedent { $$ = $3; }
            | ':' trhsIdExpr { $$ = new InlineStmt($2); }
            | ':' treturnexpr { $$ = new InlineStmt($2); }
            | ':' tnext { $$ = new InlineStmt(new Next()); }
            ;

tfile       : tfile teol tfileExpr {
                File* ret = (File*) $1;
                ret->add(new Stmt($3));
                $$ = ret;
            }
            | tfileExpr {
                parsed = new File();
                if ($1)
                    parsed->add(new Stmt($1));
                $$ = parsed;
            }
            | tfile teof { $$ = $1; }
            ;
