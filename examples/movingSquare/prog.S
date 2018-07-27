	.syntax	unified
	.arch	armv7-m

	.section .binary_layout
  .globl __binary_layout
__binary_layout:
  .long	__text_start__
  .long __text_end__
  .long __got_start__
  .long __got_end__
	.long	__data_start__
	.long	__data_end__
  .long __bss_start__
  .long __bss_end__
  .long 0x400       /* !!! HEAP SIZE !!! */
	.long 0xc800			 /* !!! STACK SIZE!!! */
  .size __binary_layout, . - __binary_layout

  .section .jump_vector
  .align	2
  .globl	__jump_vector
__jump_vector:

	/* entry point */
  .long main     // 0
	.size	__jump_vector, . - __jump_vector
