/* File ggets.cpp - safe replacement for gets-style line reading */

#include "ggets.h"

#include <algorithm>
#include <type_traits>

template<class TTCHAR>
static void copyQStringToBuffer(const QString& line, TTCHAR* buffer, int capacity)
{
    const int copyLen = std::min(capacity - 1, line.size());
    for (int i = 0; i < copyLen; ++i) {
        if constexpr (std::is_same_v<TTCHAR, QChar>) {
            buffer[i] = line.at(i);
        } else {
            buffer[i] = static_cast<TTCHAR>(line.at(i).toLatin1());
        }
    }
    buffer[copyLen] = TTCHAR('\0');
}

template<class TTCHAR>
TTCHAR* ReallocString(TTCHAR* ptr, size_t newsize, size_t oldsize, bool& bOwnBuffer)
{
    if (newsize <= oldsize) {
        return ptr;
    }

    TTCHAR* fresh = new TTCHAR[newsize];
    std::fill(fresh, fresh + newsize, TTCHAR('\0'));
    if (ptr) {
        const size_t copyCount = std::min(oldsize, newsize);
        std::copy(ptr, ptr + copyCount, fresh);
        if (bOwnBuffer) {
            delete[] ptr;
        }
    }
    bOwnBuffer = true;
    return fresh;
}

template<class TTCHAR>
TTCHAR* GetFileString(TTCHAR* string, int n, QTextStream* stream)
{
    if (!stream || !string || n <= 0) {
        return nullptr;
    }

    const QString line = stream->readLine();
    if (line.isNull() && stream->atEnd()) {
        return nullptr;
    }

    copyQStringToBuffer(line, string, n);
    return string;
}

template<class TTCHAR>
TTCHAR* FindCharInString(const TTCHAR* string, int c)
{
    if (!string) {
        return nullptr;
    }

    const TTCHAR target = static_cast<TTCHAR>(c);
    const TTCHAR* p = string;
    while (*p != TTCHAR('\0')) {
        if (*p == target) {
            return const_cast<TTCHAR*>(p);
        }
        ++p;
    }
    return nullptr;
}

#define INITSIZE 112
#define DELTASIZE (INITSIZE + 16)

template<class TTCHAR>
int fggets(TTCHAR** ln, QTextStream* f, bool& bOwnBuffer, const int nDefaultBufferSizeChars)
{
    if (!ln || !f) {
        return FGGETS_NOMEM;
    }

    bOwnBuffer = false;

    TTCHAR* buffer = nullptr;
    int cursize = nDefaultBufferSizeChars > 0 ? nDefaultBufferSizeChars : INITSIZE;
    if (nDefaultBufferSizeChars > 0 && *ln) {
        buffer = *ln;
    } else {
        buffer = new TTCHAR[cursize];
        std::fill(buffer, buffer + cursize, TTCHAR('\0'));
        bOwnBuffer = true;
    }

    const QString line = f->readLine();
    if (line.isNull() && f->atEnd()) {
        if (bOwnBuffer) {
            delete[] buffer;
        }
        *ln = nullptr;
        return EOF;
    }

    const int needed = line.size() + 1;
    if (needed > cursize) {
        buffer = ReallocString(buffer, static_cast<size_t>(needed), static_cast<size_t>(cursize), bOwnBuffer);
        cursize = needed;
    }
    copyQStringToBuffer(line, buffer, cursize);
    *ln = buffer;
    return FGGETS_OK;
}
