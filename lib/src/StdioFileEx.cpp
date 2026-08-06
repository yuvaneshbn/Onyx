#include "StdioFileEx.h"

#include <cstring>

CStdioFileEx::CStdioFileEx()
    : QFile()
    , m_bIsUnicodeText(false)
    , m_nFlags(0)
    , m_nFileCodePage(-1)
    , m_cUnicodeFillerChar(DEFAULT_UNICODE_FILLER_CHAR)
    , m_bWriteBOM(true)
    , m_bReadBOM(false)
    , m_bCheckFilePos(true)
    , m_currentPos(0)
{
    std::memset(m_arrUnicodeDefaultBuffer, 0, sizeof(m_arrUnicodeDefaultBuffer));
    std::memset(m_arrMultibyteDefaultBuffer, 0, sizeof(m_arrMultibyteDefaultBuffer));
}

bool CStdioFileEx::open(QIODevice::OpenMode mode)
{
    return QFile::open(mode);
}

bool CStdioFileEx::open(const QString& fileName, QIODevice::OpenMode mode)
{
    setFileName(fileName);
    return QFile::open(mode);
}

bool CStdioFileEx::Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
    Q_UNUSED(pError);

#ifdef UNICODE
    QString fileName = QString::fromWCharArray(lpszFileName);
#else
    QString fileName = QString::fromLocal8Bit(lpszFileName);
#endif

    ProcessFlags(nOpenFlags);

    QIODevice::OpenMode mode = QIODevice::NotOpen;
    if ((nOpenFlags & CFile::modeReadWrite) == CFile::modeReadWrite)
        mode |= QIODevice::ReadWrite;
    else if (nOpenFlags & CFile::modeWrite)
        mode |= QIODevice::WriteOnly;
    else
        mode |= QIODevice::ReadOnly;

    if (nOpenFlags & CFile::modeCreate)
        mode |= QIODevice::Truncate;

    setFileName(fileName);
    const bool ok = QFile::open(mode);
    if (!ok)
        return false;

    if (!(nOpenFlags & CFile::modeCreate) && (mode & QIODevice::ReadOnly))
    {
        const QByteArray bom = read(3);
        if (bom.size() >= 2)
        {
            const quint16 bomValue = static_cast<quint8>(bom[0]) | (static_cast<quint8>(bom[1]) << 8);
            if (bomValue == UNICODE_BOM)
            {
                m_bIsUnicodeText = true;
                m_bReadBOM = true;
            }
            else if (bom.size() == 3
                     && static_cast<quint8>(bom[0]) == 0xEF
                     && static_cast<quint8>(bom[1]) == 0xBB
                     && static_cast<quint8>(bom[2]) == 0xBF)
            {
                m_bIsUnicodeText = false;
                m_nFileCodePage = CP_UTF8;
                m_bReadBOM = true;
            }
            else
            {
                QFile::seek(0);
            }
        }
        else
        {
            QFile::seek(0);
        }
    }

    m_bCheckFilePos = true;
    m_currentPos = 0;
    return true;
}

UINT CStdioFileEx::ProcessFlags(UINT& nOpenFlags)
{
    m_bIsUnicodeText = (nOpenFlags & modeWriteUnicode) != 0;
    nOpenFlags &= ~CFile::typeText;
    nOpenFlags |= CFile::typeBinary;
    m_nFlags = nOpenFlags;
    return nOpenFlags;
}

bool CStdioFileEx::ReadString(QString& rString)
{
    if (!isOpen() || !isReadable())
        return false;

    if (m_bCheckFilePos && pos() == 0)
        m_bCheckFilePos = false;

    const bool readData = m_bIsUnicodeText ? ReadUnicodeLine(rString)
                                           : ReadMultiByteLine(rString);

    if (readData && rString.endsWith('\r'))
        rString.chop(1);

    m_bCheckFilePos = false;
    return readData;
}

