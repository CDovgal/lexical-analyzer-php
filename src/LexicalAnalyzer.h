#pragma once

#include <QStringList>
#include <QVector>
#include <Token.h>

class LexicalAnalyzer
{
public:
    LexicalAnalyzer();
    LexicalAnalyzer(const QString& i_php_source);

    bool isEnd() const;

    Token nextToken();

    void setSource(const QString& i_php_source);

    void reset();

private:
    Token next_token();

    quint32 trim_front(QString& i_str);

    QString m_source_origin;

    QStringList m_source_lines;

    quint32 m_current_pos;
    quint32 m_current_line;

    bool m_end;
};
