   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.13.2 - 04 Jun 2024
   3                     ; Generator (Limited) V4.6.3 - 22 Aug 2024
  56                     ; 3 main()
  56                     ; 4 {
  58                     	switch	.text
  59  0000               _main:
  61  0000 89            	pushw	x
  62       00000002      OFST:	set	2
  65                     ; 6     GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
  67  0001 4bc0          	push	#192
  68  0003 4b20          	push	#32
  69  0005 ae5005        	ldw	x,#20485
  70  0008 cd0000        	call	_GPIO_Init
  72  000b 85            	popw	x
  73  000c               L72:
  74                     ; 11         GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
  76  000c 4b20          	push	#32
  77  000e ae5005        	ldw	x,#20485
  78  0011 cd0000        	call	_GPIO_WriteHigh
  80  0014 84            	pop	a
  81                     ; 12         for(i=0; i<10000; i++);
  83  0015 5f            	clrw	x
  84  0016 1f01          	ldw	(OFST-1,sp),x
  86  0018               L33:
  90  0018 1e01          	ldw	x,(OFST-1,sp)
  91  001a 1c0001        	addw	x,#1
  92  001d 1f01          	ldw	(OFST-1,sp),x
  96  001f 9c            	rvf
  97  0020 1e01          	ldw	x,(OFST-1,sp)
  98  0022 a32710        	cpw	x,#10000
  99  0025 2ff1          	jrslt	L33
 100                     ; 13         GPIO_WriteLow(GPIOB, GPIO_PIN_5);
 102  0027 4b20          	push	#32
 103  0029 ae5005        	ldw	x,#20485
 104  002c cd0000        	call	_GPIO_WriteLow
 106  002f 84            	pop	a
 107                     ; 14         for(i=0; i<10000; i++);
 109  0030 5f            	clrw	x
 110  0031 1f01          	ldw	(OFST-1,sp),x
 112  0033               L14:
 116  0033 1e01          	ldw	x,(OFST-1,sp)
 117  0035 1c0001        	addw	x,#1
 118  0038 1f01          	ldw	(OFST-1,sp),x
 122  003a 9c            	rvf
 123  003b 1e01          	ldw	x,(OFST-1,sp)
 124  003d a32710        	cpw	x,#10000
 125  0040 2ff1          	jrslt	L14
 127  0042 20c8          	jra	L72
 140                     	xdef	_main
 141                     	xref	_GPIO_WriteLow
 142                     	xref	_GPIO_WriteHigh
 143                     	xref	_GPIO_Init
 162                     	end
