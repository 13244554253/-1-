# OpenMPAPI

OpenMP由三个主要的API组件组成：
  1. [**编译器指令**](./OpenMPDirectives.md)
  1. [**运行时库函数**](./OpenMPRuntime.md)
  1. [**环境变量**](./OpenMPEV.md)
- 后来的一些API包含了这三个相同的组件，但是增加了指令、运行时库函数和环境变量的数量。
- 开发人员决定如何使用这些组件，最简单的情况下，只需要其中的几个

## 1.编译器指令

编译器指令在源代码中以注释的形式出现，默认情况下编译器会忽略。因此需要对其进行特殊标注。

**OpenMP编译器指令常被用于一下几种情况:**

- 生成一个并行区域
- 在线程之间划分代码块
- 在线程之间分配循环迭代
- 序列化代码段
- 线程之间的工作同步

**编译器指令语法如下:**
```c++
sebtinel directive-name [clause, ...]

// 例如：
#pragma omp parallel default(shared) private(beta,pi)
```

关于编译器指令的详解请[点击此处](./OpenMPDirectives.md)查看


## 2.运行时库函数(Run-time Library Routines) 

OpenMP API 包括多种多样的库函数，这些函数的作用如下:

- 设置和查询线程的数量
- 查询线程的唯一标识符(线程ID)、父线程的标识符、线程团队的大小
- 设置和查询动态线程特性
- 查询是否在一个并行区域，以及在什么级别
- 设置和查询嵌套并行性
- 设置、初始化终止锁以及嵌套锁
- 查询 wall clock time 和分辨率

对于C/C++来说，所有运行时库函数都是实际的子程序。C/C++使用OpenMP需要包含对应的头文件。具体示例如下:
```c++
#include <omp.h>
int omp_get_num_threads(void)
```
关于运行时库函数的详解请[点击此处](./OpenMPRuntime.md)查看

## 3.环境变量

OpenMP提供了几个环境变量，用于在运行时控制并行代码的执行。


**环境变量的作用如下：**
- 设置线程数
- 指定如何换份循环交互
- 将线程绑定到处理器
- 启用/禁用嵌套的并行性；设置嵌套并行度最大级别
- 启用/禁用动态线程
- 设置线程堆栈大小
- 设置线程等待策略

下面是使用环境变量的例子,不同的shell有不同的语法。
```shell
csh/tcsh: setenv OMP_NUM_THREADS 8
sh/bash: export OMP_NUM_THREADS=8
```

关于环境变量的详解请[点击此处](./OpenMPEV.md)查看
