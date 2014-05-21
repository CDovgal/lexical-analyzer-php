/**@file   TEST_Lexical_Analyzer.cpp
* @brief   
* @date    Date Created:: 11/05/14 Dovgal Kostiantyn
*          $Date:: 2014-11-05 18:32#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#include <gtest\gtest.h>

#include <LexicalAnalyzer.h>
#include <Token.h>
#include <QFile>
#include <algorithm>

#include "TEST_BASE_Lexical_analyzer.h"

TEST(Lexical_analyzer, test_interface)
{
  QString source_path(".\\Data\\source.php");
  QFile source_file(source_path);
  if(!source_file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    ASSERT_TRUE(false) << "Source file " << source_path.toStdString() << " not found";
  }

  QString source_file_text("");
  while (!source_file.atEnd())
  {
    source_file_text += source_file.readLine();
  }

  QVector<Token> expected_tokens;
  expected_tokens.push_back(Token(E_TT_TAG,        "<?php",                2, 0));
  expected_tokens.push_back(Token(E_TT_KEYWORD,    "if",                   3, 0));
  expected_tokens.push_back(Token(E_TT_DELIMITER,  "(",                    3, 3));
  expected_tokens.push_back(Token(E_TT_IDENTIFIER, "$a",                   3, 4));
  expected_tokens.push_back(Token(E_TT_OPERATOR,   ">",                    3, 7));
  expected_tokens.push_back(Token(E_TT_IDENTIFIER, "$b",                   3, 9));
  expected_tokens.push_back(Token(E_TT_DELIMITER,  ")",                    3, 11));
  expected_tokens.push_back(Token(E_TT_KEYWORD,    "echo",                 4, 2));
  expected_tokens.push_back(Token(E_TT_CONSTEXPR,  "\"a bigger than b\"",  4, 7));
  expected_tokens.push_back(Token(E_TT_DELIMITER,  ";",                    4, 24));
  expected_tokens.push_back(Token(E_TT_TAG,        "?>",                   5, 0));

  QVector<Token> actual_tokens;
  LexicalAnalyzer analyzer(source_file_text);
  for (Token temp; analyzer.nextToken(temp);)
    actual_tokens.push_back(temp);

  ASSERT_EQ(expected_tokens.size(), actual_tokens.size())
    << "Mismatch of expected number of tokens and actual result";

  std::equal(expected_tokens.cbegin(), expected_tokens.cend(), 
               actual_tokens.cbegin(), [] (const Token& expected_token, const Token& actual_token) -> bool
    {
      EXPECT_EQ(expected_token, actual_token) << "Token mismatch\n";
      return true;
    }
  );
}


class TEST_nextToken : public TEST_ISource
{
public:
  TEST_nextToken() {}
  virtual ~TEST_nextToken() {}

  virtual void CommencePositive()
  {
    const auto& expected_tokens = init_data()->m_tokens;

    //TokensArray actual_tokens;
    //LexicalAnalyzer lex(source_code());
    //for (Token token; lex.nextToken(token);)
    //  actual_tokens.push_back(token);

    //ASSERT_EQ(expected_tokens.size(), actual_tokens.size())
    //  << "Mismatch of expected number of tokens and actual result";

    //std::equal(expected_tokens.cbegin(), expected_tokens.cend(),
    //  actual_tokens.cbegin(), [] (const Token& expected_token, const Token& actual_token) -> bool
    //  {
    //    EXPECT_EQ(expected_token, actual_token) << "Token mismatch\n";
    //    return true;
    //  }
    //);
  }
};

DECLARE_TESTS(nextToken)

DECLARE_SPECIFIC_TEST(nextToken, Positive)



static const PARAM_Lexical_analyzer a(".\\Data\\source.php", {
    Token(E_TT_TAG,           "<?php", 2, 0)
  , Token(E_TT_KEYWORD,       "if", 3, 0)
  , Token(E_TT_DELIMITER,     "(", 3, 3)
  , Token(E_TT_IDENTIFIER,    "$a", 3, 4)
  , Token(E_TT_OPERATOR,      ">", 3, 7)
  , Token(E_TT_IDENTIFIER,    "$b", 3, 9)
  , Token(E_TT_DELIMITER,     ")", 3, 11)
  , Token(E_TT_KEYWORD,       "echo", 4, 2)
  , Token(E_TT_CONSTEXPR,     "\"a bigger than b\"", 4, 7)
  , Token(E_TT_DELIMITER,     ";", 4, 24)
  , Token(E_TT_TAG,           "?>", 5, 0)
}, true);



static const PARAM_Lexical_analyzer* kFileParams_Positive[] = {
  &a
};

INSTANTIATE_TEST_CASE_P(nextToken,
  TEST_AUTO_PositivenextToken,
  ::testing::ValuesIn(kFileParams_Positive));
