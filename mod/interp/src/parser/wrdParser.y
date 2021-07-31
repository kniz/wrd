%code top {
/*  ============================================================================================
    |                                          PROLOGUE                                        |
    ============================================================================================  */
    #pragma once

    #include <iostream>
    using std::cout;
    #include "interpretable.hpp"
}

/*  ============================================================================================
    |                                          C++ DECLS                                       |
    ============================================================================================  */

%code requires {
    typedef void* yyscan_t;
}

%code provides {
    //extern wrd::sobj* root;
    extern int yylineno;
    extern char* yytext;

    extern "C" {
        int yylex(YYSTYPE* val, YYLTYPE* loc, wrd::interpretable* interp, yyscan_t scanner);
        void yyerror(YYLTYPE* loc, wrd::interpretable* interp, yyscan_t scanner, const char* msg);
    }
}

/*  ============================================================================================
    |                                         BISON FLAGS                                      |
    ============================================================================================  */

%union {
    int nint; // n means 'native'.
    void* voidp;
}

%define api.pure
%glr-parser
%locations

%lex-param {wrd::interpretable* interp} {yyscan_t scanner}
%parse-param {wrd::interpretable* interp} {yyscan_t scanner}

/*  ============================================================================================
    |                                        BISON SYMBOLS                                     |
    ============================================================================================  */

%start prog

%token <nint> INT
%type <voidp> prog

/*%type ?? ?? */

/*  ============================================================================================
    |                                     OPERATOR PRECEDENCE                                  |
    ============================================================================================  */
/*%right ?? ?? */
/*%left ?? ?? */


/*  ============================================================================================
    |                                           RULES                                          |
    ============================================================================================  */
%%

prog: INT ';' {
    // TODO:
}

%%

/*  ============================================================================================
    |                                         EPILOGUE                                         |
    ============================================================================================  */

void yyerror(YYLTYPE* loc, wrd::interpretable* interp, yyscan_t scanner, const char* msg) {
    // TODO:
}
