#include <openssl/evp.h>
#include <stdint.h>
#include <stdio.h>

#include "secret.h"

void handleErrors(void) {
  ERR_print_errors_fp(stderr);
  abort();
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext) {
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /*
   * Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits
   */
  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /*
   * Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary.
   */
  if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /*
   * Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

uint8_t *XORCipher(uint8_t *data, uint8_t key, int dataLen) {
  uint8_t *output = (uint8_t *)malloc((sizeof(uint8_t) * dataLen) + 1);

  for (int i = 0; i < dataLen; ++i) {
    output[i] = data[i] ^ key;
  }

  return output;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf(
        "Please provide the number to unlock the real answer to the "
        "universe!\n");
    return -1;
  }

  int num = 0;
  if (sscanf(argv[1], "%d", &num) != 1) {
    printf("This is not a valid number.\n");
    return -2;
  }
  uint8_t realnum = num & 0xFF;
  printf("So you think that %d will unlock everithing...\n", realnum);
  if (realnum < num) {
    printf("How can you know such big numbers?!?!?!?!?!\n");
    printf(
        "   ▄▄▄▄▄▄▄ ▄ ▄ ▄   ▄▄▄▄  ▄▄▄▄▄▄▄  \n"
        "   █ ▄▄▄ █ ▄██▀▀█  █ █ ▄ █ ▄▄▄ █  \n"
        "   █ ███ █ █▀▀▄██▄▄▄▀ ▄▀ █ ███ █  \n"
        "   █▄▄▄▄▄█ ▄ █▀▄▀█ ▄ █▀█ █▄▄▄▄▄█  \n"
        "   ▄▄▄▄  ▄ ▄ █▄█  ▀███▄ ▄  ▄▄▄ ▄  \n"
        "    █▀█▀█▄▄█▄█▄█ █ ██▀▀▀ ▀▀█ ▄▄▀  \n"
        "   ▄ ▄ █▄▄▀███ ▄█▀  ▄▀█ ▄ ▀▀▄▄ ▀  \n"
        "   █▀█▄██▄█▄  █▀▄ ▀▄▄▄█ ▄▀ ▄ ███  \n"
        "   ▄▄▀█▀ ▄██▀█▀ ▄█▄▀ █▀▀█▀█▄▄ █   \n"
        "   ▄  ▄▀ ▄   ▀  ▀▀█▄   █▄ █▄▀█▀   \n"
        "    ▄▀▄▄▀▄▄ ▄▀▀▄  ▀▀█ ▀▄██▄▄▄█    \n"
        "   ▄▄▄▄▄▄▄ ▀▀ ██▄█ ███▄█ ▄ ██▀█▀  \n"
        "   █ ▄▄▄ █  █▄▄ █▀▀ ▄▀▀█▄▄▄█▀▄ ▄  \n"
        "   █ ███ █ █▀█  ▄ ▀██ ▀█▀ █▀▀▄ █  \n"
        "   █▄▄▄▄▄█ ████▄▄██▀ █▄█▄█ ▀█ █   \n");
    return -2;
  }

  uint8_t *the_key = XORCipher(secret_xored_key, realnum, SECRET_KEY_SIZE);
  if (!the_key) {
    printf("Not enough mem for such good program...\n");
    return -7;
  }
  the_key[SECRET_KEY_SIZE] = 0;
  printf("Seems that the real key is:\n");
  for (uint8_t i = 0; i < 32; i++) printf("%02X", the_key[i]);
  printf("\n");
  FILE *f = fopen(secret_file, "wb");
  if (!f) {
    printf("I cannot write in this directory, but why?");
    free(the_key);
    return -5;
  }

  printf(
      "Let's try to dump the master signal captured from the deeps of the "
      "space with that...\n");

  char *dec = malloc(SECRET_SIZE);
  if (!the_key) {
    printf("Not enough mem for such good program x2...\n");
    return -512;
  }
  int len = decrypt(secret, SECRET_SIZE, the_key, iv, dec);
  fwrite(dec, sizeof(char), len, f);

  free(dec);

  fclose(f);
  free(the_key);

  return 0;
}
