#!/bin/sh
# function tut {
#     g++ -std=c++14 -c -o library.o library.cpp
#     gcc -shared -o liblibrary.so library.o
#     g++ -Llib/ -Wall -o code main.cpp -llibrary
#     # if needed: 
#     export LD_LIBRARY_PATH=lib/:$LD_LIBRARY_PATH
#     ./code
# }
# Thanks to https://www.baeldung.com/linux/bash-get-location-within-script
SCRIPT_PATH="${BASH_SOURCE}"
while [ -L "${SCRIPT_PATH}" ]; do
  SCRIPT_DIR="$(cd -P "$(dirname "${SCRIPT_PATH}")" >/dev/null 2>&1 && pwd)"
  SCRIPT_PATH="$(readlink "${SCRIPT_PATH}")"
  [[ ${SCRIPT_PATH} != /* ]] && SCRIPT_PATH="${SCRIPT_DIR}/${SCRIPT_PATH}"
done
SCRIPT_PATH="$(readlink -f "${SCRIPT_PATH}")"
SCRIPT_DIR="$(cd -P "$(dirname -- "${SCRIPT_PATH}")" >/dev/null 2>&1 && pwd)"

echo "Base path is: '$SCRIPT_DIR' vs '$(pwd)'"
mkdir -p ./lib ./build ./bin
g++ -std=c++20 -Wfatal-errors -Iinclude/       -c -o build/dirStream.o include/eFScru/dirStream.inl && 
g++ -std=c++20 -Wfatal-errors -Iinclude/ -fPIC -c -o build/FSTree.o    src/FSTree.cpp &&
gcc    -shared -Wfatal-errors -Iinclude/ -fPIC    -o $SCRIPT_DIR/lib/lib-eFScru.so  build/FSTree.o

# gcc -shared -o liblibrary.so library.o
export LD_LIBRARY_PATH=lib/:$LD_LIBRARY_PATH
rm -frd ./build/
#g++ -Llib/ -Wall -Iinclude/ -o ./bin/test    main.cpp                 -llibrary
g++ -L./lib -Wfatal-errors -Iinclude/ -o ./bin/test    src/examples/libTest.cpp $SCRIPT_DIR/lib/lib-eFScru.so  #-leFScru
g++ -L./lib -Wfatal-errors -Iinclude/ -o ./bin/example src/examples/example.cpp $SCRIPT_DIR/lib/lib-eFScru.so #-leFScru

# reproduce:
# # This will produce the amazum.o
# g++ -fPIC -c amazum.cpp
# # make dynamic
# g++ -shared -fPIC -o libamazum.so amazum.o
# # link program: one of the lines below (both lines DO THE SAME)
# g++       -o program program.cpp libamazum.so
# g++ -Wall -o program program.cpp -L. -lamazum
# # Let's run the program:
# ./program
# -> ./program: error while loading shared libraries: libamazum.so: cannot open shared object file: No such file or directory
# Fix:
# g++ -shared -o libamazum.so amazum.o -Wl,-soname=./libamazum.so
# g++ -g -Wall -o program program.cpp libamazum.so
