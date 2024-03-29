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
      if (KEYWORD_FUNCTION  == token().m_lexem ||
          KEYWORD_FOR       == token().m_lexem ||
          KEYWORD_IF        == token().m_lexem ||
          KEYWORD_SWITCH    == token().m_lexem)
      {
        readKeywordProduction(io_production);
        break;
      }
      prev();
      return;

    case E_TT_IDENTIFIER:
      if (token().m_lexem[0] != '$')
      {
        readCallFunc(io_production);
        Delimiter semicolon;
        if (!readDelimiter(io_production, semicolon) || DELIMITER_SEMICOLON != semicolon)
        {
          INFO_MESSAGE_FINISHED_FAILED("CALLFUNCTION");
        }
        break;
      }
      readExpression(io_production);
      if (E_SA_SUBREAD == m_state)
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
  
  readSubProduction(io_production);
  Triade triade = std::make_tuple("RETURN", "", "");
  Triade subtriade = std::make_tuple("", "", "");
  bool additional_triade = false;
  Keyword keyword_return;
  if (readKeyword(io_production, keyword_return) && KEYWORD_RETURN == keyword_return)
  {
    INFO_MESSAGE_RULE_SATISFIED("function", "return");
    
    bool is_ok = true;
    for (; is_ok;)
    {
      ConstExpr constexpR;
      Identifier identifier;
      if (readConstExpr(io_production, constexpR))
      {
        INFO_MESSAGE_RULE_SATISFIED("CONSTEXPRESSION", constexpR);
        additional_triade 
          ? std::get<1>(subtriade) += constexpR 
          : std::get<2>(subtriade) += constexpR;
      }
      else if (readIdentifier(io_production, identifier))
      {
        INFO_MESSAGE_RULE_SATISFIED("IDENTIFIER", identifier);
        additional_triade
          ? std::get<1>(subtriade) += identifier
          : std::get<2>(subtriade) += identifier;
      }
      else
      {
        INFO_MESSAGE_DISMATCH_TOKEN("CONSTEXPRESSION");
        INFO_MESSAGE_DISMATCH_TOKEN("IDENTIFIER");
      }
      
      Operator operation;
      Delimiter semicolon;
      if (readOperator(io_production, operation))
      {
        INFO_MESSAGE_RULE_SATISFIED("OPERATOR", operation);
        std::get<0>(subtriade) += operation;
        additional_triade = true;
      }
      else if (readDelimiter(io_production, semicolon) || DELIMITER_SEMICOLON == semicolon)
      {
        is_ok = false;
        INFO_MESSAGE_RULE_SATISFIED("DELIMITER", ";");
        break;
      }
      else
      {
        INFO_MESSAGE_DISMATCH_TOKEN(";");
        INFO_MESSAGE_FINISHED_FAILED("function");
        return false;
        is_ok = false;
        break;
      }
    }
  }
  if (additional_triade)
  {
    m_triades.push_back(subtriade);
    m_triades.push_back(std::make_tuple("RETURN", "(" + QString::number(m_triades.size()) + ")", ""));
  }
  else
  {
    m_triades.push_back(std::make_tuple("RETURN", std::get<2>(subtriade), ""));
  }

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
  SCOPED_DEPTH_COUNTER;
  Triade condition = std::make_tuple(QString(), QString(), QString());
  INFO_MESSAGE_START("if");

  Delimiter open_round_bracket;
  if (!readDelimiter(io_production, open_round_bracket) || BRACKET_ROUND_OPEN != open_round_bracket)
  {
    INFO_MESSAGE_FINISHED_FAILED("if");
    return false;
  }

  ConstExpr constexpR;
  Identifier identifier;
  if (readConstExpr(io_production, constexpR))
  {
    INFO_MESSAGE_RULE_SATISFIED("CONSTEXPRESSION", constexpR);
    std::get<2>(condition) = constexpR;
  }
  else if (readIdentifier(io_production, identifier))
  {
    INFO_MESSAGE_RULE_SATISFIED("IDENTIFIER", identifier);
    std::get<2>(condition) = identifier;
  }
  else
  {
    INFO_MESSAGE_DISMATCH_TOKEN("CONSTEXPRESSION");
    INFO_MESSAGE_DISMATCH_TOKEN("IDENTIFIER");
    INFO_MESSAGE_FINISHED_FAILED("EXPRESSION");
    return false;
  }

  Operator operation;
  if (readOperator(io_production, operation))
  {
    INFO_MESSAGE_RULE_SATISFIED("OPERATOR", operation);
    std::get<0>(condition) = operation;
    ConstExpr constexpR;
    Identifier identifier;
    if (readConstExpr(io_production, constexpR))
    {
      INFO_MESSAGE_RULE_SATISFIED("CONSTEXPRESSION", constexpR);
      std::get<1>(condition) = constexpR;
    }
    else if (readIdentifier(io_production, identifier))
    {
      INFO_MESSAGE_RULE_SATISFIED("IDENTIFIER", identifier);
      std::get<1>(condition) = identifier;
    }
    else
    {
      INFO_MESSAGE_DISMATCH_TOKEN("CONSTEXPRESSION");
      INFO_MESSAGE_DISMATCH_TOKEN("IDENTIFIER");
      INFO_MESSAGE_FINISHED_FAILED("EXPRESSION");
      return false;
    }
  }
  m_triades.push_back(condition);
  Delimiter close_round_bracket;
  if (!readDelimiter(io_production, close_round_bracket) || BRACKET_ROUND_CLOSE != close_round_bracket)
  {
    INFO_MESSAGE_FINISHED_FAILED("if");
    return false;
  }

  Delimiter open_figure_bracket;
  if (!readDelimiter(io_production, open_figure_bracket) || BRACKET_FIGURE_OPEN != open_figure_bracket)
  {
    INFO_MESSAGE_FINISHED_FAILED("if");
    return false;
  }
  int then_start = m_triades.size();
  
  readSubProduction(io_production);

  Delimiter close_figure_bracket;
  if (!readDelimiter(io_production, close_figure_bracket) || BRACKET_FIGURE_CLOSE != close_figure_bracket)
  {
    INFO_MESSAGE_FINISHED_FAILED("if");
    return false;
  }
  int else_start = m_triades.size();
  Keyword keyword_else;
  if (readKeyword(io_production, keyword_else))
  {
    if (KEYWORD_ELSE == keyword_else)
    {
      Delimiter open_figure_bracket;
      if (!readDelimiter(io_production, open_figure_bracket) || BRACKET_FIGURE_OPEN != open_figure_bracket)
      {
        INFO_MESSAGE_FINISHED_FAILED("if");
        return false;
      }

      readSubProduction(io_production);

      Delimiter close_figure_bracket;
      if (!readDelimiter(io_production, close_figure_bracket) || BRACKET_FIGURE_CLOSE != close_figure_bracket)
      {
        INFO_MESSAGE_FINISHED_FAILED("if");
        return false;
      }
    }
    prev();
  }
  int else_finish = m_triades.size();

  m_triades.insert(then_start, std::make_tuple("BRF", "(" + QString::number(else_start + 3) + ")", "(" + QString::number(then_start) + ")"));

  m_triades.insert(else_start + 1, std::make_tuple("BR", "(" + QString::number(else_finish + 3) + ")", ""));

  INFO_MESSAGE_FINISHED_SUCCESS("if");
  return true;
}

