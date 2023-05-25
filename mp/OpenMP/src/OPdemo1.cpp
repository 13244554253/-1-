#include <stdio.h>
#include <iostream>
#include <omp.h>


int main(int argc, char *argv[]){
    int nthreads, tid;

// 编译器指令
#pragma omp parallel private(tid) //下面的代码将以多线程执行，tid为每个线程的私有变量（每个线程都有自己的tid）
    {
        tid = omp_get_thread_num(); //运行时库函数  作用：返回在团队中执行此调用的线程的线程号
        std::cout<<"Hello World form thread:"<<tid<<std::endl;

        if(tid == 0){
            nthreads = omp_get_num_threads(); //运行时库函数 返回当前在团队中执行并行区域的线程数,该区域时调用线程的地方
            std::cout<<"Number of threads:"<<nthreads<<std::endl;
        }
    }
    return 0;
}