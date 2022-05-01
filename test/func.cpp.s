/.............line:0001............/
begin class: a
/.............line:0002............/
declare link|b x
/.............line:0003............/
declare link|a x
begin func: ref|b func
block begin:
formal ref| this
/.............line:0004............/
formal int| i
formal ref|b a
declare ref| t0
/.............line:0005............/
t0 = a -> b
declare ref| t1
t1 = t0 + 1
y -> x bind with t1
declare ref| t2
t2 = y -> x
declare ref| t3
t3 = t2 -> a
declare ref| t4
t4 = t3 -> b
declare ref| t5
/.............line:0006............/
t5 = y -> z
x -> y bind with t5
declare ref| t6
t6 = x -> y
a -> b bind with t6
declare ref| t7
t7 = a -> b
t7 -> x bind with 1
block end
end func
end class
begin class: b
end class
