

tests: spec_tlsf
	./spec_tlsf


spec_tlsf: spec_tlsf.c ../tlsf.c_inc nanocspec/nanocspec.h
	$(CC) -g -Wall -o $@ $<

.PHONY: tests

