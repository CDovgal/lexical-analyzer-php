/**@file   TEST_BASE_Lexical_Analyzer.cpp
* @brief
* @date    Date Created:: 21/05/14 Dovgal Kostiantyn
*          $Date:: 21-05-2014 09:37#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#include "TEST_BASE_Lexical_analyzer.h"

#include <LexicalAnalyzer.h>

#include <QFile>
#include <sstream>

PARAM_Lexical_analyzer::PARAM_Lexical_analyzer(const QString& i_filename, const TokensArray& i_tokens, bool i_file_existing)
  : m_filename(i_filename)
  , m_tokens(i_tokens)
  , m_is_exist(i_file_existing)
{}

TEST_Lexical_analyzer::TEST_Lexical_analyzer()
{
  SCOPED_TRACE(ToString());
}

const PARAM_Lexical_analyzer* TEST_Lexical_analyzer::init_data() const
{
  return GetParam();
}

QString TEST_Lexical_analyzer::filename() const
{
  return GetParam()->m_filename;
}

bool TEST_Lexical_analyzer::is_exist() const
{
  return GetParam()->m_is_exist;
}

const TokensArray& TEST_Lexical_analyzer::expectedTokens() const
{
  return GetParam()->m_tokens;
}

const QString& TEST_Lexical_analyzer::source_code() const
{
  return m_source_code;
}

void TEST_Lexical_analyzer::SetUp()
{
  Init();
}

void TEST_Lexical_analyzer::TearDown()
{

}

void TEST_Lexical_analyzer::Init()
{
  SCOPED_TRACE(ToString());
  QFile source_file(filename());
  if (!source_file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    if (is_exist())
      ASSERT_FALSE(true)
        << "\nFile " << filename().toStdString() << " was not found.\n";
  }

  m_source_code.clear();

  for (; !source_file.atEnd();)
    m_source_code += source_file.readLine();
}

std::string TEST_Lexical_analyzer::ToString() const
{
  std::stringstream str;
  str <<
    "\tFilename: " << filename().toStdString() <<
    "\nIs exist: " << (is_exist() ? "yes" : "no") <<
    "\nNumber of tokens: " << expectedTokens().length() << "\n";
  return str.str();
}
