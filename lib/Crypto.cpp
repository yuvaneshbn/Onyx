#include "Crypto.h"
#include <cstring>

// Tell the linker to link to the Cryptography API.
#pragma comment(lib, "Advapi32.lib")

using namespace MFC;

// Constructor, initialises Crypto API.
CCrypto::CCrypto() : m_hCryptProv(NULL), m_hKey(NULL), m_hHash(NULL)
{
    // Create the Crypt context.
    if (!::CryptAcquireContext(&m_hCryptProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        if (::GetLastError() == NTE_BAD_KEYSET) {
            if (!::CryptAcquireContext(&m_hCryptProv,
                NULL,
                NULL,
                PROV_RSA_FULL,
                CRYPT_NEWKEYSET))
            {
                return;
            }
        } else {
            return;
        }
    }

    // Create an empty hash object.
    if (!::CryptCreateHash(m_hCryptProv, CALG_MD5, 0, 0, &m_hHash))
        return;

    // QBuffer is opened automatically in QIODevice::ReadWrite mode.
    m_buffer.open(QIODevice::ReadWrite);
}

// Destructor, frees Crypto stuff.
CCrypto::~CCrypto()
{
    // Close the buffer.
    m_buffer.close();

    // Clean up.
    if (m_hHash)
        ::CryptDestroyHash(m_hHash);

    if (m_hKey)
        ::CryptDestroyKey(m_hKey);

    if (m_hCryptProv)
        ::CryptReleaseContext(m_hCryptProv, 0);
}

// Derive a key from a password.
bool CCrypto::DeriveKey(const QString &strPassword)
{
    // Return failure if we don't have a context or hash.
    if (m_hCryptProv == NULL || m_hHash == NULL)
        return false;

    // If we already have a hash, trash it.
    if (m_hHash) {
        CryptDestroyHash(m_hHash);
        m_hHash = NULL;
        if (!CryptCreateHash(m_hCryptProv, CALG_MD5, 0, 0, &m_hHash))
            return false;
    }

    // If we already have a key, destroy it.
    if (m_hKey) {
        ::CryptDestroyKey(m_hKey);
        m_hKey = NULL;
    }

    // Hash the password. This will have a different result in UNICODE mode, as it
    // will hash the UNICODE string (this is by design, allowing for UNICODE passwords, but
    // it's important to be aware of this behaviour.
    QByteArray passwordData = strPassword.toUtf8(); // Match original: UNICODE string as bytes
    if (!CryptHashData(m_hHash, (const BYTE*)passwordData.constData(), passwordData.size() * sizeof(wchar_t), 0))
        return false;

    // Create a session key based on the hash of the password.
    if (!CryptDeriveKey(m_hCryptProv, CALG_RC2, m_hHash, CRYPT_EXPORTABLE, &m_hKey))
        return false;

    // And we're done.
    return true;
}

bool CCrypto::Encrypt(CObject &serializable, QByteArray &arData)
{
    // Return failure if we don't have a context or key.
    if (m_hCryptProv == NULL || m_hKey == NULL)
        return false;

    // Return failure if the object is not serializable.
    if (serializable.IsSerializable() == false)
        return false;

    // Before we write to the buffer, trash it.
    m_buffer.buffer().clear();
    m_buffer.seek(0);

    // Create a storing stream from the memory buffer.
    QDataStream stream(&m_buffer);
    stream.setVersion(QDataStream::Qt_5_0); // stable version

    // Write the data to the memory buffer.
    serializable.Serialize(stream);

    // Encrypt the contents of the memory buffer and store the result in the array.
    return InternalEncrypt(arData);
}

bool CCrypto::Decrypt(const QByteArray &arData, CObject &serializable)
{
    // Return failure if we don't have a context or key.
    if (m_hCryptProv == NULL || m_hKey == NULL)
        return false;

    // Return failure if the object is not serializable.
    if (serializable.IsSerializable() == false)
        return false;

    // Decrypt the contents of the array to the memory buffer.
    if (InternalDecrypt(arData) == false)
        return false;

    // Create a reading stream from the memory buffer.
    QDataStream stream(&m_buffer);
    stream.setVersion(QDataStream::Qt_5_0);

    // Read the data from the memory buffer.
    serializable.Serialize(stream);

    // And we're done.
    return true;
}

bool CCrypto::Encrypt(const QString &str, QByteArray &arData)
{
    // Return failure if we don't have a context or key.
    if (m_hCryptProv == NULL || m_hKey == NULL)
        return false;

    // Before we write to the buffer, trash it.
    m_buffer.buffer().clear();
    m_buffer.seek(0);

    // Create a storing stream from the memory buffer.
    QDataStream stream(&m_buffer);
    stream.setVersion(QDataStream::Qt_5_0);

    // Write the string to the memory buffer.
    stream << str;

    // Encrypt the contents of the memory buffer and store the result in the array.
    return InternalEncrypt(arData);
}

bool CCrypto::Decrypt(const QByteArray &arData, QString &str)
{
    // Return failure if we don't have a context or key.
    if (m_hCryptProv == NULL || m_hKey == NULL)
        return false;

    // Decrypt the contents of the array to the memory buffer.
    if (InternalDecrypt(arData) == false)
        return false;

    // Create a reading stream from the memory buffer.
    QDataStream stream(&m_buffer);
    stream.setVersion(QDataStream::Qt_5_0);

    // Read the data from the memory buffer.
    stream >> str;

    // And we're done.
    return true;
}

bool CCrypto::InternalEncrypt(QByteArray &arDestination)
{
    // Get the length of the data in the buffer.
    QByteArray &bufferData = m_buffer.buffer();
    ULONGLONG uLength = bufferData.size();
    ULONGLONG uCapacity = uLength * 2;
    bufferData.resize(static_cast<int>(uCapacity)); // increase capacity for encryption

    // We need a DWORD to tell encrypt how much data we're encrypting.
    DWORD dwDataLength = static_cast<DWORD>(uLength);

    // Now encrypt the memory buffer (operates in-place).
    if (!::CryptEncrypt(m_hKey, NULL, TRUE, 0, (BYTE*)bufferData.data(), &dwDataLength, static_cast<DWORD>(uCapacity)))
    {
        return false;
    }

    // Assign all of the data we have encrypted to the byte array.
    arDestination.clear();
    arDestination.append(bufferData.constData(), dwDataLength);

    return true;
}

bool CCrypto::InternalDecrypt(const QByteArray &arSource)
{
    // Trash the buffer.
    m_buffer.buffer().clear();
    m_buffer.seek(0);

    // Write the contents of the byte array to the memory buffer.
    m_buffer.write(arSource);

    QByteArray &bufferData = m_buffer.buffer();
    DWORD dwDataLength = static_cast<DWORD>(bufferData.size());
    DWORD dwOldDataLength = dwDataLength;

    // Now decrypt the data (in-place).
    if (!::CryptDecrypt(m_hKey, NULL, TRUE, 0, (BYTE*)bufferData.data(), &dwDataLength))
    {
        return false;
    }

    // Resize the buffer to the actual decrypted size.
    bufferData.resize(dwDataLength);
    m_buffer.seek(0); // rewind for reading

    return true;
}