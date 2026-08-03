#ifndef STDIOFILEEX_H
#define STDIOFILEEX_H

#include <windows.h>
#include <QFile>
#include <QByteArray>
#include <QString>

// ----------------------------------------------------------------------
// Constants (mirroring original)
// ----------------------------------------------------------------------
constexpr quint16 UNICODE_BOM = 0xFEFF;
constexpr int STDIOFILEEX_DEFAULT_BUFFER_SIZE = 4096;
constexpr char DEFAULT_UNICODE_FILLER_CHAR = '#';

struct CFileException {};

struct CFile
{
    enum OpenFlags : UINT
    {
        modeRead = 0x0000,
        modeWrite = 0x0001,
        modeReadWrite = 0x0002,
        modeCreate = 0x0010,
        typeText = 0x0020,
        typeBinary = 0x0040,
        shareExclusive = 0x0100,
        shareDenyWrite = 0x0200,
        shareDenyNone = 0x0400,
        shareDenyRead = 0x0800
    };

    enum SeekPosition
    {
        begin = 0,
        current = 1,
        end = 2
    };
};

// ----------------------------------------------------------------------
// Smart pointer for arrays (equivalent to CTemplateSmartPtrArray)
// ----------------------------------------------------------------------
template<typename T>
class SmartPtrArray
{
public:
    SmartPtrArray(T* ptr = nullptr, bool owned = true)
        : m_ptr(ptr), m_owned(owned) {}

    ~SmartPtrArray() { reset(); }

    void reset(T* ptr = nullptr, bool owned = true)
    {
        if (m_owned && m_ptr)
            delete[] m_ptr;
        m_ptr = ptr;
        m_owned = owned;
    }

    T* get() const { return m_ptr; }
    T* operator->() { return m_ptr; }
    T& operator*() { return *m_ptr; }
    operator T*() { return m_ptr; }

    void assign(T* ptr, bool owned = true) { reset(ptr, owned); }
    T* GetBuffer() const { return m_ptr; }

private:
    T* m_ptr;
    bool m_owned;
};

// ----------------------------------------------------------------------
// Main class
// ----------------------------------------------------------------------
class CStdioFileEx : public QFile
{
public:
    // Additional flag for Unicode writing (not used in Qt, but kept for compatibility)
    static const quint32 modeWriteUnicode = 0x200000;

    CStdioFileEx();
    virtual ~CStdioFileEx() {}

    // Overrides
    virtual bool open(QIODevice::OpenMode mode);
    virtual bool open(const QString& fileName, QIODevice::OpenMode mode);
    bool Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL);  // kept for compatibility

    virtual bool ReadString(QString& rString);
    virtual QString ReadString(qint64 maxLen);  // not used; kept for signature

    virtual void WriteString(const QString& lpsz);

    qint64 GetCharCount();
    virtual qint64 Seek(qint64 lOff, int nFrom);

    bool IsFileUnicodeText() const { return m_bIsUnicodeText; }

    void SetCodePage(int nCodePage);
    void SetFillerChar(char cFiller);
    void SetWriteBOM(bool bWrite);
    // void SetUnicode(bool bIsUnicode); // not used

    // Static conversion utilities (reimplemented using Qt)
    static int GetUnicodeStringFromMultiByteString(const QByteArray& multiByte,
                                                   QString& unicodeOut,
                                                   int codePage = -1);

    static int GetRequiredUnicodeLengthFromMultiByteString(const QByteArray& multiByte,
                                                           int codePage = -1);

    static int GetNewUnicodeStringFromMultiByteString(const QByteArray& multiByte,
                                                      SmartPtrArray<wchar_t>& spUnicodeString,
                                                      int defaultBufferSize = 0,
                                                      int codePage = -1);

    static int GetMultiByteStringFromUnicodeString(const QString& unicode,
                                                   QByteArray& multiByteOut,
                                                   int codePage = -1,
                                                   char fillerChar = DEFAULT_UNICODE_FILLER_CHAR);

    static int GetRequiredMultiByteLengthForUnicodeString(const QString& unicode,
                                                          int codePage = -1);

    static int GetNewMultiByteStringFromUnicodeString(const QString& unicode,
                                                      SmartPtrArray<char>& spMultiByteString,
                                                      int defaultBufferSize = 0,
                                                      int codePage = -1,
                                                      char fillerChar = DEFAULT_UNICODE_FILLER_CHAR);

    static bool IsFileUnicode(const QString& filePath);
    static int GetCurrentLocaleCodePage();

protected:
    // Internal reading/writing helpers (called by public methods)
    bool ReadUnicodeLine(QString& output);
    bool ReadMultiByteLine(QString& output);
    void WriteUnicodeLine(const QString& input);
    void WriteMultiByteLine(const QString& input);

    UINT ProcessFlags(UINT& nOpenFlags);

private:
    // Members
    bool m_bIsUnicodeText;
    UINT m_nFlags;
    int m_nFileCodePage;
    char m_cUnicodeFillerChar;
    bool m_bWriteBOM;
    bool m_bReadBOM;
    bool m_bCheckFilePos;

    // Default buffers (to avoid frequent allocation)
    wchar_t m_arrUnicodeDefaultBuffer[STDIOFILEEX_DEFAULT_BUFFER_SIZE];
    char m_arrMultibyteDefaultBuffer[STDIOFILEEX_DEFAULT_BUFFER_SIZE];

    // Internal file position tracking
    qint64 m_currentPos;

    // Actual I/O: we use QFile and QTextStream but we manage encoding manually
    QFile* m_pFile;  // we are deriving from QFile, so we can use this directly
};

#endif // STDIOFILEEX_H
