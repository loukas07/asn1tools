/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * This file was generated by asn1tools version 0.145.2 Thu Jan 24 07:11:13 2019.
 */

#include <string.h>

#include "boolean_uper.h"

struct encoder_t {
    uint8_t *buf_p;
    ssize_t size;
    ssize_t pos;
};

struct decoder_t {
    const uint8_t *buf_p;
    ssize_t size;
    ssize_t pos;
};

static void encoder_init(struct encoder_t *self_p,
                         uint8_t *buf_p,
                         size_t size)
{
    self_p->buf_p = buf_p;
    self_p->size = (8 * (ssize_t)size);
    self_p->pos = 0;
}

static ssize_t encoder_get_result(struct encoder_t *self_p)
{
    if (self_p->size >= 0) {
        return ((self_p->pos + 7) / 8);
    } else {
        return (self_p->pos);
    }
}

static void encoder_abort(struct encoder_t *self_p,
                          ssize_t error)
{
    if (self_p->size >= 0) {
        self_p->size = -error;
        self_p->pos = -error;
    }
}

static ssize_t encoder_alloc(struct encoder_t *self_p,
                             size_t size)
{
    ssize_t pos;

    if (self_p->pos + (ssize_t)size <= self_p->size) {
        pos = self_p->pos;
        self_p->pos += (ssize_t)size;
    } else {
        pos = -ENOMEM;
        encoder_abort(self_p, ENOMEM);
    }

    return (pos);
}

static void encoder_append_bit(struct encoder_t *self_p,
                               int value)
{
    ssize_t pos;

    pos = encoder_alloc(self_p, 1);

    if (pos < 0) {
        return;
    }

    if ((pos % 8) == 0) {
        self_p->buf_p[pos / 8] = 0;
    }

    self_p->buf_p[pos / 8] |= (value << (7 - (pos % 8)));
}

static void encoder_append_bool(struct encoder_t *self_p, bool value)
{
    encoder_append_bit(self_p, value ? 1 : 0);
}

static void decoder_init(struct decoder_t *self_p,
                         const uint8_t *buf_p,
                         size_t size)
{
    self_p->buf_p = buf_p;
    self_p->size = (8 * (ssize_t)size);
    self_p->pos = 0;
}

static ssize_t decoder_get_result(struct decoder_t *self_p)
{
    if (self_p->size >= 0) {
        return ((self_p->pos + 7) / 8);
    } else {
        return (self_p->pos);
    }
}

static void decoder_abort(struct decoder_t *self_p,
                          ssize_t error)
{
    if (self_p->size >= 0) {
        self_p->size = -error;
        self_p->pos = -error;
    }
}

static ssize_t decoder_free(struct decoder_t *self_p,
                            size_t size)
{
    ssize_t pos;

    if (self_p->pos + (ssize_t)size <= self_p->size) {
        pos = self_p->pos;
        self_p->pos += (ssize_t)size;
    } else {
        pos = -EOUTOFDATA;
        decoder_abort(self_p, EOUTOFDATA);
    }

    return (pos);
}

static int decoder_read_bit(struct decoder_t *self_p)
{
    ssize_t pos;
    int value;

    pos = decoder_free(self_p, 1);

    if (pos >= 0) {
        value = ((self_p->buf_p[pos / 8] >> (7 - (pos % 8))) & 1);
    } else {
        value = 0;
    }

    return (value);
}

static bool decoder_read_bool(struct decoder_t *self_p)
{
    return (decoder_read_bit(self_p) != 0);
}

static void boolean_uper_boolean_a_encode_inner(
    struct encoder_t *encoder_p,
    const struct boolean_uper_boolean_a_t *src_p)
{
    encoder_append_bool(encoder_p, src_p->value);
}

static void boolean_uper_boolean_a_decode_inner(
    struct decoder_t *decoder_p,
    struct boolean_uper_boolean_a_t *dst_p)
{
    dst_p->value = decoder_read_bool(decoder_p);
}

ssize_t boolean_uper_boolean_a_encode(
    uint8_t *dst_p,
    size_t size,
    const struct boolean_uper_boolean_a_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    boolean_uper_boolean_a_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t boolean_uper_boolean_a_decode(
    struct boolean_uper_boolean_a_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    boolean_uper_boolean_a_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}
