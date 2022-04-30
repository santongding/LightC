/.............line:0001............/
begin class: test
begin class: inner
/.............line:0003............/
declare int| x
/.............line:0004............/
declare int| y
begin func: void| func
formal ref| this
/.............line:0005............/
formal int| x
formal ref|asd v
end func
end class
/.............line:0009............/
declare int| a
/.............line:0010............/
declare int| b
/.............line:0011............/
declare ref|val c
begin func: void| func
formal ref| this
block begin:
ifz a goto label0
block begin:
/.............line:0014............/
a = 1
block end
goto label1
label label0
block begin:
/.............line:0016............/
c = 2
block end
label label1
label label2
ifz b goto label3
goto label2
label label3
block end
end func
end class
