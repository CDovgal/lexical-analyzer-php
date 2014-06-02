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

enum E_SA_STATE : int
{
    E_SA_NOT_INIT = 0
  , E_SA_TAG
  , E_SA_SUBREAD
};

QMap<QString, int> CODE = 
    {
    { "ERROR"                     ,   0 }
  , { "PROGRAM"                   ,   1 }
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
  , { KEYWORD_SWITCH              , 212 }
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
  , { "IDENTIFIER"                , 500 }
  , { "ARGUMENT"                  , 501 }
  , { "ARGUMENTSLIST"             , 502 }
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

  return result;
}

void SyntaxAnalyzer::readSubProduction(ProductionResult& io_production)
{
  INFO_MESSAGE_START("PROGRAM");

  for (; next();)
  {
    SCOPED_DEPTH_COUNTER;
    switch (token().m_token_type)
    {
    case E_TT_NONE:
      break;

    case E_TT_TAG:
      if (TAG_OPEN == token().m_lexem)
      {
        INFO_MESSAGE_START("TOKEN")
      }
      else
      {
        INFO_MESSAGE_FINISHED_SUCCESS("TOKEN")
      }
      break;

    case E_TT_KEYWORD:
      readKeywordProduction(io_production);
      break;

    case E_TT_IDENTIFIER:
      readExpression(io_production);
      if (m_state == E_SA_SUBREAD)
        return;
      break;

    default:
      INFO_MESSAGE("Something has gone wrong.");
      break;
    }
  }

  INFO_MESSAGE_FINISHED_SUCCESS("PROGRAM")
  return;
}

bool SyntaxAnalyzer::readKeywordProduction(ProductionResult& io_production)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("KEYWORD");

  auto lexem = token().m_lexem;
  bool result;

  m_state = E_SA_SUBREAD;

  if (KEYWORD_FUNCTION == lexem)
  {
    result = readFunction(io_production);
  }

  if (KEYWORD_IF == lexem)
  {
    result = readIf(io_production);
  }

  if (KEYWORD_FOR == lexem)
  {
    result = readFor(io_production);
  }

  if (KEYWORD_SWITCH == lexem)
  {
    result = readSwitch(io_production);
  }

  INFO_MESSAGE_FINISHED_SUCCESS("KEYWORD");
  m_state = E_SA_TAG;
  return false;
}

bool SyntaxAnalyzer::readFunction(ProductionResult& io_production)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("function");

  Identifier function_name;
  if (!readIdentifier(io_production, function_name))
  {
    INFO_MESSAGE_FINISHED_FAILED("function");
    return false;
  }
  
  Delimiter open_round_bracket;
  if (!readDelimiter(io_production, open_round_bracket))
  {
    INFO_MESSAGE_FINISHED_FAILED("function");
    return false;
  }

  if (BRACKET_ROUND_OPEN != open_round_bracket)
  {
    INFO_MESSAGE_FINISHED_FAILED("function");
    return false;
  }
  
  ArgumentList list;
  readArgumentList(io_production, list);

  Delimiter close_round_bracket;
  if (!readDelimiter(io_production, close_round_bracket) || BRACKET_ROUND_CLOSE != close_round_bracket)
  {
    INFO_MESSAGE_FINISHED_FAILED("function");
    return false;
  }
  
  Delimiter semicolon;
  if (readDelimiter(io_production, semicolon) && DELIMITER_SEMICOLON == semicolon)
  {
    INFO_MESSAGE_FINISHED_SUCCESS("function");
    return true;
  }
  
  prev();
  
  Delimiter open_figure_bracket;
  if (!readDelimiter(io_production, open_figure_bracket) || BRACKET_FIGURE_OPEN != open_figure_bracket)
  {
      INFO_MESSAGE_FINISHED_FAILED("function");
      return false;
  }
  
  //expression
  readSubProduction(io_production);
  
  Delimiter close_figure_bracket;
  if (!readDelimiter(io_production, close_figure_bracket) || BRACKET_FIGURE_CLOSE != close_figure_bracket)
  {
    INFO_MESSAGE_FINISHED_FAILED("function");
    return false;
  }
  
  INFO_MESSAGE_FINISHED_SUCCESS("function");
  return true;
}

bool SyntaxAnalyzer::readIf(ProductionResult& io_production)
{
  return false;
}

bool SyntaxAnalyzer::readFor(ProductionResult& io_production)
{
  return false;
}

bool SyntaxAnalyzer::readSwitch(ProductionResult& io_production)
{
  return false;
}

bool SyntaxAnalyzer::readIdentifier(ProductionResult& io_production, Identifier& io_identifier)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("IDENTIFIER");

  CHECK_NEXT_TOKEN;

  if (E_TT_IDENTIFIER == token().m_token_type)
  {
    io_identifier = token().m_lexem;
    INFO_MESSAGE_FINISHED_SUCCESS("IDENTIFIER");
    return true;
  }

  INFO_MESSAGE_DISMATCH_TOKEN("IDENTIFIER");

  prev();
  return false;
}

bool SyntaxAnalyzer::readDelimiter(ProductionResult& io_production, Delimiter& io_delimiter)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("DELIMITER");

  CHECK_NEXT_TOKEN;

  if (E_TT_DELIMITER == token().m_token_type)
  {
    io_delimiter = token().m_lexem;
    INFO_MESSAGE_FINISHED_SUCCESS("DELIMITER");
    return true;
  }

  INFO_MESSAGE_DISMATCH_TOKEN("DELIMITER");

  prev();
  return false;
}

