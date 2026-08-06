#ifndef MARKUP_H
#define MARKUP_H

#include <QString>
#include <QVector>
#include <QChar>

class CMarkup
{
public:
    CMarkup() { SetDoc(QString()); }
    CMarkup(const QString& szDoc) { SetDoc(szDoc); }
    CMarkup(const CMarkup& markup) { *this = markup; }
    void operator=(const CMarkup& markup);
    virtual ~CMarkup() {}

    // Create
    QString GetDoc() const { return m_csDoc; }
    bool AddElem(const QString& szName, const QString& szData = QString())
        { return x_AddElem(szName, szData, false, false); }
    bool AddChildElem(const QString& szName, const QString& szData = QString())
        { return x_AddElem(szName, szData, false, true); }
    bool AddAttrib(const QString& szAttrib, const QString& szValue)
        { return x_SetAttrib(m_iPos, szAttrib, szValue); }
    bool AddChildAttrib(const QString& szAttrib, const QString& szValue)
        { return x_SetAttrib(m_iPosChild, szAttrib, szValue); }
    bool SetAttrib(const QString& szAttrib, const QString& szValue)
        { return x_SetAttrib(m_iPos, szAttrib, szValue); }
    bool SetChildAttrib(const QString& szAttrib, const QString& szValue)
        { return x_SetAttrib(m_iPosChild, szAttrib, szValue); }

    // Navigate
    bool SetDoc(const QString& szDoc);
    bool IsWellFormed();
    bool FindAttrib(const QString& szName = QString());
    bool FindElem(const QString& szName = QString());
    bool FindChildElem(const QString& szName = QString());
    bool IntoElem();
    bool OutOfElem();
    void ResetChildPos() { x_SetPos(m_iPosParent, m_iPos, 0); }
    void ResetMainPos() { x_SetPos(m_iPosParent, 0, 0); }
    void ResetPos() { x_SetPos(0, 0, 0); }
    QString GetTagName() const;
    QString GetChildTagName() const { return x_GetTagName(m_iPosChild); }
    QString GetData() const { return x_GetData(m_iPos); }
    QString GetChildData() const { return x_GetData(m_iPosChild); }
    QString GetAttrib(const QString& szAttrib) const { return x_GetAttrib(m_iPos, szAttrib); }
    QString GetChildAttrib(const QString& szAttrib) const { return x_GetAttrib(m_iPosChild, szAttrib); }
    QString GetError() const { return m_csError; }

    enum MarkupNodeType
    {
        MNT_ELEMENT                 = 1,
        MNT_TEXT                    = 2,
        MNT_WHITESPACE              = 4,
        MNT_CDATA_SECTION           = 8,
        MNT_PROCESSING_INSTRUCTION  = 16,
        MNT_COMMENT                 = 32,
        MNT_DOCUMENT_TYPE           = 64,
        MNT_EXCLUDE_WHITESPACE      = 123
    };

protected:
#ifdef _DEBUG
    const QChar* m_pMainDS = nullptr;
    const QChar* m_pChildDS = nullptr;
#endif

    QString m_csDoc;
    QString m_csError;

    struct ElemPos
    {
        ElemPos() { Clear(); }
        ElemPos(const ElemPos& pos) { *this = pos; }
        bool IsEmptyElement() const { return (nStartR == nEndL + 1); }
        void Clear()
        {
            nStartL = 0; nStartR = 0; nEndL = 0; nEndR = 0; nReserved = 0;
            iElemParent = 0; iElemChild = 0; iElemNext = 0;
        }
        void AdjustStart(int n) { nStartL += n; nStartR += n; }
        void AdjustEnd(int n)   { nEndL += n; nEndR += n; }
        int nStartL, nStartR, nEndL, nEndR, nReserved;
        int iElemParent, iElemChild, iElemNext;
    };

    QVector<ElemPos> m_aPos;
    int m_iPosParent = 0;
    int m_iPos       = 0;
    int m_iPosChild  = 0;
    int m_iPosFree   = 1;
    int m_nNodeType  = 0;

    struct TokenPos
    {
        TokenPos(const QChar* sz) { Clear(); szDoc = sz; }
        bool IsValid() const { return (nL <= nR); }
        void Clear() { nL = 0; nR = -1; nNext = 0; bIsString = false; }
        bool Match(const QString& szName) const
        {
            int nLen = nR - nL + 1;
            if (nLen != szName.length()) return false;
            for (int i = 0; i < nLen; ++i)
                if (szDoc[nL + i] != szName[i])
                    return false;
            // Also check that the next character is not part of the name (or is separator)
            // The original Match also checked szName[nLen] is '\0' or separator.
            // We handle that by caller comparing exact string; but we mimic original.
            // Actually, original Match returned true if name matches up to nLen and next char is terminator or separator.
            // But we can simply compare the full token string.
            return true; // full token matched exactly
        }
        int nL, nR, nNext;
        const QChar* szDoc;
        bool bIsString;
    };

    void x_SetPos(int iPosParent, int iPos, int iPosChild)
    {
        m_iPosParent = iPosParent;
        m_iPos = iPos;
        m_iPosChild = iPosChild;
        m_nNodeType = iPos ? MNT_ELEMENT : 0;
#ifdef _DEBUG
        m_pMainDS = (m_iPos && m_iPos < m_aPos.size())
                    ? m_csDoc.constData() + m_aPos[m_iPos].nStartL
                    : nullptr;
        m_pChildDS = (m_iPosChild && m_iPosChild < m_aPos.size())
                     ? m_csDoc.constData() + m_aPos[m_iPosChild].nStartL
                     : nullptr;
#endif
    }

    int x_GetFreePos();
    int x_ReleasePos();
    int x_ParseElem(int iPos);
    int x_ParseError(const QString& szError, const QString& szName = QString());
    static bool x_FindChar(const QChar* szDoc, int& nChar, QChar c);
    static bool x_FindAny(const QChar* szDoc, int& nChar);
    static bool x_FindToken(TokenPos& token);
    QString x_GetToken(const TokenPos& token) const;
    int x_FindElem(int iPosParent, int iPos, const QString& szPath);
    QString x_GetTagName(int iPos) const;
    QString x_GetData(int iPos) const;
    QString x_GetAttrib(int iPos, const QString& szAttrib) const;
    bool x_AddElem(const QString& szName, const QString& szValue, bool bInsert, bool bAddChild);
    bool x_FindAttrib(TokenPos& token, const QString& szAttrib = QString()) const;
    bool x_SetAttrib(int iPos, const QString& szAttrib, const QString& szValue);
    void x_LocateNew(int iPosParent, int& iPosRel, int& nOffset, int nLength, int nFlags);
    int x_ParseNode(TokenPos& token);
    void x_DocChange(int nLeft, int nReplace, const QString& csInsert);
    void x_Adjust(int iPos, int nShift, bool bAfterPos = false);
    QString x_TextToDoc(const QString& szText, bool bAttrib = false) const;
    QString x_TextFromDoc(int nLeft, int nRight) const;
};

#endif // MARKUP_H