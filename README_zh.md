# plmidi

## 介绍
`[physicsLab](https://github.com/GoodenoughPhysicsLab/plmidi)`的`music`中的二进制拓展实现部分  
之所以单独将`plmidi`分离出来，是因为`physicsLab`比`plmidi`更通用
而physicsLab的midi拓展是一个可有可无的功能（虽然我怀着极大的热情写了相关代码），因此我将本应是physicsLab的一部分的库分离出来

## 下载
```shell
pip install plmidi
```

# 自行构建
首先要做一些准备工作`(windows)`
```shell
py -m pip install build
```
请下载`Visual Studio`或者确保你拥有`MSVC`  
这个工具非常智能，会去自动寻找你的编译器  
虽然其他编译器可能也行，不过我没成功  

然后就是构建了  
```shell
py plmidi_setup.py build
```

这个工具也会自动去调用`Python.h`，因此不需用你自己操心去配路径  

<del>虽然这个工具看上去已经做了很多非常智能的操作了，不过你如果是第一次尝试的会应该依然会耗费很多时间</del>  

当然，我也写了批处理文件，在`plmidi`根目录下输入
```
.\make.bat
```
