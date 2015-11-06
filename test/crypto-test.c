/*
 * emv-tools - a set of tools to work with EMV family of smart cards
 * Copyright (C) 2015 Dmitry Eremin-Solenikov
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "openemv/crypto.h"
#include "openemv/dump.h"

#include <stdlib.h>
#include <string.h>

static int test_genkey(unsigned int keylength, unsigned char *msg, size_t msg_len)
{
	int ret = 1;
	size_t tmp_len, tmp2_len;
	unsigned char *tmp, *tmp2;
	struct crypto_pk *pk;

	printf("Testing key length %d\n", keylength);

	pk = crypto_pk_genkey(PK_RSA, 1, keylength, 3);
	if (!pk)
		goto out;

	tmp_len = crypto_pk_get_nbits(pk);
	if (tmp_len != keylength)
		goto close;

	tmp = crypto_pk_decrypt(pk, msg, msg_len, &tmp_len);
	if (!tmp)
		goto close;

	tmp2 = crypto_pk_encrypt(pk, tmp, tmp_len, &tmp2_len);
	if (!tmp2)
		goto free_tmp;

	if (tmp2_len == msg_len && !memcmp(tmp2, msg, tmp2_len))
		ret = 0;

	free(tmp2);
free_tmp:
	free(tmp);
close:
	crypto_pk_close(pk);

out:
	return ret;
}

static unsigned char message[4096 / 8] =
	"aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbb"
	"ccccccccccccccccdddddddddddddddd"
	"eeeeeeeeeeeeeeeeffffffffffffffff"
	"gggggggggggggggghhhhhhhhhhhhhhhh"
	"iiiiiiiiiiiiiiiijjjjjjjjjjjjjjjj"
	"kkkkkkkkkkkkkkkkllllllllllllllll"
	"mmmmmmmmmmmmmmmmnnnnnnnnnnnnnnnn"
	"oooooooooooooooopppppppppppppppp"
	"qqqqqqqqqqqqqqqqrrrrrrrrrrrrrrrr"
	"sssssssssssssssstttttttttttttttt"
	"uuuuuuuuuuuuuuuuvvvvvvvvvvvvvvvv"
	"wwwwwwwwwwwwwwwwxxxxxxxxxxxxxxxx"
	"yyyyyyyyyyyyyyyyzzzzzzzzzzzzzzzz"
	"aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbb"
	"ccccccccccccccccdddddddddddddddd"
	"eeeeeeeeeeeeeeeeffffffffffffffff"
	;

static unsigned char pk_N[] = {
	0xdb, 0x12, 0xe4, 0xf1, 0x8d, 0x43, 0x74, 0xf0, 0xec, 0x38, 0xdc, 0xfb, 0xf9, 0x20, 0x75, 0x6d,
	0x05, 0xf4, 0x36, 0xc2, 0x21, 0xac, 0x34, 0x0d, 0x16, 0xc5, 0x23, 0xcb, 0xfc, 0x9a, 0x8a, 0xd1,
	0xe0, 0xbd, 0xda, 0xe5, 0x77, 0xd5, 0xaf, 0x65, 0x8d, 0x6b, 0x62, 0x5c, 0xcd, 0x89, 0x06, 0x8d,
	0x11, 0x19, 0x6b, 0x0e, 0x3e, 0xe2, 0x80, 0x45, 0xf6, 0x44, 0x55, 0x21, 0x9c, 0x86, 0x90, 0x00,
	0xa8, 0xaf, 0x8c, 0x94, 0xde, 0x3f, 0xe8, 0x56, 0x52, 0xfe, 0xee, 0xa5, 0x36, 0x72, 0x07, 0xf2,
	0xcf, 0x8e, 0xf0, 0xbd, 0xff, 0x36, 0xd5, 0xf2, 0xad, 0x74, 0x1d, 0x17, 0xd0, 0xb7, 0x93, 0xe2,
	0x2c, 0x8d, 0x3f, 0xb6, 0x7c, 0x65, 0x19, 0x9f, 0xa7, 0x80, 0x1f, 0x9f, 0xe5, 0x2f, 0x2d, 0x75,
	0xc9, 0xc2, 0xe9, 0x70, 0xfa, 0x1e, 0x5a, 0xc6, 0xa3, 0x82, 0xd1, 0x29, 0x5a, 0x60, 0xce, 0x1f,
	0x40, 0x2e, 0xfc, 0x2a, 0x5e, 0xde, 0xc9, 0x67, 0xfc, 0x45, 0x18, 0xce, 0xf2, 0x83, 0x94, 0x53,
	0xd6, 0x4f, 0x2e, 0xc5, 0x2d, 0xa1, 0xa5, 0x7a, 0x63, 0x26, 0x70, 0xcb, 0x76, 0xfc, 0xb5, 0x8d,
	0x0f, 0x88, 0x4c, 0x07, 0xba, 0xfa, 0x8b, 0xbc, 0xa0, 0xea, 0xea, 0x0a, 0xe6, 0xa5, 0x44, 0xa5,
	0x0d, 0x12, 0x66, 0x2b, 0xf7, 0xc4, 0x76, 0xa3, 0x82, 0xa6, 0x2b, 0xb2, 0x5a, 0x27, 0xcd, 0x11,
	0xd2, 0x9d, 0x42, 0x86, 0x8c, 0x82, 0xc8, 0xe1, 0xff, 0x7d, 0xf1, 0xd9, 0xd9, 0xa1, 0xf3, 0x3d,
	0xc3, 0x12, 0x4e, 0x47, 0xc8, 0xa2, 0xcd, 0x72, 0x5a, 0x18, 0xea, 0x89, 0x5c, 0x73, 0x28, 0x52,
	0xf8, 0xdb, 0x70, 0xdc, 0x92, 0xc9, 0xb7, 0x98, 0x10, 0x94, 0x79, 0xdc, 0x9e, 0x12, 0x6c, 0x14,
	0x78, 0xf9, 0x5a, 0xad, 0x00, 0x98, 0xc8, 0x17, 0x79, 0x8a, 0xed, 0xe7, 0xc3, 0xd3, 0xa7, 0x8b,
};

static unsigned char pk_E[] = {
	0x01, 0x00, 0x01,
};

static unsigned char pk_D[] = {
	0x01, 0x17, 0xd4, 0x0a, 0x9c, 0x80, 0xd4, 0xa9, 0x8b, 0x14, 0x31, 0x8e, 0x14, 0x4d, 0x24, 0x28,
	0xda, 0x19, 0xc0, 0xd8, 0x31, 0x20, 0xd1, 0xd5, 0xaa, 0xe2, 0x6a, 0xee, 0x4e, 0xa1, 0x5a, 0xc5,
	0xf7, 0x50, 0x1b, 0x32, 0x7f, 0xe9, 0x92, 0x09, 0x78, 0xae, 0x2b, 0x7c, 0x79, 0x0e, 0x10, 0xf9,
	0x4d, 0x37, 0x8a, 0x40, 0x34, 0xf2, 0x1e, 0x5f, 0xba, 0xfd, 0xd6, 0x4a, 0xe7, 0xa4, 0x08, 0x3d,
	0xe8, 0x99, 0x8f, 0xa3, 0x02, 0x84, 0xe1, 0x1c, 0xe5, 0x27, 0x1e, 0x7b, 0xb6, 0x8c, 0xd5, 0x1b,
	0x52, 0x0b, 0xcd, 0x89, 0xb5, 0x27, 0x49, 0xe3, 0xff, 0x17, 0x90, 0x39, 0x99, 0x32, 0x01, 0x4b,
	0xe4, 0x9b, 0x03, 0xd1, 0x5e, 0x47, 0x86, 0xdc, 0x34, 0x12, 0xc0, 0x95, 0xa4, 0xa8, 0x1a, 0x9a,
	0xf6, 0xd9, 0xc1, 0x1e, 0x6e, 0x31, 0x0e, 0x94, 0xe5, 0x25, 0xf6, 0xf3, 0x34, 0xdf, 0x3c, 0xc8,
	0x0a, 0xc5, 0x8c, 0x00, 0x5c, 0x59, 0x55, 0x06, 0xd1, 0x39, 0x84, 0x35, 0x96, 0x40, 0xe8, 0xb2,
	0xf7, 0x13, 0x83, 0x37, 0xe1, 0xe2, 0x79, 0x41, 0x90, 0x2a, 0xc3, 0x71, 0xc5, 0xcf, 0xf0, 0xaa,
	0x01, 0x2f, 0x48, 0x9c, 0x3f, 0x29, 0x7b, 0xb7, 0x5c, 0xef, 0x25, 0xde, 0x34, 0x23, 0x81, 0x7a,
	0x4c, 0x3a, 0x9b, 0xe4, 0xa7, 0x44, 0x73, 0xbf, 0xf7, 0x39, 0x43, 0xa4, 0x39, 0xa0, 0x1b, 0xf7,
	0x4f, 0x5f, 0x14, 0x49, 0x32, 0x0e, 0x66, 0xd0, 0x29, 0xb5, 0x80, 0xe0, 0xba, 0x3b, 0x88, 0x2b,
	0x14, 0xa4, 0x26, 0x00, 0x2f, 0x50, 0x20, 0x4e, 0xfa, 0xc2, 0x44, 0x72, 0x72, 0x6c, 0x2a, 0x77,
	0x85, 0x20, 0xe0, 0x1d, 0x95, 0x6a, 0x66, 0xe7, 0xb8, 0xca, 0x5b, 0xc9, 0xc3, 0xf3, 0x39, 0xef,
	0xd7, 0xd5, 0x45, 0xb6, 0x3e, 0x19, 0xea, 0x7c, 0x56, 0x20, 0x1b, 0x95, 0x86, 0x2e, 0xc7, 0x51,
};

static unsigned char pk_P[] = {
	0xf5, 0x93, 0x0f, 0x76, 0x00, 0xab, 0x37, 0x01, 0xb9, 0x52, 0xb6, 0x82, 0xf9, 0xf5, 0xae, 0x29,
	0x8f, 0xd5, 0x08, 0xbc, 0xf7, 0x9f, 0x84, 0xb6, 0x4c, 0x94, 0xb5, 0xfc, 0xfe, 0xe1, 0xcd, 0x6a,
	0xf4, 0x9c, 0xa7, 0x33, 0xdb, 0xd8, 0xc8, 0xc1, 0xc0, 0x8d, 0x65, 0xed, 0x29, 0x99, 0x6c, 0x5c,
	0xbe, 0x08, 0xac, 0x04, 0xe4, 0x3a, 0x18, 0xe2, 0x0f, 0x70, 0x26, 0x70, 0x9b, 0x71, 0xfc, 0x9f,
	0x22, 0xea, 0x90, 0x3b, 0xc2, 0xa5, 0x16, 0x7a, 0xcd, 0x04, 0x3e, 0xa6, 0x37, 0x49, 0xa7, 0xee,
	0xaa, 0xe4, 0x9d, 0xaa, 0x9b, 0xb0, 0xe2, 0x6a, 0x9d, 0x1e, 0xcd, 0x83, 0x4e, 0xd8, 0x59, 0x6d,
	0x03, 0xd5, 0x4c, 0x5e, 0xc5, 0x22, 0x10, 0xb7, 0xcc, 0x0c, 0x90, 0x76, 0x05, 0x21, 0xe7, 0x77,
	0x5c, 0x88, 0x5f, 0xd0, 0x5f, 0x9e, 0x2e, 0x49, 0x56, 0xf4, 0x2b, 0xa9, 0x99, 0x57, 0x74, 0x19,
};

static unsigned char pk_Q[] = {
	0xe4, 0x5f, 0xd2, 0x28, 0xbd, 0xf3, 0xdd, 0x70, 0x3d, 0xfd, 0x01, 0x23, 0xae, 0x93, 0x6a, 0x91,
	0xca, 0x68, 0xb1, 0xdb, 0x81, 0xab, 0x1e, 0x63, 0x76, 0x9b, 0x6d, 0xaa, 0x41, 0x87, 0x5a, 0x79,
	0xe7, 0xce, 0xd6, 0x84, 0x32, 0x53, 0xf5, 0xfc, 0xb7, 0x41, 0x7c, 0xcb, 0x88, 0x09, 0xcb, 0xe9,
	0x07, 0x16, 0x28, 0x55, 0x23, 0xe5, 0xf2, 0xf5, 0x23, 0xf5, 0xee, 0x2b, 0x9d, 0x91, 0x56, 0xc6,
	0x30, 0x91, 0x4d, 0x16, 0x11, 0x6c, 0x48, 0x45, 0xe8, 0x5d, 0x0e, 0x9e, 0x04, 0xc8, 0xb6, 0xdd,
	0xba, 0x0d, 0xdf, 0x53, 0x56, 0xfa, 0x0b, 0x0b, 0x99, 0x8d, 0xea, 0x5c, 0x45, 0x7d, 0xed, 0xad,
	0x1f, 0xc5, 0xc1, 0x7d, 0x63, 0x31, 0xf8, 0x32, 0xb5, 0x33, 0xb0, 0xef, 0xce, 0x2e, 0x74, 0x1e,
	0x77, 0x2a, 0x18, 0x35, 0x3d, 0x6e, 0x01, 0xba, 0xde, 0x21, 0x8e, 0x14, 0x12, 0xc3, 0x0d, 0x43,
};

static unsigned char pk_dP[] = {
	0x5a, 0xc8, 0xf7, 0x1a, 0x44, 0xbd, 0x07, 0x24, 0xd8, 0x02, 0x3f, 0xfe, 0xc3, 0xb1, 0x93, 0xa5,
	0x41, 0xcb, 0x1b, 0xe3, 0xe0, 0x17, 0x54, 0xd4, 0xa0, 0x13, 0x0a, 0x04, 0x71, 0xa5, 0xc0, 0x6f,
	0x1d, 0xe7, 0x1b, 0xd9, 0x0c, 0x19, 0x64, 0x7e, 0x5c, 0x54, 0xe9, 0xad, 0x77, 0x87, 0x84, 0x8b,
	0xf4, 0xa4, 0xf8, 0x13, 0x06, 0xdc, 0x83, 0x7e, 0x6e, 0xfe, 0xa2, 0xf7, 0x56, 0x40, 0x19, 0x88,
	0x2b, 0x3c, 0x53, 0xfe, 0x03, 0xc3, 0x4c, 0x40, 0x31, 0xb2, 0xb4, 0x06, 0x76, 0xc2, 0x00, 0x17,
	0x37, 0x8e, 0x34, 0xcb, 0x71, 0xab, 0x3e, 0xc8, 0xf3, 0x35, 0x03, 0xfc, 0xdb, 0x15, 0x18, 0x5a,
	0x38, 0xe4, 0x8d, 0xcb, 0x2b, 0x4d, 0xa0, 0xa8, 0x92, 0x02, 0xc3, 0x15, 0x1e, 0x68, 0x9e, 0x4d,
	0x7e, 0x23, 0xdc, 0x68, 0x08, 0x31, 0x4e, 0x23, 0x46, 0xc6, 0x15, 0xae, 0x29, 0x46, 0x2f, 0x61,
};

static unsigned char pk_dQ[] = {
	0x33, 0x61, 0x9f, 0xae, 0x0c, 0xf6, 0xc6, 0x16, 0x8f, 0xcb, 0xd1, 0xaa, 0xce, 0x87, 0x5a, 0x4d,
	0xcc, 0xe5, 0x7b, 0x46, 0xb0, 0xc8, 0xe8, 0x40, 0x66, 0x9a, 0x17, 0xb5, 0x5b, 0xa2, 0xf1, 0x67,
	0x46, 0x11, 0x52, 0x50, 0x51, 0xe6, 0x74, 0x0c, 0xd4, 0xca, 0x46, 0x22, 0xa0, 0xcb, 0xdb, 0x75,
	0xe5, 0x63, 0x45, 0xd5, 0xca, 0x0a, 0xdd, 0x7b, 0xec, 0x08, 0x53, 0xfa, 0xba, 0x2b, 0xce, 0x03,
	0x2f, 0x40, 0x31, 0xc0, 0xca, 0x50, 0xbb, 0x7e, 0x07, 0x06, 0x90, 0xd8, 0x5a, 0xa9, 0x32, 0x03,
	0x76, 0xed, 0xd2, 0x16, 0x35, 0x16, 0x72, 0xcf, 0xbc, 0x4f, 0xa2, 0xaf, 0xf9, 0xee, 0x98, 0x40,
	0x00, 0x4b, 0x04, 0xfa, 0x8a, 0x0b, 0xdf, 0x14, 0xc1, 0x92, 0x0c, 0xb8, 0x17, 0x82, 0x7a, 0x1b,
	0xb4, 0xa1, 0xe2, 0xea, 0x6f, 0x94, 0xc5, 0x8c, 0xde, 0x97, 0x5c, 0x19, 0x06, 0x13, 0x9e, 0x73,
};

static unsigned char pk_I[] = {
	0x75, 0x40, 0xc6, 0x02, 0x7e, 0x4f, 0xad, 0xdb, 0x95, 0xac, 0x07, 0x8d, 0x80, 0xb6, 0x80, 0x02,
	0x06, 0xdd, 0xb8, 0x5d, 0x92, 0x65, 0x69, 0x26, 0x86, 0x61, 0x6c, 0x87, 0x4e, 0xe5, 0x03, 0x68,
	0xc6, 0x10, 0x15, 0x8c, 0x43, 0x3a, 0x45, 0x63, 0x48, 0xb7, 0x8a, 0x8c, 0xa2, 0x2b, 0x34, 0xb6,
	0x83, 0xfe, 0xa8, 0x10, 0xa5, 0x74, 0xa5, 0xa9, 0x52, 0x42, 0x1f, 0xa0, 0x80, 0x6a, 0xc5, 0x35,
	0xe8, 0xb8, 0xc2, 0xa0, 0x3f, 0x49, 0x18, 0xcf, 0x0e, 0x54, 0x3c, 0x70, 0x11, 0x11, 0xd3, 0x85,
	0x8c, 0xb2, 0xe5, 0x74, 0xdf, 0x98, 0xea, 0x6c, 0xc0, 0x5f, 0x7f, 0xff, 0x69, 0xbf, 0x08, 0x8d,
	0x1b, 0xc4, 0x90, 0xcc, 0xa4, 0xcd, 0xcc, 0x34, 0x58, 0xe5, 0x91, 0x53, 0x3a, 0xd5, 0x39, 0xf4,
	0xd4, 0x42, 0xc9, 0x17, 0xb2, 0x2c, 0x92, 0x12, 0x37, 0x1b, 0xd3, 0xc5, 0x79, 0xd2, 0x65, 0x61,
};

static int test_pk(void)
{
	int ret = 1;
	size_t tmp_len, tmp2_len;
	unsigned char *tmp, *tmp2;
	struct crypto_pk *pubk, *privk;
	unsigned char *msg = message;
	size_t msg_len = sizeof(pk_N);

	printf("Testing public keys interfaces\n");

	pubk = crypto_pk_open(PK_RSA,
			pk_N, sizeof(pk_N),
			pk_E, sizeof(pk_E));
	if (!pubk)
		return 1;

	tmp_len = crypto_pk_get_nbits(pubk);
	if (tmp_len != sizeof(pk_N) * 8)
		goto close_pub;

	tmp = crypto_pk_get_parameter(pubk, 0, &tmp_len);
	if (tmp_len != sizeof(pk_N) || memcmp(tmp, pk_N, tmp_len)) {
		free(tmp);
		goto close_pub;
	}
	free(tmp);

	tmp = crypto_pk_get_parameter(pubk, 1, &tmp_len);
	if (tmp_len != sizeof(pk_E) || memcmp(tmp, pk_E, tmp_len)) {
		free(tmp);
		goto close_pub;
	}
	free(tmp);

	privk = crypto_pk_open_priv(PK_RSA,
			pk_N, sizeof(pk_N),
			pk_E, sizeof(pk_E),
			pk_D, sizeof(pk_D),
			pk_P, sizeof(pk_P),
			pk_Q, sizeof(pk_Q),
			pk_dP, sizeof(pk_dP),
			pk_dQ, sizeof(pk_dQ),
			pk_I, sizeof(pk_I));

	if (!privk)
		goto close_pub;


	tmp_len = crypto_pk_get_nbits(privk);
	if (tmp_len != sizeof(pk_N) * 8)
		goto close_pub;

	tmp = crypto_pk_get_parameter(privk, 0, &tmp_len);
	if (tmp_len != sizeof(pk_N) || memcmp(tmp, pk_N, tmp_len)) {
		free(tmp);
		goto close;
	}
	free(tmp);

	tmp = crypto_pk_get_parameter(privk, 1, &tmp_len);
	if (tmp_len != sizeof(pk_E) || memcmp(tmp, pk_E, tmp_len)) {
		free(tmp);
		goto close;
	}
	free(tmp);

	tmp = crypto_pk_decrypt(privk, msg, msg_len, &tmp_len);
	if (!tmp)
		goto close;

	tmp2 = crypto_pk_encrypt(pubk, tmp, tmp_len, &tmp2_len);
	if (!tmp2)
		goto free_tmp;

	if (tmp2_len == msg_len && !memcmp(tmp2, msg, tmp2_len))
		ret = 0;

	free(tmp2);
free_tmp:
	free(tmp);

close:
	crypto_pk_close(privk);
close_pub:
	crypto_pk_close(pubk);

	return ret;
}

int main(void)
{
	unsigned int keylengths[] = {1024, 1152, 1408, 1984, 2048/*, 3072, 4096*/};
	int i;
	int ret;

	ret = test_pk();
	if (ret)
		return ret;

	for (i = 0; i < sizeof(keylengths) / sizeof(keylengths[0]); i++) {
		unsigned int kl = keylengths[i];
		ret = test_genkey(kl, message, kl / 8);
		if (ret)
			return ret;
	}

	return 0;
}
