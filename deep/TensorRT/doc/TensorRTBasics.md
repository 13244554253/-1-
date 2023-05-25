# TensorRT基础
## API
python API 和C++ API
## 编程模型
1. 构造阶段（模型定义和优化）
2. 运行时阶段（推理）
## 基本流程
* 创建网络定义。
* 指定构建器的配置。
* 调用构建器来创建引擎。
***
* 反序列化构造好的引擎的.plan。
* 从引擎创建执行上下文。
* 填充输入缓冲区以进行推理。
* 调用enqueue()或者execute()在执行上下文上运行推理。

## 插件
用于支持一下trt本身不支持的操作的实现。在转换网络时，ONNX 解析器可以找到使用 TensorRT 的PluginRegistry创建和注册的插件。TensorRT 附带一个插件库，其中许多插件和一些附加插件的源代码可以在此处找到。[插件库](https://github.com/NVIDIA/TensorRT/tree/main/plugin)

## 精度控制
TensorRT默认选择的CUDA内核执行浮点计算的核函数是FP32精度实现，可通过配置BuilderFlag 选项来指示计算精度，降低计算精度值至FP16可以提高计算速度，对于对于输入动态范围约为 1 的正则化模型计算会产生明显加速。

## Float量化
Tensorrt可以将浮点数float值线性压缩并四舍五入到八位整数int,这样会显著提高算术吞吐量,但在量化float张量前,Tensorrt必须知道该数值的dynamic范围,动态范围信息可由构建器进行校准,也可以在框架中执行量化感知训练,最后将动态信息和模型一起导入Tensorrt.

## 张量和数据格式
Tensorrt会在优化网络的同时,在内部执行数据格式转换(.hwc,或其他),以使用尽可能快的cuda kernel.  
支持动态形状输入,具体实现通过指定构建器中一个模型实例的多个配置文件以生成优化的推理引擎, TensorRT 为每个配置文件创建一个优化的引擎，选择适用于 [最小、最大] 范围内的所有形状并且对于优化点最快的 CUDA 内核 - 通常每个配置文件都有不同的内核。

## trtexec
示例目录中包含一个名为trtexec的命令行包装工具。 trtexec是一种无需开发自己的应用程序即可快速使用 TensorRT 的工具。 trtexec工具有三个主要用途：  

* 在随机或用户提供的输入数据上对网络进行基准测试。
* 从模型生成序列化引擎。
* 从构建器生成序列化时序缓存。


## 模型推理引擎构建之.pt->.engine
* 导出网络定义以及相关权重
* 解析网络定义以及相关权重
* 根据显卡算子构造出最优执行计划
* 将执行计划序列化存储
* 反序列化执行计划
* 进行推理  
__第三步表明tensorrt转换的模型是与硬件绑定的，当cuda和cudnn发生改变，那模型就得重新转换__

### 模型转换方式
1. Tensorrt 接口
使用训练框架自带的TensorRT 接口，如 TF-TRT、Torch-TRT。

2. trtexec  
示例：
```
def torch2onnx(model_path,onnx_path):
    model = load_model(model_path)
    test_arr = torch.randn(1,3,32,448)
    input_names = ['input']
    output_names = ['output']
    tr_onnx.export(
        model,
        test_arr,
        onnx_path,
        verbose=False,
        opset_version=11,
        input_names=input_names,
        output_names=output_names,
        dynamic_axes={"input":{3:"width"}}            #动态推理W纬度，若需其他动态纬度可以自行修改，不需要动态推理的话可以注释这行
    )
    print('->>模型转换成功！')
```
执行指令  
> ./trtexec --onnx=repvgg_a1.onnx --saveEngine=repvgg_a1.engine --workspace=1024  --fp16

## 模型推理引擎构建之tensorrt API搭建

TensorRT API 的整个构建过程可以分为构建阶段和运行阶段。  
__构建阶段：__
* 添加算子&数据
* 网络参数配置
* 算子间逻辑连接
* 组建模型网生成 TensorRT Engine  

__运行阶段：__
* 调用构建阶段成的 TensorRT Engine 进行前向推理计算。

__关键模块：__
![var](../s/enginbuild.png)

__C++ API:__  
可以通过头文件NvInfer.h访问，并且位于nvinfer1命名空间中。
```
#include “NvInfer.h”

using namespace nvinfer1;
```

### 基本流程
__构建阶段:__
1. 构建Logger日志记录器  
要创建构建器，首先需要实例化ILogger接口。此示例捕获所有警告消息，但忽略信息性消息：
```
class Logger : public ILogger           
{
    void log(Severity severity, const char* msg) noexcept override
    {
        // suppress info-level messages
        if (severity <= Severity::kWARNING)
            std::cout << msg << std::endl;
    }
} logger;

//创建logger实例
IBuilder* builder = createInferBuilder(logger);
```

2. 创建网络定义  
可使用ONNX模型解析器API进行网络定义填充。

```
uint32_t flag = 1U <<static_cast<uint32_t>(NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);   //32位的无符号整型uinit32

INetworkDefinition* network = builder->createNetworkV2(flag);
IParser*  parser = createParser(*network, logger);  //模型解析器，创建了一个指向IParser类实例对象的指针parser，可访问其成员函数和变量。

//解析onnx模型进行网络定义
parser->parseFromFile(modelFile, 
    static_cast<int32_t>(ILogger::Severity::kWARNING));
for (int32_t i = 0; i < parser.getNbErrors(); ++i)
{
std::cout << parser->getError(i)->desc() << std::endl;
}

```

3. 构建序列化推理引擎
```
IBuilderConfig* config = builder->createBuilderConfig();  //构建配置可以通过设置属性来控制Tensorrt进行网络优化，如maximum workspace size等
// 配置属性设置总全局内存大小
config->setMemoryPoolLimit(MemoryPoolType::kWORKSPACE, 1U << 20);


//引擎构建
IHostMemory*  serializedModel = builder->buildSerializedNetwork(*network, *config);  //序列化引擎

//序列化引擎不能跨平台或 TensorRT 版本移植。引擎特定于构建它们的确切 GPU 模型（除了平台和 TensorRT 版本）。

```

__由onnx模型解析并构建Tensorrt引擎并序列化保存示例：https://www.pudn.com/news/62c920045f75f3409e902dbc.html__


### plugin插件构建
plugin有[官方库](https://github.com/NVIDIA/TensorRT/tree/master/plugin)同时也支持自定义plugin。
要添加自己的算子，可以仿照官方plugin库进行修改添加，将生成的libnvinfer_plugin.so.7替换原本的.so文件即可。或者自己写一个类似于官方plugin的组件，将名称替换一下，同样生成.so，在TensorRT的推理项目中引用这个动态链接库即可。

__具体自定义plugin参考：https://zhuanlan.zhihu.com/p/297002406__
