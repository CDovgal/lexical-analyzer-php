#include "LA_Aux.h"

#include <QVector>

/////////////////////////////////////////////
// TAGS
QString TAG_OPEN  = "<?php";
QString TAG_CLOSE = "?>";
/////////////////////////////////////////////

////////////////////////////////////////////
// COMMENTS
QString COMMENT_LINE = "//";
QString COMMENT_OPEN = "/*";
QString COMMENT_END  = "*/";
////////////////////////////////////////////

////////////////////////////////////////////
// BRACKETS, SEMICOLON
QString BRACKET_ROUND_OPEN    = "(";
QString BRACKET_ROUND_CLOSE   = ")";
QString BRACKET_SQUARE_OPEN   = "[";
QString BRACKET_SQUARE_CLOSE  = "(";
QString BRACKET_FIGURE_OPEN   = "{";
QString BRACKET_FIGURE_CLOSE  = "}";
QString SEMICOLON             = ";";
////////////////////////////////////////////

/////////////////////////////////////////////
// KEYWORDS
QString KEYWORD_DEFINE      = "define";
QString KEYWORD_INCLUDE     = "include";
QString KEYWORD_NULL        = "NULL";
QString KEYWORD_FUNCTION    = "function";
QString KEYWORD_CONST       = "const";
QString KEYWORD_RETURN      = "return";
QString KEYWORD_FOR         = "for";
QString KEYWORD_CONTINUE    = "continue";
QString KEYWORD_BREAK       = "break";
QString KEYWORD_IF          = "if";
QString KEYWORD_ELSE_IF     = "elseif";
QString KEYWORD_ELSE        = "else";
QString KEYWORD_SWICTH      = "switch";
QString KEYWORD_CASE        = "case";
QString KEYWORD_DEFAULT     = "default";
QString KEYWORD_ENDSWITCH   = "endswitch";
QString KEYWORD_ECHO        = "echo";
////////////////////////////////////////////

////////////////////////////////////////////
// OPERATORS
QString OPERATOR_LOGICAL_AND          = "and";
QString OPERATOR_LOGICAL_XOR          = "xor";
QString OPERATOR_IDENTICAL            = "===";
QString OPERATOR_NOT_IDENTICAL        = "!==";
QString OPERATOR_LOGICAL_OR           = "or";
QString OPERATOR_EQUAL                = "==";
QString OPERATOR_NOT_EQUAL_           = "!=";
QString OPERATOR_NOT_EQUAL_BRACKET    = "<>";
QString OPERATOR_AND                  = "&&";
QString OPERATOR_OR                   = "||";
QString OPERATOR_SHIFT_LEFT           = "<<";
QString OPERATOR_SHIFT_RIGHT          = ">>";
QString OPERATOR_LESS_OR_EQUAL        = "<=";
QString OPERATOR_GREATER_OR_EQUAL     = ">=";
QString OPERATOR_INCREMENT            = "++";
QString OPERATOR_DECREMENT            = "--";
QString OPERATOR_ASSIGNMENT_PLUS      = "+=";
QString OPERATOR_ASSIGNMENT_MINUS     = "-=";
QString OPERATOR_ASSIGNMENT_MULTIPLY  = "*=";
QString OPERATOR_ASSIGNMENT_DIVISION  = "/=";
QString OPERATOR_ASSIGNMENT_MOD2      = "%=";
QString OPERATOR_NOT                  = "!";
QString OPERATOR_ASSIGNMENT           = "=";
QString OPERATOR_PLUS                 = "+";
QString OPERATOR_MINUS                = "-";
QString OPERATOR_MULTIPLY             = "*";
QString OPERATOR_DIVISION             = "/";
QString OPERATOR_PERCENTAGE           = "%";
QString OPERATOR_BIT_AND              = "&";
QString OPERATOR_BIT_OR               = "|";
QString OPERATOR_BIT_XOR              = "^";
QString OPERATOR_BIT_NOT              = "~";
QString OPERATOR_LESS                 = "<";
QString OPERATOR_GREATER              = ">";
QString OPERATOR_DOT                  = ".";
QString OPERATOR_COMA                 = ",";
QString OPERATOR_TERNARY_QUESTION     = "?";
QString OPERATOR_TERNARY_DOUBLE_DOT   = ":";
////////////////////////////////////////////


