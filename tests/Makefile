

tests: spec_tlsf_sl2 spec_tlsf_sl0
	echo "# spec_tlsf_sl2" > test.log
	./spec_tlsf_sl2 | tee -a test.log
	echo "# spec_tlsf_sl0" >> test.log
	./spec_tlsf_sl0 | tee -a test.log

clean:
	rm -f spec_tlsf_sl2 spec_tlsf_sl0

TLSF_DEPS:= spec_tlsf.c ../tlsf.c_inc nanocspec/nanocspec.h
CFLAGS+= --std=gnu1x -O3 -g -Wall \
	 -ffunction-sections -fdata-sections -Wl,--gc-sections

spec_tlsf_sl2: ${TLSF_DEPS}
	${CC} ${CFLAGS} -DTEST_SL=2 -o $@ $<

spec_tlsf_sl0: ${TLSF_DEPS}
	${CC} ${CFLAGS} -DTEST_SL=0 -o $@ $<


.PHONY: tests

