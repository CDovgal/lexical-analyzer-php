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
  virtual ~PARAM_Lexical_analyzer() {}

  virtual std::string toStdString() const;

  bool m_is_exist;
  QString m_filename;
  TokensArray m_tokens;
};

class TEST_ISourceCode
{
public:
  virtual ~TEST_ISourceCode() {};

  virtual void Init() = 0;

  virtual bool isFileExist() const = 0;
  virtual QString filename() const = 0;
  virtual const QString& sourceCode() const = 0;
  virtual const TokensArray& expectedTokens() const = 0;
};

class TEST_Lexical_analyzer : public ::testing::TestWithParam< const PARAM_Lexical_analyzer* >, public TEST_ISourceCode
{
public:
  TEST_Lexical_analyzer();
  virtual ~TEST_Lexical_analyzer() {};

  virtual void Init();

  virtual bool isFileExist() const;
  virtual QString filename() const;
  virtual const QString& sourceCode() const;
  virtual const TokensArray& expectedTokens() const;

  virtual void SetUp();
  virtual void TearDown();

  virtual std::string toStdString() const;

private:
  QString m_source_code;
};

#define DECLARE_TESTS(TESTNAME) \
class AUTOTEST_##TESTNAME : public TEST_Lexical_analyzer, public TEST_##TESTNAME{ \
public: \
  AUTOTEST_##TESTNAME() {} \
  virtual ~AUTOTEST_##TESTNAME() {} \
  virtual void Init() { TEST_Lexical_analyzer::Init(); } \
  virtual bool isFileExist() const{ return TEST_Lexical_analyzer::isFileExist(); }; \
  virtual QString filename() const{ return TEST_Lexical_analyzer::filename(); }; \
  virtual const QString& sourceCode() const{ return TEST_Lexical_analyzer::sourceCode(); }; \
  virtual const TokensArray& expectedTokens() const{ return TEST_Lexical_analyzer::expectedTokens(); }; \
};

#define DECLARE_SPECIFIC_TEST(TESTNAME, TYPENAME) \
  typedef AUTOTEST_##TESTNAME AUTOTEST_##TYPENAME##TESTNAME; \
  TEST_P(AUTOTEST_##TYPENAME##TESTNAME, ##TYPENAME) {\
  SCOPED_TRACE(toStdString()); \
  AUTOTEST_##TESTNAME::Commence##TYPENAME(); \
  }
