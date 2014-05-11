#pragma once

#include <QString>

#include "Token.h"

QString TokenToString(E_TOKEN_TYPE i_token_type);

bool isTag(const QString& i_str);

bool isKeyword(const QString& i_str);

bool isOperator(const QString& i_str);

bool isDelimiter(const QString& i_str);

bool isIdentifier(const QString& i_str);

bool isValue(const QString& i_str);
