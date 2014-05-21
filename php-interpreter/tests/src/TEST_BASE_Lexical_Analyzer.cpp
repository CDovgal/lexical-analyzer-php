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

PARAM_Lexical_analyzer::PARAM_Lexical_analyzer(const QString& i_filename, const TokensArray& i_tokens, bool i_file_existing)
  : m_filename(i_filename)
  , m_tokens(i_tokens)
  , m_is_exist(i_file_existing)
{}

TEST_Lexical_analyzer::TEST_Lexical_analyzer()
{

}

const PARAM_Lexical_analyzer* TEST_Lexical_analyzer::data() const
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

const TokensArray& TEST_Lexical_analyzer::ExpectedTokens() const
{
  return GetParam()->m_tokens;
}

const TokensArray& TEST_Lexical_analyzer::ActualTokens() const
{
  return m_actual_tokens;
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
  QFile source_file(filename());
  if (!source_file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    if (is_exist())
      ASSERT_FALSE(true)
        << "\nFile " << filename().toStdString() << " was not found.\n";
  }

  QString source_file_text;
  for (; !source_file.atEnd();)
    source_file_text += source_file.readLine();

  LexicalAnalyzer lex(source_file_text);
  for (Token token; lex.nextToken(token);)
    m_actual_tokens.push_back(token);

}

void TEST_Lexical_analyzer::Terminate()
{

}

std::string TEST_Lexical_analyzer::ToString() const
{
  return "ololo";
}
