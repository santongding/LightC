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
declare any| t0
/.............line:0005............/
t0 = a -> b
declare any| t1
t1 = t0 + 1
y -> x bind with t1
declare any| t2
t2 = y -> x
declare any| t3
t3 = t2 -> a
declare any| t7
t7 = t3 -> b
declare any| t4
t4 = b -> n
declare any| t5
t5 = t -> y
declare any| t6
t6 = t5 -> j
actual t7
actual t4
actual t6
actual 3
declare any| t8
t8 = call t7 -> c
declare any| t9
/.............line:0006............/
t9 = y -> z
x -> y bind with t9
declare any| t10
t10 = x -> y
a -> b bind with t10
declare any| t11
t11 = a -> b
t11 -> x bind with 1
declare any| t12
t12 = t11 -> x
block end
end func
end class
begin class: b
end class
