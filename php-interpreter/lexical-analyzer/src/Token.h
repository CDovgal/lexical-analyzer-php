#pragma once

#include <QString>
#include "LA_Aux.h"

class Token
{
public:
  Token()
    : m_token_type(E_TT_NONE)
  {}

  Token(E_TOKEN_TYPE i_token_type, const QString& i_lexem,
    int i_row, int i_col)
    : m_token_type(i_token_type)
    , m_lexem(i_lexem)
    , m_row(i_row)
    , m_column(i_col)
  {}

  E_TOKEN_TYPE m_token_type;
  QString m_lexem;

  int m_row;
  int m_column;
};
