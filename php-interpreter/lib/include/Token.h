#pragma once

#include <QString>
#include <QPair>

#include "Aux_def.h"

#include <iostream>

typedef QPair<int, int> TokenPosition;

enum E_TOKEN_TYPE
{
  E_TT_NONE = 0,

  E_TT_ERROR,         // sdf01

  E_TT_TAG,           // <?php, ?>
  E_TT_KEYWORD,       // define, if, while, ...
  E_TT_OPERATOR,      // +, - , =, ....
  E_TT_DELIMITER,     // ;, [, ), ., {, }, ...
  E_TT_IDENTIFIER,    // $var1, ...
  E_TT_CONSTEXPR,     // 1, "abc", true, ...
  E_TT_COMMENT        // //, /*, */ 
};

class PHP_LIB_API Token
{
public:
  Token()
    : m_token_type(E_TT_NONE)
    , m_lexem("no_lexem")
    , m_row(-1)
    , m_column(-1)
  {}

  Token(E_TOKEN_TYPE i_token_type, const QString& i_lexem,
    int i_row, int i_col)
    : m_token_type(i_token_type)
    , m_lexem(i_lexem)
    , m_row(i_row)
    , m_column(i_col)
  {}

  Token(E_TOKEN_TYPE i_token_type, const QString& i_lexem, const TokenPosition& i_pos)
    : Token(i_token_type, i_lexem, i_pos.first, i_pos.second)
  {}

  E_TOKEN_TYPE m_token_type;
  QString m_lexem;

  int m_row;
  int m_column;
};

PHP_LIB_API std::ostream& operator<<(std::ostream& i_stream, E_TOKEN_TYPE i_token_type);

PHP_LIB_API std::ostream& operator<<(std::ostream& i_stream, const Token& i_token_type);

PHP_LIB_API bool operator==(const Token& lhv, const Token& rhv);

PHP_LIB_API bool operator!=(const Token& lhv, const Token& rhv);

PHP_LIB_API QString toString(E_TOKEN_TYPE i_token_type);
