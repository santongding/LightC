# LightC Language
@[toc]
## 1. 概述
LightC是一个简易的面向对象语言, 支持简单的内存回收.
### 1.1. 目前已完成的特性


1. 类型声明
2. 强制类型检测
3. 垃圾回收
4. 弱引用

### 1.2. 计划要完成的特性
1. 类型转换以及通过类型转换实现多态
2. 循环引用的检测及gc效率的优化
3. 更多的基本数据类型支持
4. import功能的支持
### 1.3. 计划不会完成的特性
1. 类的继承

## 2. 语法
### 	2.1. token定义



- letter = A|...|Z|a|...|z
- digit = 0|...|9
- **IDENTIFIER** = letter[letter|digit]*
- **INTERGER** = digit+

### 2.2. 形式定义
1. program => classes

2. classes => class | classes
3. class => **CLASS** identifier **{** class_members **}** | **CLASS** identifier **{** **}**

4. class_members => class_member | class_members class_member

5. class_member => function | member_declare_statement

6. function => var_type identifier **(** params **)** block identifier **:** **IDENTIFIER**

7. params => | param | params **,** param
8. param => identifier **:** var_type

9. block => **{** statement_list **}** | **{** **}**
10. statement_list => statement | statement_list statement

11. statement => expression_statement **|** return_statement **|** if_statement; **|** while_statement; **|** declare_statement **;**

12. type_id => identifier

13. var_type => type_id | **INT**

14. member_declare_statement => identifier **:** var_type **;** | identifier **:** **LINK** var_type **;**

15. declare_statement => identifier **:** var_type **;** | identifier **:** **NEW** var_type ;


16. while_statement => **WHILE** **(** expression **)** block


17. if_statement => **IF** **(** expression **)** block | **IF** **(** expression **)** block **ELSE** block

18. return_statement => RETURN expression ;

19. expression_statement => expression_list ;

20. expression_list => **ε** | expression | expression_list **,** expression

