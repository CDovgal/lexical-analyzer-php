//
//  TokenIterator.cpp
//  php-interpreter
//
//  Created by Dovgal Kostiantyn on 5/28/14.
//
//

#include "TokenSource.h"

TokenSource::TokenSource(const QVector<Token>& i_tokens)
  : m_tokens(i_tokens)
  , m_current(-1)
{}

const Token* TokenSource::next()
{
  ++m_current;
  if ( isValid() )
    return &m_tokens[m_current];

  return nullptr;
}

Token TokenSource::token() const
{
  if ( isValid() )
    return m_tokens[m_current];

  return Token();
}

const Token* TokenSource::prev()
{
  --m_current;
  if ( isValid() )
    return &m_tokens[m_current];

  return nullptr;
}

bool TokenSource::isValid() const
{
  return m_current >= 0 && m_current < m_tokens.size();
}
