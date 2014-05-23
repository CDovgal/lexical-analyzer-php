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

std::string PARAM_Lexical_analyzer::toStdString() const
{
  std::stringstream str;
#ifdef  _NDEBUG
  str <<
    "\tFilename: " << m_filename.toStdString()    << std::endl <<
    "\nIs exist: " << (m_is_exist ? "yes" : "no") <<
    "\nNumber of tokens: "  << m_tokens.length()  << '\n'; 
#endif //  _DEBUG

  return str.str();
}

TEST_Lexical_analyzer::TEST_Lexical_analyzer()
{}

void TEST_Lexical_analyzer::Init()
{ 
  QFile source_file(filename());
  if (!source_file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    if (isFileExist())
      ASSERT_FALSE(true)
      << "\nFile " << toStdString() << " was not found.\n";
  }

  m_source_code.clear();

  for (; !source_file.atEnd();)
    m_source_code += source_file.readLine();
}

void TEST_Lexical_analyzer::SetUp()
{
  SCOPED_TRACE(GetParam()->toStdString());
  Init();
}

void TEST_Lexical_analyzer::TearDown()
{
  SCOPED_TRACE(GetParam()->toStdString());
}

std::string TEST_Lexical_analyzer::toStdString() const
{
  return GetParam()->toStdString();
}

bool TEST_Lexical_analyzer::isFileExist() const
{
  return GetParam()->m_is_exist;
}

QString TEST_Lexical_analyzer::filename() const
{
  return GetParam()->m_filename;
}

const QString& TEST_Lexical_analyzer::sourceCode() const
{
  return m_source_code;
}

const TokensArray& TEST_Lexical_analyzer::expectedTokens() const
{
  return GetParam()->m_tokens;
}