21. expression => identifier **=** expression | expression **.** identifier **=** expression | expression **.** identifier | expression **+** expression | expression **-** expression | expression **\*** expression | expression **/** expression | **-** expression | expression **==** expression | expression "!=" expression | expression "<" expression | expression "<=" expression | expression ">" expression | expression ">=" expression | **(** expression_list **)** | **INTEGER** | identifier | call_expression | error
22. call_expression => expression **.** identifier **(** expression_list **)**

### 2.3. 使用示例
#### 2.3.1. 类的定义
LightC的程序结构类似java, 只有类的定义而没有全局空间. 程序的入口点为main类的main成员函数.
若要定义一个类, 可以参照如下示例:


```
class main{
	v : int; // 声明一个整型变量
	obj : object; // 声明一个强引用类型的object变量, 其行为与C++中shared_ptr相同, 但回收内存的时机略有不同, 初始时指向NULL
	lk : link object; // 声明一个object类型的弱引用, 其行为与C++中weak_ptr相同, 初始时指向NULL
	int main(){
		... //成员函数内容
	}
}
class object{}
```

#### 2.3.2. 声明函数局部变量
在一个成员函数中, 有3种形式声明局部变量


1.  v : int; // 声明一个int类型的变量, 初值为0
2.  obj : object; // 声明一个object类型的强引用, 其行为与C++中shared_ptr相同, 但回收内存的时机略有不同, 初始时指向NULL
3.  obj : new object; //创建一个object类型的变量, obj为强引用, 其成员变量若为int, 则初值为0, 若为引用, 则指向NULL

#### 2.3.3. 逻辑控制
LightC中逻辑控制及函数调用语句与C语言中基本类似, 不再赘述.
#### 	2.3.4. 变量作用域
LightC中目前不支持全局变量, 所以变量作用域仅限于每个成员函数自己的栈帧.
每个函数会隐式传入一个this指针, 可以通过这个指针来调用当前类的其他成员, 如下所示:



 ```
class main{
	v : int;
	obj : object; 
	lk : link object;
	int main(){
		// a = this.v; error, a变量不存在
		// this.obj = v; error, this中obj的类型与this.v不匹配
		int v; // 与this中的v不冲突
		// this.v = this.obj.func(v); error. this.obj指向NULL
		obj : new object;
		this.obj = obj;
		this.v = this.obj.func(v); //this.v = 0+233 = 233
	}
}
class object{
	int func(int v){
		return v+233;
	}
}
```

## 3. 编程模型
LightC鼓励使用组件模式而不是面向对象模式, 并没有支持标准的面向对象模型, 更偏向于Golang中的struct + interface的模型, 禁止了继承但可以通过类型转换实现多态,  避免继承层数过多带来不必要的心智负担. 同时适当对多态的支持也有利益对程序的封装.
## 4. 内存模型
LightC的内存分配通过调用C++编写的库来实现.
### 4.1. 引用root/声明栈
在类实例的引用树中, 需要有一个root节点, 通过这个节点进行搜索可以遍历到其他所有活跃实例, 而没有被遍历到的实例则是循环引用造成的, 在后期会通过一些手段处理掉. 在LightC中是通过维护一个声明栈来实现的.
#### 4.1.1. 入栈
入栈有以下三种情况:

1. 由于LightC只有成员函数, 所以在每个成员函数被调用(包括主函数)前, 会向栈中push一个null, 用以表示此成员函数生命周期内声明栈的起始点.
   当使用new语句时, 会为该实例分配内存,设置引用计数为1, 同时向栈中push该实例的id.
2. 当函数返回值是一个引用时, 会在清空当前函数的声明栈后, 向栈中push返回值的实例id, 同时增加实例的引用计数(实际上为了避免实例被销毁, 引用计数的增加放在了清空栈之前)
3. 弱引用被赋值给其他变量时, 会先判断该弱引用是否存在, 若不存在抛出error(后期会提供判断是否存在的内置函数), 若存在则入栈其id, 且增加引用计数.
#### 4.1.2. 出栈
成员函数结束时, 会一直pop栈顶直到遇到null. 对于每个pop出的id, 会减少该实例的引用计数. 最后会pop掉栈顶的null.
### 4.2. 引用计数的变更
引用计数除了在上诉声明过程中会变更外, 只有在实例的成员间赋值会产生变更. 注意, 函数中的临时变量即使是引用类型, 在赋值过程中也不会产生变更, 因为声明栈中已经保留了实例的引用, 临时变量的行为将会与share_ptr相同.
示例:
1. x.a = b; //a为强引用类型, b为强引用类型, 则b实例的引用计数+1
2. x.ta = b; //ta为弱引用类型, b为强引用类型, 则b实例的引用计数不变
3.  x.ta = tb; //ta为弱引用类型, tb为弱引用类型, 则tb实例会被放入声明栈中, 引用计数+1
### 4.3. 成员变量寻址
在以下情况会调用内置函数库进行实现:
1. x.a = b; 会调用内置函数令x实例的a变量赋值为b
2. b = x.a; 会调用内置函数让x实例的a赋给b

调用成员函数时直接通过类名和函数名进行跳转, 无需使用内置函数:
x.f() ⇒ jr __LIGHTC_MEM_FUN_x_f
## 5. 编译过程
从源代码到汇编大致经过了以下几个过程
###	5.1.  词法分析
使用flex进行词法分析, 生成token.
### 5.2. 文法分析
使用bison进行文法分析, 生成不具有类型推断的三地址码.
### 5.3. class声明

遍历已生成的三地址码中类型声明和成员变量/函数声明部分, 生成class及其成员列表.
### 5.4. 类型检查
根据已生成的class表遍历三地址码, 对其中函数块中的变量类型进行检查和推断. 同时对三地址码进行修改, 赋予其类型信息和其他信息(比如函数入口名称).
### 5.5. 寄存器分配
根据修改后的三地址码进行寄存器分配, 生成伪汇编代码.
### 5.6. 伪汇报翻译和链接
将伪汇编代码翻译成指定平台的汇编代码, 同时与通过c++写的函数库进行链接, 生成可执行程序.
