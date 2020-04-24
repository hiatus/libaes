libaes
======
Implementation of the current AES (Rijndael cipher) in ECB and CBC modes for the purpose of studying.

API
---
The default `aes.h` header defines the key size as 128 bits and enables both ECB and CBC functions.

```
#define AES_ECB_MODE
#define AES_CBC_MODE

#define AES128
// #define AES192
// #define AES256

#if defined(AES_ECB_MODE)
void aes_ecb_init(struct aes_ctx *ctx, const uint8_t *key);

void aes_ecb_encrypt(const struct aes_ctx *ctx, uint8_t *buffer, size_t len);
void aes_ecb_decrypt(const struct aes_ctx *ctx, uint8_t *buffer, size_t len);
#endif

#if defined(AES_CBC_MODE)
void aes_cbc_init(struct aes_ctx *ctx, const uint8_t *key, const uint8_t *iv);

void aes_cbc_encrypt(struct aes_ctx *ctx, uint8_t *buffer, size_t len);
void aes_cbc_decrypt(struct aes_ctx *ctx, uint8_t *buffer, size_t len);
#endif
```

In order to keep the code small and portable, no padding is done. Therefore, in bytes, the size of `key` must equal `AES_KEY_SIZE`, the size of `iv` must equal `AES_BLK_SIZE` (16) and the size of `buffer` (`len`) must be a multiple of `AES_BLK_SIZE` (16).

Testing
-------
- Compile the test binary (`libaes.so` will also be compiled)
```
$ git clone https://github.com/hiatus/libaes.git && cd libaes && make
```

- Run it
```
$ ./bin/test
```

References
---------
- https://github.com/kokke/tiny-AES-c
- https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
