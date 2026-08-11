#include "langpack.h"
#include <QFile>
#include <QTextStream>
#include <QStringConverter>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QAction>
#include <QAbstractButton>
#include <QLabel>
#include <QGroupBox>
#include <cstring>
#include <algorithm>

LangPackStruct langPack;

static void TrimString(char *str)
{
    size_t start, len = strlen(str);
    while (len > 0 && (unsigned char)str[len - 1] <= ' ') str[--len] = 0;
    for (start = 0; str[start] && (unsigned char)str[start] <= ' '; start++);
    memmove(str, str + start, len - start + 1);
}

static void TrimStringSimple(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[--len] = '\0';
    if (len > 0 && str[len - 1] == '\r') str[len - 1] = '\0';
}

static int IsEmpty(char *str)
{
    while (*str) {
        if (*str != ' ' && *str != '\r' && *str != '\n')
            return 0;
        str++;
    }
    return 1;
}

static void ConvertBackslashes(char *str)
{
    char *pstr = str;
    while (*pstr) {
        if (*pstr == '\\') {
            switch (pstr[1]) {
            case 'n': *pstr = '\n'; break;
            case 't': *pstr = '\t'; break;
            case 'r': *pstr = '\r'; break;
            default:  *pstr = pstr[1]; break;
            }
            memmove(pstr + 1, pstr + 2, strlen(pstr + 2) + 1);
        }
        pstr++;
    }
}

unsigned int hash(const void *key, unsigned int len)
{
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    unsigned int h = len;
    const unsigned char *data = (const unsigned char *)key;

    while (len >= 4) {
        unsigned int k = *(unsigned int *)data;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }

    switch (len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    }
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}

unsigned int hashstr(const char *key)
{
    if (!key) return 0;
    return hash(key, (unsigned int)strlen(key));
}

static int SortLangPackHashesProc(const LangPackEntry *arg1, const LangPackEntry *arg2)
{
    if (arg1->englishHash < arg2->englishHash) return -1;
    if (arg1->englishHash > arg2->englishHash) return 1;
    if (arg1->linePos < arg2->linePos) return -1;
    if (arg1->linePos > arg2->linePos) return 1;
    return 0;
}

static int SortLangPackHashesProc2(const LangPackEntry *arg1, const LangPackEntry *arg2)
{
    if (arg1->englishHash < arg2->englishHash) return -1;
    if (arg1->englishHash > arg2->englishHash) return 1;
    return 0;
}

int LoadLangPack(const QString &szLangPack)
{
    QFile file(szLangPack);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    langPack.filename = szLangPack;

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    QString line;
    int entriesAlloced = 0;
    unsigned int linePos = 1;
    bool headersDone = false;
    Q_UNUSED(headersDone);

    line = stream.readLine();
    if (line.isEmpty() && stream.atEnd()) {
        file.close();
        return 2;
    }

    while (!stream.atEnd()) {
        line = stream.readLine();
        if (line.isEmpty()) continue;
        QByteArray utf8line = line.toUtf8();
        char *pline = utf8line.data();
        TrimString(pline);
        if (IsEmpty(pline) || pline[0] == ';') continue;
        if (pline[0] == '[') break;
        char *pszColon = strchr(pline, ':');
        if (!pszColon) {
            file.close();
            return 3;
        }
        *pszColon = 0;
        char *value = pszColon + 1;
        TrimString(value);
        if (!strcmp(pline, "Language")) langPack.language = QString::fromUtf8(value);
        else if (!strcmp(pline, "Last-Modified-Using")) langPack.lastModifiedUsing = QString::fromUtf8(value);
        else if (!strcmp(pline, "Authors")) langPack.authors = QString::fromUtf8(value);
        else if (!strcmp(pline, "Author-email")) langPack.authorEmail = QString::fromUtf8(value);
        else if (!strcmp(pline, "RTL")) langPack.rtl = (strcmp(value, "1") == 0);
        else if (!strcmp(pline, "Locale")) {
            langPack.localeID = (quint32)strtol(value, nullptr, 16);
            langPack.defaultANSICp = 0;
        }
    }

    while (!stream.atEnd()) {
        QByteArray utf8line = stream.readLine().toUtf8();
        char *pline = utf8line.data();
        if (IsEmpty(pline) || pline[0] == ';') continue;
        TrimStringSimple(pline);
        ConvertBackslashes(pline);
        if (pline[0] == '[' && pline[strlen(pline) - 1] == ']') {
            if (langPack.entryCount > 0 && langPack.entry[langPack.entryCount - 1].local == nullptr) {
                free(langPack.entry[langPack.entryCount - 1].english);
                langPack.entryCount--;
            }
            char *pszLine = pline + 1;
            pline[strlen(pline) - 1] = '\0';
            TrimStringSimple(pszLine);
            if (++langPack.entryCount > entriesAlloced) {
                entriesAlloced += 128;
                langPack.entry = (LangPackEntry*)realloc(langPack.entry, sizeof(LangPackEntry) * entriesAlloced);
            }
            langPack.entry[langPack.entryCount - 1].english = nullptr;
            langPack.entry[langPack.entryCount - 1].englishHash = hashstr(pszLine);
            langPack.entry[langPack.entryCount - 1].local = nullptr;
            langPack.entry[langPack.entryCount - 1].wlocal = nullptr;
            langPack.entry[langPack.entryCount - 1].linePos = linePos++;
        } else if (langPack.entryCount) {
            LangPackEntry *E = &langPack.entry[langPack.entryCount - 1];
            if (!E->local) {
                E->local = _strdup(pline);
                QString wide = QString::fromUtf8(pline);
                E->wlocal = (wchar_t*)malloc((wide.size() + 1) * sizeof(wchar_t));
                wide.toWCharArray(E->wlocal);
                E->wlocal[wide.size()] = L'\0';
            } else {
                size_t oldLen = strlen(E->local);
                E->local = (char*)realloc(E->local, oldLen + strlen(pline) + 2);
                strcat(E->local, "\n");
                strcat(E->local, pline);
                QString wide = QString::fromWCharArray(E->wlocal);
                wide += QLatin1Char('\n');
                wide += QString::fromUtf8(pline);
                E->wlocal = (wchar_t*)realloc(E->wlocal, (wide.size() + 1) * sizeof(wchar_t));
                wide.toWCharArray(E->wlocal);
                E->wlocal[wide.size()] = L'\0';
            }
        }
    }
    file.close();

    qsort(langPack.entry, langPack.entryCount, sizeof(LangPackEntry),
          (int(*)(const void*, const void*))SortLangPackHashesProc);

    return 0;
}

