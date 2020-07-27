#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../aes.h"

// A multiple of AES_BLK_SIZE
#define DATA_SIZE 8192
// Iterations when calculating average throughput
#define DATA_ITER 1024

// Calculate throughput in Mbps
#define DATA_MBPS(rtime) \
	((double)((DATA_SIZE * DATA_ITER) / rtime / 1048576.0))

static inline void memrand(void *dst, size_t n)
{
	srand((unsigned int)clock());

	for (size_t i = 0; i < n; ++i)
		*((uint8_t *)dst + i) = rand();
}

static inline void hex(void *src, size_t n, int end)
{
	for (size_t i = 0; i < n; ++i)
		printf(" %02x", *((uint8_t *)src + i));

	if (end != EOF)
		putchar(end);
}


static void test_ecb_mode(uint8_t *);
static void test_cbc_mode(uint8_t *);

int main(void)
{
	uint8_t data[DATA_SIZE];

	memrand(data, DATA_SIZE);

	printf("AES key length: %u bits\n", AES_KEY_SIZE * 8);
	printf("Plaintext size: %u bytes\n\n", DATA_SIZE);

	puts("[ECB]");
	test_ecb_mode(data);

	putchar('\n');

	puts("[CBC]");
	test_cbc_mode(data);

	return 0;
}

static void test_ecb_mode(uint8_t *data)
{
	double rtime;

	uint8_t tmp[DATA_SIZE];
	uint8_t key[AES_KEY_SIZE];

	struct aes_ctx ectx, dctx;

	memrand(key, AES_KEY_SIZE);

	printf("\tKey:       ");
	hex(key, AES_KEY_SIZE, '\n');

	aes_ecb_init(&ectx, key);
	aes_ecb_init(&dctx, key);

	memcpy(tmp, data, DATA_SIZE);

	printf("\n\tPlaintext: ");
	hex(tmp, AES_BLK_SIZE, EOF); puts(" ..");

	aes_ecb_encrypt(&ectx, tmp, DATA_SIZE);

	printf("\tCiphertext:");
	hex(tmp, AES_BLK_SIZE, EOF); puts(" ..");

	aes_ecb_decrypt(&dctx, tmp, DATA_SIZE);

	printf("\tDeciphered:");
	hex(tmp, AES_BLK_SIZE, EOF); puts(" ..");

	if (memcmp(data, tmp, DATA_SIZE))
		puts("\n\t[!] Decryption failed");

	// Encryption speed
	rtime = (double)clock();

	for (size_t i = 0; i < DATA_ITER; ++i)
		aes_ecb_encrypt(&ectx, tmp, DATA_SIZE);

	rtime = ((double)clock() - rtime) / CLOCKS_PER_SEC;

	printf(
		"\n\tEncrypted %u bytes in %.3f seconds, %.2f Mb/s",
		DATA_SIZE * DATA_ITER, rtime, DATA_MBPS(rtime)
	);

	// Decryption speed
	rtime = (double)clock();

	for (size_t i = 0; i < DATA_ITER; ++i)
		aes_ecb_decrypt(&dctx, tmp, DATA_SIZE);

	rtime = ((double)clock() - rtime) / CLOCKS_PER_SEC;

	printf(
		"\n\tDecrypted %u bytes in %.3f seconds, %.2f Mb/s\n",
		DATA_SIZE * DATA_ITER, rtime, DATA_MBPS(rtime)
	);
}

static void test_cbc_mode(uint8_t *data)
{
	double rtime;

	uint8_t tmp[DATA_SIZE];

	uint8_t iv [AES_BLK_SIZE];
	uint8_t key[AES_KEY_SIZE];

	struct aes_ctx ectx, dctx;

	memrand(iv,  AES_BLK_SIZE);
	memrand(key, AES_KEY_SIZE);

	printf("\tIV:        ");
	hex(iv, AES_BLK_SIZE, '\n');

	printf("\tKey:       ");
	hex(key, AES_KEY_SIZE, '\n');

	aes_cbc_init(&ectx, key, iv);
	aes_cbc_init(&dctx, key, iv);

	memcpy(tmp, data, DATA_SIZE);

	printf("\n\tPlaintext: ");
	hex(tmp, AES_BLK_SIZE, EOF); puts(" ..");

	aes_cbc_encrypt(&ectx, tmp, DATA_SIZE);

	printf("\tCiphertext:");
	hex(tmp, AES_BLK_SIZE, EOF); puts(" ..");

	aes_cbc_decrypt(&dctx, tmp, DATA_SIZE);

	printf("\tDeciphered:");
	hex(tmp, AES_BLK_SIZE, EOF); puts(" ..");

	if (memcmp(data, tmp, DATA_SIZE))
		puts("\n\t[!] Decryption failed");

	// Encryption speed
	rtime = (double)clock();

	for (size_t i = 0; i < DATA_ITER; ++i)
		aes_cbc_encrypt(&ectx, tmp, DATA_SIZE);

	rtime = ((double)clock() - rtime) / CLOCKS_PER_SEC;

	printf(
		"\n\tEncrypted %u bytes in %.3f seconds, %.2f Mb/s",
		DATA_SIZE * DATA_ITER, rtime, DATA_MBPS(rtime)
	);

	// Decryption speed
	rtime = (double)clock();

	for (size_t i = 0; i < DATA_ITER; ++i)
		aes_cbc_decrypt(&dctx, tmp, DATA_SIZE);

	rtime = ((double)clock() - rtime) / CLOCKS_PER_SEC;

	printf(
		"\n\tDecrypted %u bytes in %.3f seconds, %.2f Mb/s\n",
		DATA_SIZE * DATA_ITER, rtime, DATA_MBPS(rtime)
	);
}
