# Qt Ftp

#### 介绍
基于qt5.12 QSocket自己实现的Ftp功能，模仿了Qt4中的QFtp 部分功能
可以实现所有的ftp命令功能
多线程下载ftp文件，提供实时下载传输速度，时间，大小

更多信息，请关注我的博客：
https://blog.csdn.net/CLinuxF?spm=1001.2101.3001.5113

#### 软件架构
软件架构说明
FtpMultithreadDownLoad Qt多线程ftp下载
FTPSocket ftp实现的核心程序

#### 安装教程

1.  xxxx
2.  xxxx
3.  xxxx

#### 使用说明

1.  如果只是有下载需求只需要使用FtpMultithreadDownLoad 
2.  其它需求可以自己继承FTPSocket来动态实现或者直接使用FTPSocket的功能
3.  继承FtpMultithreadDownLoad ，只需要调用ftpStartDown方法即可，然后关注对应的信号就行
4.  下载实时提供当前下载速度，剩余时间 

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 码云特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  码云官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解码云上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是码云最有价值开源项目，是码云综合评定出的优秀开源项目
5.  码云官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  码云封面人物是一档用来展示码云会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
