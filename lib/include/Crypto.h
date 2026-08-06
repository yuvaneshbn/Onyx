#pragma once

// Include the standard cryptography header.
#include "define.h"
#include <wincrypt.h>
#include <QString>
#include <QByteArray>
#include <QBuffer>
#include <QDataStream>

namespace MFC
{
    // Minimal replacement for MFC's CObject serialization.
    // Derive your own serializable classes from this and implement Serialize().
    class CObject
    {
    public:
        virtual bool IsSerializable() const { return true; }
        virtual void Serialize(QDataStream &stream) = 0;
        virtual ~CObject() {}
    };

    // A simple class to wrap the Crypto API.
    class CCrypto
    {
    public:
        // All of the Cryptography API initialisation is done in the 
        // constructor, so constructing this object is expensive. I recommend having
        // only one instance only, and keep it in your application class.
        CCrypto();
        virtual ~CCrypto();

        // These functions are essential to using the crypto object – you must
        // have a key from some source or other.

        // Derive a key from a password.
        virtual bool DeriveKey(const QString &strPassword);

        // These functions handle encryption and decryption.
        virtual bool Encrypt(CObject &serializable, QByteArray &arData);
        virtual bool Decrypt(const QByteArray &arData, CObject &serializable);

        virtual bool Encrypt(const QString &str, QByteArray &arData);
        virtual bool Decrypt(const QByteArray &arData, QString &str);

    protected:
        // Encrypt the contents of the memory buffer and store in the passed array.
        virtual bool InternalEncrypt(QByteArray &arDestination);

        // Decrypt the contents of the passed array and store in the memory buffer.
        virtual bool InternalDecrypt(const QByteArray &arSource);

        // Handle to the cryptography provider.
        HCRYPTPROV m_hCryptProv;

        // Handle to the cryptography key.
        HCRYPTKEY m_hKey;

        // Handle to the hash object.
        HCRYPTHASH m_hHash;

        // Internally, the encryption/decryption of data is done with
        // a QBuffer intermediate.
        QBuffer m_buffer;
    };
}
