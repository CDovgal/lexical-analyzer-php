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

class TEST_NextToken : public TEST_ISourceCode
{
public:
  TEST_NextToken() {}
  virtual ~TEST_NextToken() {}

  virtual void CommencePositive()
  {
    EXPECT_EQ(1, 1);

    TokensArray actual_tokens;
    LexicalAnalyzer lex(sourceCode());
    for (Token token; lex.nextToken(token);)
      actual_tokens.push_back(token);

    ASSERT_EQ(expectedTokens().size(), actual_tokens.size())
      << "Mismatch of expected number of tokens and actual result";

    std::equal(expectedTokens().cbegin(), expectedTokens().cend(),
      actual_tokens.cbegin(), [] (const Token& expected_token, const Token& actual_token) -> bool
      {
        EXPECT_EQ(expected_token, actual_token) << "Token mismatch\n";
        return true;
      }
    );
  }
};

DECLARE_TESTS(NextToken)

DECLARE_SPECIFIC_TEST(NextToken, Positive)

static const TokensArray tokensarray = {
  Token(E_TT_TAG, "<?php", 2, 0)
  , Token(E_TT_KEYWORD, "if", 3, 0)
  , Token(E_TT_DELIMITER, "(", 3, 3)
  , Token(E_TT_IDENTIFIER, "$a", 3, 4)
  , Token(E_TT_OPERATOR, ">", 3, 7)
  , Token(E_TT_IDENTIFIER, "$b", 3, 9)
  , Token(E_TT_DELIMITER, ")", 3, 11)
  , Token(E_TT_KEYWORD, "echo", 4, 2)
  , Token(E_TT_CONSTEXPR, R"("a bigger than b")", 4, 7)
  , Token(E_TT_DELIMITER, ";", 4, 24)
  , Token(E_TT_TAG, "?>", 5, 0)
};

static const PARAM_Lexical_analyzer a(".\\Data\\source.php", tokensarray, true);



static const PARAM_Lexical_analyzer* kFileParams_Positive[] = {
  &a
};

INSTANTIATE_TEST_CASE_P(NextToken,
  AUTOTEST_PositiveNextToken,
  ::testing::ValuesIn(kFileParams_Positive));
