#include <config.h>
#include <version.h>

#include <s5pc110.h>
#include "smdkc110_val.h"
#include "setclock_1st.h"

	.globl setclock_1st
setclock_1st:

	ldr	r0, =ELFIN_CLOCK_POWER_BASE	@0xe0100000

	/* Set Mux to FIN */
	ldr	r1, =0x0
	str	r1, [r0, #CLK_SRC0_OFFSET]

	ldr	r1,	=APLL_LOCKTIME_VAL
	str	r1,	[r0, #APLL_LOCK_OFFSET]


	/* Disable PLL */
	ldr	r1, =0x0
	str	r1, [r0, #APLL_CON0_OFFSET]
	ldr	r1, =0x0
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr	r1, =0x0
	str	r1, [r0, #MPLL_CON_OFFSET]

#if 0
	ldr	r1, =0x0
	str	r1, [r0, #EPLL_CON_OFFSET]
#endif

	ldr   	r1, [r0, #CLK_DIV0_OFFSET]
	ldr	r2, =CLK_DIV0_MASK
	bic	r1, r1, r2

	ldr	r2, =__CLK_DIV0_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV0_OFFSET]

	ldr	r1, =__APLL_VAL
	str	r1, [r0, #APLL_CON0_OFFSET]

	ldr	r1, =__MPLL_VAL
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr	r1, =__VPLL_VAL
	str	r1, [r0, #VPLL_CON_OFFSET]


#if defined(CONFIG_EVT1)
	ldr	r1, =AFC_ON
	str	r1, [r0, #APLL_CON1_OFFSET]
#endif

#if 1
    mov	r1, #0x10000
#else
	mov	r1, #0xF00000
	mov	r1, #0x10000
#endif

1:	subs	r1, r1, #1
	bne	1b

	ldr	r1, [r0, #CLK_SRC0_OFFSET]
	ldr	r2, =0x10001111
	orr	r1, r1, r2
	str	r1, [r0, #CLK_SRC0_OFFSET]


	mov	pc, lr
