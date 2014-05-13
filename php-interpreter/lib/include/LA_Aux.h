#pragma once

#include <QString>

#include "Aux_def.h"
#include "Token.h"

PHP_LIB_API QString toString(E_TOKEN_TYPE i_token_type);

PHP_LIB_API const QVector<QString>& tags();
PHP_LIB_API const QVector<QString>& comments();
PHP_LIB_API const QVector<QString>& brackents();
PHP_LIB_API const QVector<QString>& keywords();
PHP_LIB_API const QVector<QString>& operators();

PHP_LIB_API const QVector<QString>& elements();

PHP_LIB_API bool isTag(const QString& i_str);
PHP_LIB_API bool isComment(const QString& i_str);
PHP_LIB_API bool isBracket(const QString& i_str);
PHP_LIB_API bool isKeyword(const QString& i_str);
PHP_LIB_API bool isOperator(const QString& i_str);

PHP_LIB_API bool isConstExpr(const QString& i_str);
PHP_LIB_API bool isIdentifier(const QString& i_str);


