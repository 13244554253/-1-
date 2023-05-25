# CudaStream
Stream 可以看作是device端的工作的一个队列，host端负责添加work到队列中，device端完成当前work并free后，在stream中调度下一个work。类似于CPU的线程流。多流执行类似于CPU开多线程。  
* CUDA的操作也是在流中，如kernal启动，内存拷贝等。
* 在同一个stream中，各操作是有序的，FIFO先进先出，不可以重叠，不同流中操作可以是重叠的，无序的。
* 如果主动开启stream流，会自动创建默认流来执行核函数，默认流与host端计算是同步的。
* 使用GPU可主动开启多stream，将要读写的文件分为多个部分，使用多个流异步去执行Mermory Copy（H2D）--> Kernel --> Mermory Copy （D2H）,开启多流后，流与CPU是异步的，这时要加上同步操作。
* 用到CUDA的程序一般需要处理海量的数据，内存带宽经常会成为主要的瓶颈。在Stream的帮助下，CUDA程序可以有效地将内存读取和数值运算并行，从而提升数据的吞吐量。  
## 操作并行类型
异步且基于stream的kernel执行和数据传输能够实现以下几种类型的并行：
* Host运算操作和device运算操作并行
* Host运算操作和host到device的数据传输并行
* Host到device的数据传输和device运算操作并行
* Device内的运算并行

Nvidia GPU支持数据拷贝和数值计算可以同时进行。两个方向的拷贝可以同时进行（GPU到CPU，和CPU到GPU）。Stream正是用来实现以上两个并行操作的重要工具。  

Stream实现以上并行操作的基本概念是将数据拆分称许多块，每一块交给一个Stream来处理。  
* 将属于该Stream的数据从CPU内存转移到GPU内存
* GPU进行运算并将结果保存在GPU内存
* 将该Stream的结果从GPU内存拷贝到CPU内存 

__使用多个Stream令数据传输和计算并行，比只用Default Stream增加相当多的吞吐量。在需要处理海量数据，Stream是一个十分重要的工具。__

在Tensorrt中针对cuda流的优化支持在两方面  
1. 使用流式传输

2. 多流并发

### Stream示例
1. API
   ```
    • 创建一个stream
    cudaStream_t stream;
    cudaStreamCreate(&stream);
    • 将host数据拷贝到device
    cudaMemcpyAsync(dst, src, size, type, stream)
    • kernel在流中执行
    kernel_name<<<grid, block, stream>>>(praments);
    • 同步和查询
    cudaError_t cudaStreamSynchronize(cudaStream_t stream)
    cudaError_t cudaStreamQuery(cudaStream_t stream);
    • 销毁流
    cudaError_t cudaStreamDestroy(cudaStream_t stream)
   ```
2. 使用
   ```
    //创建多个流
    cudaStream_t stream[3];
    for(int i=0; i<3; i++)
        cudaStreamCreat(&stream[i]);

    float* hostPtr;
    cudaMallocHost(&hostPtr, 3*size);
    ......
    //开启了三个流，将数据分成三份分别交给了三个流进行计算
    for(int i=0; i<3; i++){
        //从CPU内存复制数据到GPU显存
        cudaMemcpyAsync(DevPtr + i*size, hostPtr + i*size, size, cudaMemcpyHostToDevice, stream[i]);
        //执行Kernel函数
        Mykernel<<<grid, block, 0, stream[i]>>>(outputDevPtr + i*size, size, cudaMemcpyDeviceToHost, stream[i]);
        //将结果拷贝回CPU内存
        cudaMemcpyAsync(hostPtr + i*size, outputDevPtr + i*size, size, cudaMemcpyDeviceToHost, stream[i]);
    }
    //同步流
    for(int i=0; i<3; i++)
        cudaStreamSynchronise(stream[i]);
    //销毁流
    for(int i=0; k<3; i++)
        cudaStreamDestroy(stream[i]);
        
    }
   ```




