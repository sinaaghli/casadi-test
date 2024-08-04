# An example of using casADi for Algorithmic Differentiation

## Step1: make sure casADi is installed

on mac make sure you have brew packet manager. then install cmake

```console
foo@bar:~$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
foo@bar:~$ brew update
foo@bar:~$ brew install cmake
```

then compile and install casady on your system

```console
foo@bar:~$ cd your_workspace_folder
foo@bar:~$ git clone https://github.com/casadi/casadi.git
foo@bar:~$ cd casadi
foo@bar:~$ mkdir build
foo@bar:~$ cd build
foo@bar:~$ cmake ..
foo@bar:~$ make -j
foo@bar:~$ sudo make install
```

## Step2: clone this repository and compile the code

```console
foo@bar:~$ cd your_workspace_folder
foo@bar:~$ git clone https://github.com/sinaaghli/casadi-test.git
foo@bar:~$ cd casadi-test
foo@bar:~$ mkdir build
foo@bar:~$ cd build
foo@bar:~$ cmake ..
foo@bar:~$ make
```

## Step3: run the example from main_simple.cpp

assuming your are currently at `casadi-test/build` folder from last step, run the example by

```console
foo@bar:~$ ./casadi_example
```

the terminal output should look something like

```
$ ./casadi_example
Original Function: ((sq(x)+x)+1)
Derivative Function: ((x+x)+1)
The value of the derivative function at x = 2 is 1000
time: 1839.06
result2: 1000
time2: 0.068
```
