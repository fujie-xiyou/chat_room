### HLW聊天室——使用Linux C网络编程技术开发的多人聊天工具
#### 安装部署
- 克隆
``` shell?linenums
git clone https://github.com/fujie-xiyou/chat_room.git
```
- 服务器端
``` shell?linenums
cd chat_room/Server
make
```
然后将`chat_room.sql`导入到你的数据库中，并修改`config.json`中的数据库信息<br>
最后`./chat_room_srv`
没有报错的话服务器就启动成功了
 - 客户端

``` shell?linenums
cd chat_room/Client
make
```
然后在config.json中修改服务器地址<br>
最后`./chat_room_cli`<br>
请先注册一个账号
####  演示视频
[聊天室视频演示](https://www.github.com/fujie-xiyou/Graph-bed/raw/master/小书匠/聊天室.md/聊天室演示.mp4)
####  项目概述
- 本项目是一个使用Linux C网络编程技术开发的字符界面聊天室，基本功能有，用户注册登录，添加好友，私聊，创建群聊，邀请好友加入群聊，群聊，聊天记录查看，文件传送等。
#### 项目设计
- 本项目基于C/S模型设计，分为客户端和服务器端两个部分，客户端直接连接到服务器，与服务器双向交换数据，服务器端用于接收客户端发来的消息，并转发给目标用户，所有数据的持久化工作也均由服务器端完成。
- 项目代码采用分层的思想进行设计，客户端由界面层和业务逻辑层(请求层)构成，服务器端由业务逻辑层和持久化层构成，代码结构如图：<br>
![代码结构](https://www.github.com/fujie-xiyou/Graph-bed/raw/master/小书匠/聊天室.md/代码结构.png)
- 数据库设计
	- 本项目开发时尚未系统学过数据库相关课程，但是后来审视本项目数据库设计，居然是符合BC范式的。
	- 项目数据库部分示例如图所示<br>
![数据库设计](https://www.github.com/fujie-xiyou/Graph-bed/raw/master/小书匠/聊天室.md/聊天室数据库设计.png)
- 文件传输功能设计
由于本项目客户端与服务器端采用JSON进行数据交互，对于文件传输需要做特殊处理，因此在此简单介绍一下文件传输的处理方式，先上图：
![文件传输处理](https://www.github.com/fujie-xiyou/Graph-bed/raw/master/小书匠/聊天室.md/聊天室文件处理过程.png)
- 如图在当用户输入要发送的文件名后，系统使用open()系统调用打开文件，然后使用read()系统调用读取指定字节的文件内容，将read的返回值记录在数据包的size字段，然后将读取到的文件内容使用base64转码，将得到的结果记录在数据包的data字段中，然后将数据包发给接收方，再根据read返回值的大小判断文件是否全部发送完成，如果read返回值小于read的大小参数，则关闭文件，否则继续read转码发送。
- 接收方收到文件包后，将data中的数据解码为二进制，然后根据size的值将存储data的转码结果的buff中的数据追加写入文件中。
