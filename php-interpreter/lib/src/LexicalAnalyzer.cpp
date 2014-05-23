#include "LexicalAnalyzer.h"

#include "LA_Aux.h"

#include <QVector>

enum E_STATE : int
{
  E_STATE_NOT_INIT = 0,

  E_STATE_CODE,

  E_STATE_OUT_OF_TAG,
  E_STATE_COMMENT,

  E_STATE_FINISHED
};

LexicalAnalyzer::LexicalAnalyzer()
  : m_current_pos(0)
  , m_current_line(0)
  , m_end(false)
  , m_state(E_STATE_NOT_INIT)
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
  return m_state != E_STATE_FINISHED;
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
  trim_spaces();

  if (m_state == E_STATE_NOT_INIT || m_state == E_STATE_FINISHED)
    return Token();

  m_token_pos = qMakePair(m_current_line, m_current_pos);

  if (m_state == E_STATE_OUT_OF_TAG)
  {
    if (shift_from_current(TAG_OPEN))
    {
      m_state = E_STATE_CODE;
      return Token(E_TT_TAG, TAG_OPEN, current_token_pos());
    }
    else
    {
      m_state = E_STATE_FINISHED;
      return Token();
    }
  }

  if (current_symbol() == '"' || current_symbol() == '\'')
  {
    return extract_constexpr_str();
  }

  QString str = m_source_lines[m_current_line].mid(m_current_pos, 2);
  if (str == TAG_CLOSE)
  {
    increase_pos(2);
    m_state = E_STATE_OUT_OF_TAG;
    return Token(E_TT_TAG, TAG_CLOSE, current_token_pos());
  }

  if (str == COMMENT_OPEN)
  {
    increase_pos(2);
    m_state = E_STATE_COMMENT;
    return Token(E_TT_COMMENT, COMMENT_OPEN, current_token_pos());
  }

  if (str == COMMENT_LINE)
  {
    ++m_current_line;
    m_current_pos = 0;
    return Token(E_TT_COMMENT, COMMENT_LINE, current_token_pos());
  }

  if (m_state == E_STATE_COMMENT)
  {
    if (shift_from_current(COMMENT_END))
    {
      m_state = E_STATE_CODE;
      return Token(E_TT_COMMENT, COMMENT_END, current_token_pos());
    }
    else
    {
      m_state = E_STATE_FINISHED;
      return Token();
    }
  }

  if (current_symbol() == '$')
  {
    int temp_curr_pos = m_current_pos;
    int temp_curr_line = m_current_line;

    int var_start = increase_pos(1) - 1;

    if ((current_symbol().isLetter() || current_symbol() == '_') && increase_pos(1))
    {
      for (; current_symbol().isLetterOrNumber() || current_symbol() == '_'; increase_pos(1));
    }

    if (m_current_line - temp_curr_line != 0)
    {
      temp_curr_pos = m_source_lines[temp_curr_line].length();
    }
    else
    {
      temp_curr_pos = m_current_pos;
    }

    QString var_str = m_source_lines[temp_curr_line].mid(var_start, temp_curr_pos - var_start);
    if (var_str.length() > 1)
    {
      return Token(E_TT_IDENTIFIER, var_str, current_token_pos());
    }
    else
    {
      return Token(E_TT_ERROR, var_str, current_token_pos());
    }
  }

  if (current_symbol().isLetter())
  {
    for (auto& keyword : keywords())
    {
      if (-1 != m_source_lines[m_current_line].indexOf(keyword, m_current_pos))
      {
        if (m_current_pos != m_source_lines[m_current_line].length())
        {
          increase_pos(keyword.length());
          return Token(E_TT_KEYWORD, keyword, current_token_pos());
        }
      }
    }
  }

  auto temp_pos = m_current_pos;
  for (auto& delimiter : delimiters())
  {
    if (temp_pos == m_source_lines[m_current_line].indexOf(delimiter, m_current_pos))
    {
      if (temp_pos != m_source_lines[m_current_line].length())
      {
        increase_pos(delimiter.length());
        return Token(E_TT_DELIMITER, delimiter, current_token_pos());
      }
      else
      {
        Q_ASSERT(false);
      }
    }
  }

  for (auto& keyword : operators())
  {
    if (temp_pos == m_source_lines[m_current_line].indexOf(keyword, m_current_pos))
    {
      if (temp_pos != m_source_lines[m_current_line].length())
      {
        increase_pos(keyword.length());
        return Token(E_TT_OPERATOR, keyword, current_token_pos());
      }
      else
      {
        Q_ASSERT(false);
      }
    }
  }

  if (current_symbol().isNumber())
    return extract_constexpr_number();


  QString func_identifier;
  for (; !isEnd() && (current_symbol().isLetterOrNumber() || current_symbol() == '_' ); increase_pos(1))
    func_identifier.push_back(current_symbol());

  return Token(E_TT_IDENTIFIER, func_identifier, m_token_pos);
}

