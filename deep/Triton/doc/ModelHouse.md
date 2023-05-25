# 模型仓库构建
存储模型的位置，在服务器启动时指定，服务器运行时，可以按照模型管理中的描述修改正在服务的模型。

triton启动时，存储库路径通过--model-repository 选项指定，可以多次指定以包含多个存储库。
> $ tritonserver --model-repository=\<model-repository-path>

## 存储库结构布局
```
  <model-repository-path>/  # 模型仓库根目录
    <model-name>/           # 模型name
      [config.pbtxt]        # 模型配置文件
      [<output-labels-file> ...]  # 输出值的标签文件
      <version>/            # 版本号文件夹
        <model-definition-file>  # 模型定义文件
      <version>/
        <model-definition-file>
      ...
    <model-name>/
      [config.pbtxt]
      [<output-labels-file> ...]
      <version>/
        <model-definition-file>
      <version>/
        <model-definition-file>
      ...
    ...
```
* 每个子目录至少有一个代表模型版本的数字子目录
* 不同的模型有特定的后端执行，需要提供特定于框架的模型文件
模型参数配置文档在[这里](./%E6%A8%A1%E5%9E%8B%E4%BB%93%E5%BA%93%E9%85%8D%E7%BD%AE%E6%96%87%E6%A1%A3.md)

## 存储位置
* 本地文件系统
指定绝对路径
> $ tritonserver --model-repository=/path/to/model/repository ...
* 云存储  
> $ tritonserver --model-repository=gs://bucket/path/to/model/repository ...

## 模型管理

### 版本
注：不以数字命名的版本子目录会被忽略。

### 子目录模型文件
取决于模型类型和支持的后端决定
* TensorRT 模型
.plan 文件，默认命名为model.plan.Tensorrt模型要将plan模型与当前硬件的计算能力关联，需要在模型配置文件设置cc_model_filenames属性。
模型仓库最小构成
```
<model-repository-path>/
    <model-name>/
      config.pbtxt
      1/
        model.plan
```
* TorchScript 模型
模型仓库最小构成
```
<model-repository-path>/
    <model-name>/
      config.pbtxt
      1/
        model.pt
```
* python 模型
Python 后端允许您在 Triton 中将 Python 代码作为模型运行。默认情况下，Python 脚本必须命名为 model.py.
```
<model-repository-path>/
    <model-name>/
      config.pbtxt
      1/
        model.py
```
