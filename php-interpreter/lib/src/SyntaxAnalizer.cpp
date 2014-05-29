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
  for (; next(); )
  switch (token().m_token_type)
  {
  case E_TT_NONE:
    break;

  case E_TT_TAG:
    if (TAG_OPEN == token().m_lexem)
    {
      io_production.push_back(output(
        "Program starts.\nRules #100 applied. Changing to the rule 101."));
    }

    if (TAG_CLOSE == token().m_lexem)
    {
      io_production.push_back(output(
        "Rules #100 applied successfully.\nProgram ends."));
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
  SCOPED_DEPTH_COUNTER

  io_production.push_back(output(
    "KEYWORDS STARTS. Rules #101 appling. Expect one of keyword."));

  auto lexem = token().m_lexem;
  bool result;

  if (KEYWORD_FUNCTION == lexem)
  {
    result = readFunction(io_production);
  }



  io_production.push_back(output(
    "KEYWORDS STARTS. Rules #101 successfully applied."));

  return false;
}

bool SyntaxAnalyzer::readFunction(ProductionResult& io_production)
{
  SCOPED_DEPTH_COUNTER

  io_production.push_back(output(
    "FUNCTION PARSING STARTS. Rules #102 appling. Read IDENTIFIER."));

  Identifier function_name;
  if (!readIdentifier(io_production, function_name))
  {
    io_production.push_back(output(
      "FUNCTION PARSING FAILED. Appling rules #102 failed."));
    return false;
  }
  
  Delimiter open_round_bracket;
  if (!readDelimiter(io_production, open_round_bracket))
  {
    io_production.push_back(output(
      "FUNCTION PARSING FAILED. Appling rules #102 failed."));
    return false;
  }

  if (BRACKET_ROUND_OPEN != open_round_bracket)
  {
    io_production.push_back(output(
      "FUNCTION PARSING FAILED. Rules #102 cannot be applied. Current token is DELIMITER but not (."));
    return false;
  }
  
  ArgumentList list;
  readArgumentList(io_production, list);

  Delimiter close_round_bracket;
  if (!readDelimiter(io_production, close_round_bracket))
  {
    io_production.push_back(output(
      "FUNCTION PARSING FAILED. Rules #102 cannot be applied. Current token is not DELIMITER at all."));
    return false;
  }

  if (BRACKET_ROUND_CLOSE != close_round_bracket)
  {
    io_production.push_back(output(
      "FUNCTION PARSING FAILED. Rules #102 cannot be applied. Current token is DELIMITER but not )."));
    return false;
  }

  // {} or ;

  io_production.push_back(output(
    "FUNCTION PARSING FINISHED. Rules #102 successfully applied."));

  return true;
}

bool SyntaxAnalyzer::readIdentifier(ProductionResult& io_production, Identifier& io_identifier)
{
  SCOPED_DEPTH_COUNTER

  io_production.push_back(output(
    "IDENTIFICATOR PARSING STARTS. Rules #104 appling. Read IDENTIFIER."));

  if (!next())
  {
    io_production.push_back(output(
      "Appling rules #104 failed. Tokens are ended."));
    return false;
  }

  if (E_TT_IDENTIFIER == token().m_token_type)
  {
    io_identifier = token().m_lexem;
    io_production.push_back(output(
      "Appling rules #104. IDENTIFIER successfully read."));
    return true;
  }

  io_production.push_back(output(
    "Appling rules #104 failed. Current token are not IDENTIFIER, but " + toString(token().m_token_type)));

  prev();
  return false;
}

bool SyntaxAnalyzer::readDelimiter(ProductionResult& io_production, Delimiter& io_delimiter)
{
  SCOPED_DEPTH_COUNTER

  io_production.push_back(output(
    "DELIMITER PARSING STARTS. Rules #105 appling. Read DELIMITER."));

  if (!next())
  {
    io_production.push_back(output(
      "Appling rules #105 failed. Tokens are ended."));
    return false;
  }

  if (E_TT_DELIMITER == token().m_token_type)
  {
    io_delimiter = token().m_lexem;
    io_production.push_back(output(
      "Appling rules #105. DELIMITER successfully read."));
    return true;
  }

  io_production.push_back(output(
    "Appling rules #104 failed. Current token are not DELIMITER, but " + toString(token().m_token_type)));

  prev();
  return false;
}

bool SyntaxAnalyzer::readArgumentList(ProductionResult& io_production, ArgumentList& io_arguments_list)
{
  SCOPED_DEPTH_COUNTER

  io_production.push_back(output(
    "ARGUMENTS LIST PARSING STARTS. Rules #110 appling. Read ARGUMENTS LIST."));

  bool is_first = true;
  for (Argument temp; readArgument(io_production, temp);)
  {
    Delimiter coma;
    if (readDelimiter(io_production, coma))
    {
      if (BRACKET_ROUND_CLOSE == coma)
      {
        prev();
        return true;
      }

      if (OPERATOR_COMA == coma)
      {
        io_production.push_back(output(
          "ARGUMENTS LIST PARSING. Rules #110 appled. Delimiter is coma. OK."));
        continue;
      }
      else
      {
        io_production.push_back(output(
          "ARGUMENTS LIST PARSING FAILED. Rules can not be #110 appled. Delimiter is not coma, but" + coma));
        prev();
        return false;
      }
    }
    else
    {
      prev();
      io_production.push_back(output(
        "ARGUMENTS LIST PARSING FAILED. Rules #110 can not be appled. UNEXPECTED ERROR."));
      return false;
    }
  }

  io_production.push_back(output(
    "ARGUMENTS LIST PARSING ENDED. Rules #110 appled."));

  return true;
}

bool SyntaxAnalyzer::readArgument(ProductionResult& io_production, Argument& io_argument)
{
  SCOPED_DEPTH_COUNTER

  io_production.push_back(output(
    "ARGUMENT PARSING STARTS. Rules #111 appling. Read ARGUMENT."));

  if (!next())
  {
    io_production.push_back(output(
      "Appling rules #111 failed. Tokens are ended."));
    return false;
  }

  if (E_TT_IDENTIFIER == token().m_token_type)
  {
    io_argument = token().m_lexem;
    io_production.push_back(output(
      "Appling rules #111. ARGUMENT successfully read."));
    return true;
  }

  io_production.push_back(output(
    "Appling rules #111 failed. Current token are not ARGUMENT, but " + toString(token().m_token_type)));

  prev();
  return false;
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