void LexicalAnalyzer::reset()
{
  m_current_pos = 0;
  m_current_line = 0;
  m_token_pos = qMakePair(0, 0);
  m_end = false;
  m_state = E_STATE_OUT_OF_TAG;
}

int next_pos(const QString& i_str)
{
  return 0;
}

void LexicalAnalyzer::trim_spaces()
{
  if (isEnd())
  {
    m_state = E_STATE_FINISHED;
    return;
  }
  for (; current_symbol().isNull() || current_symbol().isSpace(); increase_pos(1));
}

bool LexicalAnalyzer::shift_from_current(const QString& i_str)
{
  for (; !isEnd(); ++m_current_line, ++m_current_pos)
  {
    m_current_pos = m_source_lines[m_current_line].indexOf(i_str, m_current_pos);

    m_token_pos = qMakePair(m_current_line, m_current_pos);

    if (m_current_pos != -1)
    {
      increase_pos(i_str.length());
      return true;
    }
  }

  return false;
}

QChar LexicalAnalyzer::current_symbol() const
{
  if (m_current_pos < m_source_lines[m_current_line].length())
    return m_source_lines[m_current_line][m_current_pos];
  else
    return QChar();
}

int LexicalAnalyzer::increase_pos(int i_pos)
{
  m_current_pos += i_pos;

  //if (m_source_lines[m_current_line].length() < m_current_pos)
  //{
  //  Q_ASSERT(false);
  //}

  if (m_current_line < m_source_lines.length() && m_source_lines[m_current_line].length() <= m_current_pos)
  {
    ++m_current_line;
    m_current_pos = 0;
  }

  if (m_current_line == m_source_lines.length() /*|| 
    ( (m_current_line == m_source_lines.length() - 1) && m_current_pos >= m_source_lines[m_current_line].length())*/
    )
    m_state = E_STATE_FINISHED;

  return m_current_pos;
}

Token LexicalAnalyzer::extract_constexpr_str()
{
  auto temp_pos = m_current_pos;
  auto curr = current_symbol();
  increase_pos(1);
  m_current_pos = m_source_lines[m_current_line].indexOf(QString(curr), m_current_pos);

  if (m_current_pos != -1)
  {
    increase_pos(1);
    return Token(E_TT_CONSTEXPR, m_source_lines[m_current_line].mid(temp_pos, m_current_pos - temp_pos), m_current_line, temp_pos);
  }
  else
  {
    return Token(E_TT_ERROR, "<error>", m_current_line, temp_pos);
  }
}

Token LexicalAnalyzer::extract_constexpr_number()
{
  auto temp_pos = m_current_pos;

  QString number;
  for (; current_symbol().isNumber(); increase_pos(1))
    number.push_back(current_symbol());

  return Token(E_TT_CONSTEXPR, number, current_token_pos());
}

bool LexicalAnalyzer::isEnd() const
{
  if (m_current_line >= m_source_lines.length())
    return true;

  if (m_current_line == m_source_lines.length() - 1 && m_current_pos == m_source_lines[m_current_line].length())
    return true;

  return false;
}

TokenPosition LexicalAnalyzer::current_token_pos() const
{
  return m_token_pos;
}