QString CStdioFileEx::ReadString(qint64)
{
    return QString();
}

void CStdioFileEx::WriteString(const QString& lpsz)
{
    if (!isOpen() || !isWritable())
        return;

    if (m_bWriteBOM && pos() == 0)
    {
        if (m_bIsUnicodeText)
        {
            const QByteArray bom;
            write(QByteArray::fromRawData("\xFF\xFE", 2));
        }
        else if (m_nFileCodePage == CP_UTF8)
        {
            write(QByteArray::fromRawData("\xEF\xBB\xBF", 3));
        }
    }

    if (m_bIsUnicodeText)
        WriteUnicodeLine(lpsz);
    else
        WriteMultiByteLine(lpsz);
}

qint64 CStdioFileEx::GetCharCount()
{
    qint64 byteCount = size();
    if (m_bIsUnicodeText)
    {
        if (m_bReadBOM)
            byteCount -= 2;
        return byteCount / 2;
    }
    return byteCount;
}

qint64 CStdioFileEx::Seek(qint64 lOff, int nFrom)
{
    m_bCheckFilePos = true;

    qint64 target = lOff;
    if (nFrom == CFile::current)
        target = pos() + lOff;
    else if (nFrom == CFile::end)
        target = size() + lOff;

    if (target < 0)
        target = 0;
    return QFile::seek(target);
}

int CStdioFileEx::GetUnicodeStringFromMultiByteString(const QByteArray& multiByte,
                                                      QString& unicodeOut,
                                                      int codePage)
{
    if (codePage == CP_UTF8 || (codePage == -1 && GetCurrentLocaleCodePage() == CP_UTF8))
        unicodeOut = QString::fromUtf8(multiByte);
    else if (codePage == CP_ACP || codePage == -1)
        unicodeOut = QString::fromLocal8Bit(multiByte);
    else
        unicodeOut = QString::fromLatin1(multiByte);
    return unicodeOut.length();
}

int CStdioFileEx::GetRequiredUnicodeLengthFromMultiByteString(const QByteArray& multiByte,
                                                              int codePage)
{
    QString tmp;
    const int len = GetUnicodeStringFromMultiByteString(multiByte, tmp, codePage);
    return len + 1;
}

int CStdioFileEx::GetNewUnicodeStringFromMultiByteString(const QByteArray& multiByte,
                                                         SmartPtrArray<wchar_t>& spUnicodeString,
                                                         int defaultBufferSize,
                                                         int codePage)
{
    QString unicode;
    const int len = GetUnicodeStringFromMultiByteString(multiByte, unicode, codePage);
    if (len == 0)
        return 0;

    const int needed = len + 1;
    if (needed > defaultBufferSize)
    {
        wchar_t* newBuf = new wchar_t[needed];
        spUnicodeString.assign(newBuf, true);
    }

    wchar_t* dest = spUnicodeString.GetBuffer();
    if (dest)
    {
        unicode.toWCharArray(dest);
        dest[len] = L'\0';
    }
    return len;
}

int CStdioFileEx::GetMultiByteStringFromUnicodeString(const QString& unicode,
                                                      QByteArray& multiByteOut,
                                                      int codePage,
                                                      char fillerChar)
{
    QByteArray encoded;
    if (codePage == CP_UTF8 || (codePage == -1 && GetCurrentLocaleCodePage() == CP_UTF8))
        encoded = unicode.toUtf8();
    else
        encoded = unicode.toLocal8Bit();

    for (int i = 0; i < encoded.size(); ++i)
    {
        if (encoded[i] == '?')
            encoded[i] = fillerChar;
    }

    multiByteOut = encoded;
    return encoded.size();
}

int CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString(const QString& unicode,
                                                             int codePage)
{
    const QByteArray encoded = (codePage == CP_UTF8 || (codePage == -1 && GetCurrentLocaleCodePage() == CP_UTF8))
        ? unicode.toUtf8()
        : unicode.toLocal8Bit();
    return encoded.size() + 1;
}

