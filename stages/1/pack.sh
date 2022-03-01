#!/bin/bash

input="$1"
output="$2"

echo "$output will decompress to $input"

key=$(cat keys/key)
iv=$(cat keys/iv)

echo -ne "#include <stdint.h>\n\n" > secret.h
echo -ne "#define SECRET_SIZE sizeof(secret)\n\n" >> secret.h
echo -ne "#define SECRET_KEY_SIZE sizeof(secret_xored_key)\n\n" >> secret.h
echo -ne "uint8_t secret_xored_key[] = {" >> secret.h
cat keys/xored_key >> secret.h
echo -ne "};\n\n" >> secret.h
echo -ne "uint8_t iv[] = {0x5a, 0x04, 0xec, 0x90, 0x26, 0x86, 0xfb, 0x05, 0xa6, 0xb7, 0xa3, 0x38, 0xb6, 0xe0, 0x77, 0x60, 0x00};" >> secret.h
echo -ne "char secret_file[] = \"" >> secret.h
basename $input | tr -d '\n' >> secret.h
echo -ne "\";\n\n" >> secret.h
echo -ne "uint8_t secret[] = {" >> secret.h
openssl enc -aes-256-cbc -nosalt -e -in $input -K $key -iv $iv | xxd -i >> secret.h
echo -ne "};\n" >> secret.h

gcc -O3 -static -s -o stage.bin main.c $(pkg-config openssl --libs --static) -L/media/sf_Projects/Personal/encrypted_gift/stages/1/openssl/pkg/usr/lib

tar -czvf $output stage.bin
