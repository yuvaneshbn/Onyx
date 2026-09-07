#ifndef CRYPTO_ENGINE_H
#define CRYPTO_ENGINE_H

#include <QByteArray>
#include <QString>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

namespace Onyx {

class CryptoEngine {
public:
    // Derive a 256-bit AES key from a passphrase using PBKDF2-HMAC-SHA256
    static QByteArray deriveKeyFromPassphrase(const QString& passphrase, const QByteArray& salt = "OnyxLANVoice2026");

    // Compute formatted SHA-256 fingerprint (e.g., "A1:B2:C3...")
    static QString computeKeyFingerprint(const QByteArray& key);

    // AES-256-GCM authenticated chunk encryption/decryption
    static bool encryptChunk(const QByteArray& plaintext, const QByteArray& key, const QByteArray& iv,
                             QByteArray& outCiphertext, QByteArray& outTag);

    static bool decryptChunk(const QByteArray& ciphertext, const QByteArray& key, const QByteArray& iv,
                             const QByteArray& tag, QByteArray& outPlaintext);

    // Envelope encryption for live streaming (Packs: [12-byte IV] + [16-byte Tag] + [Ciphertext])
    static QByteArray encryptEnvelope(const QByteArray& plaintext, const QByteArray& key);
    static bool decryptEnvelope(const QByteArray& envelope, const QByteArray& key, QByteArray& outPlaintext);

    // Streaming SHA-256 file checksum calculation
    static QString computeFileSha256(const QString& filePath);

    // Cryptographically secure random bytes
    static QByteArray generateRandomKey(int length = 32); // 256-bit key
    static QByteArray generateRandomIv(int length = 12);   // 96-bit GCM IV
};

} // namespace Onyx

#endif // CRYPTO_ENGINE_H
