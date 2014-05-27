/**@file   TEST_Lexical_Analyzer.h
* @brief
* @date    Date Created:: 22/05/14 Dovgal Kostiantyn
*          $Date:: 22-05-2014 18:32#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#pragma once

#include <gtest/gtest.h>

#include <Token.h>
#include <LexicalAnalyzer.h>
#include <algorithm>

#include "TEST_BASE_Lexical_analyzer.h"

class TEST_NextToken : public TEST_ISourceCode
{
public:
  TEST_NextToken() {}
  virtual ~TEST_NextToken() {}

  virtual void CommencePositive()
  {
    TokensArray actual_tokens;
    LexicalAnalyzer lex(sourceCode());
    for (Token token; lex.nextToken(token);)
      actual_tokens.push_back(token);

    //ASSERT_EQ(expectedTokens().size(), actual_tokens.size())
    //  << "\nMismatch of expected number of tokens and actual result";

    std::equal(expectedTokens().cbegin(), expectedTokens().cend(),
      actual_tokens.cbegin(), [](const Token& expected_token, const Token& actual_token) -> bool
    {
      EXPECT_EQ(expected_token,
        actual_token) << "\nToken mismatch\n";
      return true;
    }
    );
  }
};

DECLARE_TESTS(NextToken)
DECLARE_SPECIFIC_TEST(NextToken, Positive)
