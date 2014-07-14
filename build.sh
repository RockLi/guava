cd deps/libuv && sh ./autogen.sh && ./configure --enable-static=yes --enable-shared=no --with-pic=yes && make

cd ../../

export ARCHFLAGS="-arch x86_64"