static QString LangPackTranslateString(const QString &english)
{
    if (langPack.entryCount == 0 || english.isEmpty())
        return english;

    QByteArray utf8 = english.toUtf8();
    unsigned int h = hash(utf8.constData(), utf8.size());

    LangPackEntry key;
    key.englishHash = h;
    LangPackEntry *entry = (LangPackEntry*)bsearch(&key, langPack.entry, langPack.entryCount,
                                                   sizeof(LangPackEntry),
                                                   (int(*)(const void*, const void*))SortLangPackHashesProc2);
    if (!entry) return english;

    while (entry > langPack.entry) {
        entry--;
        if (entry->englishHash != h) {
            entry++;
            return QString::fromWCharArray(entry->wlocal);
        }
    }
    return QString::fromWCharArray(entry->wlocal);
}

QString LangPackTranslate(const QString &english)
{
    return LangPackTranslateString(english);
}

static void TranslateWidgetRecursive(QWidget *widget)
{
    if (!widget) return;

    QString title = widget->windowTitle();
    if (!title.isEmpty()) {
        widget->setWindowTitle(Translate(title));
    }

    const QMetaObject *mo = widget->metaObject();
    if (mo->inherits(&QAbstractButton::staticMetaObject) ||
        mo->inherits(&QLabel::staticMetaObject) ||
        mo->inherits(&QGroupBox::staticMetaObject) ||
        mo->inherits(&QAction::staticMetaObject)) {
        if (widget->property("text").isValid()) {
            QString txt = widget->property("text").toString();
            if (!txt.isEmpty())
                widget->setProperty("text", Translate(txt));
        }
        if (widget->property("title").isValid()) {
            QString t = widget->property("title").toString();
            if (!t.isEmpty())
                widget->setProperty("title", Translate(t));
        }
    }

    foreach (QObject *child, widget->children()) {
        QWidget *w = qobject_cast<QWidget*>(child);
        if (w) TranslateWidgetRecursive(w);
    }
}

int TranslateDialog(QWidget *widget)
{
    if (!widget) return 0;
    TranslateWidgetRecursive(widget);
    return 0;
}

void LoadLangPackModule()
{
    QString appDir = QApplication::applicationDirPath();
    QDir dir(appDir);
    QStringList filters;
    filters << "langpack_*.txt";
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    if (!files.isEmpty()) {
        QString langFile = files.first().absoluteFilePath();
        LoadLangPack(langFile);
    }
}

void UnloadLangPackModule()
{
    for (int i = 0; i < langPack.entryCount; i++) {
        free(langPack.entry[i].english);
        free(langPack.entry[i].local);
        free(langPack.entry[i].wlocal);
    }
    free(langPack.entry);
    langPack.entry = nullptr;
    langPack.entryCount = 0;
    langPack.rtl = false;
}
