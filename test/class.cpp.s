test<2>:
	[int] x<7>
	[int] y<8>
	[int] a<1>
	[int] b<3>
	[a<1>] dsa<18>

a<1>:
	[test<2>] b<3>
	[int] hu<4>
	[test<2>] c<5>
	[test<2>] bc<6>

/.............line:0001............/
begin class: a
/.............line:0002............/
declare ref|test b
/.............line:0003............/
declare int| hu
/.............line:0004............/
declare ref|test c
/.............line:0005............/
declare ref|test bc
end class
begin class: test
/.............line:0009............/
declare int| x
/.............line:0010............/
declare int| y
begin func: ref|void func
formal ref| this
/.............line:0012............/
formal int| x
formal ref|asd v
block begin:
/.............line:0013............/
a = 3
ifz 1 goto label0
block begin:
/.............line:0015............/
b = 1
block end
goto label1
label label0
block begin:
/.............line:0017............/
a = 1
block end
label label1
ifz 1 goto label2
block begin:
/.............line:0020............/
x = 2
block end
label label2
ifz 1 goto label3
goto label4
label label3
block begin:
/.............line:0025............/
y = 1
block end
label label4
/.............line:0027............/
as = 2
ifz as goto label5
block begin:
/.............line:0028............/
as = 3
block end
label label5
label label6
ifz x goto label7
goto label6
label label7
/.............line:0034............/
declare ref|intdsaas a
/.............line:0035............/
declare int| a
/.............line:0036............/
declare ref|sad b
/.............line:0037............/
pointer t0 = x -> y
pointer t1 = t0 -> z
pointer t2 = a -> b
actual t2
actual 213
actual AS
ret value t3
t3 = call t2 -> c
t1 = t3
block end
end func
/.............line:0041............/
declare int| a
/.............line:0042............/
declare int| b
/.............line:0043............/
declare ref|a dsa
begin func: ref|void func
formal ref| this
block begin:
ifz a goto label8
block begin:
/.............line:0047............/
a = 1
block end
goto label9
label label8
block begin:
/.............line:0049............/
c = 2
block end
label label9
label label10
ifz b goto label11
block begin:
/.............line:0052............/
return 2
block end
goto label10
label label11
block end
end func
end class
