	/* Dummy implementation for now.  Libunwind-based setjmp/longjmp
	   can't work on x86 until we take advantage of DWARF2 unwind info.  */

	.globl _UI_siglongjmp_cont
	.globl _UI_longjmp_cont

_UI_siglongjmp_cont:
_UI_longjmp_cont:
#warning fix me
	ret
