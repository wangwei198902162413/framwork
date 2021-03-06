/* libunwind - a platform-independent unwind library
   Copyright (C) 2003-2004 Hewlett-Packard Co
	Contributed by David Mosberger-Tang <davidm@hpl.hp.com>

This file is part of libunwind.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#define UNW_LOCAL_ONLY

#include <assert.h>
#include <libunwind.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>

#include "jmpbuf.h"

#if UNW_TARGET_IA64
# include "ia64/rse.h"
#endif

void
_longjmp (jmp_buf env, int val)
{
  extern int _UI_longjmp_cont;
  unw_context_t uc;
  unw_cursor_t c;
  unw_word_t sp;
  unw_word_t *wp = (unw_word_t *) env;

  if (unw_getcontext (&uc) < 0 || unw_init_local (&c, &uc) < 0)
    abort ();

  do
    {
      if (unw_get_reg (&c, UNW_REG_SP, &sp) < 0)
	abort ();
      if (sp != wp[JB_SP])
	continue;

#if UNW_TARGET_IA64
      {
	unw_word_t bsp, pfs, sol;

	if (unw_get_reg (&c, UNW_IA64_BSP, &bsp) < 0
	    || unw_get_reg (&c, UNW_IA64_AR_PFS, &pfs) < 0)
	  abort ();

	/* simulate the effect of "br.call setjmp" on ar.bsp: */
	sol = (pfs >> 7) & 0x7f;
	bsp = ia64_rse_skip_regs (bsp, sol);

	if (bsp != wp[JB_BSP])
	  continue;

	if (unlikely (sol == 0))
	  {
	    unw_word_t prev_sp;
	    unw_cursor_t tmp = c;

	    /* The caller of {sig,}setjmp() cannot have a NULL-frame.
	       If we see a NULL-frame, we haven't reached the right
	       target yet.  To have a NULL-frame, the number of locals
	       must be zero and the stack-frame must also be
	       empty.  */

	    if (unw_step (&tmp) < 0)
	      abort ();

	    if (unw_get_reg (&tmp, UNW_REG_SP, &prev_sp) < 0)
	      abort ();

	    if (sp == prev_sp)
	      /* got a NULL-frame; keep looking... */
	      continue;
	  }
      }
#endif

      /* found the right frame: */

      if (UNW_NUM_EH_REGS >= 4)
	{
	  if (unw_set_reg (&c, UNW_REG_EH + 0, wp[JB_RP]) < 0
	      || unw_set_reg (&c, UNW_REG_EH + 1, val) < 0
	      || unw_set_reg (&c, UNW_REG_IP,
			      (unw_word_t) &_UI_longjmp_cont))
	    abort ();
	}
      else
	abort ();

      unw_resume (&c);

      abort ();
    }
  while (unw_step (&c) >= 0);

  abort ();
}

#ifdef __GNUC__
void longjmp (jmp_buf env, int val) __attribute__ ((alias ("_longjmp")));
#else

void
longjmp (jmp_buf env, int val)
{
  _longjmp (env, val);
}

#endif
