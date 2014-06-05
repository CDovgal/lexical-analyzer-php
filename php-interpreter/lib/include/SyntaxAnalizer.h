/**@file   SyntaxAnalyzer.h
* @brief
* @date    Date Created:: 26/05/14 Dovgal Kostiantyn
*          $Date:: 26-05-2014 09:10#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#pragma once

#include "Token.h"
#include <QStack>
#include <functional>
#include <QStringList>
#include "TokenSource.h"

PHP_LIB_API typedef QString     Production;
PHP_LIB_API typedef QStringList ProductionResult;

PHP_LIB_API typedef QString     Argument;
PHP_LIB_API typedef QStringList ArgumentList;

PHP_LIB_API typedef QString     Variable;
PHP_LIB_API typedef QString     Delimiter;
PHP_LIB_API typedef QString     Identifier;

PHP_LIB_API typedef QString     ConstExpr;

PHP_LIB_API typedef QString     Operator;
PHP_LIB_API typedef QString     Keyword;

enum E_SA_STATE : int;

class PHP_LIB_API SyntaxAnalyzer
{
public:
  SyntaxAnalyzer(const TokenSource& i_token_iter);
  
  ProductionResult readProduction();

private:

  const Token* next ();
  Token  token() const;
  const Token* prev ();
  
  void readSubProduction(ProductionResult& io_production);
  
  bool readKeywordProduction(ProductionResult& io_production);
  
  bool readFunction(ProductionResult& io_production);

  bool readIf(ProductionResult& io_production);

  bool readFor(ProductionResult& io_production);

  bool readSwitch(ProductionResult& io_production);
  
  bool readIdentifier(ProductionResult& io_production, Identifier& io_identifier);
  
  bool readDelimiter(ProductionResult& io_production, Delimiter& io_delimiter);
  
  bool readArgument(ProductionResult& io_production, Argument& io_argument);
  bool readArgumentList(ProductionResult& io_production, ArgumentList& io_arguments_list);

  bool readOperator(ProductionResult& io_production, Operator& io_operator);
  bool readConstExpr(ProductionResult& io_production, ConstExpr& io_constexpr);

  bool readExpression(ProductionResult& io_production);
  
  bool readVariable(Variable& io_variable);

  bool readKeyword(ProductionResult& io_production, Keyword& io_keyword);
  
  QString output(QString&& str);
  QString output(const QString& str);
  
  TokenSource m_source;
  
  class ScopedDepthMeter
  {
  public:
    ScopedDepthMeter(int& i_depth) : ref_depth(i_depth) {++ref_depth;}
    ~ScopedDepthMeter() {--ref_depth;}
    
  private:
    int& ref_depth;
  };
    
  int m_depth;

  bool m_hack;

  E_SA_STATE m_state;
  
  QVector<Triade> m_triedes;
};

#define SCOPED_DEPTH_COUNTER \
  ScopedDepthMeter scope(m_depth);

#define INFO_MESSAGE(code_key) \
  io_production.push_back(output(code_key));

#define INFO_MESSAGE_START(code_key) \
  io_production.push_back(output(info_message_start(code_key)));

#define INFO_MESSAGE_TOKEN_ENDS \
  io_production.push_back(output(info_message_token_ends()));

#define INFO_MESSAGE_FINISHED_SUCCESS(code_key) \
  io_production.push_back(output(info_message_finished_success(code_key)));

#define INFO_MESSAGE_DISMATCH_TOKEN(code_key) \
  io_production.push_back(output(info_message_dismatch_token(code_key)));

#define INFO_MESSAGE_RULE_SATISFIED(code_key_rule, code_key_token) \
  io_production.push_back(output(info_message_rule_satisfied(code_key_rule, code_key_token)));

#define INFO_MESSAGE_FINISHED_FAILED(code_key) \
  io_production.push_back(output(info_message_finished_failed(code_key)));

#define INFO_MESSAGE_WRONG_TOKEN(code_key_expected, code_key_actual) \
  io_production.push_back(output(info_message_wrong_token(code_key_expected, code_key_actual)));

#define SET_SCOPE_NAME(arg) \
  QString SCOPE_NAME = arg;


#define CHECK_NEXT_TOKEN \
if (!next()) \
{ \
  INFO_MESSAGE(info_message_token_ends())\
  return false; \
}

QString info_message_token_ends();
QString info_message_start            (const QString& code_key);
QString info_message_finished_success (const QString& code_key);
QString info_message_dismatch_token   (const QString& code_key);
QString info_message_rule_satisfied   (const QString& code_key_rule,
                                       const QString& code_key_token);
QString info_message_wrong_token      (const QString& code_key_expect, 
                                       const QString& code_key_actual);
QString info_message_finished_failed  (const QString& code_key);

