#pragma once

#include <QString>

enum E_TOKEN_TYPE
{
    E_TT_NONE = 0,

    E_TT_TAG,           // <?php, ?>
    E_TT_KEYWORD,       // define, if, while, ...
    E_TT_OPERATOR,      // +, - , =, ....
    E_TT_DELIMITER,     // [, ), ., ...
    E_TT_IDENTIFIER,    // $this, $var1, ...
    E_TT_CONSTEXPR      // 1, "abc", true, ...
};

QString TokenToString(E_TOKEN_TYPE i_token_type);

bool isTag(const QString& i_str);

bool isKeyword(const QString& i_str);

bool isOperator(const QString& i_str);

bool isDelimiter(const QString& i_str);

bool isIdentifier(const QString& i_str);

bool isValue(const QString& i_str);
