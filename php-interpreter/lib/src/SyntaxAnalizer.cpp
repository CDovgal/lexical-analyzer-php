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
  : m_source(i_token_source)
  , m_depth(0)
  , m_hack(false)
{}

ProductionResult SyntaxAnalyzer::readProduction()
{
  ProductionResult result;

  readSubProduction(result);

  //if (!m_hack)
  //{
  //  m_hack = true;
  //  return{ output("1"), output("2") };
  //}
  //else
  //{
  //  return result;
  //}

  return result;
}

void SyntaxAnalyzer::readSubProduction(ProductionResult& io_production)
{
  switch (token().m_token_type)
  {
  case E_TT_NONE:
    return;

  case E_TT_TAG:
    if (TAG_OPEN == token().m_lexem)
    {
      io_production.push_back(output(
        "Program starts. Rules #100 applied. Changing to the rule 101.\n"));
    }

    if (TAG_CLOSE == token().m_lexem)
    {
      io_production.push_back(output(
        "Program ends. Rules #100 applied.\n"));
    }
    break;

  case E_TT_KEYWORD:
    readKeywordProduction(io_production);
    break;


  default:
    io_production.push_back(output(
      "Something has gone wrong."));
    break;
  }

  return;
}

bool SyntaxAnalyzer::readKeywordProduction(ProductionResult& io_production)
{
  SCOPED_DEPTH_METER

  io_production.push_back(output(
    "KEYWORDS STARTS. Rules #101 applied. Expect one of keyword.\n"));

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

  io_production.push_back(output(
    "FUNCTION STARTS. Rules #102 applied. Read IDENTIFIER.\n"));

  Identifier function_name;
  if (!readIdentifier(function_name))
    return false;
  
  Delimiter open_round_bracket;
  if (!readDelimiter(open_round_bracket))
  {
    io_production.push_back(output(
      "FUNCTION PARSING FAILED. Rules #102 cannot be applied. Current token is not IDENTIFIER.\n"));
    return false;
  }

  if (BRACKET_ROUND_OPEN != open_round_bracket)
  {
    io_production.push_back(output(
      "FUNCTION PARSING FAILED. Rules #102 cannot be applied. Current token is IDENTIFIER but not (.\n"));
    return false;
  }
  
  ArgumentList list;
  readArgumentList(list);

  Delimiter close_round_bracket;
  if (!readDelimiter(close_round_bracket))
  {
    io_production.push_back(output(
      "FUNCTION PARSING FAILED. Rules #102 cannot be applied. Current token is not IDENTIFIER.\n"));
    return false;
  }

  if (BRACKET_ROUND_CLOSE != close_round_bracket)
  {
    io_production.push_back(output(
      "FUNCTION PARSING FAILED. Rules #102 cannot be applied. Current token is IDENTIFIER but not ).\n"));
    return false;
  }

  // {} or ;

  return true;
}

bool SyntaxAnalyzer::readIdentifier(Identifier& io_identifier)
{
  SCOPED_DEPTH_METER

  if (!next())
    return false;

  if (E_TT_IDENTIFIER == token().m_token_type)
  {
    io_identifier = token().m_lexem;
    return true;
  }

  prev();
  return false;
}

//bool SyntaxAnalyzer::readIdentifier(ProductionResult& io_production)
//{
//  SCOPED_DEPTH_METER
//
//  io_production.push_back(output(
//    "IDENTIFIER STARTS. Rules #103 ready to apply. Expect IDENTIFIER."));
//
//  if (!next())
//  {
//    io_production.push_back(output(
//      "UNEXPECTED END OF LEXEMS. RETURN OUT\n"));
//    return false;
//  }
//
//  if (E_TT_IDENTIFIER == token().m_token_type)
//  {
//    auto lexem = token().m_lexem;
//    io_production.push_back(output(
//      "IDENTIFIER STARTS. Rules #102 applied successfully. Result: " + lexem + "\n"));
//    return true;
//  }
//
//  io_production.push_back(output(
//    "UNEXPECTED ERROR. SyntaxAnalyzer::readIdentifier. RETURN OUT.\n"));
//  return false;
//}

bool SyntaxAnalyzer::readDelimiter(Delimiter& io_delimiter)
{
  SCOPED_DEPTH_METER

  if (!next())
    return false;

  if (E_TT_DELIMITER == token().m_token_type)
  {
    io_delimiter = token().m_lexem;
    return true;
  }

  prev();
  return false;
}

//bool SyntaxAnalyzer::readDelimiter(ProductionResult& io_production)
//{
//  SCOPED_DEPTH_METER
//
//  io_production.push_back(output(
//    "IDENTIFIER STARTS. Rules #102 ready to apply. Expect DELIMITER.\n"));
//
//  if (!next())
//  {
//    io_production.push_back(output(
//      "UNEXPECTED END OF LEXEMS. RETURN OUT\n"));
//    return false;
//  }
//
//  if (E_TT_DELIMITER == token().m_token_type)
//  {
//    auto lexem = token().m_lexem;
//    io_production.push_back(output(
//      "IDENTIFIER STARTS. Rules #102 applied successfully. Result: " + lexem + "\n"));
//    return true;
//  }
//
//  io_production.push_back(output(
//    "UNEXPECTED ERROR. SyntaxAnalyzer::readDelimiter. RETURN OUT.\n"));
//  return false;
//}

bool SyntaxAnalyzer::readArgumentList(ArgumentList& io_arguments_list)
{
  SCOPED_DEPTH_METER

  for (Argument temp; readArgument(temp);)
  {
    Delimiter coma;
    if (!readDelimiter(coma) || OPERATOR_COMA != coma)
      return false;
  }

  prev();
  return true;
}

bool SyntaxAnalyzer::readArgument(Argument& io_argument)
{
  return readIdentifier(io_argument);
}

const Token* SyntaxAnalyzer::next()
{
  return m_source.next();
}

Token  SyntaxAnalyzer::token() const
{
  return m_source.token();
}

const Token* SyntaxAnalyzer::prev()
{
  return m_source.prev();
}

QString SyntaxAnalyzer::output(QString&& str)
{
  return QString(m_depth, '\t') + str;
}

QString SyntaxAnalyzer::output(const QString& str)
{
  return QString(m_depth, '\t') + str;
}
