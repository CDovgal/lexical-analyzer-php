/**@file   SyntaxAnalyzer.cpp
* @brief
* @date    Date Created:: 27/05/14 Dovgal Kostiantyn
*          $Date:: 27-05-2014 22:10#$
* @version $Revision:: 1#$
* @author  Copyright (c) 2014
*/

#include "SyntaxAnalizer.h"

enum SA_State : int
{
  SA_STATE_NOTHING = 0,



};

SyntaxAnalizer::SyntaxAnalizer()
  :  m_state(SA_STATE_NOTHING)
{}

bool SyntaxAnalizer::readNextToken(const Token& i_token)
{

    return false;
}
