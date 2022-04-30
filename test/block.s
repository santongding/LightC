block begin:
/.............line:0002............/
a = 3
ifz 1 goto label0
block begin:
/.............line:0004............/
b = 1
block end
goto label1
label label0
block begin:
/.............line:0006............/
a = 1
block end
label label1
ifz 1 goto label2
block begin:
/.............line:0009............/
x = 2
block end
label label2
ifz 1 goto label3
goto label4
label label3
block begin:
/.............line:0014............/
y = 1
block end
label label4
/.............line:0016............/
as = 2
ifz as goto label5
block begin:
/.............line:0017............/
as = 3
block end
label label5
label label6
ifz x goto label7
goto label6
label label7
block end
