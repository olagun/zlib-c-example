// gcc zlib.c -lz

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

typedef unsigned char Byte;

typedef struct {
  Byte *bytes;
  uLong len;
  uLong og_len;
} Buffer;

Buffer *compress_bytes(Byte *uncompressed_bytes, uLong uncompressed_len) {
  // Compressed files can be slightly larger than uncompressed files because
  // zLib has a minimum file size
  int min_size = 100;

  uLong compressed_len = uncompressed_len + min_size;
  Byte *compressed_bytes = malloc(compressed_len * sizeof(char));

  int error_code =
      compress(compressed_bytes,  // Compressed bytes will go here
               &compressed_len,   // Will be modified once compression happens
               uncompressed_bytes, uncompressed_len);

  if (error_code != Z_OK) {
    printf("%s\n", zError(error_code));
    return NULL;
  }

  Buffer *buffer = malloc(sizeof(Buffer));

  buffer->bytes = compressed_bytes;
  buffer->len = compressed_len;
  buffer->og_len = uncompressed_len;

  return buffer;
}

Byte *decompress_bytes(Buffer *buffer) {
  uLong uncompressed_len = buffer->og_len;
  Byte *uncompressed_bytes = malloc(uncompressed_len * sizeof(char));

  int error_code = uncompress(
      uncompressed_bytes,  // Uncompressed bytes will go here
      &uncompressed_len,   // Will be modified once decompression happens
      buffer->bytes, buffer->len);

  if (error_code != Z_OK) {
    printf("%s\n", zError(error_code));
    return NULL;
  }

  return uncompressed_bytes;
}

int main(int argc, char **argv) {
  Byte *hello_world = (Byte *)"hello world";

  Buffer *compressed = compress_bytes(hello_world, strlen((char *)hello_world));
  Byte *decompressed = decompress_bytes(compressed);

  printf("%s\n", decompressed);

  return 0;
}