/**@file   SyntaxAnalyzer.cpp
* @brief
* @date    Date Created:: 27/05/14 Dovgal Kostiantyn
*          $Date:: 27-05-2014 22:10#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#include "SyntaxAnalizer.h"
#include "LA_Aux.h"

SyntaxAnalyzer::SyntaxAnalyzer(const TokenSource& i_token_source)
  :  m_source(i_token_source)
  ,  m_depth(0)
{
}

ProductionResult SyntaxAnalyzer::readProduction()
{
  ProductionResult result;
  
  readSubProduction(result);
  
  return result;
}

void SyntaxAnalyzer::readSubProduction(ProductionResult& io_production)
{
  if ( nullptr == next() )
  {
    return;
  }
  
  switch (token().m_token_type)
  {
    case E_TT_TAG:
      if (TAG_OPEN == token().m_lexem)
      {
        io_production.push_back(output(
        "Program starts. Rules #100 applied. Changing to the rule 101."));
      }
      
      if (TAG_CLOSE == token().m_lexem)
      {
        io_production.push_back(output(
        "Program ends. Rules #100 applied."));
      }
    break;
      
    case E_TT_KEYWORD:
      readKeywordProduction(io_production);
    break;
    
      
    default:
    io_production.push_back(
    "Something has gone wrong.");
      break;
  }
  
  return;
}

bool SyntaxAnalyzer::readKeywordProduction(ProductionResult& io_production)
{
  SCOPED_DEPTH_METER
  
  io_production.push_back(
  "KEYWORDS");
  
  auto lexem = token().m_lexem;
  
  if (KEYWORD_FUNCTION == lexem)
  {
    return readFunction(io_production);
  }

  
  

  return false;
}

bool SyntaxAnalyzer::readFunction(ProductionResult& io_production)
{
  SCOPED_DEPTH_METER
  
  Identifier function_name;
  if (!readIdentifier(function_name))
    return false;
  
  Delimiter open_round_bracket;
  if (!readDelimiter(open_round_bracket) || BRACKET_ROUND_OPEN != open_round_bracket)
    return false;
  
  
  Delimiter close_round_bracket;
  if (!readDelimiter(close_round_bracket) || BRACKET_ROUND_CLOSE != close_round_bracket)
    return false;
  
  
  return true;
}

bool SyntaxAnalyzer::readIdentifier(Identifier& io_identifier)
{
  SCOPED_DEPTH_METER
  
  if(!next())
    return false;
  
  if (E_TT_IDENTIFIER == token().m_token_type)
  {
    io_identifier = token().m_lexem;
    
    return true;
  }
  
  return false;
}


bool SyntaxAnalyzer::readDelimiter(Delimiter& io_delimiter)
{
  SCOPED_DEPTH_METER
  
  auto token = *next();
  
  if (E_TT_DELIMITER == token.m_token_type)
  {
    io_delimiter = token.m_lexem;
    
    return true;
  }
  
  return false;
}

Token* SyntaxAnalyzer::next()
{
  return m_source.next();
}
      
Token  SyntaxAnalyzer::token() const
{
  return m_source.token();
}
      
Token* SyntaxAnalyzer::prev()
{
  return m_source.prev();
}

QString SyntaxAnalyzer::output(QString&& str)
{
  return QString(m_depth, '\t') + str;
}

QString SyntaxAnalyzer::output(const QString& str)
{
  return QString(m_depth, 't') + str;
}
