#pragma once

#include <QString>
#include <QList>
#include <QFile>
#include <QDir>
#include <QWidget>

class QMenu;

// Retain original structure names and members where possible.
// TCHAR replaced by QChar/wchar_t; we now use QString for Unicode strings.

struct LangPackEntry {
    unsigned linePos;
    unsigned int englishHash;       // DWORD -> unsigned int
    char *english;                  // not currently used
    char *local;                    // ANSI string (kept as char* for simplicity, or could be QByteArray)
    wchar_t *wlocal;                // Unicode version
};

struct LangPackStruct {
    QString filename;               // original TCHAR filename[MAX_PATH]
    QString language;               // char -> QString
    QString lastModifiedUsing;
    QString authors;
    QString authorEmail;
    bool rtl;
    LangPackEntry *entry;
    int entryCount;
    quint32 localeID;              // LCID -> unsigned int (LCID on Windows, but we store the raw value)
    uint defaultANSICp;            // UINT -> unsigned int (codepage)
};

using LangPack = LangPackStruct;

extern LangPackStruct langPack;

int LoadLangPack(const QString &szLangPack);
void LoadLangPackModule();
void UnloadLangPackModule();
int TranslateDialog(QWidget *widget);           // was HWND
// Returns a pointer to the translated string (now a QByteArray or wchar_t*).
// In Qt we return QString for Unicode convenience.
QString LangPackTranslate(const QString &english);

// Original low-level hash functions kept unchanged
unsigned int hash(const void *key, unsigned int len);
unsigned int hashstr(const char *key);

// Inline helper for backwards compatibility (calls LangPackTranslate)
inline QString Translate(const QString &source)
{
    return LangPackTranslate(source);
}

inline QString Translate(const wchar_t *source)
{
    return source ? LangPackTranslate(QString::fromWCharArray(source)) : QString();
}

inline QString Translate(const char *source)
{
    return source ? LangPackTranslate(QString::fromUtf8(source)) : QString();
}
