/**
 * @file
 * @brief Spec for slab.c_inc file
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
#include <stdlib.h>

struct test_zonedata { unsigned int dummy; };

#define SLAB_ZONE_MALLOC(x) (malloc(x))
#define SLAB_ZONE_FREE(x)   (free(x))
#define SLAB_PAGE_SHIFT     (12)
#define SLAB_PAGE_ALLOC()   (NULL)
#define SLAB_PAGE_FREE(pg)  (-1)
#define SLAB_ASSERT(x)      do { assert(x); } while (0)
#include "../slab.c_inc"


slab_zone_t zone0;
uint8_t __attribute__((aligned(4096))) pages[100][1 << SLAB_PAGE_SHIFT];

#define STRESSMODEL_PREFIX      slab_
#define STRESSMODEL_MALLOC(sz)  (slab_alloc((sz), zone0))
#define STRESSMODEL_FREE(ptr)   do { slab_free(ptr, zone0); } while (0)
#define STRESSMODEL_PRINTF(fmt, ...) do { nanocspec_quote(fmt, __VA_ARGS__); } while (0)
#include "./stressmodel.c_inc"





/* ************************************************************************ */

describe(slab_create_zone, "Create a new memory zone.")

  it("reject wrong zone.")

  it("might create zone with correct settings")
    slab_zone_t zone = slab_create_zone(4, 4, 100);
    should_ne(NULL, zone);
    slab_destroy_zone(zone);

  it("accept page");
    slab_zone_t zone = slab_create_zone(4, 4, 100);
    should_ne(NULL, zone);
    slab_add_page((uintptr_t)pages, 100, zone);
    slab_destroy_zone(zone);

end_describe



describe(slab_stress, "Stres")
  zone0 = slab_create_zone(4, 16, 250);
  slab_add_page((uintptr_t)pages, 100, zone0);


  it("model A")
    struct slab_stressmodel_bin bins[] = {
      { 16, 1000, }, { 32, 1000, }, { 64, 1000 },
      { 128, 1000, }, { 256, 1000, }, { 512, 1000, },
      { 1024, 1000, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_A(bins + i);


  it("model B")
    struct slab_stressmodel_bin bins[] = {
      { 16, 100, }, { 32, 100, }, { 64, 100 },
      { 128, 100, }, { 256, 100, }, { 512, 100, },
      { 1024, 100, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_B(bins + i);

  it("model C")
    struct slab_stressmodel_bin bins[] = {
      { 16, 100, }, { 32, 100, }, { 64, 100 },
      { 128, 100, }, { 256, 100, }, { 512, 100, },
      { 1024, 100, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_C(bins + i);

  it("model D")
    struct slab_stressmodel_bin bins[] = {
      { 16, 100, }, { 32, 100, }, { 64, 100 },
      { 128, 100, }, { 256, 100, }, { 512, 100, },
      { 1024, 100, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_D(bins + i);

   it("model E")
    struct slab_stressmodel_bin bins[] = {
      { 16, 100, }, { 32, 100, }, { 64, 100 },
      { 128, 100, }, { 256, 100, }, { 512, 100, },
      { 1024, 100, }
      };
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_E(bins + i);


end_describe


/* ************************************************************************ */

int main(int argc, char* argv[]) {
  run_spec(slab_create_zone);
  run_spec(slab_stress);
  return print_test_report();
}

