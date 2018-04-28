/*================================================================
*   Copyright (C) 2018 Vertiv Tech Co Ltd. All rights reserved.
*   
*   文件名称：codeconvert.hpp
*   创 建 者：yunzhongke
*   创建日期：2018年04月07日12时
*   描    述：
*
================================================================*/


#ifndef _CODECONVERT_H
#define _CODECONVERT_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iconv.h"

static inline int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen,
        char *outbuf, size_t outlen) {
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset, from_charset);
    if (cd == 0)
        return -1;
    memset(outbuf, 0, outlen);
    if ((int)iconv(cd, pin, &inlen, pout, &outlen) == -1)
        return -1;
    iconv_close(cd);
    //*pout = NULL;

    return 0;
}

static inline int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
    return code_convert((char*)"utf-8", (char*)"gb18030", inbuf, inlen, outbuf, outlen);
}

static inline int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
    return code_convert((char*)"gb18030", (char*)"utf-8", inbuf, inlen, outbuf, outlen);
}


#endif //CODECONVERT_H
