ifneq (mips, $(findstring mips, $(CC)))
	target = Makefile.x86
else
	target = Makefile.mips
endif

all:
	make -f $(target)

clean:
	make -f $(target) clean
