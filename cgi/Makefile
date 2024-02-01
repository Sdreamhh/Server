target = adder


$(target) : adder.c run.c
	gcc adder.c -o adder
	gcc run.c -o run


risc: 
	../duo-sdk/riscv64-linux-musl-x86_64/bin/riscv64-unknown-linux-musl-gcc -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -o adder adder.c

test: test.c
	gcc -o test test.c ../src/decode.c -I../inc/

clean: 
	-rm $(target)