static QVector<QString> TAGS = {
    TAG_OPEN
  , TAG_CLOSE 
};

static QVector<QString> COMMENTS = {
    COMMENT_LINE
  , COMMENT_OPEN
  , COMMENT_END
};

static QVector<QString> DELIMITERS = {
    BRACKET_ROUND_OPEN
  , BRACKET_ROUND_CLOSE
  , BRACKET_SQUARE_OPEN
  , BRACKET_SQUARE_CLOSE
  , BRACKET_FIGURE_OPEN
  , BRACKET_FIGURE_CLOSE
  , SEMICOLON
};

static QVector<QString> KEYWORDS = {
    KEYWORD_DEFINE
  , KEYWORD_INCLUDE
  , KEYWORD_NULL
//  , KEYWORD_FUNCTION
//  , KEYWORD_CONST
  , KEYWORD_RETURN
  , KEYWORD_FOR
  , KEYWORD_CONTINUE
  , KEYWORD_BREAK
  , KEYWORD_IF
  , KEYWORD_ELSE
  , KEYWORD_SWICTH
  , KEYWORD_CASE
  , KEYWORD_DEFAULT
  , KEYWORD_ENDSWITCH
  , KEYWORD_ECHO
};

static QVector<QString> OPERATORS = {
    OPERATOR_LOGICAL_AND
  , OPERATOR_LOGICAL_XOR
  , OPERATOR_IDENTICAL
  , OPERATOR_NOT_IDENTICAL
  , OPERATOR_LOGICAL_OR
  , OPERATOR_EQUAL
  , OPERATOR_NOT_EQUAL_
  , OPERATOR_NOT_EQUAL_BRACKET
  , OPERATOR_AND
  , OPERATOR_OR
  , OPERATOR_SHIFT_LEFT
  , OPERATOR_SHIFT_RIGHT
  , OPERATOR_LESS_OR_EQUAL
  , OPERATOR_GREATER_OR_EQUAL
  , OPERATOR_INCREMENT
  , OPERATOR_DECREMENT
  , OPERATOR_ASSIGNMENT_PLUS
  , OPERATOR_ASSIGNMENT_MINUS
  , OPERATOR_ASSIGNMENT_MULTIPLY
  , OPERATOR_ASSIGNMENT_DIVISION
  , OPERATOR_ASSIGNMENT_MOD2
  , OPERATOR_NOT
  , OPERATOR_ASSIGNMENT
  , OPERATOR_PLUS
  , OPERATOR_MINUS
  , OPERATOR_MULTIPLY
  , OPERATOR_DIVISION
  , OPERATOR_PERCENTAGE
  , OPERATOR_BIT_AND
  , OPERATOR_BIT_OR
  , OPERATOR_BIT_XOR
  , OPERATOR_BIT_NOT
  , OPERATOR_LESS
  , OPERATOR_GREATER
  , OPERATOR_DOT
  , OPERATOR_COMA
  , OPERATOR_TERNARY_QUESTION
  , OPERATOR_TERNARY_DOUBLE_DOT
};

