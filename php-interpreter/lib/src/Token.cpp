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
    "Token type:\t" << i_token_type.m_token_type           << '\n' <<
    "Lexem:\t"      << i_token_type.m_lexem.toStdString()  << '\n' <<
    "Row:\t"        << i_token_type.m_row                  << '\n' <<
    "Column:\t"     << i_token_type.m_column                         ;

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
