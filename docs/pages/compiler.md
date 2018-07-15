<!---
 Copyright (c) 2017 vargaconsulting, Toronto,ON Canada
 Author: Varga, Steven <steven@vargaconsulting.ca>
--->


Compiling arbitrary POD types to HDF file compund types with H5CPP compiler   {#link_h5cpp_compiler}
====================================================================================================

h5cpp compiler is a fully functional clang based source code transformation tool, that identifies POD C/C++ struct types referenced by 
`h5::create | h5::read | h5::write | h5::append` operators in each TU translation units and generates the necessary HDF5 data descriptions. 

What does this mean in plain English? You take some include files, and a valid c++ program file, then all you have to do to create and HDF5 dataset is 
the following: 
```c++
#include <h5cpp/core>
#include <ddl.h>       // this will be generated by h5cpp compiler
#include <h5cpp/io> 

int main() {
	h5::create<some_complicated_struct>("file.h5", "dataset_name", h5::gzip{9} | h5::chunk{512} );
	return 0;
}
```
then invoke the compiler which will create the data definitions for you:
```bash
h5cpp  struct.cpp --  -I../path/to/h5cpp-llvm  -Dddl.h
```


Install gcc-8 
```bash
sudo add-apt-repository ppa:jonathonf/gcc-8.1 
sudo apt-get update 
sudo apt-get upgrade
sudo apt-get install gcc-8 g++-8
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 100 --slave /usr/bin/g++ g++ /usr/bin/g++-8
```


To compile and install the source code transformation tool you will need the most recent [cmake](https://github.com/Kitware/CMake) and [clang 7.0.0](http://llvm.org/docs/GettingStarted.html#checkout ) tool chain installed. Be certain there is 50GB free disk and 16GB memory space on the system you compiling llvm + clang to prevent spurious error messages in the compile or linking phase.  AWS EC2 m3.2xlarge instance has suitable local disk and memory space.

Obtain cmake, compile and install:
```bash
git clone https://github.com/Kitware/CMake.git
cd CMake && ./bootstrap && make
sudo make install
```

The source code transformation tool is based on clang 7.0.0 tooling, however the generated `h5cpp` compiler may be linked static yielding to a stand alone binary. In case of static linking only the llvm include files still must be present. Here are the instructions to compile clang 7.0.0
```bash
git clone https://git.llvm.org/git/llvm.git/
cd llvm/tools
git clone https://git.llvm.org/git/clang.git/
# begin-optional
cd ../projects
git clone https://git.llvm.org/git/openmp.git/
git clone https://git.llvm.org/git/libcxx.git/
git clone https://git.llvm.org/git/libcxxabi.git/
# end-optional
cd ../../ && mkdir build && cd build
cmake  -DCMAKE_INSTALL_PREFIX=/usr/local CMAKE_BUILD_TYPE=MinSizeRel -DLLVM_TARGETS_TO_BUILD=X86  ../llvm
nohup make -j8&
sudo make install
```

The template library only needs c++17 capable compiler, here is gcc-8 an alternative to clang++ 7.0.0:


[Download OrangeFS](https://s3.amazonaws.com/download.orangefs.org/current/source/orangefs-2.9.7.tar.gz) 
make sure you have the prerequisites, the `libattr1-dev` is not listed on OrangeFS webpage!:
```bash
sudo apt-get install -y gcc flex bison libssl-dev libdb-dev linux-source perl make autoconf linux-headers-`uname -r` zip openssl automake autoconf patch g++ libattr1-dev

make -j4 && sudo make install
```
[Download OpnemMPI]()
```bash
gunzip -c openmpi-3.1.1.tar.gz | tar xf -
cd openmpi-3.1.1
./configure --with-sge --with-pvfs2 --prefix=/usr/local
make -j4 && sudo make install
```
to verify SGE grid engine execute `ompi_info | grep gridengine`




