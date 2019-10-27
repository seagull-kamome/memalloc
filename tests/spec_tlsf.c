/**
 * @file
 * @brief Spec for tlsf.c_inc file
 * @section LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2019 Hattori, Hiroki
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#define NANOCSPEC_MAIN
#include "./nanocspec/nanocspec.h"

struct test_zonedata { unsigned int dummy; };

#define TLSF_FFS        __builtin_ffsl
#define TLSF_CLZ        __builtin_clzl
#define TLSF_SLSHIFT   (2)
#define TLSF_PREFIX     my_
#define TLSF_EXTRA_ZONEDATA_T struct test_zonedata
#define TLSF_DEBUG     (1)
#include "../tlsf.c_inc"


uint8_t blk_10k[2][1024 * 10];
uint8_t blk_100k[2][1024 * 10];


my_tlsf_zone_t zone0;

/* ************************************************************************ */
describe(tlsf_create_zone, "Zone creation of TLSF memory allocation algoryhtm.")

  it("couldn't create zone at NULL.")
    should_eq(NULL, my_tlsf_create_zone(NULL, 1024));
    should_eq(NULL, my_tlsf_create_zone(NULL, 0));

  it("couldn't create zone for empty block.")
    should_eq(NULL, my_tlsf_create_zone(blk_10k, 0));

  it("couldn't create zone for too small block.")
    should_eq(NULL, my_tlsf_create_zone(blk_10k[0], 20));

  it("might create zone for enough size of memory.")
    size_t i;
    for (i = 1; i < 1024 * 10; ++i) {
      zone0 = my_tlsf_create_zone(blk_10k, i);
      if (zone0) break;
    }
    nanospec_printf("\n  >>> Inspected minimum size is %lu\n", i);
    assert_ne(NULL, zone0);


#if 0
  it("create empty zone for minimal zone.")
    should_eq(NULL, my_tlsf_alloc(0, zone0));
    should_Eq(NULL, my_tlsf_alloc(1, zone0));
    should_Eq(NULL, my_tlsf_alloc(2, zone0));
    should_Eq(NULL, my_tlsf_alloc(3, zone0));

  it("returns NULL for zero-size allocation. (for non-empty zone)")
    should_eq(NULL, my_tlsf_alloc(0, zone0));
#endif

end_describe


/* ************************************************************************ */

int main(int argc, char* argv[]) {
  run_spec(tlsf_create_zone);
  return print_test_report();
}

