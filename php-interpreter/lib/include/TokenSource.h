//
//  TokenIterator.h
//  php-interpreter
//
//  Created by Dovgal Kostiantyn on 5/28/14.
//
//

#pragma once

#include "Token.h"

#include <QPair>
#include <QVector>

typedef QPair<Token, int> SemanticResultEntity;
typedef QVector<SemanticResultEntity> SemanticResult;

class PHP_LIB_API TokenSource
{
public:
  TokenSource(const QVector<Token>& i_tokens);
  
  const Token* next();
  Token token() const;
  const Token* prev();

  bool isValid() const;
  
private:
  QVector<Token> m_tokens;
  int m_current;
};
