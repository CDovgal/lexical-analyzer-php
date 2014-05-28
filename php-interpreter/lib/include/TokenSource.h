//
//  TokenIterator.h
//  php-interpreter
//
//  Created by Dovgal Kostiantyn on 5/28/14.
//
//

#pragma once

#include "Token.h"

class TokenSource
{
public:
  
  Token* next();
  Token token() const;
  Token* prev();
  
private:
  
};
