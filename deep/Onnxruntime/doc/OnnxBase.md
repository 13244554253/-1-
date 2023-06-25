# ONNX 基础

### 1.什么是ONNX

- ONNX是微软和Facebook提出用来表示深度学习模型的开放格式。ONNX定义了一组和环境，平台均无关的标准格式，来增强各种AI模型的可交互性。

- 无论你使用何种训练框架训练模型（比如TensorFlow/Pytorch/OneFlow/Paddle），在训练完毕后你都可以将这些框架的模型统一转换为ONNX这种统一的格式进行存储。

### 2.ONNX的文件格式

- **ONNX文件是基于Protobuf进行序列化**。

 