/.............line:0001............/
tmp t0 = a -> b
t0 = 1
/.............line:0002............/
actual 1
actual 2
tmp t1
t1 = call this -> y
t1 = 1
/.............line:0003............/
tmp t2 = x -> y
t2 = 1
tmp t3 = x -> y
tmp t4 = t3 -> z
t4 = 2
abc = t4
actual 321
actual eq
tmp t5
t5 = call abc -> func
/.............line:0004............/
return
