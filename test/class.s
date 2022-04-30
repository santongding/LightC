/.............line:0001............/
begin class: test
/.............line:0003............/
declare int| x
/.............line:0004............/
declare int| y
begin func: void| func
formal ref| this
/.............line:0005............/
formal int| x
formal ref|asd v
block begin:
/.............line:0006............/
a = 3
ifz 1 goto label0
block begin:
/.............line:0008............/
b = 1
block end
goto label1
label label0
block begin:
/.............line:0010............/
a = 1
block end
label label1
ifz 1 goto label2
block begin:
/.............line:0013............/
x = 2
block end
label label2
ifz 1 goto label3
goto label4
label label3
block begin:
/.............line:0018............/
y = 1
block end
label label4
/.............line:0020............/
as = 2
ifz as goto label5
block begin:
/.............line:0021............/
as = 3
block end
label label5
label label6
ifz x goto label7
goto label6
label label7
/.............line:0027............/
declare link|intdsaas a
/.............line:0028............/
declare int| a
/.............line:0029............/
declare ref|sad b
block end
end func
/.............line:0033............/
declare int| a
/.............line:0034............/
declare int| b
/.............line:0035............/
declare ref|val c
begin func: void| func
formal ref| this
block begin:
ifz a goto label8
block begin:
/.............line:0038............/
a = 1
block end
goto label9
label label8
block begin:
/.............line:0040............/
c = 2
block end
label label9
label label10
ifz b goto label11
goto label10
label label11
block end
end func
end class