bool SyntaxAnalyzer::readFor(ProductionResult& io_production)
{
  SCOPED_DEPTH_COUNTER;
  
  INFO_MESSAGE_START("for");

  Delimiter open_round_bracket;
  if (!readDelimiter(io_production, open_round_bracket) || BRACKET_ROUND_OPEN != open_round_bracket)
  {
    //INFO_MESSAGE_FINISHED_FAILED("for");
    return false;
  }
  //
  next();
  int for_before_init = m_triades.size() + 1;
  readExpression(io_production);
  //Delimiter semicolon1;
  //if (!readDelimiter(io_production, semicolon1) || DELIMITER_SEMICOLON != semicolon1)
  //{
  //  //INFO_MESSAGE_FINISHED_FAILED("for");
  //  return false;
  //}
  //
  next();
  int for_before_condition = m_triades.size() + 1;
  readExpression(io_production);
  //Delimiter semicolon2;
  //if (!readDelimiter(io_production, semicolon2) || DELIMITER_SEMICOLON != semicolon2)
  //{
  //  //INFO_MESSAGE_FINISHED_FAILED("for");
  //  return false;
  //}
  //
  next();
  int for_before_post_action = m_triades.size() + 1;
  readExpression(io_production);
  Delimiter close_round_bracket;
  if (!readDelimiter(io_production, close_round_bracket) || BRACKET_ROUND_CLOSE != close_round_bracket)
  {
    //INFO_MESSAGE_FINISHED_FAILED("for");
    return false;
  }
  Triade condition = std::make_tuple(
    "Substitute instead of: (" + QString::number(for_before_init) + ")",
    "(" + QString::number(for_before_condition) + ")", 
    "(" + QString::number(for_before_post_action) + ")");
  Delimiter open_figure_bracket;
  if (!readDelimiter(io_production, open_figure_bracket) || BRACKET_FIGURE_OPEN != open_figure_bracket)
  {
    //INFO_MESSAGE_FINISHED_FAILED("for");
    return false;
  }
  m_triades.push_back(condition);
  int start_cycle = m_triades.size() + 1;
  readSubProduction(io_production);
  int end_cycle = m_triades.size() + 1;
  Delimiter close_figure_bracket;
  if (!readDelimiter(io_production, close_figure_bracket) || BRACKET_FIGURE_CLOSE != close_figure_bracket)
  {
    //INFO_MESSAGE_FINISHED_FAILED("for");
    return false;
  }
  Triade body = std::make_tuple("Substitute (" + QString::number(for_before_post_action + 1) + ")",
    "(" + QString::number(start_cycle) + ")",
    "(" + QString::number(end_cycle) + ")");
  m_triades.push_back(body);
  INFO_MESSAGE_FINISHED_SUCCESS("for");
  return true;
}

