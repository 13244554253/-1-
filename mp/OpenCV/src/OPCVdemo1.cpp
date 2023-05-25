#include <iostream>
#include <opencv2/core.hpp>
#include<stdlib.h>

//定义parllel类实现并行化循环矩阵元素加运算
class ParallelAdd : public cv::ParallelLoopBody
{
public:
	ParallelAdd(int* src1, int* src2, int* out, int blc, int size):src1(src1), src2(src2), out(out), blc(blc), size(size)
	{	}
	//重写operator函数，参数为Range类型
	void operator()(const cv::Range& range) const
	{
		//定义并行操作
		for (int i = 4; i < 17; i++)
		{
			int64 sum = 0;
			for (int j = range.start; j < range.end; j++)
			{
				int p1 = src1[j];
				int p2 = src2[j];
				int t = (p1 - blc) * i + blc;
				t = t >= 0 ? t : 0;
				t = t <= 4095 ? t : 4095;		// clip
				t = t - p2;
				t = t >=0 ? t : - t;					// abs
				sum +=t;
			}
			out[i-4] = (int32_t)sum / size;
		}
	}
 
private:
	int* src1;
	int* src2;
	int* out;
	int blc;
	int size;
};
 

//串行计算函数定义
void test_for(int* src1, int* src2, int* out, int blc, int size)
{
	for (int i = 4; i < 17; i++)
	{
		int64 sum = 0;
		int64 sum1 = 0;
		int64 sum2 = 0;
		int64 sum3 = 0;
		for (int j = 0; j < size / 4; j++)
		{
			// np.mean(np.abs(((short - blc) * i + blc).clip(0, 4095) - long))
				int p1 = src1[4 * j + 0];
				int p2 = src2[4 * j + 0]  ;
				int t = (p1 - blc) * i + blc;
				t = t >= 0 ? t : 0;
				t = t <= 4095 ? t : 4095;		// clip
				t = t - p2;
				t = t >=0 ? t : - t;					// abs
				sum +=t;

				p1 = src1[4 * j + 1];
				p2 = src2[4 * j + 1];
				t = (p1 - blc) * i + blc;
				t = t >= 0 ? t : 0;
				t = t <= 4095 ? t : 4095;		// clip
				t = t - p2;
				t = t >=0 ? t : - t;					// abs
				sum1 +=t;

				p1 = src1[4 * j + 2];
				p2 = src2[4 * j + 2];
				t = (p1 - blc) * i + blc;
				t = t >= 0 ? t : 0;
				t = t <= 4095 ? t : 4095;		// clip
				t = t - p2;
				t = t >=0 ? t : - t;					// abs
				sum2 +=t;

				p1 = src1[4 * j + 3];
				p2 = src2[4 * j + 3];
				t = (p1 - blc) * i + blc;
				t = t >= 0 ? t : 0;
				t = t <= 4095 ? t : 4095;		// clip
				t = t - p2;
				t = t >=0 ? t : - t;					// abs
				sum3 +=t;
		}
		out[i-4] = (int32_t)(sum + sum1 + sum2 + sum3)/ size;
	}
	
}
 
int main()
{
    int height = 1920;
    int width = 1080;
	int blc = 23;
	int out_dim = 13;

	int bytes = height * width * sizeof(int);
	int size = height * width;
	int *src1 = (int*)malloc(bytes);
	memset(src1, 1, bytes);

	int *src2 = (int*) malloc(bytes);
	memset(src2, 2, bytes);

	int *src3 = (int*) malloc(out_dim * sizeof(int));
 
	int64 t1 = cv::getTickCount();

	//直接for循环
	test_for(src1, src2, src3, blc, size);
	int64 t2 = cv::getTickCount();
	std::cout << "time: " << (t2 - t1)/cv::getTickFrequency() * 1000 << " ms" << std::endl;

	// parallel_for_并行
	cv::parallel_for_(cv::Range(0, size), ParallelAdd(src1, src2, src3, blc, size));
	int64 t3 = cv::getTickCount();
	std::cout << "parallel time: " << (t3 - t2) / cv::getTickFrequency() * 1000 << " ms" << std::endl;

 
	return 0;
}
 
/*
time: 203.233 ms
parallel time: 15.3361 ms
*/
