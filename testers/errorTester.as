invalid.line.length.number.of.chars.so.far.is.49.111111111112222222222333333333344

		;**********instructions**********

		;invalid instruction name:
instruction 5,6,7
Add
		;invalid number of operands:
		
	;'R' arithmetic and logical instruction:
or
and $3
add $3, $19
sub $3, $19, $8, $30

	;'R' copy instructions:
mvlo
move $3
mvhi $3, $19, $8
	
	;'I' arithmetic and logical instructions:
ori
andi $9
addi $9, -45
	subi $9, -45, $8, $9
	
	;'I' Conditional branching instructions:
blt
bne $5 
beq $5, $24
	blt $5, $24, loop, $30
	
	;'I' Instructions for loading and keeping in memory:
lw
lb $9
sb $9, 34
lh $9, 34, $2, $4
	
	;'J' instructions - jump:
jmp
jmp label1, label2
jmp $6, $7

	;'J' instructions - la / call:
la
call label1, label2
	
	;stop instruction:
stop ab12

		;invalid operands:
		
	;'R' arithmetic and logical instruction:
add $3, symbol, $8
or  3, $19, $8
and $3, $19, $32
sub $3, $32, $8

	;'R' copy instructions:
move $3, symbol
mvhi 3, $5
mvlo $40, $5

	;'I' arithmetic and logical instructions:
ori 9, -45, $8
andi $9, 100000000, $8
addi $9, -45, label
nori $65, -45, $8

	;'I' Conditional branching instructions:
blt 5, $24, loop
bne $5, $44, loop
beq $5, $24, $3
bgt $5, $24, 5

	;'I' Instructions for loading and keeping in memory:
lw 9, 34, $2
lb $9, $34, $2
sb $9, 34, symbol
lh $9, 34, $32
sw $9, symbol, $20

	;'J' instructions - jump:
jmp 44

	;'J' instructions - la / call:
la $4
call 4


;		**********directives**********
		
		;invalid directive name:
.Asciz "abc"
.directive 9, 7, 2

		;invalid number of operands:
.db
.dw
.dh
.asciz

		;invalid operands:
.db $5
.db symbol
.db 128
.db -129

.dh $5
.dh symbol
.dh 32768
.dh -32769

.dw $5
.dw symbol
.dw 2147483648
.dw -2147483649

.asciz abc
.asciz "abc
.asciz "abc""
.asciz ""abc"
.asciz ""abc
.asciz abc""
.asciz "	"
.asciz """

		;**********commas**********
		
add ,$3, $19, $20
add ,	$3, $19, $20
mvhi $3, $19,
mvhi $3, $19	,
addi $9,,-45, $8
blt $5,	,$24, loop
lh $9 34, $2
lh $9,34	 $2

;		**********labels**********
add:
asciz:
label:
label :
1abel:
l able:
lab;l:
theLengthOfLabelLimitedTo31chars:

labelV1: .asciz "abcd"
labelV1: .asciz "abcd"
labelV1: add $1, $2, $3
.extern labelV1



