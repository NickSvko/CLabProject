;file name: 'validTest.as'
;this file represent a valid input file to the assembler program.

	labelV1: addi $1, -2, $3
labelV2: add $10, $20, $30

labelV4: blt $5  , $15,  V3

	.entry labelV3
	.entry 	labelV4


blt $5   ,$24, labelV5

	.entry labelV5

jmp labelV5
jmp $5

labelV5: lh $4,14,$24

labelV3: 	move $2, $12

	.extern 	labelV11

la   labelV11

call labelV12

	.extern labelV12

labelV6: .db -128,-100	 ,-50,	 0,	 +10,	 +20 ,	30,	 127
	.db 1, 2

labelV7: .asciz "1he c2rrent 5tr+ng is v5lid!"
	.asciz 	"asciz directive without label definition"

labelV8: .dh -32768  , 200, +32767
	.dh 10  , 20

	.dw 100, 500
labelV9: .dw -2147483648,+2147483647, 11

labelV10: stop







