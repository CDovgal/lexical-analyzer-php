#include "Token.h"

#include "LA_Aux.h"

PHP_LIB_API std::ostream& operator<<(std::ostream& i_stream, E_TOKEN_TYPE i_token_type)
{
  i_stream << toString(i_token_type).toStdString();
  return i_stream;
}

PHP_LIB_API std::ostream& operator<<(std::ostream& i_stream, const Token& i_token_type)
{
  i_stream << 
    "\nToken type:\t" << i_token_type.m_token_type           <<
    "\nLexem:\t"      << i_token_type.m_lexem.toStdString()  <<
    "\nRow:\t"        << i_token_type.m_row                  <<
    "\nColumn:\t"     << i_token_type.m_column               ;

  return i_stream;
}

PHP_LIB_API bool operator==(const Token& lhv, const Token& rhv)
{
  if(lhv.m_token_type != rhv.m_token_type)
    return false;

  if(lhv.m_lexem != rhv.m_lexem)
    return false;

  if(lhv.m_row != rhv.m_row)
    return false;

  if(lhv.m_column != rhv.m_column)
    return false;

  return true;
}

PHP_LIB_API bool operator!=(const Token& lhv, const Token& rhv)
{
  return !(lhv == rhv);
}

PHP_LIB_API QString toString(E_TOKEN_TYPE i_token_type)
{
  QString str;

  switch (i_token_type)
  {
  case E_TT_NONE:
    str = "None"; break;
  case E_TT_TAG:
    str = "Tag"; break;
  case E_TT_KEYWORD:
    str = "Keyword"; break;
  case E_TT_OPERATOR:
    str = "Operator"; break;
  case E_TT_DELIMITER:
    str = "Delimiter"; break;
  case E_TT_IDENTIFIER:
    str = "Identifier"; break;
  case E_TT_CONSTEXPR:
    str = "Const expession"; break;
  case E_TT_COMMENT:
    str = "Comment"; break;
  default:
    str = "Error";
  }

  return str;
}
