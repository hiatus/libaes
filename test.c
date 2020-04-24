#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "aes.h"

static inline void fill_buffer(uint8_t *, size_t);
static inline void print_buffer(uint8_t *, size_t);

int main(void)
{
	uint8_t iv [AES_BLK_SIZE];
	uint8_t key[AES_KEY_SIZE];

	struct aes_ctx e_ctx, d_ctx;

	char buffer [33];
	char message[33] = "this message should be encrypted";

	srand(time(NULL));
	strncpy(buffer, message, 33);

	fill_buffer(iv,  AES_BLK_SIZE);
	fill_buffer(key, AES_KEY_SIZE);

	puts("[*] Key");
	print_buffer(key, AES_KEY_SIZE);

	puts("\n[*] Initialization vector");
	print_buffer(iv, AES_BLK_SIZE);

	puts("\n\n[+] ECB mode");

	aes_ecb_init(&e_ctx, key);
	aes_ecb_init(&d_ctx, key);

	aes_ecb_encrypt(&e_ctx, (uint8_t *)buffer, 32);

	printf("\n- Encrypted plaintext\n");
	print_buffer((uint8_t *)buffer, 32);

	aes_ecb_decrypt(&d_ctx, (uint8_t *)buffer, 32);

	printf("\n- Decrypted ciphertext\n");
	print_buffer((uint8_t *)buffer, 32);

	if (memcmp(buffer, message, 32))
		puts("\n[-] Decryption failed");
	else
		printf("\n[+] Decryption succeeded: '%s'\n", buffer);

	puts("\n\n[+] CBC mode");

	aes_cbc_init(&e_ctx, key, iv);
	aes_cbc_init(&d_ctx, key, iv);

	aes_cbc_encrypt(&e_ctx, (uint8_t *)buffer, 32);

	printf("\n- Encrypted plaintext\n");
	print_buffer((uint8_t *)buffer, 32);

	aes_cbc_decrypt(&d_ctx, (uint8_t *)buffer, 32);

	printf("\n- Decrypted ciphertext\n");
	print_buffer((uint8_t *)buffer, 32);

	if (memcmp(buffer, message, 32))
		puts("\n[-] Decryption failed");
	else
		printf("\n[+] Decryption succeeded: '%s'\n", buffer);

	return 0;
}

static inline void fill_buffer(uint8_t *buffer, size_t len)
{
	for (size_t i = 0; i < len; ++i)
		buffer[i] = (uint8_t)rand();
}

static inline void print_buffer(uint8_t *buffer, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i) {
		if (i % 16 == 0)
			putchar('\n');

		printf("%02x ", buffer[i]);
	}

	putchar('\n');
}
