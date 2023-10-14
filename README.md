# ExactBooleanMinimization

Reads a single-output Boolean function in PLA format and outputs the corresponding ROBDD in pictorial DOT format.



## Requirement

- gcc/clang with C++17 supported
- CMake



## Build

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```



## Support Platforms

GCC/Clang



## Run

```bash
./main [input_pla] [output_pla]
```



## Feature

|          | Name                       | Description                                      |
| :------: | -------------------------- | ------------------------------------------------ |
| &#x2705; | Read file                  | Read the content of the pla file.                |
| &#x2705; | Handle commands            | Handle the commands from read from the pla file. |
| &#x2705; | Exact boolean minimization | Exact booleans minimization                      |
| &#x2705; | Output file                | Output the pla format file.                      |



## Test Result

### Case 1

[example/case1](./example/case1)

#### Input PLA

``` pla
.i 4
.o 1
.ilb a b c d
.ob f
.p 7
00-0 1
0-11 1
1-01 1
0101 1
1111 -
100- 1
-01- 1
.e
```

#### Output PLA

``` dot
.i 4
.o 1
.ilb a b c d 
.ob f
.p 4
-0-0 1
-1-1 1
-01- 1
10-- 1
.e
```



### Case 2
[example/case2](./example/case2)

#### Input PLA

``` pla
.i 5
.o 1
.ilb a b c d e
.ob f
.p 18
00000 -
00011 -
00101 -
00110 1
00111 -
01001 1
01010 -
01011 -
01110 1
01111 1
10100 -
10101 1
10111 1
11000 -
11001 -
11010 1
11101 1
11110 1
.e
```

#### Output PLA

``` dot
.i 6
.o 1
.ilb a b c d e f 
.ob f
.p 2
1-01-1 1
11-10- 1
.e
```



### Case 3

[example/case3](./example/case3)

#### Input PLA

``` pla
.i 6
.o 1
.ilb a b c d e g
.ob f
.p 7
001000 1
0-1011 1
1-0101 1
11-100 1
111101 -
1-0111 1
-0111- 1
.e
```

#### Output PLA

``` dot
.i 6
.o 1
.ilb a b c d e f 
.ob f
.p 2
1-01-1 1
11-10- 1
.e
```



## Support

Email: allen0523win@gmail.com

Email: B11130225@mail.ntust.edu.tw  
