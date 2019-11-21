
TESTS=spec_tlsf_sl2 spec_tlsf_sl0 spec_slab

tests: ${TESTS}
	rm test.log
	for i in ${TESTS}; do \
	  echo "# $$i" | tee -a test.log; \
	  ./$$i | tee -a test.log; \
	done


clean:
	rm -f ${TESTS}

COPT?=-O3
CFLAGS+= --std=gnu1x ${COPT} -g -Wall \
	 -ffunction-sections -fdata-sections -Wl,--gc-sections


TLSF_DEPS:= spec_tlsf.c ../tlsf.c_inc nanocspec/nanocspec.h
spec_tlsf_sl2: ${TLSF_DEPS}
	${CC} ${CFLAGS} -DTEST_SL=2 -o $@ $<

spec_tlsf_sl0: ${TLSF_DEPS}
	${CC} ${CFLAGS} -DTEST_SL=0 -o $@ $<

SLAB_DEPS:= spec_slab.c ../slab.c_inc nanocspec/nanocspec.h
spec_slab: ${SLAB_DEPS}
	${CC} ${CFLAGS} -o $@ $<


.PHONY: tests

