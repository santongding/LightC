yacc -d -Wcex LightC.y -o bison.cpp
mv ./bison.hpp ../../include/bison.hpp
flex -+ LightC.l