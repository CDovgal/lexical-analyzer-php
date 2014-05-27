/**@file   SyntaxAnalyzer.h
* @brief
* @date    Date Created:: 26/05/14 Dovgal Kostiantyn
*          $Date:: 26-05-2014 09:10#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#pragma once

#include "Token.h"

enum SA_State : int;

class SyntaxAnalizer
{
public:
  SyntaxAnalizer();

  bool readNextToken(const Token& i_token);

private:
  SA_State m_state;
};