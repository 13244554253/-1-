#include <omp.h>
#include <iostream>
#define N 1909

// sections指令示例： vector相加
// 该程序主要演示不同工作块将由不同的线程完成

int main(){
    int i;
    float a[N],b[N],c[N],d[N];

    for (i = 0; i< N; i++){
        a[i] = i * 1.5;
        b[i] = i + 22.35;
    }

// 指令：
// parallel: 下面的代码将被多个线程并行执行
// 子句：
// shared(a,b,c,d): 数组a b c d 将由所有线程共享
// private(i): 变量i对每个线程都是私有的，每个线程都有自己唯一的副本
#pragma omp parallel shared(a,b,c,d) private(i)// 下面的代码将以多线程执行，a,b,c,d各个线程直线共享，i为各个线程的私有变量
    {
        std::cout<<"Thread nums is:"<<omp_get_num_threads()<<std::endl; //当并行区域内线程数
    // nowait: 线程在完成各自的工作后将不会同步
    #pragma omp sections nowait  // 下面的代码线程在完成各自的工作后将不会同步
        {
        #pragma omp section
            {
                for (i = 0; i < N; i++)
                    c[i] = a[i] + b[i];  // 给数组C赋值
                std::cout<<"C id is:"<<omp_get_thread_num()<<std::endl; //完成C数组赋值的线程id
            }
        #pragma omp section
            {
                for (i = 0; i< N; i++)
                d[i] = a[i] * b[i]; // 给数组D赋值
                std::cout<<"D id is:"<<omp_get_thread_num()<<std::endl; //完成D数组赋值的线程id
            }
        }
    }
    return 0;
}


