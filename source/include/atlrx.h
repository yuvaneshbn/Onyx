#pragma once

#include <QString>
#include <regex>
#include <string>

enum REParseError
{
    REPARSE_ERROR_OK = 0,
    REPARSE_ERROR_FAILURE = 1
};

template <typename = void>
class CAtlREMatchContext
{
public:
    using RECHAR = wchar_t;

    unsigned m_uNumGroups = 0;

    bool GetMatch(int index, const RECHAR **start, const RECHAR **end) const
    {
        if (index < 0 || static_cast<std::size_t>(index) >= m_match.size() || m_storage.empty())
            return false;
        const auto &m = m_match[static_cast<std::size_t>(index)];
        if (!m.matched)
            return false;
        const auto pos = m_match.position(static_cast<std::size_t>(index));
        const auto len = m_match.length(static_cast<std::size_t>(index));
        if (start)
            *start = m_storage.c_str() + pos;
        if (end)
            *end = m_storage.c_str() + pos + len;
        return true;
    }

private:
    template <typename> friend class CAtlRegExp;

    std::wstring m_storage;
    std::wsmatch m_match;
};

template <typename = void>
class CAtlRegExp
{
public:
    REParseError Parse(const wchar_t *pattern, bool ignoreCase = false)
    {
        try {
            std::wstring converted = pattern ? std::wstring(pattern) : std::wstring();
            for (auto &ch : converted) {
                if (ch == L'{') ch = L'(';
                else if (ch == L'}') ch = L')';
            }
            auto flags = std::regex_constants::ECMAScript;
            if (ignoreCase)
                flags |= std::regex_constants::icase;
            m_regex = std::wregex(converted, flags);
            return REPARSE_ERROR_OK;
        } catch (...) {
            return REPARSE_ERROR_FAILURE;
        }
    }

    bool Match(const QString &text, CAtlREMatchContext<> *ctx) const
    {
        if (!ctx)
            return false;
        ctx->m_storage = text.toStdWString();
        if (!std::regex_search(ctx->m_storage, ctx->m_match, m_regex))
            return false;
        ctx->m_uNumGroups = ctx->m_match.size() > 0 ? static_cast<unsigned>(ctx->m_match.size() - 1) : 0;
        return true;
    }

private:
    std::wregex m_regex;
};
