#ifndef __BASE64_H__
#define __BASE64_H__

char *base64_encode(unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

unsigned char *base64_decode(char *data,
                    size_t input_length,
                    size_t *output_length);

void build_decoding_table();
void base64_cleanup();

#endif