static QVector<QString> ELEMENTS = {
    TAG_OPEN
  , TAG_CLOSE

  , COMMENT_LINE
  , COMMENT_OPEN
  , COMMENT_END

  , BRACKET_ROUND_OPEN
  , BRACKET_ROUND_CLOSE
  , BRACKET_SQUARE_OPEN
  , BRACKET_SQUARE_CLOSE
  , BRACKET_FIGURE_OPEN
  , BRACKET_FIGURE_CLOSE

  , KEYWORD_DEFINE
  , KEYWORD_INCLUDE
  , KEYWORD_NULL
  , KEYWORD_FUNCTION
  , KEYWORD_CONST
  , KEYWORD_RETURN
  , KEYWORD_FOR
  , KEYWORD_CONTINUE
  , KEYWORD_BREAK
  , KEYWORD_IF
  , KEYWORD_ELSE
  , KEYWORD_SWICTH
  , KEYWORD_CASE
  , KEYWORD_DEFAULT
  , KEYWORD_ENDSWITCH
  , KEYWORD_ECHO

  , OPERATOR_LOGICAL_AND
  , OPERATOR_LOGICAL_XOR
  , OPERATOR_IDENTICAL
  , OPERATOR_NOT_IDENTICAL
  , OPERATOR_LOGICAL_OR
  , OPERATOR_EQUAL
  , OPERATOR_NOT_EQUAL_
  , OPERATOR_NOT_EQUAL_BRACKET
  , OPERATOR_AND
  , OPERATOR_OR
  , OPERATOR_SHIFT_LEFT
  , OPERATOR_SHIFT_RIGHT
  , OPERATOR_LESS_OR_EQUAL
  , OPERATOR_GREATER_OR_EQUAL
  , OPERATOR_INCREMENT
  , OPERATOR_DECREMENT
  , OPERATOR_ASSIGNMENT_PLUS
  , OPERATOR_ASSIGNMENT_MINUS
  , OPERATOR_ASSIGNMENT_MULTIPLY
  , OPERATOR_ASSIGNMENT_DIVISION
  , OPERATOR_ASSIGNMENT_MOD2
  , OPERATOR_NOT
  , OPERATOR_ASSIGNMENT
  , OPERATOR_PLUS
  , OPERATOR_MINUS
  , OPERATOR_MULTIPLY
  , OPERATOR_DIVISION
  , OPERATOR_PERCENTAGE
  , OPERATOR_BIT_AND
  , OPERATOR_BIT_OR
  , OPERATOR_BIT_XOR
  , OPERATOR_BIT_NOT
  , OPERATOR_LESS
  , OPERATOR_GREATER
  , OPERATOR_DOT
  , OPERATOR_COMA
  , OPERATOR_TERNARY_QUESTION
  , OPERATOR_TERNARY_DOUBLE_DOT
};

PHP_LIB_API bool isTag(const QString& i_str)
{
  return TAGS.indexOf(i_str) != -1;
}

PHP_LIB_API bool isComment(const QString& i_str)
{
  return COMMENTS.indexOf(i_str) != -1;
}

PHP_LIB_API bool isDelimiters(const QString& i_str)
{
  return DELIMITERS.indexOf(i_str) != -1;
}

PHP_LIB_API bool isKeyword(const QString& i_str)
{
  return KEYWORDS.indexOf(i_str) != -1;
}

PHP_LIB_API bool isOperator(const QString& i_str)
{
  return OPERATORS.indexOf(i_str) != -1;
}

PHP_LIB_API bool isIdentifier(const QString& i_str)
{
  // add regexp here
  return !i_str.isEmpty() && i_str[0] == '$';
}

PHP_LIB_API bool isConstExpr(const QString& i_str)
{
  return true;
}

PHP_LIB_API const QVector<QString>& tags()
{
  return TAGS;
}

PHP_LIB_API const QVector<QString>& comments()
{
  return COMMENTS;
}

PHP_LIB_API const QVector<QString>& delimiters()
{
  return DELIMITERS;
}

PHP_LIB_API const QVector<QString>& keywords()
{
  return KEYWORDS;
}

PHP_LIB_API const QVector<QString>& operators()
{
  return OPERATORS;
}

PHP_LIB_API const QVector<QString>& elements()
{
  return ELEMENTS;
}

