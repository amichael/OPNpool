/**
 * @brief OPNpool - Linux sk_buff inspired continuous memory for tx
 *
 *   Since this code originated from code which is public domain, I
 *   hereby declare this code to be public domain as well.
 *
 *   Derived more functionality from macros to C++ template.
 *   Coert Vonk, 2015, 2019
 *
 *   loosely based on http://www.keil.com/download/docs/200.asp
 ****************************************************************************
 *
 *   Since this code originated from code which is public domain, I
 *   hereby declare this code to be public domain as well.
 *
 *   Dave Hylands - dhylands@gmail.com
 *
 ****************************************************************************
 * 
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty. You should have
 * received a copy of the CC0 Public Domain Dedication along with this software.
 * If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 * 
 * SPDX-License-Identifier: CC0-1.0
 */

#include <esp_system.h>
#include <esp_log.h>

#include "skb.h"

//static char const * const TAG = "skb";

skb_handle_t
skb_alloc(size_t size)
{
    skb_t * const skb = malloc(sizeof(skb_t) + size);
    assert(skb);
    skb->len = 0;
    skb->size = size;
    skb->priv.head =
    skb->priv.data =
    skb->priv.tail = skb->buf;
    skb->priv.end  = skb->buf + size;
    return skb;
}

void
skb_free(skb_handle_t const skb)
{
    free(skb);
}

// reserve headroom for protocol headers
void
skb_reserve(skb_handle_t const skb, size_t const header_len)
{
    assert(skb->priv.head == skb->priv.tail);  // only once after skb_alloc()
    assert(skb->priv.tail + header_len <= skb->priv.end);
    skb->priv.data += header_len;
    skb->priv.tail += header_len;
}

// returns pointer to write user data
uint8_t *
skb_put(skb_handle_t const skb, size_t const user_data_len)
{
    assert(skb->priv.tail + user_data_len <= skb->priv.end);
    uint8_t * const ret = skb->priv.tail;
    skb->len += user_data_len;
    skb->priv.tail += user_data_len;
    return ret;
}

// reclaims user_data, and returns pointer to user data
uint8_t *
skb_call(skb_handle_t const skb, size_t const user_data_adj)
{
    assert(skb->priv.tail - user_data_adj >= skb->priv.head);
    skb->len -= user_data_adj;
    skb->priv.tail -= user_data_adj;
    return skb->priv.data;
}

// returns pointer to write header protocol data
uint8_t *
skb_push(skb_handle_t const skb, size_t const header_len)
{
    assert(skb->priv.data - header_len >= skb->priv.head);
    skb->len += header_len;
    return skb->priv.data -= header_len;
}

// reclaims header, and returns pointer to user data
uint8_t *
skb_pull(skb_handle_t const skb, size_t const header_len)
{
    assert(skb->priv.data - header_len >= skb->priv.head);
    skb->len -= header_len;
    return skb->priv.data -= header_len;
}

void
skb_reset(skb_handle_t skb)
{
    skb->len  = 0;
    skb->priv.head =
    skb->priv.data =
    skb->priv.tail = skb->buf;
    skb->priv.end  = skb->buf + skb->size;
}

size_t
skb_print(char const * const tag, skb_handle_t const skb, char * const buf, size_t const buf_size)
{
    size_t len = 0;
    for (size_t ii = 0; ii < skb->len; ii++) {
        len += snprintf(buf + len, buf_size - len, "%02x ", skb->priv.data[ii]);
    }
    return len;
}
