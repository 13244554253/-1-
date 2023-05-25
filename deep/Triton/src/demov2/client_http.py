from pkgutil import ImpImporter
from sre_parse import FLAGS
import numpy as np
import tritonclient.http as httpclient
import torch
import time

from PIL import Image

if __name__ == '__main__':
    start = time.time()
    classification_dict = {0:"cat", 1:"dog"}

    triton_client = httpclient.InferenceServerClient(url='10.71.10.49:40183') # 创建clinet对象
   
    #对输入的图像进行预处理
    image = Image.open('./test_cat/cat.1042.jpg')
    image = image.resize((256, 256), Image.ANTIALIAS)
    image = np.asarray(image)
    image = image / 255
    image = np.expand_dims(image, axis=0)
    image = np.transpose(image, axes=[0, 3, 1, 2])
    image = image.astype(np.float32)

    inputs = []
    inputs.append(httpclient.InferInput('INPUT__0', image.shape, "FP32")) #声明进行推理请求的输入张量格式
    inputs[0].set_data_from_numpy(image, binary_data=False)

    outputs = []
    outputs.append(httpclient.InferRequestedOutput('OUTPUT__0', binary_data=False))  #声明进行推理请求的输出张量格式

    results = triton_client.infer('cat_pytorch', inputs=inputs, outputs=outputs)  #选择Triton Inference Server上的指定模型进行推理

    output_data0 = results.as_numpy('OUTPUT__0') #将Triton对象的返回值转化为numpy，用于类别的计算
    
    pre_data = torch.tensor(output_data0) 
    predicted = torch.max(pre_data, 1)[1].data
    
    print( classification_dict[predicted.item()])
    print("Spend time: {}".format(time.time() - start))
