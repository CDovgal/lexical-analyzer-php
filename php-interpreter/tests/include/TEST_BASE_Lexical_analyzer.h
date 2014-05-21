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
#include <string>

typedef QVector<Token> TokensArray;

class PARAM_Lexical_analyzer
{
public:
  PARAM_Lexical_analyzer(const QString& i_filename, const TokensArray& i_tokens, bool i_file_existing = true);
  
  QString m_filename;
  const TokensArray& m_tokens;
  bool m_is_exist;
};

class TEST_ISource
{
public:
  virtual void Init() = 0;

  virtual const PARAM_Lexical_analyzer* init_data() const = 0;
  virtual const QString& source_code() const = 0;
};

class TEST_Lexical_analyzer : public ::testing::TestWithParam< const PARAM_Lexical_analyzer* >, public TEST_ISource
{
public:
  TEST_Lexical_analyzer();

  virtual const PARAM_Lexical_analyzer* init_data() const;

  QString filename() const;
  bool is_exist() const;
  const TokensArray& expectedTokens() const;

  virtual const QString& source_code() const;

  virtual void Init();

  virtual void SetUp();
  virtual void TearDown();

  virtual std::string ToString() const;

private:
  QString m_source_code;
};

#define DECLARE_TESTS(TESTNAME) \
class TEST_AUTO_##TESTNAME : public TEST_Lexical_analyzer, public TEST_##TESTNAME{ \
public: \
  TEST_AUTO_##TESTNAME() {} \
  virtual ~TEST_AUTO_##TESTNAME() {} \
  virtual void Init() { TEST_Lexical_analyzer::Init(); } \
  virtual const PARAM_Lexical_analyzer* init_data() const { return TEST_Lexical_analyzer::init_data(); } \
  virtual const QString& source_code() const { return TEST_Lexical_analyzer::source_code(); } \
};

#define DECLARE_SPECIFIC_TEST(TESTNAME, TYPENAME) \
  typedef TEST_AUTO_##TESTNAME TEST_AUTO_##TYPENAME##TESTNAME; \
  TEST_P(TEST_AUTO_##TYPENAME##TESTNAME, ##TYPENAME) {\
\
  SCOPED_TRACE(ToString()); \
  TEST_AUTO_##TESTNAME::Commence##TYPENAME(); \
  }
