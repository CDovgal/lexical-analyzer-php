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


typedef QStringList ProductionResult;

typedef QString Identifier;
typedef QString Delimiter;
typedef QString Variable;
typedef QString Argument;

typedef QStringList ArgumentList;

enum SA_State : int;

class SyntaxAnalyzer
{
public:
  SyntaxAnalyzer(const TokenSource& i_token_iter);
  
  ProductionResult readProduction();

  bool readNextToken(const Token& i_token, QString& o_info);

private:
  
  Token* next();
  Token  token() const;
  Token* prev();
  
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
};

#define SCOPED_DEPTH_METER \
ScopedDepthMeter scope(m_depth);
