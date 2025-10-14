movi R0, 0
movi R1, 546
movi R2, 2310
beq R1, R0, 16    
beq R2, R0, 14 
beq R1, R2, 12
blt R1, R2, 5
sub R1, R1, R2
j 14
sub R2, R2, R1
j 14
halt