bool SyntaxAnalyzer::readArgumentList(ProductionResult& io_production, ArgumentList& io_arguments_list)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("ARGUMENTSLIST");

  bool is_empty_list = true;
  for (Argument temp; readArgument(io_production, temp);)
  {
    is_empty_list = false;
    Delimiter coma;
    if (readDelimiter(io_production, coma))
    {
      if (BRACKET_ROUND_CLOSE == coma)
        break;

      if (DELIMITER_COMA == coma)
      {
        INFO_MESSAGE_RULE_SATISFIED("ARGUMENTSLIST", DELIMITER_COMA);
        continue;
      }
    }

    prev();
    INFO_MESSAGE_FINISHED_FAILED("ARGUMENTSLIST");
    return false;
  }

  if(!is_empty_list)
    prev();
  INFO_MESSAGE_FINISHED_SUCCESS("ARGUMENTSLIST");

  return true;
}

bool SyntaxAnalyzer::readArgument(ProductionResult& io_production, Argument& io_argument)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("ARGUMENT");

  CHECK_NEXT_TOKEN;

  if (E_TT_IDENTIFIER == token().m_token_type)
  {
    io_argument = token().m_lexem;
    INFO_MESSAGE_FINISHED_SUCCESS("ARGUMENT");
    return true;
  }

  INFO_MESSAGE_DISMATCH_TOKEN("ARGUMENT");

  prev();
  return false;
}

bool SyntaxAnalyzer::readOperator(ProductionResult& io_production, Operator& io_operator)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("OPERATOR");

  CHECK_NEXT_TOKEN;

  if (E_TT_OPERATOR == token().m_token_type)
  {
    io_operator = token().m_lexem;
    INFO_MESSAGE_FINISHED_SUCCESS("OPERATOR");
    return true;
  }

  INFO_MESSAGE_DISMATCH_TOKEN("OPERATOR");

  prev();
  return false;
}

bool SyntaxAnalyzer::readConstExpr(ProductionResult& io_production, ConstExpr& io_constexpr)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("CONSTEXPR");

  CHECK_NEXT_TOKEN;

  if (E_TT_CONSTEXPR == token().m_token_type)
  {
    io_constexpr = token().m_lexem;
    INFO_MESSAGE_FINISHED_SUCCESS("CONSTEXPR");
    return true;
  }

  INFO_MESSAGE_DISMATCH_TOKEN("CONSTEXPR");

  prev();
  return false;
}

bool SyntaxAnalyzer::readExpression(ProductionResult& io_production)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("EXPRESSION");

  if (E_TT_IDENTIFIER != token().m_token_type)
  {
    INFO_MESSAGE_DISMATCH_TOKEN("IDENTIFIER");
    return false;
  }

  INFO_MESSAGE_RULE_SATISFIED("IDENTIFIER", token().m_lexem);

  bool is_ok = true;
  for (; is_ok;)
  {
    Operator operation;
    if (!readOperator(io_production, operation))//  E_TT_OPERATOR != token().m_token_type)
    {
      INFO_MESSAGE_DISMATCH_TOKEN("OPERATOR");
      is_ok = false;
//      prev();
      break;
    }

    INFO_MESSAGE_RULE_SATISFIED("OPERATOR", token().m_lexem);

    ConstExpr constexpR;
    Identifier identifier;
    if (readConstExpr(io_production, constexpR))
    {
      INFO_MESSAGE_RULE_SATISFIED("CONSTEXPRESSION", constexpR);
    }
    else if (readIdentifier(io_production, identifier))
    {
      INFO_MESSAGE_RULE_SATISFIED("IDENTIFIER", identifier);
    }
    else
    {
      INFO_MESSAGE_DISMATCH_TOKEN("CONSTEXPRESSION");
      INFO_MESSAGE_DISMATCH_TOKEN("IDENTIFIER");
      INFO_MESSAGE_FINISHED_FAILED("EXPRESSION");
      is_ok = false;
//      prev();
      break;
    }
  }

  Delimiter semicolon;
  if (!readDelimiter(io_production, semicolon) || DELIMITER_SEMICOLON != semicolon)
  {
    INFO_MESSAGE_DISMATCH_TOKEN("DELIMITER");
    INFO_MESSAGE_FINISHED_FAILED("EXPRESSION");
    return false;
  }

  INFO_MESSAGE_FINISHED_SUCCESS("EXPRESSION");

  return true;
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

QString info_message_token_ends()
{
  return "Appling rule failed. Tokens are ended.";
}

QString info_message_start(const QString& code_key)
{
  return QString(
    "%1 parsing starts. Rule #%2 ready to apply.")
    .arg(code_key)
    .arg(CODE[code_key]);
}

QString info_message_finished_success(const QString& code_key)
{
  return QString(
    "Rule #%1 applied. %2 successfully read.")
    .arg(CODE[code_key])
    .arg(code_key);
}


QString info_message_dismatch_token(const QString& code_key)
{
  return QString(
  "Current token are not %1.")
  .arg(code_key);
}

QString info_message_rule_satisfied(const QString& code_key_rule,
                                    const QString& code_key_token)
{
  return QString(
    "Rule #%1 satisfied by %2 token.")
    .arg(CODE[code_key_rule])
    .arg(code_key_token);
}

QString info_message_finished_failed(const QString& code_key)
{
  return QString(
    "%1 parsing failed. Rule #%2 cannot be appled. UNEXPECTED ERROR.")
    .arg(code_key)
    .arg(CODE[code_key]);
}

QString info_message_wrong_token(const QString& code_key_expect, 
                                 const QString& code_key_actual)
{
  return QString(
    "Appling rule #%1 failed. Current token are not %2, but %3")
    .arg(CODE[code_key_expect])
    .arg(code_key_expect)
    .arg(code_key_actual);
}
