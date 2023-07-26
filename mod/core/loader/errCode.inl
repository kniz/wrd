// common:
PAIRX(UNKNOWN,                      "unknown")
PAIRX(MSG,                          "%s")
PAIRX(IS_NULL,                      "%s is null")
PAIRX(FILE_NOT_OPEN,                "can't open %s file.")

// location:
PAIRX(NO_ROW_INFO,                  "this expr doesn't have row info")
PAIRX(NO_COL_INFO,                  "this expr doesn't have col info")

// node:
PAIRX(CANT_ACCESS,                  "there is no '%s' in '%s'")
PAIRX(DUP_VAR,                      "duplicated variable '%s' found")
PAIRX(ACCESS_TO_INCOMPLETE,         "accesses to incomplete object")

// obj:
PAIRX(CANT_CAST_TO_NODE,            "obj can't be cast into node class")
PAIRX(OBJ_WRONG_ARGS,               "I can't run with this argument types '%s'. they should be '%s'")
PAIRX(OBJ_ALREADY_EXIST,            "\'%s\' object already exists at %s")

// func:
PAIRX(FUNC_DONT_HAVE_ME,            "parameters of func not included 'me'")
PAIRX(FUNC_REDIRECTED_OBJ,          "all funcs should be redirected by object")
PAIRX(MAIN_FUNC_RET_TYPE,           "\'main\' func return type should be int or void")
PAIRX(ALREADY_DEFINED_FUNC,         "same name and params of the func '%s' was already defined in this scope")
PAIRX(ALREADY_DEFINED_IDENTIFIER,   "same identifier of func '%s' was already defined in this scope")

// parser:
PAIRX(SYNTAX_ERR,                   "syntax error %s expected, but %s found")
PAIRX(UNEXPECTED_TOK,               "unexpected token '%s' found")
PAIRX(WRONG_INDENT_LV,              "invalid indentation level found. expected %d columns but it was %d. I'm assuming you were trying to give %d columns")
PAIRX(PARSING_HAS_ERR,              "parsing has done. but something went wrong: %d")
PAIRX(NO_SRC,                       "source code not provided")

// pack:
PAIRX(NO_PACK_TRAY,                 "pack tray hasn't been created before")
PAIRX(NO_PACK,                      "no pack has been specified")
PAIRX(PACK_NOT_MATCH,               "pack %s expected, but it was %s")

// expr:
PAIRX(NOT_EXPR,                     "\'%s\' isn't a expression")
PAIRX(EXPR_EVAL_NULL,               "expression evaluated as null type")
//  defVar:
PAIRX(CANT_DEF_VAR,                 "definining variable '%s %s' wouldn't allowed here")
PAIRX(ALREADY_DEFINED_VAR,          "variable '%s %s' was already defined in this scope")
PAIRX(HAS_NO_NAME,                  "variable should have a name")
PAIRX(NO_ORIGIN,                    "there is no origin %s object")
PAIRX(TYPE_NOT_EXIST,               "when defines variable '%s', couldn't find the type of it.")
PAIRX(DONT_HAVE_CTOR,               "%s's type doesn't have a constructor without parameter.")
PAIRX(TYPE_NOT_DEDUCED,             "type can't be deduced into meaningful one.")
PAIRX(NO_VOID_CONTAINER,            "you can't make a void container")
//  defSeq:
PAIRX(SEQ_SHOULD_INT_COMPATIBLE,    "start and end of a sequence should be sort of an int.")
//  assign:
PAIRX(TYPE_NOT_COMPATIBLE,          "rhs type '%s' is not compatible to lhs '%s' of this assigment")
PAIRX(ASSIGN_TO_RVALUE,             "you can't assign %s to this unamed %s. please define it first")
//  casting:
PAIRX(CAST_NOT_AVAILABLE,           "couldn't cast %s type as %s")
PAIRX(LHS_IS_NULL,                  "lhs of the expression is null")
PAIRX(RHS_IS_NULL,                  "rhs of the expression is null")
PAIRX(EXPR_SHOULDNT_BE_HERE,        "expression shouldn't be here")
PAIRX(IMPLICIT_CAST_NOT_AVAILABLE,  "couldn't implicit cast %s type as %s")
PAIRX(VOID_NOT_CAST,                "casting to the void type isn't allowed")
PAIRX(CAST_TO_UNKNOWN,              "cast to unknown type")
//  arithmetic operation:
PAIRX(LHS_IS_NOT_ARITH,             "lhs '%s' is not arithmetic type")
PAIRX(RHS_IS_NOT_ARITH,             "rhs '%s' is not arithmetic type")
//  defObj:
PAIRX(MAKE_GENERIC_FAIL,            "construction '%s' for generic obj has failed.")
//  generics:
PAIRX(SHOULD_TYPE_PARAM_NAME,       "'%s' type parameter wasn't be defined as new name.")
//  func-call:
PAIRX(FUNC_NOT_EXIST,               "this func doesn't exist.")
PAIRX(DONT_KNOW_ME,                 "I don't know what the 'me' object is to call this func.")
//  retState:
PAIRX(BREAK_OUTSIDE_OF_LOOP,        "use 'break' outside of a loop")
PAIRX(NEXT_OUTSIDE_OF_LOOP,         "use 'next' outside of a loop")
//  if:
PAIRX(CONDITION_IS_EMPTY,           "condition expression is empty")
PAIRX(CONDITION_CANT_CAST_TO_BOOL,  "condition expression can't cast to bool type.")
//  getExpr:
PAIRX(WHAT_IS_THIS_IDENTIFIER,      "I don't know what '%s' is.")
//  defAssign:
PAIRX(RHS_NOT_EVALUATED,            "I can't define a new variable here because there is a problem with the expression on the right.")
PAIRX(VOID_CANT_DEFINED,            "you can't define a variable to void type")

// func:("verify: arithmeticExpr: lhs & rhs should bind something.");
//
// if(nul(it.getLeft) || nul(it.getRight()))
//
PAIRX(PARAM_HAS_VAL,                "parameter must take a form of '<name> <type>'")
PAIRX(FUNC_HAS_NO_NAME,             "a function should have a name")
PAIRX(WRONG_RET_TYPE,               "%s is not a valid return type")
PAIRX(NO_RET_TYPE,                  "return type is not specified")
PAIRX(NO_FUNC_INFO,                 "function info doesn't exist")
PAIRX(NO_STMT_IN_FUNC,              "function need to contain more than 1 statement inside")
PAIRX(RET_TYPE_NOT_MATCH,           "%s return type isn't valid to %s return type of func")
PAIRX(PARAM_NOT_VOID,               "parameter '%s' shouldn't be a void type")

// block:
PAIRX(BLK_CANT_MAKE_FRAME,          "block statement failed to construct frame structure")

// container:
PAIRX(ELEM_TYPE_IS_NULL,            "there is no elem type")
PAIRX(ELEM_TYPE_DEDUCED_NULL,       "elem type has been deduced to null")
PAIRX(ELEM_TYPE_DEDUCED_WRONG,      "elem type has been deduced to %s type")
PAIRX(ELEM_TYPE_NOT_VOID,           "elem type shouldn't be the void type")
PAIRX(CONTAINER_IS_NULL,            "there is no container")

// operator:
PAIRX(STRING_IS_NOT_LOGICAL,        "string type isn't suitable to logical operators.")
