# OpenMP环境变量

- OpenMP提供了环境变量来控制并行代码的执行
- 所有环境变量名都是大写的

##### OMP_SCHEDULE

只适用于DO, PARALLEL DO(Fortran)和for， parallel for(C/C++)指令，它们的schedule子句设置为运行时,此变量的值觉得如何在处理器上调度循环的迭代

```C++
setenv OMP_SCHEDULE "guided, 4"
setenv OMP_SCHEDULE "dynamic"
```

##### OMP_NUM_THREADS

设置执行期间使用的最大线程数

```C++
setenv OMP_NUM_THREADS 8
```

##### OMP_DYNAMIC

启用或禁用可用于并行区域执行的线程数量的动态调整。参数值为TRUE和FALSE

```C++
setenv OMP_DYNAMIC TRUE
```

##### OMP_PROC_BIND

启用或禁用可用于并行区域执行的线程数量的动态调整。参数值为TRUE和FALSE

```C++
setenv OMP_PROC_BIND TRUE
```

##### OMP_NESTED

启用或禁用嵌套并行性。参数值为TRUE和FALSE

```C++
setenv OMP_NESTED TRUE
```

##### OMP_STACKSIZE

控制已创建(非主)线程的堆栈大小

```C++
setenv OMP_STACKSIZE 2000500B
setenv OMP_STACKSIZE "300 k"
setenv OMP_STACKSIZE 10M
setenv OMP_STACKSIZE "10M"
setenv OMP_STACKSIZE "20m"
setenv OMP_STACKSIZE "1G"
setenv OMP_STACKSIZE 20000
```

##### OMP_WAIT_POLICY

为 OpenMP 实现提供有关等待线程的所需行为的提示。一个兼容的OpenMP实现可能遵守也可能不遵守环境变量的设置。
有效值分为 ACTIVE 和 PASSIVE。
- ACTIVE 指定等待的线程大部分应该是活动的，即，在等待时消耗处理器周期。
- PASSIVE 指定等待的线程大部分应该是被动的即，而不是在等待时消耗处理器周期。


ACTIVE 和 PASSIVE 行为的细节是由实现定义的。

```C++
setenv OMP_WAIT_POLICY ACTIVE
setenv OMP_WAIT_POLICY active
setenv OMP_WAIT_POLICY PASSIVE
setenv OMP_WAIT_POLICY passive
```

##### OMP_MAX_ACTIVE_LEVELS

控制嵌套的活动并行区域的最大数目，该环境变量的值必须是非负整数。
如果该环境变量的请求值大于实现所能支持的嵌套活动并行级别的最大数量，或者该值不是一个非负整数，则程序行为是由实现定义的

```C++
setenv OMP_MAX_ACTIVE_LEVELS 2
```

##### OMP_THREAD_LIMIT

设置用于整个OpenMP程序的OpenMP线程的数量。该环境变量的值必须是正整数。
如果该环境变量的请求值大于实现所能支持的线程数，或者该值不是正整数，则程序行为是由实现定义的

```C++
setenv OMP_THREAD_LIMIT 8
```