bool SyntaxAnalyzer::readSwitch(ProductionResult& io_production)
{
  return false;
}

bool SyntaxAnalyzer::readKeyword(ProductionResult& io_production, Keyword& io_keyword)
{
  SCOPED_DEPTH_COUNTER;

  INFO_MESSAGE_START("KEYWORD");

  CHECK_NEXT_TOKEN;

  if (E_TT_KEYWORD == token().m_token_type)
  {
    io_keyword = token().m_lexem;
    INFO_MESSAGE_FINISHED_SUCCESS("KEYWORD");
    return true;
  }

  INFO_MESSAGE_DISMATCH_TOKEN("KEYWORD");

  prev();
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
    io_arguments_list.push_back(temp);
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

  Triade triade = std::make_tuple(QString(), QString(), QString());

  INFO_MESSAGE_START("EXPRESSION");

  if (E_TT_IDENTIFIER != token().m_token_type)
  {
    INFO_MESSAGE_DISMATCH_TOKEN("IDENTIFIER");
    return false;
  }
  std::get<2>(triade) = token().m_lexem;
  INFO_MESSAGE_RULE_SATISFIED("IDENTIFIER", token().m_lexem);

  bool is_ok = true;
  for (; is_ok;)
  {
    Operator operation;
    if (!readOperator(io_production, operation))
    {
      INFO_MESSAGE_DISMATCH_TOKEN("OPERATOR");
      is_ok = false;
      break;
    }

    INFO_MESSAGE_RULE_SATISFIED("OPERATOR", token().m_lexem);
    std::get<0>(triade) = operation;

    ConstExpr constexpR;
    Identifier identifier;
    if (readConstExpr(io_production, constexpR))
    {
      INFO_MESSAGE_RULE_SATISFIED("CONSTEXPRESSION", constexpR);
      std::get<1>(triade) = constexpR;
    }
    else if (readIdentifier(io_production, identifier))
    {
      if (identifier[0]!= '$' && readCallFunc(io_production))
      {
        INFO_MESSAGE_RULE_SATISFIED("FUNCTIONCALL", identifier);
      }
      else
      {
        INFO_MESSAGE_RULE_SATISFIED("IDENTIFIER", identifier);
      }
      std::get<1>(triade) = identifier;
    } 
    else
    {
      INFO_MESSAGE_DISMATCH_TOKEN("CONSTEXPRESSION");
      INFO_MESSAGE_DISMATCH_TOKEN("IDENTIFIER");
      INFO_MESSAGE_FINISHED_FAILED("EXPRESSION");
      is_ok = false;
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
  m_triades.push_back(triade);
  return true;
}

bool SyntaxAnalyzer::readCallFunc(ProductionResult& io_production)
{
  SCOPED_DEPTH_COUNTER;

  Triade triade = std::make_tuple("CALL", token().m_lexem, "");

  INFO_MESSAGE_START("CALLFUNCTION");

  Delimiter open_round_bracket;
  if (!readDelimiter(io_production, open_round_bracket) || BRACKET_ROUND_OPEN != open_round_bracket)
  {
    INFO_MESSAGE_FINISHED_FAILED("CALLFUNCTION");
    return false;
  }

  ArgumentList list;
  readArgumentList(io_production, list);

  Delimiter close_round_bracket;
  if (!readDelimiter(io_production, close_round_bracket) || BRACKET_ROUND_CLOSE != close_round_bracket)
  {
    INFO_MESSAGE_FINISHED_FAILED("CALLFUNCTION");
    return false;
  }
  std::get<2>(triade) = list.join(", ");
  m_triades.push_back(triade);
  INFO_MESSAGE_FINISHED_SUCCESS("CALLFUNCTION");

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

TriadesContainer SyntaxAnalyzer::triades() const
{
  return m_triades;
}