# LightC
 A sample oop&gc language.
 ## Support:
 * Class definition
 * Type coercion
 * Garbage collection
 * Type cast and polymorphism(not finish yet)
 * Circular reference detection(not finish yet)
 ## Should not support:
 * Class inherit
 ## Platform
 * Currently aarch64&linux only, could easily extend to other platforms. 
 ## Use(Will be improved recently)
 * Build the project, generate the executable file
 * Run LightC "input file path" "output file dir"
 * Move include files to "output file dir"
 * Run  g++ -g LIGHTC_BUILTIN.cpp LIGHTC_ASM.s -o main 
 * Run ./main
 * Run echo $?, will output the main function's return variable
 ## Example
```
 class a{
    x:int;
    z:link b;
    y:int;
    a f(i:b, j:int){
        this.y;
        this.y = this.y + i.x(j); //this.y = 545
        x:new b;
        if(this.y){
            z:int;
            z = this.y - 45; //500
            this.x = z; //500
            return this;
        }
        return this;
    }
}

class b{
    int x(i:int){
        return 233 + i;
    }
}

class main{
    int main(){
        bb:new b;
        aa:new a;
        x:int;
        x = 2;
        return aa.f(bb, 312).x / x; //250
    }
}
```
  
