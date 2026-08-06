#include "CSVFile.h"

CCSVFile::CCSVFile()
    : m_file(nullptr), m_stream(nullptr)
{
    // Original CStdioFileEx constructor had no extra work.
}

CCSVFile::~CCSVFile()
{
    Close();
}

bool CCSVFile::Open(const QString &fileName, QIODevice::OpenMode mode)
{
    Close(); // Ensure clean state
    m_file = new QFile(fileName);
    if (!m_file->open(mode))
    {
        delete m_file;
        m_file = nullptr;
        return false;
    }
    m_stream = new QTextStream(m_file);
    if (m_codePage == CP_UTF8) {
        m_stream->setEncoding(QStringConverter::Utf8);
    }
    return true;
}

void CCSVFile::Close()
{
    delete m_stream;
    m_stream = nullptr;
    delete m_file;
    m_file = nullptr;
}

bool CCSVFile::ReadData(QStringList &arr)
{
    if (!m_stream || m_stream->atEnd())
        return false;

    // Read next line (equivalent to CStdioFileEx::ReadString)
    QString sLine = m_stream->readLine();
    // Trim whitespace exactly as the original CString::Trim() does
    sLine = sLine.trimmed();

    if (sLine.isNull() && m_stream->atEnd())
        return false; // EOF after trim (mimics original)

    int nValue = 0;
    arr.clear(); // start fresh (original didn't clear, but removed extra items)

    // Parse values in this line, exactly following the original pointer arithmetic
    int i = 0;
    const QChar *p = sLine.constData();
    while (*p != QChar('\0'))
    {
        QString s;  // String to hold this value

        if (*p == QChar('"'))
        {
            // Bump past opening quote
            p++;
            // Parse quoted value
            while (*p != QChar('\0'))
            {
                if (*p == QChar('"'))
                {
                    // Found one quote
                    p++;
                    // If pair of quotes, keep one
                    // Else interpret as end of value
                    if (*p != QChar('"'))
                    {
                        p++; // move past closing quote
                        break;
                    }
                }
                // Add this character to value
                s.append(*p);
                p++;
            }
        }
        else
        {
            // Parse unquoted value
            while (*p != QChar('\0') && *p != QChar(','))
            {
                s.append(*p);
                p++;
            }
            // Advance to next character (if not already end of string)
            if (*p != QChar('\0'))
                p++; // skip the comma
        }
        // Add this string to value array
        if (nValue < arr.size())
            arr[nValue] = s;
        else
            arr.append(s);
        nValue++;
    }
    // Trim off any unused array values (if original array had more items)
    while (arr.size() > nValue)
        arr.removeLast();
    // Original returns true if ReadString succeeded, even if no values.
    return true;
}

void CCSVFile::WriteData(const QStringList &arr)
{
    if (!m_stream)
        return;

    QChar chQuote = QChar('"');
    QChar chComma = QChar(',');

    // Loop through each string in array
    for (int i = 0; i < arr.size(); i++)
    {
        // Separate this value from previous
        if (i > 0)
            *m_stream << chComma;

        // We need special handling if string contains
        // comma or double quote
        bool bComma = arr[i].contains(chComma);
        bool bQuote = arr[i].contains(chQuote);

        if (bComma || bQuote)
        {
            *m_stream << chQuote;
            if (bQuote)
            {
                for (int j = 0; j < arr[i].length(); j++)
                {
                    // Pairs of quotes interpreted as single quote
                    if (arr[i].at(j) == chQuote)
                        *m_stream << chQuote;   // double the quote
                    *m_stream << arr[i].at(j);
                }
            }
            else
            {
                *m_stream << arr[i];
            }
            *m_stream << chQuote;
        }
        else
        {
            *m_stream << arr[i];
        }
    }
    *m_stream << '\n';  // original WriteString("\n")
}
