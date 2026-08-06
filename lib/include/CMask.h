#pragma once
#include <QString>

class CMask
{
public:
    bool WildMatch(const QString &sWild, const QString &sString, const QString &sLimitChar);
};