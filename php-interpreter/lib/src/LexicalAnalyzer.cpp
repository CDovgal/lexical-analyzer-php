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

  if (m_state == E_STATE_OUT_OF_TAG)
  {
    if (shift_from_current(TAG_OPEN))
    {
      m_state = E_STATE_CODE;
      return Token(E_TT_TAG, TAG_OPEN, m_current_line, m_current_pos);
    }
    else
    {
      m_state = E_STATE_FINISHED;
      return Token();
    }
  }

  if (current_symbol() == '"')
  {
    return extract_constexpt_str();

    if (shift_from_current("\""))
    {
      m_state = E_STATE_CODE;
      return Token(E_TT_CONSTEXPR, "\"", m_current_line, m_current_pos);
    }
    else
    {
      m_state = E_STATE_FINISHED;
      return Token();
    }
  }

  QString str = m_source_lines[m_current_line].mid(m_current_pos, 2);
  if (str == TAG_CLOSE)
  {
    increase_pos(2);
    m_state = E_STATE_OUT_OF_TAG;
    return Token(E_TT_TAG, TAG_CLOSE, m_current_line, m_current_pos);
  }

  if (str == COMMENT_OPEN)
  {
    increase_pos(2);
    m_state = E_STATE_COMMENT;
    return Token(E_TT_COMMENT, COMMENT_OPEN, m_current_line, m_current_pos);
  }

  if (m_state == E_STATE_COMMENT)
  {
    if (shift_from_current(COMMENT_END))
    {
      m_state = E_STATE_CODE;
      return Token(E_TT_COMMENT, COMMENT_END, m_current_line, m_current_pos);
    }
    else
    {
      m_state = E_STATE_FINISHED;
      return Token();
    }
  }

  if (current_symbol() == SEMICOLON)
  {
    increase_pos(1);
    return Token(E_TT_DELIMITER, SEMICOLON, m_current_line, m_current_pos);
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
      return Token(E_TT_IDENTIFIER, var_str, temp_curr_line, var_start);
    }
    else
    {
      return Token(E_TT_ERROR, var_str, temp_curr_line, temp_curr_pos);
    }
  }

  if (current_symbol().isLower())
  {
    for (auto& keyword : keywords())
    {
      if (-1 != m_source_lines[m_current_line].indexOf(keyword, m_current_pos))
      {
        if (m_current_pos != m_source_lines[m_current_line].length())
        {
          increase_pos(keyword.length());
          return Token(E_TT_KEYWORD, keyword, m_current_line, m_current_pos);
        }
      }
    }
  }

  auto temp_pos = m_current_pos;
  for (auto& keyword : brackets())
  {
    if (temp_pos == m_source_lines[m_current_line].indexOf(keyword, m_current_pos))
    {
      if (temp_pos != m_source_lines[m_current_line].length())
      {
        increase_pos(keyword.length());
        return Token(E_TT_DELIMITER, keyword, m_current_line, temp_pos);
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
        return Token(E_TT_DELIMITER, keyword, m_current_line, temp_pos);
      }
      else
      {
        Q_ASSERT(false);
      }
    }
  }

  //for (auto& keyword : operators())
  //{
  //  if (-1 != m_source_lines[m_current_line].indexOf(keyword, m_current_pos))
  //  {
  //    if (m_current_pos != m_source_lines[m_current_line].length())
  //    {
  //      increase_pos(keyword.length());
  //      return Token(E_TT_OPERATOR, keyword, m_current_line, m_current_pos);
  //    }
  //  }
  //}

  m_state = E_STATE_FINISHED;
  return Token(E_TT_ERROR, "wft", m_current_line, m_current_pos);
}

int next_pos(const QString& i_str)
{
  return 0;
}

QString LexicalAnalyzer::eat_keyword(QString& i_str)
{
  return "";
}

void LexicalAnalyzer::trim_spaces()
{
  if (isEnd())
  {
    m_state = E_STATE_FINISHED;
    return;
  }
  for (; current_symbol().isSpace(); increase_pos(1));
}

QStringList split(QString i_line)
{
  QVector<QString> operators;
  //    = {"(", ")", "[", "]", ":", ";", "//", "/", "\"", "!", ".", "<=", ">", ">=", "==", "!=", "===", "!==", "<>", "<" , "&&", "||", "=", "+=", "-=", "*=", "/=", "%=", "*", "/", "%", "+", "-", "and", "xor", "or"};

  for (int i = 0; i < i_line.size() - 1; ++i)
  {
    int pos1 = operators.indexOf(QString(i_line[i]));

    if (pos1 == -1)
      continue;

    if (i_line[i + operators[pos1].length()] != ' ')
      i_line.insert(i + operators[pos1].length(), ' ');
  }

  return i_line.split(" ");
}

bool LexicalAnalyzer::shift_from_current(const QString& i_str)
{
  for (;
    m_current_line < m_source_lines.length();
    ++m_current_line, ++m_current_pos)
  {
    m_current_pos = m_source_lines[m_current_line].indexOf(i_str, m_current_pos);

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
  return m_source_lines[m_current_line][m_current_pos];
}

int LexicalAnalyzer::increase_pos(int i_pos)
{
  m_current_pos += i_pos;

  if (m_source_lines[m_current_line].length() < m_current_pos)
  {
    Q_ASSERT(false);
  }

  if (m_source_lines[m_current_line].length() == m_current_pos)
  {
    ++m_current_line;
    m_current_pos = 0;
  }

  if (m_current_line == m_source_lines.length() && m_current_pos == m_source_lines[m_current_line].length())
    m_state = E_STATE_FINISHED;

  return m_current_pos;
}

Token LexicalAnalyzer::extract_constexpt_str()
{
  auto temp_pos = m_current_pos;
  increase_pos(1);
  m_current_pos = m_source_lines[m_current_line].indexOf("\"", m_current_pos);

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

bool LexicalAnalyzer::isEnd() const
{
  if (m_current_line == m_source_lines.length() - 1 && m_current_pos == m_source_lines[m_current_line].length())
  {
    return true;
  }
  return false;
}
