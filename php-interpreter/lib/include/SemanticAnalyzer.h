//
//  SemanticAnalyzer.h
//  php-interpreter
//
//  Created by Dovgal Kostiantyn on 6/3/14.
//
//

#pragma once

#include "TokenSource.h"


class SemanticAnalysis
{
public:
  SemanticAnalysis(const TokenSource& i_token_iter);

  SemanticResult result();

private:
  TokenSource m_source;
};

