/**@file   TEST_Lexical_Analyzer.cpp
* @brief   
* @date    Date Created:: 11/05/14 Dovgal Kostiantyn
*          $Date:: 2014-11-05 18:32#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#include "TEST_Lexical_Analyzer.h"

static const TokensArray tokensarray_source_1 = 
{
    Token(E_TT_TAG,         "<?php",                2, 0)
  , Token(E_TT_KEYWORD,     "if",                   3, 0)
  , Token(E_TT_DELIMITER,   "(",                    3, 3)
  , Token(E_TT_IDENTIFIER,  "$a",                   3, 4)
  , Token(E_TT_OPERATOR,    ">",                    3, 7)
  , Token(E_TT_IDENTIFIER,  "$b",                   3, 9)
  , Token(E_TT_DELIMITER,   ")",                    3, 11)
  , Token(E_TT_KEYWORD,     "echo",                 4, 2)
  , Token(E_TT_CONSTEXPR,  R"("a bigger than b")",  4, 7)
  , Token(E_TT_DELIMITER,   ";",                    4, 24)
  , Token(E_TT_TAG,         "?>",                   5, 0)
};

static const TokensArray tokensarray_source_2 = 
{
    Token(E_TT_TAG,         "<?php",                2, 0)
  , Token(E_TT_IDENTIFIER,  "$a",                   3, 0)
  , Token(E_TT_OPERATOR,    "=",                    3, 3)
  , Token(E_TT_CONSTEXPR,   "1",                    3, 5)
  , Token(E_TT_DELIMITER,   ";",                    3, 6)
  , Token(E_TT_IDENTIFIER,  "$b",                   4, 0)
  , Token(E_TT_OPERATOR,    "=",                    4, 3)
  , Token(E_TT_CONSTEXPR,   "3",                    4, 5)
  , Token(E_TT_DELIMITER,   ";",                    4, 6)
  , Token(E_TT_IDENTIFIER,  "$str",                 5, 0)
  , Token(E_TT_OPERATOR,    "=",                    5, 5)
  , Token(E_TT_CONSTEXPR,  R"("abc")",              5, 7)
  , Token(E_TT_DELIMITER,   ";",                    5, 12)
  , Token(E_TT_KEYWORD,     "if",                   6, 0)
  , Token(E_TT_DELIMITER,   "(",                    6, 3)
  , Token(E_TT_IDENTIFIER,  "$a",                   6, 4)
  , Token(E_TT_OPERATOR,    ">",                    6, 7)
  , Token(E_TT_IDENTIFIER,  "$b",                   6, 9)
  , Token(E_TT_DELIMITER,   ")",                    6, 11)
  , Token(E_TT_KEYWORD,     "echo",                 7, 2)
  , Token(E_TT_CONSTEXPR,  R"("a bigger than b")",  7, 7)
  , Token(E_TT_DELIMITER,   ";",                    7, 24)
  , Token(E_TT_COMMENT,     "//",                   9, 2)
  , Token(E_TT_COMMENT,     "/*",                   10, 2)
  , Token(E_TT_COMMENT,     "*/",                   12, 2)
  , Token(E_TT_KEYWORD,     "for",                  14, 0)
  , Token(E_TT_DELIMITER,   "(",                    14, 3)
  , Token(E_TT_IDENTIFIER,  "$i",                   14, 4)
  , Token(E_TT_OPERATOR,    "=",                    14, 7)
  , Token(E_TT_CONSTEXPR,   "0",                    14, 9)
  , Token(E_TT_DELIMITER,   ";",                    14, 10)
  , Token(E_TT_IDENTIFIER,  "$i",                   14, 12)
  , Token(E_TT_OPERATOR,    "<",                    14, 15)
  , Token(E_TT_CONSTEXPR,   "5",                    14, 17)
  , Token(E_TT_DELIMITER,   ")",                    14, 18)
  , Token(E_TT_DELIMITER,   "{",                    15, 0)
  , Token(E_TT_KEYWORD,     "echo",                 16, 4)
  , Token(E_TT_CONSTEXPR,  R"("$i")",               16, 9)
  , Token(E_TT_DELIMITER,   ";",                    16, 13)
  , Token(E_TT_OPERATOR,    "++",                   17, 14)
  , Token(E_TT_IDENTIFIER,  "$i",                   17, 16)
  , Token(E_TT_DELIMITER,   ";",                    17, 18)
  , Token(E_TT_DELIMITER,   "}",                    18, 0)
  , Token(E_TT_TAG,         "?>",                   19, 0)
};

static const PARAM_Lexical_analyzer source_code_1(".\\Data\\source_1.php", tokensarray_source_1, true);
static const PARAM_Lexical_analyzer source_code_2(".\\Data\\source_2.php", tokensarray_source_2, true);


static const PARAM_Lexical_analyzer* kFileParams_Positive[] = {
    &source_code_1
  , &source_code_2
};

INSTANTIATE_TEST_CASE_P(NextToken,
  AUTOTEST_PositiveNextToken,
  ::testing::ValuesIn(kFileParams_Positive));
