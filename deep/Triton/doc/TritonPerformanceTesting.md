# triton 测试分析优化调研报告 
对于深度模型的推理服务性能提升，可以从两方面进行优化，1. 从模型本身，优化网络的计算图结构，减少计算量，加速推理 2. 权衡模型的延迟和吞吐量，了解模型的资源利用率，减少延迟并增加模型的吞吐量。  
## 优化方向
***
_模型调度层面_
* 最大化模型吞吐量-确保放置在每个 GPU 上的模型总和不超过可用内存和 GPU 利用率的某个阈值。
* 优化硬件使用-检查 GPU 内存要求，以在更少的硬件上运行更多模型。
* 提高可靠性- 保证在 GPU 上加载的模型不会超出其能力，消除内存不足错误。  

_非调度层面_
* 更好的硬件大小- 确定运行模型所需的确切硬件数量，使用内存要求。
* 更好的模型大小- 比较和对比不同的模型，使用计算需求作为模型执行情况的附加数据点。这有助于生成更轻量级的模型并减少推理需求所需的内存量。

## 工具

[Triton模型分析器(Analyzer)](https://github.com/triton-inference-server/model_analyzer)是一个 CLI 工具，可帮助更好地理解 Triton 推理服务器上模型的计算和内存要求。这些报告将帮助用户更好地了解不同配置中的权衡，并选择能够最大限度地提高 Triton Inference Server 性能的配置。   
[Triton模型导航器(Navigator)](https://github.com/triton-inference-server/model_analyzer)一种能够自动将模型从源格式转变到最佳的模型格式和配置的工具，以便模型在 Triton 推理服务器上的部署。最后会结合模型分析器进行最佳模型配置的搜索，以提供最佳性能。

## 模型导航器(Navigator)
1. 模型导出
* 将模型从源代码框架以二进制文件格式导出并保存
* TensorFlow2 支持输出格式有SavedModel
* Pytorch 支持输出格式有ONNX、Torch-TensorRT、TorchScript
2. 模型转换
* 使用Model Navigator CLI将模型从导出的源格式转换为目标格式。
* TensorFlow支持SavedModel -> TensorFlow-TensorRT SavedModel, ONNX -> TensorRT
* Pytorch支持ONNX->TensorRT
3. 转换正确性验证
* 正确性验证：计算导出模型和目标模型输出的Absolute tolerance和Relative tolerance进行正确性验证。
* 性能验证：转换后的目标模型进行[性能分析测试](https://github.com/triton-inference-server/server/blob/main/docs/perf_analyzer.md)，验证转换后是否有性能优化和加速。
* 手动验证：自定义指标验证，Model Navigator 导出函数返回一个 PackageDescriptor 对象，用户可以加载目标模型执行额外测试。
4. 结果输出
* 模型导航器的导出结果位于navigator_workdir目录下的<model_name>.nav.workspace目录中。
* model_input包含数据加载器生成的输入样本，格式Numpy npz 
* model_output包含模型返回的输出数据
* navigator.log包含生成的日志和运行时的错误消息
* status.yaml包含导出模型的状态、模型导航器 API 配置和有关环境的信息。

## 模型转换
Triton Model Navigator 收集了一组用于格式之间模型转换的工具。目前支持：
| Input Model Format    | Target Model Format   |
|-----------------------|-----------------------|
| TensorFlow SavedModel | TensorFlow SavedModel |
| TensorFlow SavedModel | TensorFlow-TensorRT |
| PyTorch TorchScript   | PyTorch TorchScript   |
| PyTorch TorchScript   | Torch-TensorRT   |
| ONNX   | ONNX   |
| ONNX  | TensorRT   |

__目标模型都是Triton inference server可处理的模型格式的子集。__  
* 例一：
```
# 由onnx模型格式生成fp16精度的Tensorrt plan模型
$ model-navigator convert \
   --model-name EfficientNet \
   --model-path /storage/efficientnet_tf2_fp32.onnx \
   --output-path efficientnet_tf2_fp16.plan \
   --min-shapes input_1=1,32,32,3 \
   --opt-shapes input_1=16,116,116,3 \
   --max-shapes input_1=32,224,224,3 \
   --target-precisions fp16

# 模型没有后缀名的需要设置model_format指明源模型文件格式 
$ model-navigator convert \
   --model-name ResNet50 \
   --model-path /storage/rn50_tf1_fp32 \
   --model-format onnx \
   --max-batch-size 32
```
* 例二：TorchScript转为ONNX
```
$ model-navigator convert \
  --model-name ResNet50_trace \
  --model-path navigator_workspace/convert/pytorch_vision/pytorch_vision/ResNet50/resnet50_ts_trace.pt \
  --target-formats onnx \
  --config-path MobileNetV2_trace_config.yaml \
  # TorchScript格式不包含模型signature，在转换时需在配置选项中设置，也可通过指定配置文件。
  --inputs image__0=-1,3,-1,-1:float32 \
  --outputs output__0=-1,1000:float32 \
  # ts2onnx转换器所需的参数
  --value-ranges image__0=0,1 \
  --dtypes image__0=float32 \
  # 对于具有动态轴的TorchScript模型，要提供最大输入数据维度
  --max-shapes image__0=128,3,600,800
```
## 模型分析器(Analyzer) 
-m 或者 --mode 标志告诉模型分析器其运行的上下文，所有子命令都可以访问。  
1. 在线模式  
这是默认模式，模型分析器在此模式下，将运行以搜索在线推理场景下的模型最佳配置，最佳配置一般是最大化吞吐量的模型配置，可通过profile子命令的--latency-budget指定延迟预算作为限制，得到的结果是给定延迟预算中吞吐量最高的模型配置。  
__在线模式下profile 和 report 子命令将生成特定于在线推理的摘要__  

2. 离线模式  
--mode=offline告诉模型分析器为离线推理场景找到最佳模型配置。同样以最大化吞吐量为目标，但是离线模式不过多考虑延迟，以profile子命令--min-throughput忽略不超过每秒最小推理次数的任何配置。  

## 模型分析器的子命令 
模型分析器的功能分为三个独立的子命令（其中一个analyze已被弃用）。每个子命令都有自己的 CLI 和配置选项。
1. profile  
```
model-analyzer profile -h
```
* 子命令profile首先在checkpoint目录下加载最新的checkpoint， 使用per analyzer进行模型推理分析，搜集相关测量指标。  
* 使用配置的YAML中指定的模型进行排序，可通过根据使用目标不同或在不同约束条件下对结果进行排序和过滤。
* 根据提供的命令行或 YAML 配置选项，子命令profile将对性能分析器和模型配置文件参数执行手动或自动搜索。
* 模型分析器将使用所有指定的运行参数运行tritonserver和性能分析器实例，并最终输出每个配置组合生成的模型配置文件。
* 模型分析器会以固定时间区间收集性能分析器的各种指标，每次收集的性能分析器的测量值会被最终统一保存在checkpoint目录中。


2. report
report子命令允许用户创建关于已分析的一个或多个模型配置的详细报告。模型分析器的报告子命令允许您详细检查模型配置变体的性能。例如，它可以向您显示模型的延迟细分，以帮助您识别模型性能中的潜在瓶颈。详细报告还包含其他可配置的图表和对该特定配置进行的测量表。
```
model-analyzer report -h
```

## shell
1. 自动配置搜索位于/home/model_repo下的resnet50_libtorch模型
```
 model-analyzer profile -m /home/model_repo --profile-models resnet50_libtorch
```
2. 自动配置搜索位于/home/model_repo下的resnet50_libtorch和vgg16_graphdef模型，并保存checkpoints到对应目录下
```
 model-analyzer profile -m /home/model_repo --profile-models resnet50_libtorch,vgg16_graphdef --checkpoint-directory=checkpoints
```
3. 自动配置搜索位于/home/model_repo下的resnet50_libtorch模型，并将模型配置变体存储库更改到/home/output_repo
```
model-analyzer profile -m /home/model_repo --output-model-repository-path=/home/output_repo --profile-models resnet50_libtorch
```
4. 运行手写定义的模型配置的YAML配置文件，测试模型是位于/home/model_repo路劲下的两个模型classification_malaria_v1和classification_chestxray_v1
```
model-analyzer profile -f /path/to/config.yaml
```
YAML文件如下
```
model_repository: /home/model_repo

run_config_search_disable: True  # 禁用配置搜索

concurrency: [2, 4, 8, 16, 32]   # 多并发list
batch_sizes: [8, 16, 64]         # batch_size_list
                                 # 手动设置配置文件测试参数
profile_models:
  classification_malaria_v1:
    model_config_parameters:
      instance_group:
        - kind: KIND_GPU
          count: [1, 2]
      dynamic_batching:
        max_queue_delay_microseconds: [100]
  classification_chestxray_v1:
    model_config_parameters:
      instance_group:
        - kind: KIND_GPU
          count: [1, 2]
      dynamic_batching:
        max_queue_delay_microseconds: [100]
```