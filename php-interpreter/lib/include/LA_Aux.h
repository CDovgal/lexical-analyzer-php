#pragma once

#include <QString>

#include "Aux_def.h"

PHP_LIB_API const QVector<QString>& tags();
PHP_LIB_API const QVector<QString>& comments();
PHP_LIB_API const QVector<QString>& delimiters();
PHP_LIB_API const QVector<QString>& keywords();
PHP_LIB_API const QVector<QString>& operators();

PHP_LIB_API const QVector<QString>& elements();

PHP_LIB_API bool isTag(const QString& i_str);
PHP_LIB_API bool isComment(const QString& i_str);
PHP_LIB_API bool isDelimiter(const QString& i_str);
PHP_LIB_API bool isKeyword(const QString& i_str);
PHP_LIB_API bool isOperator(const QString& i_str);

PHP_LIB_API bool isConstExpr(const QString& i_str);
PHP_LIB_API bool isIdentifier(const QString& i_str);
 
/////////////////////////////////////////////
// DELIMITERS
extern QString DELIMITER_SEMICOLON;
extern QString DELIMITER_DOT;
extern QString DELIMITER_COMA;
/////////////////////////////////////////////

/////////////////////////////////////////////
// TAGS
extern QString TAG_OPEN;
extern QString TAG_CLOSE;
/////////////////////////////////////////////

////////////////////////////////////////////
// COMMENTS
extern QString COMMENT_LINE;
extern QString COMMENT_OPEN;
extern QString COMMENT_END;
////////////////////////////////////////////

////////////////////////////////////////////
// BRACKETS
extern QString BRACKET_ROUND_OPEN;
extern QString BRACKET_ROUND_CLOSE;
extern QString BRACKET_SQUARE_OPEN;
extern QString BRACKET_SQUARE_CLOSE;
extern QString BRACKET_FIGURE_OPEN;
extern QString BRACKET_FIGURE_CLOSE;
////////////////////////////////////////////

/////////////////////////////////////////////
// KEYWORDS
extern QString KEYWORD_DEFINE;
extern QString KEYWORD_INCLUDE;
extern QString KEYWORD_NULL;
extern QString KEYWORD_FUNCTION;
extern QString KEYWORD_RETURN;
extern QString KEYWORD_FOR;
extern QString KEYWORD_CONTINUE;
extern QString KEYWORD_BREAK;
extern QString KEYWORD_IF;
extern QString KEYWORD_ELSE_IF;
extern QString KEYWORD_ELSE;
extern QString KEYWORD_SWITCH;
extern QString KEYWORD_CASE;
extern QString KEYWORD_DEFAULT;
extern QString KEYWORD_ENDSWITCH;
extern QString KEYWORD_ECHO;
////////////////////////////////////////////

////////////////////////////////////////////
// OPERATORS
extern QString OPERATOR_LOGICAL_AND;
extern QString OPERATOR_LOGICAL_XOR;
extern QString OPERATOR_IDENTICAL;
extern QString OPERATOR_NOT_IDENTICAL;
extern QString OPERATOR_LOGICAL_OR;
extern QString OPERATOR_EQUAL;
extern QString OPERATOR_NOT_EQUAL_;
extern QString OPERATOR_NOT_EQUAL_BRACKET;
extern QString OPERATOR_AND;
extern QString OPERATOR_OR;
extern QString OPERATOR_SHIFT_LEFT;
extern QString OPERATOR_SHIFT_RIGHT;
extern QString OPERATOR_LESS_OR_EQUAL;
extern QString OPERATOR_GREATER_OR_EQUAL;
extern QString OPERATOR_INCREMENT;
extern QString OPERATOR_DECREMENT;
extern QString OPERATOR_ASSIGNMENT_PLUS;
extern QString OPERATOR_ASSIGNMENT_MINUS;
extern QString OPERATOR_ASSIGNMENT_MULTIPLY;
extern QString OPERATOR_ASSIGNMENT_DIVISION;
extern QString OPERATOR_ASSIGNMENT_MOD2;
extern QString OPERATOR_NOT;
extern QString OPERATOR_ASSIGNMENT;
extern QString OPERATOR_PLUS;
extern QString OPERATOR_MINUS;
extern QString OPERATOR_MULTIPLY;
extern QString OPERATOR_DIVISION;
extern QString OPERATOR_PERCENTAGE;
extern QString OPERATOR_BIT_AND;
extern QString OPERATOR_BIT_OR;
extern QString OPERATOR_BIT_XOR;
extern QString OPERATOR_BIT_NOT;
extern QString OPERATOR_LESS;
extern QString OPERATOR_GREATER;
extern QString OPERATOR_TERNARY_QUESTION;
extern QString OPERATOR_TERNARY_DOUBLE_DOT;
////////////////////////////////////////////
