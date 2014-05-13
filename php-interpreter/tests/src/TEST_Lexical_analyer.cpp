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

TEST(Lexical_analyzer, test_interface)
{
  QString source_path(".\\Data\\source.txt");
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
  expected_tokens.push_back(Token(E_TT_IDENTIFIER, "$a",                   3, 5));
  expected_tokens.push_back(Token(E_TT_OPERATOR,   ">",                    3, 8));
  expected_tokens.push_back(Token(E_TT_IDENTIFIER, "$b",                   3, 10));
  expected_tokens.push_back(Token(E_TT_DELIMITER,  ")",                    3, 12));
  expected_tokens.push_back(Token(E_TT_KEYWORD,    "echo",                 4, 3));
  expected_tokens.push_back(Token(E_TT_CONSTEXPR,  "\"a bigger than b\"",  4, 8));
  expected_tokens.push_back(Token(E_TT_TAG,        "?>",                   5, 0));

  QVector<Token> actual_tokens;
  LexicalAnalyzer analyzer(source_file_text);
  for(Token temp; analyzer.nextToken(temp); )
    actual_tokens.push_back(temp);

  ASSERT_EQ(expected_tokens.size(), actual_tokens.size())
    << "Mismatch of expected number of tokens and actual result";

  std::equal(expected_tokens.cbegin(), expected_tokens.cend(), 
              actual_tokens.cbegin(), [] (const Token& expected_token, const Token& actual_token) -> bool
  //for(auto iter1 = expected_tokens.cbegin(), iter2 = actual_tokens.cbegin(); iter1 != expected_tokens.cend(); ++iter1, ++iter2)
  {
    //EXPECT_EQ(*iter1, *iter2) << "Token mismatch: " << *iter1 << "not equal to " << *iter2;
    EXPECT_EQ(expected_token, actual_token) << "Token mismatch\n";// << lhv << "\nnot equal to " << rhv << '\n';
    return true;
  }
  );
}
