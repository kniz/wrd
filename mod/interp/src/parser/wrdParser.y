%code top {
/*  ============================================================================================
    |                                          PROLOGUE                                        |
    ============================================================================================  */
    #pragma once

    #include "scanner.hpp"
}

/*  ============================================================================================
    |                                          C++ DECLS                                       |
    ============================================================================================  */

%code requires {

}

/*  ============================================================================================
    |                                         BISON FLAGS                                      |
    ============================================================================================  */

/*TODO: %union {
}*/

%glr-parser
%verbose

/*  ============================================================================================
    |                                        BISON SYMBOLS                                     |
    ============================================================================================  */

/*TODO: %start ??*/
/*%token ?? ?? */
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

%%

/*  ============================================================================================
    |                                         EPILOGUE                                         |
    ============================================================================================  */
