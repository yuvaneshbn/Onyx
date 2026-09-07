#include "CryptoEngine.h"
#include <QFile>
#include <QDebug>
#include <openssl/kdf.h>

namespace Onyx {

QByteArray CryptoEngine::deriveKeyFromPassphrase(const QString& passphrase, const QByteArray& salt) {
    QByteArray key(32, 0);
    QByteArray passBytes = passphrase.toUtf8();
    
    // 10,000 iterations PBKDF2-HMAC-SHA256
    PKCS5_PBKDF2_HMAC(passBytes.constData(), passBytes.size(),
                      reinterpret_cast<const unsigned char*>(salt.constData()), salt.size(),
                      10000,
                      EVP_sha256(),
                      32,
                      reinterpret_cast<unsigned char*>(key.data()));
    return key;
}

QString CryptoEngine::computeKeyFingerprint(const QByteArray& key) {
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(key.constData()), key.size(), md);
    
    QString fingerprint;
    for (int i = 0; i < 16; ++i) { // First 16 bytes formatted
        if (i > 0) fingerprint += ":";
        fingerprint += QString("%1").arg(md[i], 2, 16, QChar('0')).toUpper();
    }
    return fingerprint;
}

QByteArray CryptoEngine::generateRandomKey(int length) {
    QByteArray key(length, 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(key.data()), length);
    return key;
}

QByteArray CryptoEngine::generateRandomIv(int length) {
    QByteArray iv(length, 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(iv.data()), length);
    return iv;
}

bool CryptoEngine::encryptChunk(const QByteArray& plaintext, const QByteArray& key, const QByteArray& iv,
                               QByteArray& outCiphertext, QByteArray& outTag) {
    if (key.size() != 32 || iv.size() != 12) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    bool ok = false;
    outCiphertext.resize(plaintext.size());
    outTag.resize(16); // 128-bit authentication tag

    int len = 0;
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) == 1 &&
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr) == 1 &&
        EVP_EncryptInit_ex(ctx, nullptr, nullptr,
                           reinterpret_cast<const unsigned char*>(key.constData()),
                           reinterpret_cast<const unsigned char*>(iv.constData())) == 1 &&
        EVP_EncryptUpdate(ctx,
                          reinterpret_cast<unsigned char*>(outCiphertext.data()), &len,
                          reinterpret_cast<const unsigned char*>(plaintext.constData()), plaintext.size()) == 1) {

        int finalLen = 0;
        if (EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(outCiphertext.data()) + len, &finalLen) == 1 &&
            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, reinterpret_cast<unsigned char*>(outTag.data())) == 1) {
            outCiphertext.resize(len + finalLen);
            ok = true;
        }
    }

    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

bool CryptoEngine::decryptChunk(const QByteArray& ciphertext, const QByteArray& key, const QByteArray& iv,
                               const QByteArray& tag, QByteArray& outPlaintext) {
    if (key.size() != 32 || iv.size() != 12 || tag.size() != 16) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    bool ok = false;
    outPlaintext.resize(ciphertext.size());

    int len = 0;
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) == 1 &&
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr) == 1 &&
        EVP_DecryptInit_ex(ctx, nullptr, nullptr,
                           reinterpret_cast<const unsigned char*>(key.constData()),
                           reinterpret_cast<const unsigned char*>(iv.constData())) == 1 &&
        EVP_DecryptUpdate(ctx,
                          reinterpret_cast<unsigned char*>(outPlaintext.data()), &len,
                          reinterpret_cast<const unsigned char*>(ciphertext.constData()), ciphertext.size()) == 1 &&
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tag.size(),
                            const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(tag.constData()))) == 1) {

        int finalLen = 0;
        int ret = EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(outPlaintext.data()) + len, &finalLen);
        if (ret > 0) {
            outPlaintext.resize(len + finalLen);
            ok = true;
        }
    }

    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

QByteArray CryptoEngine::encryptEnvelope(const QByteArray& plaintext, const QByteArray& key) {
    if (key.size() != 32 || plaintext.isEmpty()) return QByteArray();

    QByteArray iv = generateRandomIv(12);
    QByteArray cipher;
    QByteArray tag;
    if (!encryptChunk(plaintext, key, iv, cipher, tag)) {
        return QByteArray();
    }

    QByteArray envelope;
    envelope.reserve(12 + 16 + cipher.size());
    envelope.append(iv);
    envelope.append(tag);
    envelope.append(cipher);
    return envelope;
}

bool CryptoEngine::decryptEnvelope(const QByteArray& envelope, const QByteArray& key, QByteArray& outPlaintext) {
    if (key.size() != 32 || envelope.size() < (12 + 16)) return false;

    QByteArray iv = envelope.left(12);
    QByteArray tag = envelope.mid(12, 16);
    QByteArray cipher = envelope.mid(28);

    return decryptChunk(cipher, key, iv, tag, outPlaintext);
}

QString CryptoEngine::computeFileSha256(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }

    EVP_MD_CTX* mdCtx = EVP_MD_CTX_new();
    if (!mdCtx) {
        file.close();
        return QString();
    }

    EVP_DigestInit_ex(mdCtx, EVP_sha256(), nullptr);

    constexpr qint64 bufSize = 64 * 1024;
    char buffer[bufSize];
    while (!file.atEnd()) {
        qint64 bytesRead = file.read(buffer, bufSize);
        if (bytesRead > 0) {
            EVP_DigestUpdate(mdCtx, buffer, bytesRead);
        }
    }
    file.close();

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen = 0;
    EVP_DigestFinal_ex(mdCtx, hash, &hashLen);
    EVP_MD_CTX_free(mdCtx);

    QByteArray hashBytes(reinterpret_cast<const char*>(hash), hashLen);
    return QString::fromLatin1(hashBytes.toHex());
}

} // namespace Onyx
