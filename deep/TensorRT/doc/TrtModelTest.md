# 基于标准集的模型测试
分别基于标准测试集下的不同类型模型进行精度测试，参与测试模型
|模型|版本|说明|
|:--:|:--:|:--:|
|airway_pt|0.3|TorchScript|
|airway_trt16|0.3|TensorRT_fp16|
|airway_trt32|0.3|TensorRT_fp32|
|nodule_pt|0.3|TorchScript|
|nodule_trt16|0.3|TensorRT_fp16|
|nodule_trt32|0.3|TensorRT_fp32|
|pulmonaryMass_pt|0.3|TorchScript|
|pulmonaryMass_trt16|0.3|TensorRT_fp16|
|pulmonaryMass_trt32|0.3|TensorRT_fp32|
## 模型依赖库及转换脚本
|依赖库名|版本|说明|
|:--:|:--:|:--:|
|onnx|1.12.0||
|onnxruntime|1.12.1||
|NVIDIA CUDA|  11.4||
|NVIDIA cuDNN|8.4.0||
|TensorRT镜像|22.04-py3 |NGC镜像|
|trtexec||转换工具|

### PyTorch->ONNX 脚本
```
# torchToOnnx.py
import torch
torch_model = torch.jit.load("/work/wuzihao/ibotModelDeploy/ibotModel/pulmonary/0.1/airway/1/model.pt") # pytorch模型加载
batch_size = 1  #批处理大小
input_shape = (1, 1, 128, 160, 112)   #输入数据

jit_layer1 = torch_model
print(jit_layer1.graph) 
print(jit_layer1.code) 

# set the model to inference mode
torch_model.eval()

x = torch.randn(1, 1, 128, 160, 112).cuda()		# 生成张量
export_onnx_file = "/work/wuzihao/ibotModelDeploy/modelTrans/result/airway/airway.onnx"	 # 目的ONNX文件名
with torch.no_grad(): 
    torch.onnx.export(torch_model, x,
                    export_onnx_file,
                    opset_version=9, 
                    input_names=["INPUT__0"],		# 输入名
                    output_names=["OUTPUT__0"],	    # 输出名
                    dynamic_axes=None
                    )

```
### ONNX->Engine 脚本
```使用TensorRT官方容器提供的trtexec引擎构建工具（__在执行推理的GPU上进行构建__）
$ //启动镜像，注意容器各版本号对齐
$ docker run --gpus '"device=0"' -it --rm -v /work:/work nvcr.io/nvidia/tensorrt:22.04-py3

$ //静态batchsize的engine生成,以fp16精度。
$ trtexec   --onnx=/work/wuzihao/ibotModelDeploy/modelTrans/result/airway/airway.onnx \
            --saveEngine=/work/wuzihao/triton/model_repository/airway_trt/1/model_trt16.trt \
            --explicitBatch  \
            --fp16 


$ //静态batchsize的engine生成,以fp32精度。
$ trtexec   --onnx=/work/wuzihao/ibotModelDeploy/modelTrans/result/airway/airway.onnx \
            --saveEngine=/work/wuzihao/ibotModelDeploy/modelTrans/result/airway/model_trt32.trt \
            --explicitBatch  \

```

## 模型测试结果

### Airway 对比测试结果
![var](../s/Airway_test.png)


### Nodule 对比测试结果
![var](../s/Nodule_test.png)
