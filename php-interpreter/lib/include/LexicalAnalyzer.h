#pragma once

#include <QStringList>
#include <QVector>
#include <Token.h>

#include "Aux_def.h"

class PHP_LIB_API LexicalAnalyzer
{
public:
    LexicalAnalyzer();
    LexicalAnalyzer(const QString& i_php_source);

    bool nextToken(Token& io_token);

    void setSource(const QString& i_php_source);

    void reset();

private:
    //Token next_token();

    quint32 trim_front(QString& i_str);

    QString m_source_origin;

    QStringList m_source_lines;

    quint32 m_current_pos;
    quint32 m_current_line;

    bool m_end;
};
