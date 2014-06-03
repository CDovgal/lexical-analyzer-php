//
//  SemanticAnalyzer.h
//  php-interpreter
//
//  Created by Dovgal Kostiantyn on 6/3/14.
//
//

#pragma once

#include "TokenSource.h"

class PHP_LIB_API SemanticAnalysis
{
public:
  SemanticAnalysis(const TokenSource& i_token_iter);

  SemanticResult result();

private:
  const Token* next();
  Token  token() const;
  const Token* prev();

  TokenSource m_source;
};

