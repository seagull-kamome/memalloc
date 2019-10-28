

tests: spec_tlsf
	./spec_tlsf


spec_tlsf: spec_tlsf.c ../tlsf.c_inc nanocspec/nanocspec.h
	$(CC) -g -Wall --function-sections --data-sections -Wl,--gc-sections -o $@ $<

.PHONY: tests

