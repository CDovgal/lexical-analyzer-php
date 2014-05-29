/**@file   SyntaxAnalyzer.cpp
* @brief
* @date    Date Created:: 27/05/14 Dovgal Kostiantyn
*          $Date:: 27-05-2014 22:10#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#include "SyntaxAnalizer.h"
#include "LA_Aux.h"

#include <QMap>

QMap<QString, int> CODE = 
    {
    { "ERROR"                     ,   0 }
  , { "TAG"                       , 100 }
  , { TAG_OPEN                    , 101 }
  , { TAG_CLOSE                   , 102 }
  , { "KEYWORD"                   , 200 }
  , { KEYWORD_DEFINE              , 201 }
  , { KEYWORD_INCLUDE             , 202 }
  , { KEYWORD_NULL                , 203 }
  , { KEYWORD_FUNCTION            , 204 }
  , { KEYWORD_RETURN              , 205 }
  , { KEYWORD_FOR                 , 206 }
  , { KEYWORD_CONTINUE            , 207 }
  , { KEYWORD_BREAK               , 208 }
  , { KEYWORD_IF                  , 209 }
  , { KEYWORD_ELSE_IF             , 210 }
  , { KEYWORD_ELSE                , 211 }
  , { KEYWORD_SWICTH              , 212 }
  , { KEYWORD_CASE                , 213 }
  , { KEYWORD_DEFAULT             , 214 }
  , { KEYWORD_ENDSWITCH           , 215 }
  , { KEYWORD_ECHO                , 216 }
  , { "DELIMITER"                 , 300 }
  , { BRACKET_ROUND_OPEN          , 301 }
  , { BRACKET_ROUND_CLOSE         , 302 }
  , { BRACKET_SQUARE_OPEN         , 303 }
  , { BRACKET_SQUARE_CLOSE        , 304 }
  , { BRACKET_FIGURE_OPEN         , 305 }
  , { BRACKET_FIGURE_CLOSE        , 306 }
  , { DELIMITER_SEMICOLON         , 307 }
  , { DELIMITER_DOT               , 308 }
  , { DELIMITER_COMA              , 309 }
  , { "OPERATOR"                  , 400 }
  , { OPERATOR_LOGICAL_AND        , 401 }
  , { OPERATOR_LOGICAL_XOR        , 402 }
  , { OPERATOR_IDENTICAL          , 403 }
  , { OPERATOR_NOT_IDENTICAL      , 404 }
  , { OPERATOR_LOGICAL_OR         , 405 }
  , { OPERATOR_EQUAL              , 406 }
  , { OPERATOR_NOT_EQUAL_         , 407 }
  , { OPERATOR_NOT_EQUAL_BRACKET  , 408 }
  , { OPERATOR_AND                , 409 }
  , { OPERATOR_OR                 , 410 }
  , { OPERATOR_SHIFT_LEFT         , 411 }
  , { OPERATOR_SHIFT_RIGHT        , 412 }
  , { OPERATOR_LESS_OR_EQUAL      , 413 }
  , { OPERATOR_GREATER_OR_EQUAL   , 414 }
  , { OPERATOR_INCREMENT          , 415 }
  , { OPERATOR_DECREMENT          , 416 }
  , { OPERATOR_ASSIGNMENT_PLUS    , 417 }
  , { OPERATOR_ASSIGNMENT_MINUS   , 418 }
  , { OPERATOR_ASSIGNMENT_MULTIPLY, 419 }
  , { OPERATOR_ASSIGNMENT_DIVISION, 420 }
  , { OPERATOR_ASSIGNMENT_MOD2    , 421 }
  , { OPERATOR_NOT                , 422 }
  , { OPERATOR_ASSIGNMENT         , 423 }
  , { OPERATOR_PLUS               , 424 }
  , { OPERATOR_MINUS              , 425 }
  , { OPERATOR_MULTIPLY           , 426 }
  , { OPERATOR_DIVISION           , 427 }
  , { OPERATOR_PERCENTAGE         , 428 }
  , { OPERATOR_BIT_AND            , 429 }
  , { OPERATOR_BIT_OR             , 430 }
  , { OPERATOR_BIT_XOR            , 431 }
  , { OPERATOR_BIT_NOT            , 432 }
  , { OPERATOR_LESS               , 433 }
  , { OPERATOR_GREATER            , 434 }
  , { OPERATOR_TERNARY_QUESTION   , 435 }
  , { OPERATOR_TERNARY_DOUBLE_DOT , 436 }
                                        };

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

      if (DELIMITER_COMA == coma)
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

bool SyntaxAnalyzer::readOperator(ProductionResult& io_production, Operator& io_operator)
{
  SCOPED_DEPTH_COUNTER

  io_production.push_back(output(
    "OPERATOR PARSING STARTS. Rules #nnn appling. Read OPERATOR."));

  if (!next())
  {
    io_production.push_back(output(
      "Appling rules #nnn failed. Tokens are ended."));
    return false;
  }

  if (E_TT_IDENTIFIER == token().m_token_type)
  {
    io_operator = token().m_lexem;
    io_production.push_back(output(
      "Appling rules #nnn. OPERATOR successfully read."));
    return true;
  }

  io_production.push_back(output(
    "Appling rules #111 failed. Current token are not OPERATOR, but " + toString(token().m_token_type)));

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
