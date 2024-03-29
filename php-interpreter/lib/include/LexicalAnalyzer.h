#pragma once

#include <QStringList>
#include <QVector>

#include "Token.h"
#include "Aux_def.h"
#include <regex>

enum E_STATE : int;

class PHP_LIB_API LexicalAnalyzer
{
public:
    LexicalAnalyzer();
    LexicalAnalyzer(const QString& i_php_source);

    bool nextToken(Token& io_token);

    void setSource(const QString& i_php_source);

    void reset();

private:
    Token next_token();

    void trim_spaces();

    int next_pos(const QString& i_str);

    bool shift_from_current(const QString& i_str);

    QChar current_symbol() const;

    int increase_pos(int i_pos);

    Token extract_constexpr_str();

    Token extract_constexpr_number();

    bool isEnd() const;

    TokenPosition current_token_pos() const;

    QString m_source_origin;

    QStringList m_source_lines;

    int m_current_pos;
    int m_current_line;

    bool m_end;

    E_STATE m_state;

    TokenPosition m_token_pos;

    static std::regex variable_id_regex;
    static std::regex constexpr_str_regex;
    static std::regex constexpr_chr_regex;
};
