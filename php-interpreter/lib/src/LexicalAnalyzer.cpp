#include "LexicalAnalyzer.h"

#include <QVector>

LexicalAnalyzer::LexicalAnalyzer()
    : m_current_pos(0)
    , m_current_line(0)
    , m_end(true)
{}

LexicalAnalyzer::LexicalAnalyzer(const QString& i_php_source)
    : m_source_origin(i_php_source)
    , m_current_pos(0)
    , m_current_line(0)
    , m_end(false)
{
    m_source_lines = m_source_origin.split("\n");
}

bool LexicalAnalyzer::nextToken(Token& io_token)
{
  io_token = Token();
  return true;
}

void LexicalAnalyzer::setSource(const QString& i_php_source)
{
    m_source_origin = i_php_source;

    m_source_lines = m_source_origin.split("\n");

    m_current_pos = 0;
    m_current_line = 0;
}

//Token LexicalAnalyzer::next_token()
//{
//    auto temp = trim_front(m_source_lines[m_current_line]);
//    m_current_pos += temp;
//
//    return Token();
//}

quint32 LexicalAnalyzer::trim_front(QString& i_str)
{
    int i = -1;

    for( ;i_str[++i].isSpace(); );
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
