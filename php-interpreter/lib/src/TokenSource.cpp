//
//  TokenIterator.h
//  php-interpreter
//
//  Created by Dovgal Kostiantyn on 5/28/14.
//
//

#include "TokenSource.h"

Token* TokenSource::next()
{
  return nullptr;
}

Token TokenSource::token() const
{
  return Token();
}

Token* TokenSource::prev()
{
  return nullptr;
}
