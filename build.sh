g++ main.cpp timer.cpp --std=c++14 -O3 -Wall -Werror -o summer_gcc
echo "summer_gcc"
g++ main.cpp timer.cpp --std=c++14 -g -Wall -Werror -o summer_gcc_d
echo "summer_gcc_d"
clang++ main.cpp timer.cpp --std=c++14 -O3 -Wall -Werror -o summer_clang
echo "summer_clang"
clang++ main.cpp timer.cpp --std=c++14 -g -Wall -Werror -o summer_clang_d
echo "summer_clang_d"
