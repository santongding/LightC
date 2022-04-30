block begin:
/.............line:0002............/
tmp t0 = a -> b
t0 = 1
/.............line:0003............/
actual 1
actual 2
tmp t1
t1 = call this -> y
t1 = 1
/.............line:0004............/
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
/.............line:0005............/
return
/.............line:0006............/
var t6
t6 = 312 + asd
ifz t6 goto label0
block begin:
/.............line:0007............/
x = 1
block end
goto label1
label label0
label label1
/.............line:0009............/
var t7
t7 = asd * 12
var t8
t8 = 312 + t7
ifz t8 goto label2
block begin:
/.............line:0010............/
sa = 2
block end
goto label3
label label2
block begin:
/.............line:0013............/
return
block end
label label3
block end
