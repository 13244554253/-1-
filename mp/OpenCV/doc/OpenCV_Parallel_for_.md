## OpenCV parallel_for_
__对于一些基本的循环运算，如果直接使用循环，即便是使用指针，运算效率也不高，如果使用并行计算，会大大提升运算效率。__

## 使用parallel_for_并行化的原因
在使用OpenCV进行图像处理时，其计算量是非常大的，在实际运行中需要高效计算，现有的方法有很多，如 OpenMp, TBB, OpenCL 还有 Nvidia 的 CUDA。  

CUDA虽然能利用GPU进行并行加速，但CPU和GPU间**传输耗时**（单一张图片使用opencv的cuda函数传输时间300ms），为实现在**多核**CPU上进行高效计算，OpenCV 提供了并行计算函数 parallel_for_。


## parallel_for_的使用步骤
1. 自定义一个类或结构体，使这个结构体或者是类继承自 ParallelLoopBody 类
   ```
   class MyParallelClass : public ParallelLoopBody
   {}
   ```
1. 在自定义的类或者是结构体中，重写括号运算符（ ），这里只能接受一个 Range 类型的参数，因为后面调用parallel_for_需要。
   ```
   void operator()(const Range& range)
   {
      //在这里定义“循环操作”
   }
   ```

1. 调用parallel_for_ 进行并行处理
   **以并发方式进行处理重载运算里面的循环操作内容**
   ```
   parallel_for_(Range(start, end), MyParallelClass(构造函数列表));
   //Range(start, end) 是一个Range类型的对象
   //MyParallelClass(构造函数列表) 是继承自ParallelLoopBody的自定义并行类的对象
   ```

## parallel_for_的调用
Parallel_for_ 被介绍为parallel data processor，有两种使用方式。
1. 包含头文件
   ```
   #include <opencv2/opencv.hpp>
   ```
2. 函数调用
   
   ```
   void cv::parallel_for_ (const Range &range, const ParallelLoopBody &body, double nstripes=-1.)
   ```


* const Range& range,  即重载函数的括号运算符里面的参数，是一个**Range**类型，range是记录parallelBody循环体的首末位置。重载函数operator中的range重新表示被独立线程处理的任务子集。分解任务是**自动**完成的，并根据CPU核心的计算能力平均分配。
* const ParallelLoopBody& body,  即自己实现的从ParallelLoopBody类继承的类或者是结构体对象
* double nstripes=-1, 设置执行任务的线程数目。（默认会使用所有可用处理器线程，但是任务会分为两个线程。）

