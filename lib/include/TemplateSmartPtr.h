#ifndef TEMPLATESMARTPTR_H
#define TEMPLATESMARTPTR_H

#include <QtGlobal>

#define bTEMPLATESMARTPTR_NOTOWNED false

// Replacement for MFC's CTemplateSmartPtrArray
template <typename TYPE>
class CTemplateSmartPtrArray
{
public:
    CTemplateSmartPtrArray()
        : m_pBuffer(nullptr), m_bOwnObject(true) {}

    explicit CTemplateSmartPtrArray(int nArrayLength)
        : m_bOwnObject(true)
    {
        InitBuffer(nArrayLength);
    }

    CTemplateSmartPtrArray(TYPE* pBuffer, bool bOwned = true)
        : m_pBuffer(nullptr), m_bOwnObject(true)
    {
        Assign(pBuffer, bOwned);
    }

    virtual ~CTemplateSmartPtrArray()
    {
        if (m_pBuffer && m_bOwnObject)
            delete[] m_pBuffer;
    }

    void InitBuffer(int nArrayLength)
    {
        m_pBuffer = new TYPE[nArrayLength];
    }

    void Assign(TYPE* pBuffer, bool bOwned = true)
    {
        if (m_pBuffer && m_bOwnObject)
            delete[] m_pBuffer;
        m_pBuffer = pBuffer;
        m_bOwnObject = bOwned;
    }

    TYPE* GetBuffer() const
    {
        Q_ASSERT(m_pBuffer);
        return m_pBuffer;
    }

    bool IsNull() const
    {
        return m_pBuffer == nullptr;
    }

    void SetBufferOwnership(bool bOwnObject)
    {
        m_bOwnObject = bOwnObject;
    }

    TYPE* m_pBuffer;
    bool m_bOwnObject;
};

// Replacement for MFC's CTemplateSmartPtr (single object)
template <typename TYPE>
class CTemplateSmartPtr
{
public:
    CTemplateSmartPtr()
        : m_bOwnObject(true)
    {
        Init();
    }

    explicit CTemplateSmartPtr(TYPE* pThing)
        : m_bOwnObject(true), m_pThing(nullptr)
    {
        Assign(pThing);
    }

    virtual ~CTemplateSmartPtr()
    {
        if (m_pThing && m_bOwnObject)
            delete m_pThing;
    }

    void Init()
    {
        m_pThing = new TYPE;
    }

    void Assign(TYPE* pThing)
    {
        if (m_pThing)
            delete m_pThing;
        m_pThing = pThing;
    }

    void Detach()
    {
        m_pThing = nullptr;
    }

    bool IsNull() const
    {
        return m_pThing == nullptr;
    }

    TYPE* GetPtr() const
    {
        Q_ASSERT(m_pThing);
        return GetPtrSafe();
    }

    TYPE* GetPtrSafe() const
    {
        return m_pThing;
    }

    void SetOwnObject(bool bOwnObject)
    {
        m_bOwnObject = bOwnObject;
    }

    TYPE* m_pThing;
    bool m_bOwnObject;
};

#endif // TEMPLATESMARTPTR_H