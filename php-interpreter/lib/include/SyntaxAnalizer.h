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

PHP_LIB_API typedef QString Variable;
PHP_LIB_API typedef QString Delimiter;
PHP_LIB_API typedef QString Identifier;

enum SA_State : int;

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
  
  bool readIdentifier(Identifier& io_identifier);
  
  bool readDelimiter(Delimiter& io_delimiter);
  
  bool readArgument(Argument& io_delimiter);
  
  bool readArgumentList(ArgumentList& io_delimiter);
  
  bool readVariable(Variable& io_variable);
  
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
};

#define SCOPED_DEPTH_METER \
ScopedDepthMeter scope(m_depth);
