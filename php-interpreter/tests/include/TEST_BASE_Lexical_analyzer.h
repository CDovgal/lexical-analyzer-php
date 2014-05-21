/**@file   TEST_BASE_Lexical_Analyzer.h
* @brief
* @date    Date Created:: 21/05/14 Dovgal Kostiantyn
*          $Date:: 21-05-2014 09:10#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#pragma once

#include <gtest\gtest.h>

#include <QString>
#include <QVector>

#include <Token.h>

typedef QVector<Token> TokensArray;

class PARAM_Lexical_analyzer
{
public:
  PARAM_Lexical_analyzer(const QString& i_filename, const TokensArray& i_tokens, bool i_file_existing = true);
  
  QString m_filename;
  const TokensArray& m_tokens;
  bool m_is_exist;
};

class TEST_Lexical_analyzer : public ::testing::TestWithParam< const PARAM_Lexical_analyzer* >
{
public:
  TEST_Lexical_analyzer();

  QString filename() const;
  bool is_exist() const;

  const TokensArray& ExpectedTokens() const;
  const TokensArray& ActualTokens() const;

  virtual void SetUp();
  virtual void TearDown();

private:

  void Init();

  TokensArray m_actual_tokens;
};