int CStdioFileEx::GetNewMultiByteStringFromUnicodeString(const QString& unicode,
                                                         SmartPtrArray<char>& spMultiByteString,
                                                         int defaultBufferSize,
                                                         int codePage,
                                                         char fillerChar)
{
    QByteArray multiByte;
    const int len = GetMultiByteStringFromUnicodeString(unicode, multiByte, codePage, fillerChar);
    if (len == 0)
        return 0;

    const int needed = len + 1;
    if (needed > defaultBufferSize)
    {
        char* newBuf = new char[needed];
        spMultiByteString.assign(newBuf, true);
    }

    char* dest = spMultiByteString.GetBuffer();
    if (dest)
    {
        std::memcpy(dest, multiByte.constData(), static_cast<std::size_t>(len));
        dest[len] = '\0';
    }
    return len;
}

bool CStdioFileEx::IsFileUnicode(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    const QByteArray data = file.read(2);
    if (data.size() == 2)
    {
        const quint16 bom = static_cast<quint8>(data[0]) | (static_cast<quint8>(data[1]) << 8);
        return bom == UNICODE_BOM;
    }
    return false;
}

int CStdioFileEx::GetCurrentLocaleCodePage()
{
    return static_cast<int>(::GetACP());
}

bool CStdioFileEx::ReadUnicodeLine(QString& output)
{
    QByteArray raw;
    wchar_t wc;
    while (read(reinterpret_cast<char*>(&wc), sizeof(wchar_t)) == sizeof(wchar_t))
    {
        if (wc == L'\n')
            break;
        raw.append(reinterpret_cast<const char*>(&wc), sizeof(wchar_t));
    }

    if (raw.size() >= static_cast<int>(sizeof(wchar_t)))
    {
        wchar_t last = 0;
        std::memcpy(&last, raw.constData() + raw.size() - static_cast<int>(sizeof(wchar_t)), sizeof(wchar_t));
        if (last == L'\r')
            raw.chop(static_cast<int>(sizeof(wchar_t)));
    }

    output = QString::fromWCharArray(reinterpret_cast<const wchar_t*>(raw.constData()),
                                     raw.size() / static_cast<int>(sizeof(wchar_t)));
    return !raw.isEmpty() || !atEnd();
}

bool CStdioFileEx::ReadMultiByteLine(QString& output)
{
    QByteArray line;
    char ch;
    while (read(&ch, 1) == 1)
    {
        if (ch == '\n')
            break;
        line.append(ch);
    }

    if (line.endsWith('\r'))
        line.chop(1);

    QString unicode;
    const int len = GetUnicodeStringFromMultiByteString(line, unicode, m_nFileCodePage);
    if (len > 0)
    {
        output = unicode;
        return true;
    }

    output = QString::fromLatin1(line);
    return !line.isEmpty() || !atEnd();
}

void CStdioFileEx::WriteUnicodeLine(const QString& input)
{
    QByteArray raw(reinterpret_cast<const char*>(input.utf16()),
                   input.length() * static_cast<int>(sizeof(QChar)));
    write(raw);

    const wchar_t nl = L'\n';
    write(reinterpret_cast<const char*>(&nl), sizeof(wchar_t));
}

void CStdioFileEx::WriteMultiByteLine(const QString& input)
{
    QByteArray multiByte;
    GetMultiByteStringFromUnicodeString(input, multiByte, m_nFileCodePage, m_cUnicodeFillerChar);
    write(multiByte);
    write("\n", 1);
}

void CStdioFileEx::SetCodePage(int nCodePage)
{
    m_nFileCodePage = nCodePage;
}

void CStdioFileEx::SetFillerChar(char cFiller)
{
    m_cUnicodeFillerChar = cFiller;
}

void CStdioFileEx::SetWriteBOM(bool bWrite)
{
    m_bWriteBOM = bWrite;
}
