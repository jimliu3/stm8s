   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.13.2 - 04 Jun 2024
   3                     ; Generator (Limited) V4.6.3 - 22 Aug 2024
  62                     ; 12 void delay(uint32_t count)
  62                     ; 13 {
  64                     	switch	.text
  65  0000               _delay:
  67  0000 5204          	subw	sp,#4
  68       00000004      OFST:	set	4
  71                     ; 15     for (i = 0; i < count; i++);
  73  0002 ae0000        	ldw	x,#0
  74  0005 1f03          	ldw	(OFST-1,sp),x
  75  0007 ae0000        	ldw	x,#0
  76  000a 1f01          	ldw	(OFST-3,sp),x
  79  000c 2009          	jra	L73
  80  000e               L33:
  84  000e 96            	ldw	x,sp
  85  000f 1c0001        	addw	x,#OFST-3
  86  0012 a601          	ld	a,#1
  87  0014 cd0000        	call	c_lgadc
  90  0017               L73:
  93  0017 96            	ldw	x,sp
  94  0018 1c0001        	addw	x,#OFST-3
  95  001b cd0000        	call	c_ltor
  97  001e 96            	ldw	x,sp
  98  001f 1c0007        	addw	x,#OFST+3
  99  0022 cd0000        	call	c_lcmp
 101  0025 25e7          	jrult	L33
 102                     ; 16 }
 105  0027 5b04          	addw	sp,#4
 106  0029 81            	ret
 133                     ; 21  void Clock_Config(void)
 133                     ; 22 {
 134                     	switch	.text
 135  002a               _Clock_Config:
 139                     ; 24     CLK_HSICmd(ENABLE);
 141  002a a601          	ld	a,#1
 142  002c cd0000        	call	_CLK_HSICmd
 145  002f               L55:
 146                     ; 27     while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);
 148  002f ae0102        	ldw	x,#258
 149  0032 cd0000        	call	_CLK_GetFlagStatus
 151  0035 4d            	tnz	a
 152  0036 27f7          	jreq	L55
 153                     ; 30     CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 155  0038 4f            	clr	a
 156  0039 cd0000        	call	_CLK_HSIPrescalerConfig
 158                     ; 33     CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
 160  003c ae0001        	ldw	x,#1
 161  003f cd0000        	call	_CLK_PeripheralClockConfig
 163                     ; 34 }
 166  0042 81            	ret
 191                     ; 36 void I2C_Config(void)
 191                     ; 37 {
 192                     	switch	.text
 193  0043               _I2C_Config:
 197                     ; 39     I2C_DeInit();
 199  0043 cd0000        	call	_I2C_DeInit
 201                     ; 40     I2C_Init(100000, 0x0011, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, 16);
 203  0046 4b10          	push	#16
 204  0048 4b00          	push	#0
 205  004a 4b01          	push	#1
 206  004c 4b00          	push	#0
 207  004e ae0011        	ldw	x,#17
 208  0051 89            	pushw	x
 209  0052 ae86a0        	ldw	x,#34464
 210  0055 89            	pushw	x
 211  0056 ae0001        	ldw	x,#1
 212  0059 89            	pushw	x
 213  005a cd0000        	call	_I2C_Init
 215  005d 5b0a          	addw	sp,#10
 216                     ; 41 }
 219  005f 81            	ret
 287                     ; 49 void EEPROM_Write(uint16_t mem_address, uint8_t *data, uint8_t size)
 287                     ; 50 {
 288                     	switch	.text
 289  0060               _EEPROM_Write:
 291  0060 89            	pushw	x
 292  0061 88            	push	a
 293       00000001      OFST:	set	1
 296                     ; 54 		I2C_AcknowledgeConfig(I2C_ACK_CURR);
 298  0062 a601          	ld	a,#1
 299  0064 cd0000        	call	_I2C_AcknowledgeConfig
 301                     ; 57     I2C_GenerateSTART(ENABLE);
 303  0067 a601          	ld	a,#1
 304  0069 cd0000        	call	_I2C_GenerateSTART
 307  006c               L521:
 308                     ; 58     while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
 310  006c ae0301        	ldw	x,#769
 311  006f cd0000        	call	_I2C_CheckEvent
 313  0072 4d            	tnz	a
 314  0073 27f7          	jreq	L521
 315                     ; 61     I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
 317  0075 aea000        	ldw	x,#40960
 318  0078 cd0000        	call	_I2C_Send7bitAddress
 321  007b               L331:
 322                     ; 62     while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
 324  007b ae0782        	ldw	x,#1922
 325  007e cd0000        	call	_I2C_CheckEvent
 327  0081 4d            	tnz	a
 328  0082 27f7          	jreq	L331
 329                     ; 65     I2C_SendData((uint8_t)((mem_address >> 8) & 0xFF));
 331  0084 7b02          	ld	a,(OFST+1,sp)
 332  0086 cd0000        	call	_I2C_SendData
 335  0089               L141:
 336                     ; 66     while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 338  0089 ae0784        	ldw	x,#1924
 339  008c cd0000        	call	_I2C_CheckEvent
 341  008f 4d            	tnz	a
 342  0090 27f7          	jreq	L141
 343                     ; 69     I2C_SendData((uint8_t)(mem_address & 0xFF));
 345  0092 7b03          	ld	a,(OFST+2,sp)
 346  0094 a4ff          	and	a,#255
 347  0096 cd0000        	call	_I2C_SendData
 350  0099               L741:
 351                     ; 70     while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 353  0099 ae0784        	ldw	x,#1924
 354  009c cd0000        	call	_I2C_CheckEvent
 356  009f 4d            	tnz	a
 357  00a0 27f7          	jreq	L741
 358                     ; 73     for (i = 0; i < size; i++)
 360  00a2 0f01          	clr	(OFST+0,sp)
 363  00a4 2016          	jra	L751
 364  00a6               L351:
 365                     ; 75         I2C_SendData(data[i]);
 367  00a6 7b01          	ld	a,(OFST+0,sp)
 368  00a8 5f            	clrw	x
 369  00a9 97            	ld	xl,a
 370  00aa 72fb06        	addw	x,(OFST+5,sp)
 371  00ad f6            	ld	a,(x)
 372  00ae cd0000        	call	_I2C_SendData
 375  00b1               L561:
 376                     ; 76         while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 378  00b1 ae0784        	ldw	x,#1924
 379  00b4 cd0000        	call	_I2C_CheckEvent
 381  00b7 4d            	tnz	a
 382  00b8 27f7          	jreq	L561
 383                     ; 73     for (i = 0; i < size; i++)
 385  00ba 0c01          	inc	(OFST+0,sp)
 387  00bc               L751:
 390  00bc 7b01          	ld	a,(OFST+0,sp)
 391  00be 1108          	cp	a,(OFST+7,sp)
 392  00c0 25e4          	jrult	L351
 393                     ; 80     I2C_GenerateSTOP(ENABLE);
 395  00c2 a601          	ld	a,#1
 396  00c4 cd0000        	call	_I2C_GenerateSTOP
 398                     ; 81 }
 401  00c7 5b03          	addw	sp,#3
 402  00c9 81            	ret
 481                     ; 157 void EEPROM_Read(uint16_t mem_address, uint8_t *data, uint8_t size)
 481                     ; 158 {
 482                     	switch	.text
 483  00ca               _EEPROM_Read:
 485  00ca 89            	pushw	x
 486  00cb 89            	pushw	x
 487       00000002      OFST:	set	2
 490                     ; 160 		uint8_t i = 0;
 492                     ; 162 		NumByteToRead= size;
 494  00cc 7b09          	ld	a,(OFST+7,sp)
 495  00ce 6b01          	ld	(OFST-1,sp),a
 497                     ; 165 		I2C_AcknowledgeConfig(I2C_ACK_CURR);
 499  00d0 a601          	ld	a,#1
 500  00d2 cd0000        	call	_I2C_AcknowledgeConfig
 503  00d5               L132:
 504                     ; 168     while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
 506  00d5 ae0302        	ldw	x,#770
 507  00d8 cd0000        	call	_I2C_GetFlagStatus
 509  00db 4d            	tnz	a
 510  00dc 26f7          	jrne	L132
 511                     ; 171     I2C_GenerateSTART(ENABLE);
 513  00de a601          	ld	a,#1
 514  00e0 cd0000        	call	_I2C_GenerateSTART
 517  00e3               L732:
 518                     ; 172     while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
 520  00e3 ae0301        	ldw	x,#769
 521  00e6 cd0000        	call	_I2C_CheckEvent
 523  00e9 4d            	tnz	a
 524  00ea 27f7          	jreq	L732
 525                     ; 175     I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
 527  00ec aea000        	ldw	x,#40960
 528  00ef cd0000        	call	_I2C_Send7bitAddress
 531  00f2               L542:
 532                     ; 176     while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
 534  00f2 ae0782        	ldw	x,#1922
 535  00f5 cd0000        	call	_I2C_CheckEvent
 537  00f8 4d            	tnz	a
 538  00f9 27f7          	jreq	L542
 539                     ; 179     I2C_SendData((uint8_t)((mem_address >> 8) & 0xFF));
 541  00fb 7b03          	ld	a,(OFST+1,sp)
 542  00fd cd0000        	call	_I2C_SendData
 545  0100               L352:
 546                     ; 180     while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 548  0100 ae0784        	ldw	x,#1924
 549  0103 cd0000        	call	_I2C_CheckEvent
 551  0106 4d            	tnz	a
 552  0107 27f7          	jreq	L352
 553                     ; 181     I2C_SendData((uint8_t)(mem_address & 0xFF));
 555  0109 7b04          	ld	a,(OFST+2,sp)
 556  010b a4ff          	and	a,#255
 557  010d cd0000        	call	_I2C_SendData
 560  0110               L162:
 561                     ; 182     while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 563  0110 ae0784        	ldw	x,#1924
 564  0113 cd0000        	call	_I2C_CheckEvent
 566  0116 4d            	tnz	a
 567  0117 27f7          	jreq	L162
 568                     ; 185     I2C_GenerateSTART(ENABLE);
 570  0119 a601          	ld	a,#1
 571  011b cd0000        	call	_I2C_GenerateSTART
 574  011e               L762:
 575                     ; 186     while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
 577  011e ae0301        	ldw	x,#769
 578  0121 cd0000        	call	_I2C_CheckEvent
 580  0124 4d            	tnz	a
 581  0125 27f7          	jreq	L762
 582                     ; 189     I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_RX);
 584  0127 aea001        	ldw	x,#40961
 585  012a cd0000        	call	_I2C_Send7bitAddress
 588  012d               L572:
 589                     ; 190     while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
 591  012d ae0302        	ldw	x,#770
 592  0130 cd0000        	call	_I2C_CheckEvent
 594  0133 4d            	tnz	a
 595  0134 27f7          	jreq	L572
 596                     ; 193     if (NumByteToRead == 1) // 單字節讀取
 598  0136 7b01          	ld	a,(OFST-1,sp)
 599  0138 a101          	cp	a,#1
 600  013a 261b          	jrne	L103
 601                     ; 195         I2C_AcknowledgeConfig(I2C_ACK_NONE);  // 禁用 ACK
 603  013c 4f            	clr	a
 604  013d cd0000        	call	_I2C_AcknowledgeConfig
 606                     ; 196         I2C_GenerateSTOP(ENABLE);            // 發送 STOP 信號
 608  0140 a601          	ld	a,#1
 609  0142 cd0000        	call	_I2C_GenerateSTOP
 612  0145               L503:
 613                     ; 197         while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
 615  0145 ae0340        	ldw	x,#832
 616  0148 cd0000        	call	_I2C_CheckEvent
 618  014b 4d            	tnz	a
 619  014c 27f7          	jreq	L503
 620                     ; 198         data[0] = I2C_ReceiveData();         // 讀取數據
 622  014e cd0000        	call	_I2C_ReceiveData
 624  0151 1e07          	ldw	x,(OFST+5,sp)
 625  0153 f7            	ld	(x),a
 627  0154               L113:
 628                     ; 222 }
 631  0154 5b04          	addw	sp,#4
 632  0156 81            	ret
 633  0157               L103:
 634                     ; 200     else if (NumByteToRead == 2) // 特殊處理兩字節讀取
 636  0157 7b01          	ld	a,(OFST-1,sp)
 637  0159 a102          	cp	a,#2
 638  015b 262a          	jrne	L313
 639                     ; 202         I2C_AcknowledgeConfig(I2C_ACK_NONE); // 禁用 ACK
 641  015d 4f            	clr	a
 642  015e cd0000        	call	_I2C_AcknowledgeConfig
 645  0161               L713:
 646                     ; 203         while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
 648  0161 ae0340        	ldw	x,#832
 649  0164 cd0000        	call	_I2C_CheckEvent
 651  0167 4d            	tnz	a
 652  0168 27f7          	jreq	L713
 653                     ; 204         data[0] = I2C_ReceiveData();         // 讀取第一字節
 655  016a cd0000        	call	_I2C_ReceiveData
 657  016d 1e07          	ldw	x,(OFST+5,sp)
 658  016f f7            	ld	(x),a
 659                     ; 205         I2C_GenerateSTOP(ENABLE);            // 發送 STOP 信號
 661  0170 a601          	ld	a,#1
 662  0172 cd0000        	call	_I2C_GenerateSTOP
 665  0175               L523:
 666                     ; 206         while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
 668  0175 ae0340        	ldw	x,#832
 669  0178 cd0000        	call	_I2C_CheckEvent
 671  017b 4d            	tnz	a
 672  017c 27f7          	jreq	L523
 673                     ; 207         data[1] = I2C_ReceiveData();         // 讀取第二字節
 675  017e cd0000        	call	_I2C_ReceiveData
 677  0181 1e07          	ldw	x,(OFST+5,sp)
 678  0183 e701          	ld	(1,x),a
 680  0185 20cd          	jra	L113
 681  0187               L313:
 682                     ; 211         for ( i = 0; i < NumByteToRead; i++)
 684  0187 0f02          	clr	(OFST+0,sp)
 687  0189 2033          	jra	L733
 688  018b               L333:
 689                     ; 213             if (i == (NumByteToRead - 1))    // 最後一個字節
 691  018b 7b01          	ld	a,(OFST-1,sp)
 692  018d 5f            	clrw	x
 693  018e 97            	ld	xl,a
 694  018f 5a            	decw	x
 695  0190 7b02          	ld	a,(OFST+0,sp)
 696  0192 905f          	clrw	y
 697  0194 9097          	ld	yl,a
 698  0196 90bf00        	ldw	c_y,y
 699  0199 b300          	cpw	x,c_y
 700  019b 2609          	jrne	L743
 701                     ; 215                 I2C_AcknowledgeConfig(I2C_ACK_NONE);  // 禁用 ACK
 703  019d 4f            	clr	a
 704  019e cd0000        	call	_I2C_AcknowledgeConfig
 706                     ; 216                 I2C_GenerateSTOP(ENABLE);            // 發送 STOP 信號
 708  01a1 a601          	ld	a,#1
 709  01a3 cd0000        	call	_I2C_GenerateSTOP
 711  01a6               L743:
 712                     ; 218             while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
 714  01a6 ae0340        	ldw	x,#832
 715  01a9 cd0000        	call	_I2C_CheckEvent
 717  01ac 4d            	tnz	a
 718  01ad 27f7          	jreq	L743
 719                     ; 219             data[i] = I2C_ReceiveData();     // 讀取當前字節
 721  01af 7b02          	ld	a,(OFST+0,sp)
 722  01b1 5f            	clrw	x
 723  01b2 97            	ld	xl,a
 724  01b3 72fb07        	addw	x,(OFST+5,sp)
 725  01b6 89            	pushw	x
 726  01b7 cd0000        	call	_I2C_ReceiveData
 728  01ba 85            	popw	x
 729  01bb f7            	ld	(x),a
 730                     ; 211         for ( i = 0; i < NumByteToRead; i++)
 732  01bc 0c02          	inc	(OFST+0,sp)
 734  01be               L733:
 737  01be 7b02          	ld	a,(OFST+0,sp)
 738  01c0 1101          	cp	a,(OFST-1,sp)
 739  01c2 25c7          	jrult	L333
 740  01c4 208e          	jra	L113
 743                     .const:	section	.text
 744  0000               L353_write_data:
 745  0000 48            	dc.b	72
 746  0001 65            	dc.b	101
 747  0002 6c            	dc.b	108
 748  0003 6c            	dc.b	108
 749  0004 6f            	dc.b	111
 750  0005 20            	dc.b	32
 751  0006 21            	dc.b	33
 752  0007 0a            	dc.b	10
 753  0008               L553_write_data1:
 754  0008 00            	dc.b	0
 755  0009 01            	dc.b	1
 756  000a 02            	dc.b	2
 757  000b 03            	dc.b	3
 758  000c 04            	dc.b	4
 759  000d 05            	dc.b	5
 760  000e 06            	dc.b	6
 761  000f 07            	dc.b	7
 762  0010               L753_read_data:
 763  0010 00            	dc.b	0
 764  0011 000000000000  	ds.b	7
 824                     ; 229 void main(void)
 824                     ; 230 {
 825                     	switch	.text
 826  01c6               _main:
 828  01c6 5218          	subw	sp,#24
 829       00000018      OFST:	set	24
 832                     ; 231     uint8_t write_data[8] = { 'H', 'e', 'l', 'l', 'o', ' ', '!', '\n' };
 834  01c8 96            	ldw	x,sp
 835  01c9 1c0001        	addw	x,#OFST-23
 836  01cc 90ae0000      	ldw	y,#L353_write_data
 837  01d0 a608          	ld	a,#8
 838  01d2 cd0000        	call	c_xymov
 840                     ; 233 		uint8_t write_data1[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
 842  01d5 96            	ldw	x,sp
 843  01d6 1c0009        	addw	x,#OFST-15
 844  01d9 90ae0008      	ldw	y,#L553_write_data1
 845  01dd a608          	ld	a,#8
 846  01df cd0000        	call	c_xymov
 848                     ; 235 	  uint8_t read_data[8] = {0};
 850  01e2 96            	ldw	x,sp
 851  01e3 1c0011        	addw	x,#OFST-7
 852  01e6 90ae0010      	ldw	y,#L753_read_data
 853  01ea a608          	ld	a,#8
 854  01ec cd0000        	call	c_xymov
 856                     ; 237 		GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);
 858  01ef 4bc0          	push	#192
 859  01f1 4b08          	push	#8
 860  01f3 ae5000        	ldw	x,#20480
 861  01f6 cd0000        	call	_GPIO_Init
 863  01f9 85            	popw	x
 864                     ; 240 		Clock_Config();
 866  01fa cd002a        	call	_Clock_Config
 868                     ; 241     I2C_Config();
 870  01fd cd0043        	call	_I2C_Config
 872  0200               L704:
 873                     ; 251     delay(10000);
 875  0200 ae2710        	ldw	x,#10000
 876  0203 89            	pushw	x
 877  0204 ae0000        	ldw	x,#0
 878  0207 89            	pushw	x
 879  0208 cd0000        	call	_delay
 881  020b 5b04          	addw	sp,#4
 882                     ; 254     EEPROM_Read(0x0011,read_data, 8);
 884  020d 4b08          	push	#8
 885  020f 96            	ldw	x,sp
 886  0210 1c0012        	addw	x,#OFST-6
 887  0213 89            	pushw	x
 888  0214 ae0011        	ldw	x,#17
 889  0217 cd00ca        	call	_EEPROM_Read
 891  021a 5b03          	addw	sp,#3
 892                     ; 256 		delay(10000);
 894  021c ae2710        	ldw	x,#10000
 895  021f 89            	pushw	x
 896  0220 ae0000        	ldw	x,#0
 897  0223 89            	pushw	x
 898  0224 cd0000        	call	_delay
 900  0227 5b04          	addw	sp,#4
 901                     ; 258 		GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
 903  0229 4b08          	push	#8
 904  022b ae5000        	ldw	x,#20480
 905  022e cd0000        	call	_GPIO_WriteHigh
 907  0231 84            	pop	a
 908                     ; 259     delay(1000);
 910  0232 ae03e8        	ldw	x,#1000
 911  0235 89            	pushw	x
 912  0236 ae0000        	ldw	x,#0
 913  0239 89            	pushw	x
 914  023a cd0000        	call	_delay
 916  023d 5b04          	addw	sp,#4
 917                     ; 260 		GPIO_WriteLow(GPIOA, GPIO_PIN_3);
 919  023f 4b08          	push	#8
 920  0241 ae5000        	ldw	x,#20480
 921  0244 cd0000        	call	_GPIO_WriteLow
 923  0247 84            	pop	a
 924                     ; 261     delay(1000);
 926  0248 ae03e8        	ldw	x,#1000
 927  024b 89            	pushw	x
 928  024c ae0000        	ldw	x,#0
 929  024f 89            	pushw	x
 930  0250 cd0000        	call	_delay
 932  0253 5b04          	addw	sp,#4
 934  0255 20a9          	jra	L704
 947                     	xdef	_main
 948                     	xdef	_EEPROM_Read
 949                     	xdef	_EEPROM_Write
 950                     	xdef	_I2C_Config
 951                     	xdef	_Clock_Config
 952                     	xdef	_delay
 953                     	xref	_I2C_GetFlagStatus
 954                     	xref	_I2C_CheckEvent
 955                     	xref	_I2C_SendData
 956                     	xref	_I2C_Send7bitAddress
 957                     	xref	_I2C_ReceiveData
 958                     	xref	_I2C_AcknowledgeConfig
 959                     	xref	_I2C_GenerateSTOP
 960                     	xref	_I2C_GenerateSTART
 961                     	xref	_I2C_Init
 962                     	xref	_I2C_DeInit
 963                     	xref	_GPIO_WriteLow
 964                     	xref	_GPIO_WriteHigh
 965                     	xref	_GPIO_Init
 966                     	xref	_CLK_GetFlagStatus
 967                     	xref	_CLK_HSIPrescalerConfig
 968                     	xref	_CLK_PeripheralClockConfig
 969                     	xref	_CLK_HSICmd
 970                     	xref.b	c_x
 971                     	xref.b	c_y
 990                     	xref	c_xymov
 991                     	xref	c_lcmp
 992                     	xref	c_ltor
 993                     	xref	c_lgadc
 994                     	end
