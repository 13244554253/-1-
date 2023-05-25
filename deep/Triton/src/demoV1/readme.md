# demo-V1
基于docker构建torch模型部署环境Triton。
实例基于已经构建模型仓库并写好基本的配置文件，实例模型为TorchScript模型model.pt

## 环境
CUDA：11.6;  
Docker: 20.10.17;  
OS：20.4.1-Ubuntu;  
GPU：NVIDIA A100-PCIE-40GB;  
CPU：Intel(R) Xeon(R) Gold 6330 CPU @ 2.00GHz;  
Repository: nvcr.io/nvidia/tritonserver-22.04-py3; 

## 指令
* triton服务启动，配置端口映射，并指定模型仓库路径:  
```
cd /full/path/triton/  
```
CPU版:  
```
docker run --rm -p8000:8000 -p8001:8001 -p8002:8002 -v /full/path/triton/model_repository:/models nvcr.io/nvidia/tritonserver:22.04-py3 tritonserver --model-repository=/models --strict-model-config=false  
```
GPU版：  
```
docker run --gpus '"device=1"' --rm -p8000:8000 -p8001:8001 -p8002:8002 -v /full/path/triton/model_repository:/models nvcr.io/nvidia/tritonserver:22.04-py3 tritonserver --model-repository=/models --strict-model-config=false
```
* 启动服务后，测试是否可连接
```
curl -v localhost:40184/v2/health/ready

//以下输出连接成功
> GET /v2/health/ready HTTP/1.1
> Host: localhost:40184
> User-Agent: curl/7.68.0
> Accept: */*
> 
* Received HTTP/0.9 when not allowed

* Closing connection 0
curl: (1) Received HTTP/0.9 when not allowed
```

* 启动客户端，发起推理请求  
```
grpc版：  
python client_grpc.py  
http版：  
python client_http.py  
```
* 测试结果  
Input: np.random.random([1, 1, 128, 160, 112]).astype(np.float32)  
CPU版   
grpc infer: 19.465958833694458, 12.187530040740967,  17.18602156639099  
http infer: 7.644216299057007, 12.498617887496948, 11.113388538360596   
GPU版   
grpc infer: 13.133647918701172, 14.847572088241577, 12.151828050613403  
http infer: 6.692427396774292, 6.625612735748291, 6.751987934112549   
* 结果分析  
  基于tritonclient构建的简版triton client端，生成模型的输入数据，对比了基于不同环境部署下的单个模型实例推理服务延迟时间，同时对比了基于不同数据传输协议下的服务延迟时间。总体而言在GPU环境下部署的服务延时会更低，传输协议的比对结果是http对于当前实例数据的请求响应更快，但这不是绝对的。
  