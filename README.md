# AutoBatch 自动批处理模板辅助工具

## 项目简介

AutoBatch是一个自动批处理工具，可以自动对文件或文件夹按照指定的命令行程序和参数模板进行批处理。

## 编译方法

- windows下使用gcc编译，需要自行安装配置gcc环境
- 配置好gcc环境后，下载源码，进入源码目录，双击build.bat即可编译

## 使用方法

- 初次使用时，可以双击AutoBatch.exe，会自动生成autobatch.txt模板名称和default.ini模板文件
- autobatch.txt记载了模板名称，可手动更改，用以切换不同的程序和参数模板
- default.ini是程序和参数模板文件，可以用于配置程序和参数模板，改名后需要在autobatch.txt中修改模板名称
- default.ini文件格式如下：
  - exe填写要执行的程序的路径，必填项
  - args填写要执行的程序的参数，可选项
  - exclude填写要排除的后缀名，多个后缀名用逗号分隔，可选项
  - target填写目标生成文件的后缀名，可选项
  - command_format填写生成命令的格式，必填项
  - command_format有三个参数，分别是args(可选),input(必须),output(可选)，可任意排列顺序以适配命令行语句，用逗号分隔
    - 例如：command_format=input,args,output
- 将需要批处理的文件，拖放到AutoBatch.exe上，即可使用选中的模板进行批处理，文件夹支持递归处理

## 注意事项

- 路径不需要用引号扩起，不要有空格
- 参数中可以有空格
- 配置ini文件不要有空格，文件名不要使用中文
