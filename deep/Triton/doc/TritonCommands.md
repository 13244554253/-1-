# Triton Inference server 常用命令

1.开启Triton 的命令

Start Triton container

创建容器

```shell
docker run --gpus all     #docker中使用哪些gpu   "device=0"
		   -it            #docker开启之后可以在container里进行一些交互
		   --rm           #当container的任务执行完之后自动关掉container
		   --shm-size=1g  #指定了container可以访问的共享内存的大小 
		   -p8000:8000 -p8001:8001 -p8002:8002  #指定需要监听的端口  -P8000指的是host端口  后面的8000是指映射到container中的端口
		                                        #8000用于http的访问
		                                        #8001用于grpc的访问
		                                        #8002用于metrics的访问
		   -v<host_model_repo>:<container_model_repo> #目录的映射
           nvcr.io/nvidatritonserver:21.07-py3  #triton docker的名称
```

Run Triton server

```shell
tritonserver --model-repository=仓库路径
```

tritonserver 常用参数

```shell
--log-verbose <integer>
	#作用 开启或者不开启Verbose的信息，0为不开启，大于0就是开启。
--strict-model-config <boolean>
	#作用 设置为true时 onnx、tensorrt、tensorflow模型就可以不编写配置文件，由triton自动生成。 
--strict-readiness <boolean>
    #作用 设置为true时 只有所有的模型都部署成功了,在检查模型状态时才会返回ready
    #作用 设置为false时 只要有一个模型部署成功了，在检查模型状态时就会返回ready
--exit-on-error <boolean>
	#作用 设置为false时 当你的模型有一部分加载失败了，也还是会把tritonserver启动起来
--http-port <integer>
	#修改端口（默认8000）
--grpc-port <integer>
	#修改端口（默认8001）
--metrics-port <integer>
	#修改端口（默认8002）
--model-control-mode <string>
	# 作用 决定用什么样的方式管理模型库
	#  none 默认模式，在server开启的时候把模型库中的所有模型都加载进来，并且不能对已经开始提供服务的模型进行动态的更新和卸载
	#  poll 动态的更新模型（服务已经启动，这时在仓库中添加新的模型就会被加载进来，如果修改了模型配置参数，那么参数也会被重新加载） 
	#  explicit 在server启动初期不加载任何模型，然后通过model_control api在客户端告诉sever要加载哪个模型和卸载哪个模型
--repository-poll-secs <integer>
	# 作用 定义了trionserver检查模型仓库的频率时多少  仅仅在--model-control-mode poll才生效
--load-model <string>
	# 作用 想让服务在初期加载某个模型，就用这个参数    仅仅在--model-control-mode explicit才生效
--pinned-memory-pool-byte-size <integer>
	# 作用 指定了tritonserver能够分配的所有pinned-memory cpu资源的大小 默认256M
	# pinned-memory 可以在模型推理的时候可以有效的提升GPU和cpu数据传输的效率
--cuda-memory-pool-byte-size \<\<integer> : <integer>>
	# 作用 指定了tritonserver能够分配的所有CUDA-memory 的大小 默认64M
--backend-directory <string>
	# 默认backend的路径为 /opt/tritonserver/backends
	# 作用 告诉tritonserver去哪里找你自己实现的backend动态库
--repoagent-directory <string>
	# 默认repoagent的路径为 /opt/tritonserver/repoagents
	# repoagent是用来预处理模型库的一个程序(比如在模型库加载之前做加密，就可以做一个加密的repoagent然后指定其路径)
	# 作用 告诉tritonserver去哪里找你自己实现的backend动态库
```

2.启动之后

检查某个server的健康状况

```shell
curl -v \<Server IP>:8000/v2/health/ready
curl -v \localhost:8000/v2/health/ready  #在同一台机器上查看
```

