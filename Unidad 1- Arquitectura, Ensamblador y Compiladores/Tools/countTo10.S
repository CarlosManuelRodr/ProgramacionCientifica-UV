LoadA <initial>
Store <counter>
Label <begin_loop>
LoadA <counter>
Increment
Store <counter>
Print <counter>
LoadA <max>
LoadB <counter>
Substract
JumpZero <end_loop>
Jump <begin_loop>
Label <end_loop>
Return
Declare <max> 10
Declare <counter> 0
Declare <initial> 0