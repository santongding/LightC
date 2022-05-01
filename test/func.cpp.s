/.............line:0001............/
begin class: a
/.............line:0002............/
declare link|b x
begin func.cpp: ref|a fa
formal ref| this
/.............line:0003............/
formal ref|a x
formal ref|b y
block begin:
/.............line:0005............/
declare pointer|any| t0
t0 = this -> x
declare ref| t1
t1 = x + y
declare pointer|any| t2
t2 = t1 -> b
t0 = t2
/.............line:0006............/
declare pointer|any| t3
t3 = y -> x
declare pointer|any| t4
t4 = y -> x
declare pointer|any| t5
t5 = x -> y
t4 = t5
t3 = t4
actual this
/.............line:0007............/
declare any| t6
t6 = call this -> f
/.............line:0011............/
return 1
block end
end func.cpp
end class
begin class: b
end class
b<2>:

a<1>:
	[link b<2>] x<3>
	fa<4>: ([a<1>] x, [b<2>] y) -> [a<1>]

