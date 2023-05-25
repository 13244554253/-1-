## 加快渲染速度的几种方法

##### 1.使用VtkImageResample, 通过线性插值的方式对数据进行重采样,来修改输出数据的Spacing以及extent.

```c++
方法：SetAxisMagnificationFactor进行设置。

SetInput(reader.GetOutput());

SetAxisMagnificationFactor(0,  0.5);

SetAxisMagnificationFactor(1,  0.5);

SetAxisMagnificationFactor(2,  0.5);//重新设置x,y,z方向上的space。

//适度放宽space，降低图形的显示质量，可以读取稍大的数据
```


##### 2. 对读入的数据采用vtkImageShrink3D进行subsampling(二次抽样)，可以使加速读入数据。

```c++
方法：SetShrinkFactors（3，3，3）和AveragingOn。SetShrinkFactors方法,定义了x,y,z 抽样精密度.
如果都是0，没必要用这个函数。如果subsampling得值过大，那么rendering的结果较差。可以使用一些平滑
函数进行优化，减少数据的梯度感和粗糙感。如高斯平滑函数：
　　
SetInputConnection(reader->GetOutput());
SetDimensionality（3）
SetRadiusFactors（1，1，0）

```

##### 4.vtkLODProp3D可以加入多个Mapper，通过设置time，决定某一时刻显示哪一个Mapper。
```c++
方法：当在旋转时选择交互性好但准确率稍差的Mapper，当停止时，又会显示比较费时但绘制准确的Mapper。
对于RayCastMapper来说，则不需要使用vtkLODProp3D。如下所示：
　　
vtkVolumeTextureMapper2D *lowresMapper = vtkVolumeTextureMapper2D::New();
lowresMapper->SetInput(reader->GetOutput());

vtkVolumeRayCaster *hiresMapper = vtkVolumeRayCaster::New();
hiresMapper->SetInput(reader->GetOutput());

vtkLODProp3D *volumeLOD = vtkLODProp3D::New();
volumeLOD->AddLOD(lowresMapper, volumeProperty, 0.0);
volumeLOD->AddLOD(hiresMapper, volumeProperty, 0.0);

```


##### 5.vtkGPUVolumeRayCastMapper.h 

- vtkGPUVolumeRayCastMapper：实现了基于GPU加速的光线投射体绘制算法