#include "LA_Aux.h"

#include <QVector>

QString TokenToString(E_TOKEN_TYPE i_token_type)
{
  switch (i_token_type)
  {
  case E_TT_NONE:
    return "None";
  case E_TT_TAG:
    return "Tag";
  case E_TT_KEYWORD:
    return "Keyword";
  case E_TT_OPERATOR:
    return "Operator";
  case E_TT_DELIMITER:
    return "Delimiter";
  case E_TT_IDENTIFIER:
    return "Identifier";
  case E_TT_CONSTEXPR:
    return "Const expession";
  default:
    return "Error.";
  }
}

bool isTag(const QString& i_str)
{
    static QVector<QString> tags;
    //= {"<?php", "?>"};

    return tags.indexOf(i_str) != -1;
}

bool isKeyword(const QString& i_str)
{
    static QVector<QString> keyword;
    //= {
    //    "function", "return", "for", "if", "else",
    //    "while", "continue", "switch", "endswitch",
    //    "case", "break", "default","const",
    //    "define", "NULL", "echo", "include"};

    return keyword.indexOf(i_str) != -1;
}

bool isOperator(const QString& i_str)
{
    static QVector<QString> operators;
    //= {
    //    "===", "!==",
    //    "<>", "<=",  ">=", "==", "!=",
    //    "+=", "-=", "*=", "/=", "%=",
    //    "&&", "||",
    //    "+", "-", "*", "/", "<", ">",
    //    "&", "|", "=",  "%",  ".", "!",
    //    "and", "xor", "or"};

    return operators.indexOf(i_str) != -1;
}

bool isDelimiter(const QString& i_str)
{
    static QVector<QString> delimiters;
      //; = {"(", ")", "[", "]"};

    return delimiters.indexOf(i_str) != -1;
}

bool isIdentifier(const QString& i_str)
{
    return !i_str.isEmpty() && i_str[0] == '$';
}

bool isValue(const QString& i_str)
{
    return true;
}

int trim_front(QString& i_str)
{
    int count = 0;
    while(i_str[0].isSpace())
    {
        ++count;
        i_str.remove(0, 1);
    }

    return count;
}
