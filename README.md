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
  
