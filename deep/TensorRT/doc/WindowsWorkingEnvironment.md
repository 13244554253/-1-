# windows环境配置文档

## 环境

| 文件     | 版本    |
| -------- | ------- |
| TensorRT | 8.2.4.2 |
| CUDA     | 11.4.0  |
| cuDNN    | 8.2.4   |

- 系统版本： windows10
- 开发工具： Visual Studio 2017

## 安装

### 1.文件下载

TensorRT [下载链接](https://developer.nvidia.com/nvidia-tensorrt-download)

CUDA [下载链接](https://developer.nvidia.com/cuda-toolkit-archive)

cuDNN [下载链接](https://developer.nvidia.com/rdp/cudnn-archive)

[TensorRT CUDA cuDNN 版本对应关系表](./TensorRT_CUDA_cuDNN.md)

### 2.安装

- **CUDA安装**：运行下载的CUDA的exe文件，一直点下一步直至安装结束。

- TensorRT 和 cuDNN直接解压文件即可。


### 3.环境配置

- 打开CUDA的安装路径，默认路径为C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.4
- 将cuDNN和TensorRT中bin，include，和lib目录下的文件都复制到CUDA bin，include，lib目录下

### 4.验证
                                   
#### cuda验证

- 在cmd中执行nvcc --version 成功显示CUDA版本即为安装成功

![var](../s/win1.png)

#### TensorRT验证(成功执行TensorRT中的sampleMNIST)
- 使用Visual Studio 2017 打开 sample_mnist_sln,然后对Vsiual Studio 2017 进行环境配置

![var](../s/win2.png)
![var](../s/win3.png)
![var](../s/win4.png)

- 配置完毕后重新生成该文件

![var](../s/win5.png)

- 最后在cmd中执行sample_mnist.exe --datadir=path\TensorRT-8.2.4.2\data\mnist\，查看是否成功进行识别

![var](../s/win6.png)
