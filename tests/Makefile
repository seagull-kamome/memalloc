
TESTS=spec_tlsf_sl2 spec_tlsf_sl0 spec_slab

tests: ${TESTS}
	rm test.log
	for i in ${TESTS}; do \
	  echo "# $$i" | tee -a test.log; \
	  ./$$i | tee -a test.log; \
	done


clean:
	rm -f ${TESTS}

TLSF_DEPS:= spec_tlsf.c ../tlsf.c_inc nanocspec/nanocspec.h
CFLAGS+= --std=gnu1x -O3 -g -Wall \
	 -ffunction-sections -fdata-sections -Wl,--gc-sections

spec_tlsf_sl2: ${TLSF_DEPS}
	${CC} ${CFLAGS} -DTEST_SL=2 -o $@ $<

spec_tlsf_sl0: ${TLSF_DEPS}
	${CC} ${CFLAGS} -DTEST_SL=0 -o $@ $<


.PHONY: tests

