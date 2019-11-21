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


#define BUDDY_PAGE_SHIFT        (12)
#define BUDDY_LINES             (5)
#define BUDDY_WARN_HANDLER(fmt, ...) \
    do { nanocspec_quote(fmt, __VA_ARGS__); } while (0)
#define BUDDY_NOMEMORY_HANDLER(zone)  (-1)
#include "../buddy.c_inc"

uint8_t __attribute__((aligned(4096))) pages[100][1 << BUDDY_PAGE_SHIFT];
struct buddy_zone buddy_zone0[1] = { BUDDY_ZONE_INITIALIZER((uintptr_t)pages) };



#define SLAB_PAGE_SHIFT     BUDDY_PAGE_SHIFT
#define SLAB_SLSHIFT        (3)
#define SLAB_CLZ            __builtin_clzl
#define SLAB_PAGE_ALLOC()   (buddy_alloc(0, buddy_zone0))
#define SLAB_PAGE_FREE(pg)  (buddy_free((uintptr_t)(pg), 0, buddy_zone0), 0)
#define SLAB_ASSERT(x)      do { assert(x); } while (0)
#include "../slab.c_inc"


struct slab_zone zone0[1] = { SLAB_ZONE_INITIALIZER({}) };

#define STRESSMODEL_PREFIX      slab_
#define STRESSMODEL_MALLOC(sz)  (slab_alloc((sz), zone0))
#define STRESSMODEL_FREE(ptr)   do { slab_free(ptr, zone0); } while (0)
#define STRESSMODEL_PRINTF(fmt, ...) do { nanocspec_quote(fmt, __VA_ARGS__); } while (0)
#include "./stressmodel.c_inc"





/* ************************************************************************ */

describe(slab_initialize_zone, "Create a new memory zone.")

  it("reject wrong zone.")

  it("might create zone with correct settings")
    slab_initialize_zone(zone0);
    slab_destroy_zone(zone0);

  it("accept page");
    slab_initialize_zone(zone0);
    // slab_give_pages((uintptr_t)pages, 100, zone0);
    slab_destroy_zone(zone0);

end_describe


struct slab_stressmodel_bin bins[] = {
  { 16, 100, }, { 32, 100, }, { 64, 100 },
  { 128, 100, }, { 256, 100, }, { 512, 100, },
  { 1024, 100, }
  };

describe(slab_stress, "Stres")
  slab_initialize_zone(zone0);
  slab_give_pages((uintptr_t)pages, 100, zone0);


  it("model A")
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_A(bins + i);


  it("model B")
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_B(bins + i);

  it("model C")
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_C(bins + i);

  it("model D")
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_D(bins + i);

   it("model E")
    for (unsigned int i = 0; i < sizeof(bins) / sizeof(bins[0]); ++i)
      slab_stressmodel_E(bins + i);

end_describe


/* ************************************************************************ */

int main(int argc, char* argv[]) {
  buddy_give_pages((uintptr_t)pages, 100, buddy_zone0);

  run_spec(slab_initialize_zone);
  run_spec(slab_stress);
  return print_test_report();
}

