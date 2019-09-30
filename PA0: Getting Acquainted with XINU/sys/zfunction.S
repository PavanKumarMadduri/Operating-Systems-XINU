          .global    zfunction
          .text

zfunction:	push %ebp
		movl %esp, %ebp
		movl 8(%esp), %eax
		andl $0xff803fff, %eax
		shl  $4, %eax
		andl $0xfffffff0, %eax
		leave
		ret
