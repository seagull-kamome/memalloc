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
#define TLSF_SLSHIFT               (TEST_SL)
#define TLSF_PREFIX                 my_
#define TLSF_EXTRA_ZONEDATA_T       struct test_zonedata
#define TLSF_CHECK_PARAM (1)
#define TLSF_WARN_HANDLER(fmt, ...) do { nanocspec_quote(fmt, __VA_ARGS__); } while (0)
//#define TLSF_WARN_HANDLER(fmt, ...) do { } while (0)
#define TLSF_ASSERT(x)              do { assert(x); } while (0)
#include "../tlsf.c_inc"


uint8_t blk_10k[2][1024 * 10];
uint8_t blk_100k[2][1024 * 100];


my_tlsf_zone_t zone0;


#define STRESSMODEL_PREFIX      my_tlsf_
#define STRESSMODEL_MALLOC(sz)  (my_tlsf_alloc((sz), zone0))
#define STRESSMODEL_FREE(ptr)   do { my_tlsf_free(ptr, zone0); } while (0)
#define STRESSMODEL_PRINTF(fmt, ...) do { nanocspec_quote(fmt, __VA_ARGS__); } while (0)
#include "./stressmodel.c_inc"



/* ************************************************************************ */
describe(tlsf_show_configs, "Configuration.")

  it("just show configuration before any tests.")
    nanocspec_quote(
        "\n  > unit_size = %zu"
        "\n  > zone_header_size = %zu"
        "\n  > zone_extra_data_size = %zu"
        "\n  > fl_shift = %d"
        "\n  > sl_shift = %d"
        "\n  > machine_alignment = %d"
        ,my_tlsf_config.unit_size, my_tlsf_config.zone_header_size
        ,my_tlsf_config.zone_extra_data_size
        ,my_tlsf_config.fl_shift, my_tlsf_config.sl_shift
        ,my_tlsf_config.machine_alignment
        );
end_describe




describe(tlsf_create_zone, "Create a new memory zone.")

  it("couldn't create zone at NULL.")
    should_eq(NULL, my_tlsf_create_zone(NULL, 1024));
    should_eq(NULL, my_tlsf_create_zone(NULL, 0));

  it("couldn't create zone for empty block.")
    should_eq(NULL, my_tlsf_create_zone(blk_10k, 0));

  it("couldn't create zone for too small block.")
    should_eq(NULL, my_tlsf_create_zone(blk_10k[0], 20));

  it("might create zone for enough size of memory.")
    should_eq(NULL, my_tlsf_create_zone(blk_10k[0], my_tlsf_config.zone_header_size - 1));
    zone0 = my_tlsf_create_zone(blk_10k, my_tlsf_config.zone_header_size);
    should_ne(NULL, zone0);

    zone0 = my_tlsf_create_zone(blk_10k[0], my_tlsf_config.zone_header_size + 101);
    should_ne(NULL, zone0);
end_describe



describe(tlsf_add_block, "Add memory block to zone.")

  it("refuse too small block.")
    zone0 = my_tlsf_create_zone(blk_10k[0], sizeof(blk_10k[0]));
    assert_ne(NULL, zone0);
    should_eq((size_t)0, my_tlsf_add_block(blk_10k[0], 19, zone0) );

  it("might expand space.")
    zone0 = my_tlsf_create_zone(blk_10k[0], sizeof(blk_10k[0]));
    assert_ne(NULL, zone0);

    should_ne((size_t)0, my_tlsf_add_block(blk_10k[1], sizeof(blk_10k[1]), zone0) );
    should_ne((size_t)0, my_tlsf_add_block(blk_100k[0], sizeof(blk_100k[0]), zone0) );
    should_ne((size_t)0, my_tlsf_add_block(blk_100k[1], sizeof(blk_100k[1]), zone0) );

end_describe


