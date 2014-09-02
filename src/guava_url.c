/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_url.h"
#include "guava_string.h"
#include <assert.h>

/**
 * Codes copy from http://www.geekhideout.com/urlcode.shtml
 */

static char hex_codes[] = "0123456789ABCDEF";

char n_from_hex(char c) {
	return isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
}

char n_to_hex(char c) {
	assert(c >= 0 && c <= 15);
	return hex_codes[c & 15];
}

guava_string_t guava_url_encode(const char *str) {
	char *pstr = (char *)str, *buf = malloc(strlen(str) * 3 + 1), *pbuf = buf;
	while (*pstr) {
		if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
			*pbuf++ = *pstr;
		} else if (*pstr == ' ') {
			*pbuf++ = '+';
		} else {
			*pbuf++ = '%', *pbuf++ = n_to_hex(*pstr >> 4), *pbuf++ = n_to_hex(*pstr & 15);
		}
		pstr++;
	}
	*pbuf = '\0';
	guava_string_t b = guava_string_new(buf);
	free(buf);

	return b;
}

guava_string_t guava_url_decode(const char *str) {
	char *pstr = (char *)str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = n_from_hex(pstr[1]) << 4 | n_from_hex(pstr[2]);
				pstr += 2;
			}
		} else if (*pstr == '+') { 
			*pbuf++ = ' ';
		} else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	guava_string_t b = guava_string_new(buf);
	free(buf);

	return b;
}