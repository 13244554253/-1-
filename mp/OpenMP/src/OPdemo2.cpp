# include <omp.h>
# include <iostream>
# define N 1000
# define CHUNKSIZE 100

//DO / for 指令示例： vector相加

int main(int argc, char *argv[]){
    int i, chunk, tid;
    float a[N], b[N], c[N];
    for ( i = 0; i < N; i++)  // 给数组a和赋值
        a[i] = b[i] = i * 1.0;

    chunk = CHUNKSIZE; //设置块大小

// 指令：
// parallel: 下面的代码将被多个线程并行执行
// 子句：
// shared(a,b,c,chunk): 数组A B C 和变量N将由所有线程共享
// private(i): 变量i对每个线程都是私有的，每个线程都有自己唯一的副本
#pragma omp parallel shared(a,b,c,chunk) private(i, tid) // 声明下面代码将会以多线程执行，同时声明了各个变量的作用域
    {
        std::cout<<"Thread nums is:"<<omp_get_num_threads()<<std::endl; //当并行区域内线程数
    
    // 指令：
    // for: 将下面代码中的循环计算任务分配到多个线程中并行执行
    // 子句：
    // schedule(dynamic, chunk): 循环迭代将在CHUNK大小的块中动态分布
    // nowait: 线程在完成各自的工作后将不会同步
    #pragma omp for schedule(dynamic, chunk) nowait //将下面的for循环交给多线程来执行，循环迭代分成小块，并在线程之间动态调度，每个线程执行chunk次迭代。执行完毕后并不会同步
        for(i = 0; i<N; i++) //
            c[i] = a[i] + b[i];  // 给数组C赋值
        std::cout<<"Run thread id is:"<<omp_get_thread_num()<<std::endl; //当前线程id
    }
    
    return 0;
}