describe(tlsf_alloc, "Allocate memory.")
  zone0 = my_tlsf_create_zone(blk_10k[0], sizeof(blk_10k[0]));
  my_tlsf_add_block(blk_10k[1], sizeof(blk_10k[1]), zone0);
  my_tlsf_add_block(blk_100k[0], sizeof(blk_100k[0]), zone0);
  my_tlsf_add_block(blk_100k[1], sizeof(blk_100k[1]), zone0);

  it("returns NULL if no spaces left.")
    should_eq(NULL, my_tlsf_alloc(sizeof(blk_100k[0]), zone0) );

  it("returns NULL for zero-bytes.")
    should_eq(NULL, my_tlsf_alloc(0, zone0));

  it("might allocate too small size.")
    should_ne(NULL, my_tlsf_alloc(my_tlsf_config.unit_size * 2 - 1, zone0));

  it("might allocate some memory.")
    should_ne(NULL, my_tlsf_alloc(100, zone0));
    should_ne(NULL, my_tlsf_alloc(1000, zone0));
    should_ne(NULL, my_tlsf_alloc(5000, zone0));
#if TEST_SL==0
    should_ne(NULL, my_tlsf_alloc(60000, zone0));
#else
    should_ne(NULL, my_tlsf_alloc(98000, zone0));
#endif

end_describe


describe(tlsf_free, "Free.")
  zone0 = my_tlsf_create_zone(blk_10k[0], sizeof(blk_10k[0]));
  my_tlsf_add_block(blk_10k[1], sizeof(blk_10k[1]), zone0);
  my_tlsf_add_block(blk_100k[0], sizeof(blk_100k[0]), zone0);
  my_tlsf_add_block(blk_100k[1], sizeof(blk_100k[1]), zone0);

  it("allows free NULL")
    my_tlsf_free(NULL, zone0);
    my_tlsf_free(zone0, NULL);

  it("may free memory")
    void *p = my_tlsf_alloc(1000, zone0);
    assert_ne(NULL, p);
    my_tlsf_free(p, zone0);

end_describe


describe(tlsf_stressmodel, "Stress")
  zone0 = my_tlsf_create_zone(blk_10k[0], sizeof(blk_10k[0]));
  my_tlsf_add_block(blk_10k[1], sizeof(blk_10k[1]), zone0);
  my_tlsf_add_block(blk_100k[0], sizeof(blk_100k[0]), zone0);
  my_tlsf_add_block(blk_100k[1], sizeof(blk_100k[1]), zone0);

  it("model A")
    struct my_tlsf_stressmodel_bin bins[] = {
      { 16, 1000, }, { 32, 1000, }, { 64, 1000 },
      { 128, 1000, }, { 256, 1000, }, { 512, 1000, },
      { 1024, 1000, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      my_tlsf_stressmodel_A(bins + i);


  it("model B")
    struct my_tlsf_stressmodel_bin bins[] = {
      { 16, 100, }, { 32, 100, }, { 64, 100 },
      { 128, 100, }, { 256, 100, }, { 512, 100, },
      { 1024, 100, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      my_tlsf_stressmodel_B(bins + i);

  it("model C")
    struct my_tlsf_stressmodel_bin bins[] = {
      { 16, 100, }, { 32, 100, }, { 64, 100 },
      { 128, 100, }, { 256, 100, }, { 512, 100, },
      { 1024, 100, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      my_tlsf_stressmodel_C(bins + i);
    //my_tlsf_dump_zone(printf, zone0);

  it("model D")
    struct my_tlsf_stressmodel_bin bins[] = {
      { 16, 100, }, { 32, 100, }, { 64, 100 },
      { 128, 100, }, { 256, 100, }, { 512, 100, },
      { 1024, 100, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      my_tlsf_stressmodel_D(bins + i);

   it("model E")
    struct my_tlsf_stressmodel_bin bins[] = {
      { 16, 100, }, { 32, 100, }, { 64, 100 },
      { 128, 100, }, { 256, 100, }, { 512, 100, },
      { 1024, 100, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      my_tlsf_stressmodel_E(bins + i);

end_describe



/* ************************************************************************ */

int main(int argc, char* argv[]) {
  run_spec(tlsf_show_configs);
  run_spec(tlsf_create_zone);
  run_spec(tlsf_add_block);
  run_spec(tlsf_alloc);
  run_spec(tlsf_free);
  run_spec(tlsf_stressmodel);
  return print_test_report();
}

