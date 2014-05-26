/**@file   SyntaxAnalyzer.h
* @brief
* @date    Date Created:: 26/05/14 Dovgal Kostiantyn
*          $Date:: 26-05-2014 09:10#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#include "Token.h"

class SyntaxAnalizer
{
public:
  SyntaxAnalizer();

  bool readNextToken(const Token& i_token);

};