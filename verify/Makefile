
HOME				= $(shell echo $$HOME)
RUN_CPM			= $(HOME)/gratze/emulator/gratze --type cpm80 --turbo
MBASIC_CPM	= $(RUN_CPM) ${HOME}/gratze/emulator/mbasic.com
BASALT			= $(HOME)/basalt/basalt

all: test_1 test_2 test_3 test_4 test_5 test_6 test_7 test_8

test_1: test_1.out test_1.c_out test_1.z_out 
	diff test_1.out test_1.c_out && diff test_1.out test_1.z_out

test_2: test_2.out test_2.c_out test_2.z_out 
	diff test_2.out test_2.c_out && diff test_2.out test_2.z_out

test_3: #test_3.out

test_4: test_4.out test_4.c_out test_4.z_out 
	diff test_4.out test_4.c_out && diff test_4.out test_4.z_out

test_5: test_5.out test_5.c_out test_5.z_out 
	diff test_5.out test_5.c_out && diff test_5.out test_5.z_out

test_6: test_6.out test_6.c_out test_6.z_out 
	diff test_6.out test_6.c_out && diff test_6.out test_6.z_out

test_7: test_7.out test_7.c_out test_7.z_out 
	diff test_7.out test_7.c_out && diff test_7.out test_7.z_out

test_8: test_8.out test_8.c_out test_8.z_out 
	diff test_8.out test_8.c_out && diff test_8.out test_8.z_out

%.out : %.bas
	$(MBASIC_CPM) $< | sed "0,/^--START/d" > $@

%.c_out : %.bas
	$(BASALT) -a ansi-c -o ${basename $<}.c $< && \
	gcc ${basename $<}.c -o ${basename $<}.bin -L.. -lbasaltrt && \
	./${basename $<}.bin | sed "0,/^--START/d" > $@

%.z_out : %.bas
	$(BASALT) -a z80 -o ${basename $<}.asm $< && \
	z80asm -o ${basename $<}.com ${basename $<}.asm && \
	$(RUN_CPM) ${basename $<}.com | sed "0,/^--START/d" > $@

clean:
	rm -f *.out *.z_out *.c_out *.c *.asm *_out *.bin *.com bdos_debug.txt