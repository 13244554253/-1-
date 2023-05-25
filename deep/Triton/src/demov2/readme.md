# demo-V2
该实例主要是一个猫狗分类模型在Triton Inference Server上的部署与client端的应用

实例基于已经构建模型仓库并写好基本的配置文件，实例模型为TorchScript模型model.pt

该模型主要用于猫狗分类,是一个二分类模型

## 环境
```shell
CUDA：11.6;  
Docker: 20.10.17;  
OS：20.4.1-Ubuntu;  
GPU：NVIDIA A100-PCIE-40GB;  
CPU：Intel(R) Xeon(R) Gold 6330 CPU @ 2.00GHz;  
Repository: nvcr.io/nvidia/tritonserver-22.04-py3; 
```

## 模型部署
* 模型的放置路径为：  
```
/work/liuhang/triton/model_repository
```
- 使用Triton部署配置好的模型

CPU:  

```shell
docker run --rm -p8000:8000 -p8001:8001 -p8002:8002 -v /work/liuhang/triton/model_repository:/models nvcr.io/nvidia/tritonserver:22.04-py3 tritonserver --model-repository=/models --strict-model-config=false  

#非docker
/opt/tritonserver/bin/tritonserver --model-repository=/work/liuhang/triton/model_repository/ --strict-model-config=false 
```

GPU:  

```shell
docker run --gpus '"device=1"' --rm -p8000:8000 -p8001:8001 -p8002:8002 -v /work/liuhang/triton/model_repository:/models nvcr.io/nvidia/tritonserver:22.04-py3 tritonserver --model-repository=/models --strict-model-config=false

#非docker
/opt/tritonserver/bin/tritonserver --model-repository=/work/liuhang/triton/model_repository/ --strict-model-config=false 
```

## 模型验证

在本地验证

```shell
curl -v localhost:8000/v2/health/ready
```

在另一台机器上验证

```shell
curl -v \<Server IP>:8000/v2/health/ready

# curl -v \10.71.10.49:40183/v2/health/ready  49服务器执行该命令
```

模型部署成功会返回如下结果：

```shell
* Trying 10.71.10.49:8000...
* TCP_NODELAY set
* Connected to 10.71.10.49 (10.71.10.49) port 8000 (#0)
> GET /v2/health/ready HTTP/1.1
> Host: 10.71.10.49:8000
> User-Agent: curl/7.68.0
> Accept: */*
>
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Length: 0
< Content-Type: text/plain
<
* Connection #0 to host 10.71.10.49 left intact
```

## 模型测试

- 启动

```shell
# http
python request_http.py
# grpc
python request_grpc.py
```

在启动命令后，模型会根据你的输入进行推理，返回cat或者dog

 