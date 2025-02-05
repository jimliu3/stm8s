   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.13.2 - 04 Jun 2024
   3                     ; Generator (Limited) V4.6.3 - 22 Aug 2024
  72                     ; 19 void delay_ms(int ms) //Function Definition 
  72                     ; 20 {
  74                     	switch	.text
  75  0000               _delay_ms:
  77  0000 89            	pushw	x
  78  0001 5204          	subw	sp,#4
  79       00000004      OFST:	set	4
  82                     ; 21 int i =0 ;
  84                     ; 22 int j=0;
  86                     ; 23 for (i=0; i<=ms; i++)
  88  0003 5f            	clrw	x
  89  0004 1f01          	ldw	(OFST-3,sp),x
  92  0006 201a          	jra	L34
  93  0008               L73:
  94                     ; 25 for (j=0; j<120; j++) // Nop = Fosc/4
  96  0008 5f            	clrw	x
  97  0009 1f03          	ldw	(OFST-1,sp),x
  99  000b               L74:
 100                     ; 26 _asm("nop"); //Perform no operation 
 103  000b 9d            nop
 105                     ; 25 for (j=0; j<120; j++) // Nop = Fosc/4
 107  000c 1e03          	ldw	x,(OFST-1,sp)
 108  000e 1c0001        	addw	x,#1
 109  0011 1f03          	ldw	(OFST-1,sp),x
 113  0013 9c            	rvf
 114  0014 1e03          	ldw	x,(OFST-1,sp)
 115  0016 a30078        	cpw	x,#120
 116  0019 2ff0          	jrslt	L74
 117                     ; 23 for (i=0; i<=ms; i++)
 119  001b 1e01          	ldw	x,(OFST-3,sp)
 120  001d 1c0001        	addw	x,#1
 121  0020 1f01          	ldw	(OFST-3,sp),x
 123  0022               L34:
 126  0022 9c            	rvf
 127  0023 1e01          	ldw	x,(OFST-3,sp)
 128  0025 1305          	cpw	x,(OFST+1,sp)
 129  0027 2ddf          	jrsle	L73
 130                     ; 28 }
 133  0029 5b06          	addw	sp,#6
 134  002b 81            	ret
 160                     ; 36 void SPI_setup(void)
 160                     ; 37 {
 161                     	switch	.text
 162  002c               _SPI_setup:
 166                     ; 38      SPI_DeInit();
 168  002c cd0000        	call	_SPI_DeInit
 170                     ; 39      SPI_Init(SPI_FIRSTBIT_MSB, 
 170                     ; 40               SPI_BAUDRATEPRESCALER_2, 
 170                     ; 41               SPI_MODE_MASTER, 
 170                     ; 42               SPI_CLOCKPOLARITY_HIGH, 
 170                     ; 43               SPI_CLOCKPHASE_1EDGE, 
 170                     ; 44               SPI_DATADIRECTION_1LINE_TX, 
 170                     ; 45               SPI_NSS_SOFT, 
 170                     ; 46               0x00);
 172  002f 4b00          	push	#0
 173  0031 4b02          	push	#2
 174  0033 4bc0          	push	#192
 175  0035 4b00          	push	#0
 176  0037 4b02          	push	#2
 177  0039 4b04          	push	#4
 178  003b 5f            	clrw	x
 179  003c cd0000        	call	_SPI_Init
 181  003f 5b06          	addw	sp,#6
 182                     ; 47      SPI_Cmd(ENABLE);
 184  0041 a601          	ld	a,#1
 185  0043 cd0000        	call	_SPI_Cmd
 187                     ; 48 }
 190  0046 81            	ret
 237                     ; 60 void SPI_write(unsigned char slave_address, unsigned char value)
 237                     ; 61 {
 238                     	switch	.text
 239  0047               _SPI_write:
 241  0047 89            	pushw	x
 242       00000000      OFST:	set	0
 245  0048               L111:
 246                     ; 62     while(SPI_GetFlagStatus(SPI_FLAG_BSY));
 248  0048 a680          	ld	a,#128
 249  004a cd0000        	call	_SPI_GetFlagStatus
 251  004d 4d            	tnz	a
 252  004e 26f8          	jrne	L111
 253                     ; 63     GPIO_WriteLow(ChipSelect_port, ChipSelect_pin);
 255  0050 4b10          	push	#16
 256  0052 ae500a        	ldw	x,#20490
 257  0055 cd0000        	call	_GPIO_WriteLow
 259  0058 84            	pop	a
 260                     ; 65     SPI_SendData(slave_address);
 262  0059 7b01          	ld	a,(OFST+1,sp)
 263  005b cd0000        	call	_SPI_SendData
 266  005e               L711:
 267                     ; 66     while(!SPI_GetFlagStatus(SPI_FLAG_TXE));
 269  005e a602          	ld	a,#2
 270  0060 cd0000        	call	_SPI_GetFlagStatus
 272  0063 4d            	tnz	a
 273  0064 27f8          	jreq	L711
 274                     ; 68     SPI_SendData(value);
 276  0066 7b02          	ld	a,(OFST+2,sp)
 277  0068 cd0000        	call	_SPI_SendData
 280  006b               L521:
 281                     ; 69     while(!SPI_GetFlagStatus(SPI_FLAG_TXE));
 283  006b a602          	ld	a,#2
 284  006d cd0000        	call	_SPI_GetFlagStatus
 286  0070 4d            	tnz	a
 287  0071 27f8          	jreq	L521
 288                     ; 71     GPIO_WriteHigh(ChipSelect_port, ChipSelect_pin);
 290  0073 4b10          	push	#16
 291  0075 ae500a        	ldw	x,#20490
 292  0078 cd0000        	call	_GPIO_WriteHigh
 294  007b 84            	pop	a
 295                     ; 72 }
 298  007c 85            	popw	x
 299  007d 81            	ret
 312                     .const:	section	.text
 313  0000               _alphabets:
 314  0000 41            	dc.b	65
 315  0001 42            	dc.b	66
 316  0002 43            	dc.b	67
 317  0003 44            	dc.b	68
 318  0004 45            	dc.b	69
 319  0005 46            	dc.b	70
 320  0006 47            	dc.b	71
 321  0007 48            	dc.b	72
 322  0008 49            	dc.b	73
 323  0009 4a            	dc.b	74
 324  000a 4b            	dc.b	75
 325  000b 4c            	dc.b	76
 326  000c 4d            	dc.b	77
 327  000d 4e            	dc.b	78
 328  000e 4f            	dc.b	79
 329  000f 50            	dc.b	80
 330  0010 51            	dc.b	81
 331  0011 52            	dc.b	82
 332  0012 53            	dc.b	83
 333  0013 54            	dc.b	84
 334  0014 55            	dc.b	85
 335  0015 56            	dc.b	86
 336  0016 57            	dc.b	87
 337  0017 58            	dc.b	88
 338  0018 59            	dc.b	89
 339  0019 5a            	dc.b	90
 340  001a               _alpha_char:
 341  001a 00            	dc.b	0
 342  001b fc            	dc.b	252
 343  001c fe            	dc.b	254
 344  001d 27            	dc.b	39
 345  001e 27            	dc.b	39
 346  001f fe            	dc.b	254
 347  0020 fc            	dc.b	252
 348  0021 00            	dc.b	0
 349  0022 00            	dc.b	0
 350  0023 fe            	dc.b	254
 351  0024 fe            	dc.b	254
 352  0025 92            	dc.b	146
 353  0026 92            	dc.b	146
 354  0027 fe            	dc.b	254
 355  0028 6c            	dc.b	108
 356  0029 00            	dc.b	0
 357  002a 00            	dc.b	0
 358  002b 7e            	dc.b	126
 359  002c ff            	dc.b	255
 360  002d 81            	dc.b	129
 361  002e 81            	dc.b	129
 362  002f c3            	dc.b	195
 363  0030 42            	dc.b	66
 364  0031 00            	dc.b	0
 365  0032 00            	dc.b	0
 366  0033 ff            	dc.b	255
 367  0034 ff            	dc.b	255
 368  0035 81            	dc.b	129
 369  0036 81            	dc.b	129
 370  0037 ff            	dc.b	255
 371  0038 7e            	dc.b	126
 372  0039 00            	dc.b	0
 373  003a 00            	dc.b	0
 374  003b ff            	dc.b	255
 375  003c ff            	dc.b	255
 376  003d 91            	dc.b	145
 377  003e 91            	dc.b	145
 378  003f 81            	dc.b	129
 379  0040 81            	dc.b	129
 380  0041 00            	dc.b	0
 381  0042 00            	dc.b	0
 382  0043 ff            	dc.b	255
 383  0044 ff            	dc.b	255
 384  0045 11            	dc.b	17
 385  0046 11            	dc.b	17
 386  0047 01            	dc.b	1
 387  0048 01            	dc.b	1
 388  0049 00            	dc.b	0
 389  004a 00            	dc.b	0
 390  004b 7e            	dc.b	126
 391  004c ff            	dc.b	255
 392  004d 81            	dc.b	129
 393  004e 91            	dc.b	145
 394  004f f1            	dc.b	241
 395  0050 71            	dc.b	113
 396  0051 00            	dc.b	0
 397  0052 00            	dc.b	0
 398  0053 ff            	dc.b	255
 399  0054 ff            	dc.b	255
 400  0055 10            	dc.b	16
 401  0056 10            	dc.b	16
 402  0057 ff            	dc.b	255
 403  0058 ff            	dc.b	255
 404  0059 00            	dc.b	0
 405  005a 00            	dc.b	0
 406  005b 81            	dc.b	129
 407  005c 81            	dc.b	129
 408  005d ff            	dc.b	255
 409  005e ff            	dc.b	255
 410  005f 81            	dc.b	129
 411  0060 81            	dc.b	129
 412  0061 00            	dc.b	0
 413  0062 00            	dc.b	0
 414  0063 40            	dc.b	64
 415  0064 c0            	dc.b	192
 416  0065 81            	dc.b	129
 417  0066 ff            	dc.b	255
 418  0067 7f            	dc.b	127
 419  0068 01            	dc.b	1
 420  0069 00            	dc.b	0
 421  006a 00            	dc.b	0
 422  006b ff            	dc.b	255
 423  006c ff            	dc.b	255
 424  006d 18            	dc.b	24
 425  006e 3c            	dc.b	60
 426  006f 66            	dc.b	102
 427  0070 c3            	dc.b	195
 428  0071 00            	dc.b	0
 429  0072 00            	dc.b	0
 430  0073 ff            	dc.b	255
 431  0074 ff            	dc.b	255
 432  0075 80            	dc.b	128
 433  0076 80            	dc.b	128
 434  0077 80            	dc.b	128
 435  0078 80            	dc.b	128
 436  0079 00            	dc.b	0
 437  007a 00            	dc.b	0
 438  007b ff            	dc.b	255
 439  007c ff            	dc.b	255
 440  007d 0c            	dc.b	12
 441  007e 18            	dc.b	24
 442  007f 0c            	dc.b	12
 443  0080 ff            	dc.b	255
 444  0081 ff            	dc.b	255
 445  0082 00            	dc.b	0
 446  0083 ff            	dc.b	255
 447  0084 ff            	dc.b	255
 448  0085 0c            	dc.b	12
 449  0086 18            	dc.b	24
 450  0087 30            	dc.b	48
 451  0088 ff            	dc.b	255
 452  0089 ff            	dc.b	255
 453  008a 00            	dc.b	0
 454  008b 7e            	dc.b	126
 455  008c ff            	dc.b	255
 456  008d 81            	dc.b	129
 457  008e 81            	dc.b	129
 458  008f ff            	dc.b	255
 459  0090 7e            	dc.b	126
 460  0091 00            	dc.b	0
 461  0092 00            	dc.b	0
 462  0093 ff            	dc.b	255
 463  0094 ff            	dc.b	255
 464  0095 11            	dc.b	17
 465  0096 11            	dc.b	17
 466  0097 1f            	dc.b	31
 467  0098 0e            	dc.b	14
 468  0099 00            	dc.b	0
 469  009a 00            	dc.b	0
 470  009b 7e            	dc.b	126
 471  009c ff            	dc.b	255
 472  009d 81            	dc.b	129
 473  009e c1            	dc.b	193
 474  009f ff            	dc.b	255
 475  00a0 be            	dc.b	190
 476  00a1 00            	dc.b	0
 477  00a2 00            	dc.b	0
 478  00a3 ff            	dc.b	255
 479  00a4 ff            	dc.b	255
 480  00a5 11            	dc.b	17
 481  00a6 31            	dc.b	49
 482  00a7 7f            	dc.b	127
 483  00a8 ce            	dc.b	206
 484  00a9 00            	dc.b	0
 485  00aa 00            	dc.b	0
 486  00ab c6            	dc.b	198
 487  00ac cf            	dc.b	207
 488  00ad 89            	dc.b	137
 489  00ae 99            	dc.b	153
 490  00af f9            	dc.b	249
 491  00b0 71            	dc.b	113
 492  00b1 00            	dc.b	0
 493  00b2 00            	dc.b	0
 494  00b3 01            	dc.b	1
 495  00b4 01            	dc.b	1
 496  00b5 ff            	dc.b	255
 497  00b6 ff            	dc.b	255
 498  00b7 01            	dc.b	1
 499  00b8 01            	dc.b	1
 500  00b9 00            	dc.b	0
 501  00ba 00            	dc.b	0
 502  00bb 7f            	dc.b	127
 503  00bc ff            	dc.b	255
 504  00bd 80            	dc.b	128
 505  00be 80            	dc.b	128
 506  00bf ff            	dc.b	255
 507  00c0 7f            	dc.b	127
 508  00c1 00            	dc.b	0
 509  00c2 00            	dc.b	0
 510  00c3 3f            	dc.b	63
 511  00c4 7f            	dc.b	127
 512  00c5 c0            	dc.b	192
 513  00c6 c0            	dc.b	192
 514  00c7 7f            	dc.b	127
 515  00c8 3f            	dc.b	63
 516  00c9 00            	dc.b	0
 517  00ca 00            	dc.b	0
 518  00cb ff            	dc.b	255
 519  00cc ff            	dc.b	255
 520  00cd 60            	dc.b	96
 521  00ce 30            	dc.b	48
 522  00cf 60            	dc.b	96
 523  00d0 ff            	dc.b	255
 524  00d1 ff            	dc.b	255
 525  00d2 00            	dc.b	0
 526  00d3 c3            	dc.b	195
 527  00d4 e7            	dc.b	231
 528  00d5 3c            	dc.b	60
 529  00d6 18            	dc.b	24
 530  00d7 3c            	dc.b	60
 531  00d8 e7            	dc.b	231
 532  00d9 c3            	dc.b	195
 533  00da 00            	dc.b	0
 534  00db 03            	dc.b	3
 535  00dc 07            	dc.b	7
 536  00dd fc            	dc.b	252
 537  00de fc            	dc.b	252
 538  00df 07            	dc.b	7
 539  00e0 03            	dc.b	3
 540  00e1 00            	dc.b	0
 541  00e2 00            	dc.b	0
 542  00e3 e3            	dc.b	227
 543  00e4 f3            	dc.b	243
 544  00e5 99            	dc.b	153
 545  00e6 8d            	dc.b	141
 546  00e7 87            	dc.b	135
 547  00e8 83            	dc.b	131
 548  00e9 00            	dc.b	0
 598                     ; 56 int string_len(char *string1)
 598                     ; 57 {
 599                     	switch	.text
 600  007e               _string_len:
 602  007e 89            	pushw	x
 603  007f 89            	pushw	x
 604       00000002      OFST:	set	2
 607                     ; 58     int len = 0;
 609  0080 5f            	clrw	x
 610  0081 1f01          	ldw	(OFST-1,sp),x
 613  0083 200e          	jra	L561
 614  0085               L161:
 615                     ; 60         string1++;
 617  0085 1e03          	ldw	x,(OFST+1,sp)
 618  0087 1c0001        	addw	x,#1
 619  008a 1f03          	ldw	(OFST+1,sp),x
 620                     ; 61         len++;
 622  008c 1e01          	ldw	x,(OFST-1,sp)
 623  008e 1c0001        	addw	x,#1
 624  0091 1f01          	ldw	(OFST-1,sp),x
 626  0093               L561:
 627                     ; 59     while(*string1 != 0) {
 629  0093 1e03          	ldw	x,(OFST+1,sp)
 630  0095 7d            	tnz	(x)
 631  0096 26ed          	jrne	L161
 632                     ; 63     return len;
 634  0098 1e01          	ldw	x,(OFST-1,sp)
 637  009a 5b04          	addw	sp,#4
 638  009c 81            	ret
 664                     ; 73 void MAX7219_init(void)
 664                     ; 74 {
 665                     	switch	.text
 666  009d               _MAX7219_init:
 670                     ; 75     GPIO_Init(ChipSelect_port, ChipSelect_pin, GPIO_MODE_OUT_PP_HIGH_FAST);
 672  009d 4bf0          	push	#240
 673  009f 4b10          	push	#16
 674  00a1 ae500a        	ldw	x,#20490
 675  00a4 cd0000        	call	_GPIO_Init
 677  00a7 85            	popw	x
 678                     ; 77     SPI_write(shutdown_reg, run_cmd);                 
 680  00a8 ae0c01        	ldw	x,#3073
 681  00ab ad9a          	call	_SPI_write
 683                     ; 78     SPI_write(decode_mode_reg, 0x00);
 685  00ad ae0900        	ldw	x,#2304
 686  00b0 ad95          	call	_SPI_write
 688                     ; 79     SPI_write(scan_limit_reg, 0x07);
 690  00b2 ae0b07        	ldw	x,#2823
 691  00b5 ad90          	call	_SPI_write
 693                     ; 80     SPI_write(intensity_reg, 0x04);
 695  00b7 ae0a04        	ldw	x,#2564
 696  00ba ad8b          	call	_SPI_write
 698                     ; 81     SPI_write(display_test_reg, test_cmd);
 700  00bc ae0f01        	ldw	x,#3841
 701  00bf ad86          	call	_SPI_write
 703                     ; 82     delay_ms(10);     
 705  00c1 ae000a        	ldw	x,#10
 706  00c4 cd0000        	call	_delay_ms
 708                     ; 83     SPI_write(display_test_reg, no_test_cmd);  
 710  00c7 ae0f00        	ldw	x,#3840
 711  00ca cd0047        	call	_SPI_write
 713                     ; 84 }
 716  00cd 81            	ret
 719                     	switch	.const
 720  00ea               L102_zeros_clr:
 721  00ea 00            	dc.b	0
 722  00eb 00            	dc.b	0
 723  00ec 00            	dc.b	0
 724  00ed 00            	dc.b	0
 725  00ee 00            	dc.b	0
 726  00ef 00            	dc.b	0
 727  00f0 00            	dc.b	0
 728  00f1 00            	dc.b	0
 772                     ; 93 void display_clear(void) {
 773                     	switch	.text
 774  00ce               _display_clear:
 776  00ce 5209          	subw	sp,#9
 777       00000009      OFST:	set	9
 780                     ; 94 	    unsigned char zeros_clr[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
 782  00d0 96            	ldw	x,sp
 783  00d1 1c0001        	addw	x,#OFST-8
 784  00d4 90ae00ea      	ldw	y,#L102_zeros_clr
 785  00d8 a608          	ld	a,#8
 786  00da cd0000        	call	c_xymov
 788                     ; 95  unsigned char j = 0x00;
 790                     ; 97 	for(j = 0; j < sizeof(zeros_clr); j++)
 792  00dd 0f09          	clr	(OFST+0,sp)
 794  00df               L522:
 795                     ; 100 			SPI_write((1 + j),zeros_clr[j]);
 797  00df 96            	ldw	x,sp
 798  00e0 1c0001        	addw	x,#OFST-8
 799  00e3 9f            	ld	a,xl
 800  00e4 5e            	swapw	x
 801  00e5 1b09          	add	a,(OFST+0,sp)
 802  00e7 2401          	jrnc	L02
 803  00e9 5c            	incw	x
 804  00ea               L02:
 805  00ea 02            	rlwa	x,a
 806  00eb f6            	ld	a,(x)
 807  00ec 97            	ld	xl,a
 808  00ed 7b09          	ld	a,(OFST+0,sp)
 809  00ef 4c            	inc	a
 810  00f0 95            	ld	xh,a
 811  00f1 cd0047        	call	_SPI_write
 813                     ; 101 			delay_ms(100);
 815  00f4 ae0064        	ldw	x,#100
 816  00f7 cd0000        	call	_delay_ms
 818                     ; 97 	for(j = 0; j < sizeof(zeros_clr); j++)
 820  00fa 0c09          	inc	(OFST+0,sp)
 824  00fc 7b09          	ld	a,(OFST+0,sp)
 825  00fe a108          	cp	a,#8
 826  0100 25dd          	jrult	L522
 827                     ; 104 }
 830  0102 5b09          	addw	sp,#9
 831  0104 81            	ret
 877                     ; 117 void display_char(int alphabet_sequence)
 877                     ; 118 {
 878                     	switch	.text
 879  0105               _display_char:
 881  0105 89            	pushw	x
 882  0106 89            	pushw	x
 883       00000002      OFST:	set	2
 886                     ; 121 		for(i=0; i<8; i++){
 888  0107 5f            	clrw	x
 889  0108 1f01          	ldw	(OFST-1,sp),x
 891  010a               L552:
 892                     ; 122 	SPI_write((i+1), alpha_char[alphabet_sequence][i]);
 894  010a 1e03          	ldw	x,(OFST+1,sp)
 895  010c 58            	sllw	x
 896  010d 58            	sllw	x
 897  010e 58            	sllw	x
 898  010f 72fb01        	addw	x,(OFST-1,sp)
 899  0112 d6001a        	ld	a,(_alpha_char,x)
 900  0115 97            	ld	xl,a
 901  0116 7b02          	ld	a,(OFST+0,sp)
 902  0118 4c            	inc	a
 903  0119 95            	ld	xh,a
 904  011a cd0047        	call	_SPI_write
 906                     ; 124 		delay_ms(100);
 908  011d ae0064        	ldw	x,#100
 909  0120 cd0000        	call	_delay_ms
 911                     ; 121 		for(i=0; i<8; i++){
 913  0123 1e01          	ldw	x,(OFST-1,sp)
 914  0125 1c0001        	addw	x,#1
 915  0128 1f01          	ldw	(OFST-1,sp),x
 919  012a 1e01          	ldw	x,(OFST-1,sp)
 920  012c a30008        	cpw	x,#8
 921  012f 25d9          	jrult	L552
 922                     ; 128 }
 925  0131 5b04          	addw	sp,#4
 926  0133 81            	ret
1002                     ; 137 void display_string(const char string[]){
1003                     	switch	.text
1004  0134               _display_string:
1006  0134 89            	pushw	x
1007  0135 5206          	subw	sp,#6
1008       00000006      OFST:	set	6
1011                     ; 141     int input_string_length  = string_len(string);
1013  0137 cd007e        	call	_string_len
1015  013a 1f03          	ldw	(OFST-3,sp),x
1017                     ; 142 		int alphabets_length  = string_len(alphabets);
1019  013c ae0000        	ldw	x,#_alphabets
1020  013f cd007e        	call	_string_len
1022                     ; 144 	for(j=0; j<input_string_length; j++){
1024  0142 0f05          	clr	(OFST-1,sp)
1027  0144 2032          	jra	L523
1028  0146               L123:
1029                     ; 145 	pos = 0;
1031  0146 0f06          	clr	(OFST+0,sp)
1034  0148 2022          	jra	L533
1035  014a               L133:
1036                     ; 148 	if(string[j] == alphabets[pos])
1038  014a 7b06          	ld	a,(OFST+0,sp)
1039  014c 5f            	clrw	x
1040  014d 97            	ld	xl,a
1041  014e 7b05          	ld	a,(OFST-1,sp)
1042  0150 905f          	clrw	y
1043  0152 9097          	ld	yl,a
1044  0154 72f907        	addw	y,(OFST+1,sp)
1045  0157 90f6          	ld	a,(y)
1046  0159 d10000        	cp	a,(_alphabets,x)
1047  015c 2606          	jrne	L143
1048                     ; 149 		display_char(pos);
1050  015e 7b06          	ld	a,(OFST+0,sp)
1051  0160 5f            	clrw	x
1052  0161 97            	ld	xl,a
1053  0162 ada1          	call	_display_char
1055  0164               L143:
1056                     ; 151 	pos++;	
1058  0164 0c06          	inc	(OFST+0,sp)
1060                     ; 152 	delay_ms(500);
1062  0166 ae01f4        	ldw	x,#500
1063  0169 cd0000        	call	_delay_ms
1065  016c               L533:
1066                     ; 146 	while(alphabets[pos]!='\0'){
1068  016c 7b06          	ld	a,(OFST+0,sp)
1069  016e 5f            	clrw	x
1070  016f 97            	ld	xl,a
1071  0170 724d0000      	tnz	(_alphabets,x)
1072  0174 26d4          	jrne	L133
1073                     ; 144 	for(j=0; j<input_string_length; j++){
1075  0176 0c05          	inc	(OFST-1,sp)
1077  0178               L523:
1080  0178 9c            	rvf
1081  0179 7b05          	ld	a,(OFST-1,sp)
1082  017b 5f            	clrw	x
1083  017c 97            	ld	xl,a
1084  017d 1303          	cpw	x,(OFST-3,sp)
1085  017f 2fc5          	jrslt	L123
1086                     ; 156 }
1089  0181 5b08          	addw	sp,#8
1090  0183 81            	ret
1126                     	switch	.const
1127  00f2               L553_input_string2:
1128  00f2 414243444546  	dc.b	"ABCDEFGHIJKLMNOPQR"
1129  0104 535455565758  	dc.b	"STUVWXYZ",0
1178                     ; 9 void main()
1178                     ; 10 {
1179                     	switch	.text
1180  0184               _main:
1182  0184 521b          	subw	sp,#27
1183       0000001b      OFST:	set	27
1186                     ; 12 const char input_string2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
1188  0186 96            	ldw	x,sp
1189  0187 1c0001        	addw	x,#OFST-26
1190  018a 90ae00f2      	ldw	y,#L553_input_string2
1191  018e a61b          	ld	a,#27
1192  0190 cd0000        	call	c_xymov
1194                     ; 14     clock_setup();
1196  0193 ad2d          	call	_clock_setup
1198                     ; 15     GPIO_setup();
1200  0195 ad5c          	call	_GPIO_setup
1202                     ; 16 		SPI_setup();
1204  0197 cd002c        	call	_SPI_setup
1206                     ; 17     MAX7219_init();
1208  019a cd009d        	call	_MAX7219_init
1210                     ; 18 		display_clear(); //Clearing the display
1212  019d cd00ce        	call	_display_clear
1214                     ; 19 		delay_ms(1000);
1216  01a0 ae03e8        	ldw	x,#1000
1217  01a3 cd0000        	call	_delay_ms
1219                     ; 20 		display_char(0); // Displaying the alphabet "A"
1221  01a6 5f            	clrw	x
1222  01a7 cd0105        	call	_display_char
1224                     ; 21 		delay_ms(4000);
1226  01aa ae0fa0        	ldw	x,#4000
1227  01ad cd0000        	call	_delay_ms
1229  01b0               L304:
1230                     ; 26 		display_clear(); //Clearing the Display
1232  01b0 cd00ce        	call	_display_clear
1234                     ; 27 		display_string(input_string2);  //Displaying a String
1236  01b3 96            	ldw	x,sp
1237  01b4 1c0001        	addw	x,#OFST-26
1238  01b7 cd0134        	call	_display_string
1240                     ; 28 		delay_ms(2000); 
1242  01ba ae07d0        	ldw	x,#2000
1243  01bd cd0000        	call	_delay_ms
1246  01c0 20ee          	jra	L304
1277                     ; 34 void clock_setup(void)
1277                     ; 35 {
1278                     	switch	.text
1279  01c2               _clock_setup:
1283                     ; 36      CLK_DeInit();
1285  01c2 cd0000        	call	_CLK_DeInit
1287                     ; 39      CLK_HSICmd(ENABLE);
1289  01c5 a601          	ld	a,#1
1290  01c7 cd0000        	call	_CLK_HSICmd
1293  01ca               L124:
1294                     ; 40      while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
1296  01ca ae0102        	ldw	x,#258
1297  01cd cd0000        	call	_CLK_GetFlagStatus
1299  01d0 4d            	tnz	a
1300  01d1 27f7          	jreq	L124
1301                     ; 41      CLK_ClockSwitchCmd(ENABLE);
1303  01d3 a601          	ld	a,#1
1304  01d5 cd0000        	call	_CLK_ClockSwitchCmd
1306                     ; 42      CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
1308  01d8 4f            	clr	a
1309  01d9 cd0000        	call	_CLK_HSIPrescalerConfig
1311                     ; 43      CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);                
1313  01dc a680          	ld	a,#128
1314  01de cd0000        	call	_CLK_SYSCLKConfig
1316                     ; 44      CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, 
1316                     ; 45      DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
1318  01e1 4b01          	push	#1
1319  01e3 4b00          	push	#0
1320  01e5 ae01e1        	ldw	x,#481
1321  01e8 cd0000        	call	_CLK_ClockSwitchConfig
1323  01eb 85            	popw	x
1324                     ; 47      CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);
1326  01ec ae0101        	ldw	x,#257
1327  01ef cd0000        	call	_CLK_PeripheralClockConfig
1329                     ; 49 }
1332  01f2 81            	ret
1357                     ; 52 void GPIO_setup(void)
1357                     ; 53 {
1358                     	switch	.text
1359  01f3               _GPIO_setup:
1363                     ; 54      GPIO_DeInit(GPIOC);
1365  01f3 ae500a        	ldw	x,#20490
1366  01f6 cd0000        	call	_GPIO_DeInit
1368                     ; 55      GPIO_Init(GPIOC, ((GPIO_Pin_TypeDef)GPIO_PIN_5 | GPIO_PIN_6), 
1368                     ; 56                GPIO_MODE_OUT_PP_HIGH_FAST);
1370  01f9 4bf0          	push	#240
1371  01fb 4b60          	push	#96
1372  01fd ae500a        	ldw	x,#20490
1373  0200 cd0000        	call	_GPIO_Init
1375  0203 85            	popw	x
1376                     ; 57 }
1379  0204 81            	ret
1392                     	xdef	_main
1393                     	xdef	_GPIO_setup
1394                     	xdef	_clock_setup
1395                     	xdef	_display_string
1396                     	xdef	_display_char
1397                     	xdef	_display_clear
1398                     	xdef	_MAX7219_init
1399                     	xdef	_string_len
1400                     	xdef	_alpha_char
1401                     	xdef	_alphabets
1402                     	xdef	_SPI_write
1403                     	xdef	_SPI_setup
1404                     	xdef	_delay_ms
1405                     	xref	_SPI_GetFlagStatus
1406                     	xref	_SPI_SendData
1407                     	xref	_SPI_Cmd
1408                     	xref	_SPI_Init
1409                     	xref	_SPI_DeInit
1410                     	xref	_GPIO_WriteLow
1411                     	xref	_GPIO_WriteHigh
1412                     	xref	_GPIO_Init
1413                     	xref	_GPIO_DeInit
1414                     	xref	_CLK_GetFlagStatus
1415                     	xref	_CLK_SYSCLKConfig
1416                     	xref	_CLK_HSIPrescalerConfig
1417                     	xref	_CLK_ClockSwitchConfig
1418                     	xref	_CLK_PeripheralClockConfig
1419                     	xref	_CLK_ClockSwitchCmd
1420                     	xref	_CLK_HSICmd
1421                     	xref	_CLK_DeInit
1422                     	xref.b	c_x
1441                     	xref	c_xymov
1442                     	end
