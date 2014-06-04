//
//  SemanticAnalyzer.cpp
//  php-interpreter
//
//  Created by Dovgal Kostiantyn on 6/3/14.
//
//

#include "SemanticAnalyzer.h"
#include <algorithm>

bool isExist(const SemanticResult& i_semantics, const QString& i_var_name)
{
  return std::any_of(std::begin(i_semantics), std::end(i_semantics), [&] (const SemanticResultEntity& i_entity)
  {
    return std::get<0>(i_entity).m_lexem == i_var_name && std::get<1>(i_entity) != "Variable not initialized.";
  });
}

SemanticAnalysis::SemanticAnalysis(const TokenSource& i_token_iter)
  : m_source(i_token_iter)
{}

SemanticResult SemanticAnalysis::result()
{
  SemanticResult result;

  int level = 0;
  for (; nullptr != next();)
  {
    Token var_to_assign = token();

    if (var_to_assign.m_lexem == "{")
      ++level;
    if (var_to_assign.m_lexem == "}")
      --level;

    if (var_to_assign.m_token_type == E_TT_IDENTIFIER && '$' == var_to_assign.m_lexem[0])
    {
      if (isExist(result, var_to_assign.m_lexem))
      {
        continue;
      }

      if (E_TT_OPERATOR == next()->m_token_type && "=" == token().m_lexem)
      {
        if (E_TT_CONSTEXPR == next()->m_token_type || isExist(result, token().m_lexem))
        {
          result.push_back(std::make_tuple(var_to_assign, token().m_lexem, level));
        }
      }
      else 
      {
        result.push_back(std::make_tuple(var_to_assign, "Variable not initialized.", level));
        continue;
      }
    }
    else if (var_to_assign.m_token_type == E_TT_IDENTIFIER)
    {
      result.push_back(std::make_tuple(var_to_assign, "function", level));
    }
  }

  return result;
}

const Token* SemanticAnalysis::next()
{
  return m_source.next();
}

Token  SemanticAnalysis::token() const
{
  return m_source.token();
}

const Token* SemanticAnalysis::prev()
{
  return m_source.prev();
}
