#include "LexicalAnalyzer.h"

#include "LA_Aux.h"

#include <QVector>

enum E_STATE : int
{
  E_STATE_NOT_INIT = 0,

  E_STATE_CODE,

  E_STATE_OUT_OF_TAG,
  E_STATE_COMMENT
};

LexicalAnalyzer::LexicalAnalyzer()
  : m_current_pos(0)
  , m_current_line(0)
  , m_end(false)
  , m_state(E_STATE_OUT_OF_TAG)
{}

LexicalAnalyzer::LexicalAnalyzer(const QString& i_php_source)
  : m_source_origin(i_php_source)
  , m_current_pos(0)
  , m_current_line(0)
  , m_end(false)
  , m_state(E_STATE_OUT_OF_TAG)
{
  m_source_lines = m_source_origin.split("\n");
}

bool LexicalAnalyzer::nextToken(Token& io_token)
{
  io_token = next_token();
  return io_token.m_token_type != E_TT_NONE;
}

void LexicalAnalyzer::setSource(const QString& i_php_source)
{
  m_source_origin = i_php_source;

  m_source_lines = m_source_origin.split("\n");

  m_current_pos = 0;
  m_current_line = 0;
}

Token LexicalAnalyzer::next_token()
{
  if(m_state == E_STATE_OUT_OF_TAG) 
  {
    for(int curr_pos = m_current_pos; curr_pos = m_source_lines[m_current_line].indexOf("<?php", curr_pos); m_current_line++)
    {
      if(m_current_pos == -1)
        curr_pos = 0;
      else
      {
        m_current_pos = curr_pos;
        break;
      }
    }
    m_state = E_STATE_CODE;
    return Token(E_TT_TAG, "<?php", m_current_line, m_current_pos);
  }

  if(m_state == E_STATE_COMMENT)
  {
    for(int curr_pos = m_current_pos; curr_pos = m_source_lines[m_current_line].indexOf("*/", curr_pos == -1 ? 0 : curr_pos); m_current_line++)
    {
      if(m_current_pos != -1)
      {
        m_current_pos = curr_pos;
        break;
      }
    }
    m_state = E_STATE_CODE;    
    return Token(E_TT_COMMENT, "*/", m_current_line, m_current_pos);
  }

  for(;;)
  {
    m_current_pos += trim_front(m_source_lines[m_current_line]);
    if(0 == m_source_lines[m_current_line].length())
    {
      ++m_current_line;
      m_current_pos = 0;
    }
  }

  if(m_source_lines[m_current_line][m_current_pos] == '$')
  {
    int space = m_source_lines[m_current_line].indexOf(" ");
    QString lexem = m_source_lines[m_current_line].left(space);
    m_current_pos += space;
    m_source_lines[m_current_line] = m_source_lines[m_current_line].right(space);

    return Token(E_TT_IDENTIFIER, lexem, m_current_line, m_current_pos);
  }

  auto symbol = m_source_lines[m_current_line][m_current_pos];

  if(symbol.isLetter())
  {
    for (auto& keyword : keywords())
    {
      auto pos = m_source_lines[m_current_line].indexOf(keyword);
      
      m_current_pos += keyword.length();
      QString lexem = m_source_lines[m_current_line].left(keyword.length());
      m_source_lines[m_current_line] = m_source_lines[m_current_line].right(keyword.length());

      return Token(E_TT_KEYWORD, lexem, m_current_line, m_current_pos);
    }
    // here error
  }

  //if(symbol.isNumber() || symbol == '"' || symbol == '')
  //{
  //  // const expr
  //}

  return Token();
}

int next_pos(const QString& i_str)
{
	return 0;
}

QString LexicalAnalyzer::eat_keyword(QString& i_str)
{
	return "";
}

int LexicalAnalyzer::trim_front(QString& i_str)
{
  int i = -1;

  for (; i_str[++i].isSpace();){};

  i_str.remove(0, i);

  return i;
}

QStringList split(QString i_line)
{
  QVector<QString> operators;
  //    = {"(", ")", "[", "]", ":", ";", "//", "/", "\"", "!", ".", "<=", ">", ">=", "==", "!=", "===", "!==", "<>", "<" , "&&", "||", "=", "+=", "-=", "*=", "/=", "%=", "*", "/", "%", "+", "-", "and", "xor", "or"};

  for(int i = 0; i < i_line.size() - 1; ++i)
  {
    int pos1 = operators.indexOf(QString(i_line[i]));

    if(pos1 == -1)
      continue;

    if(i_line[i + operators[pos1].length()] != ' ')
      i_line.insert(i + operators[pos1].length(), ' ');
  }

  return i_line.split(" ");
}
