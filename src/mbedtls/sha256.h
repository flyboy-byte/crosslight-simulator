#pragma once

#include <cstddef>
#include <cstdint>

// Real SHA-256 on the host, via the platform's own crypto library (the same
// split MD5Builder.h uses: CommonCrypto on macOS, OpenSSL on Linux, which the
// sample platformio inis already link with -lssl -lcrypto).
//
// This was previously a rolling XOR stub. That silently made every hash the
// firmware computes wrong in the simulator, which matters because real code
// *compares* these: firmware_flash validates an image's SHA-256 trailer, and
// CrossLight's device lock stores a hash of the passphrase. With a stub, such a
// check either passes or fails for reasons unrelated to the code under test --
// the worst kind of simulator lie.

#if defined(__APPLE__)
#include <CommonCrypto/CommonDigest.h>

struct mbedtls_sha256_context {
  CC_SHA256_CTX ctx{};
};

inline void mbedtls_sha256_init(mbedtls_sha256_context *) {}

inline int mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int) {
  if (ctx) CC_SHA256_Init(&ctx->ctx);
  return 0;
}

inline int mbedtls_sha256_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen) {
  if (ctx && input) CC_SHA256_Update(&ctx->ctx, input, static_cast<CC_LONG>(ilen));
  return 0;
}

inline int mbedtls_sha256_finish(mbedtls_sha256_context *ctx, unsigned char output[32]) {
  if (ctx && output) CC_SHA256_Final(output, &ctx->ctx);
  return 0;
}

inline void mbedtls_sha256_free(mbedtls_sha256_context *) {}

#else
#include <openssl/evp.h>

// EVP rather than the SHA256_* functions, which OpenSSL 3 deprecates.
struct mbedtls_sha256_context {
  EVP_MD_CTX *ctx = nullptr;
};

inline void mbedtls_sha256_init(mbedtls_sha256_context *ctx) {
  if (ctx) ctx->ctx = EVP_MD_CTX_new();
}

inline int mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int) {
  if (!ctx) return -1;
  if (!ctx->ctx) ctx->ctx = EVP_MD_CTX_new();
  return EVP_DigestInit_ex(ctx->ctx, EVP_sha256(), nullptr) == 1 ? 0 : -1;
}

inline int mbedtls_sha256_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen) {
  if (!ctx || !ctx->ctx || !input) return -1;
  return EVP_DigestUpdate(ctx->ctx, input, ilen) == 1 ? 0 : -1;
}

inline int mbedtls_sha256_finish(mbedtls_sha256_context *ctx, unsigned char output[32]) {
  if (!ctx || !ctx->ctx || !output) return -1;
  unsigned int len = 0;
  return EVP_DigestFinal_ex(ctx->ctx, output, &len) == 1 ? 0 : -1;
}

inline void mbedtls_sha256_free(mbedtls_sha256_context *ctx) {
  if (ctx && ctx->ctx) {
    EVP_MD_CTX_free(ctx->ctx);
    ctx->ctx = nullptr;
  }
}
#endif
