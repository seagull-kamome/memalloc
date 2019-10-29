

tests: spec_tlsf_sl2 spec_tlsf_sl0
	echo "# spec_tlsf_sl2" > test.log
	./spec_tlsf_sl2 | tee -a test.log
	echo "# spec_tlsf_sl0" >> test.log
	./spec_tlsf_sl0 | tee -a test.log

TLSF_DEPS:= spec_tlsf.c ../tlsf.c_inc nanocspec/nanocspec.h

spec_tlsf_sl2: ${TLSF_DEPS}
	$(CC) -DTEST_SL=2 -O3 -g -Wall --function-sections --data-sections -Wl,--gc-sections -o $@ $<

spec_tlsf_sl0: ${TLSF_DEPS}
	$(CC) -DTEST_SL=0 -O3 -g -Wall --function-sections --data-sections -Wl,--gc-sections -o $@ $<


.PHONY: tests

