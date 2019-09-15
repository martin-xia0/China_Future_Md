##接口层（interface_layer）文档

### 功能

1. 数据接口：从上期所ctp接口获得tick数据
2. 消息队列：将tick数据置入kafka消息队列
3. 数据存储：将tick数据存入mysql数据库

### 预装环境

#### 预装软件

为kafka创建一个tmux专属管理会话

##### zookeeper

##### kafka

https://www.vultr.com/docs/how-to-install-apache-kafka-on-centos-7

https://www.digitalocean.com/community/tutorials/how-to-install-apache-kafka-on-centos-7

##### java 1.8

mysql（若需同步数据存储则必须安装）

kafka manager（可选）

安装流程

https://medium.com/@FrissonAI/yahoos-kafka-manager-installation-82fdc4bd1222

https://github.com/yahoo/kafka-manager

##### 开通端口

需开通端口映射，使zookeeper与kafka正常运行

2181：zookeeper

9092：kafka

9000(不必须)：kafka manager

3306(不必须)：mysql

#### Python库

##### kafka-python

https://kafka-python.readthedocs.io/en/master/usage.html

pykafka是kafka的python封装，需要用pip安装。

**pymysql**

pymysql是mysql的python封装，需要用pip安装。

#### C++库

##### librdkafka

librdkafka是基于kafka的封装构件，需要源码在本机安装。

https://github.com/edenhill/librdkafka

##### cppkafka

cppkafka是kafka的C++封装库，需通过源码在本机编译，然后将头文件和库文件导入主程序。

```
>> git clone "https://github.com/mfontanini/cppkafka.git"
>> unzip cppkafka.git
>> cd cppkafka
>> mkdir build
>> cd build
>> cmake ..
编译
>> make
获得cppkafka.so
```

动态链接库：libcppkafka.so

##### 上期所接口

上期所接口建立在TCP协议上，简称为Thost。Api接口分行情接口和交易接口，接口层只使用行情接口，即只对接广播通讯模式。初始化完成之后，对应Spi的OnFrontConnected函数将会被调用。

本系统使用上期所最新v6.3.15_P2版本接口

动态链接库：libthostmduserapi.so

### 启动

#### 启动软件

```
>> tmux a -t kafka													进入tmux
# ------------------------启动zookeeper--------------------#
>> cd /opt/zookeeper 												进入zookeeper目录
>> ./bin/zkServer.sh start											启动zookeeper
>> ./bin/zkServer.sh status											查看状态
显示Mode：standalone 启动完成
# ------------------------启动kafka------------------------#
>> cd /opt/kafka									 				进入kafka所在目录
>> bin/kafka-server-start.sh config/server.properties				启动kafka
已启动kafka
>> ctrl+b + n					进入本回话的下一窗口
# ------------------启动kafka-manager(不必须)---------------#
>> cd /opt/kafka-manager											进入manager所在目录
>> ./bin/kafka-manager -Dkafka-manager.zkhosts="localhost:2181"  	启动manager
使用浏览器访问ip:9000可以进入管理界面
>> tmux+b + d
>> netstat -npl|grep -E "9000|9092|2181"							查看端口状态
三个端口都有监听
```

####主程序编译

主程序在 /home/interface_layermain.cpp 中

动态链接库在 /usr/local/lib 中

```
>> /home/interface_layer	进入接口层目录
#--------------------------编译---------------------------#
>> mkdir build
>> cd build
>> cmake ..
在本目录下生成makfile等文件
>> make
完成编译
>> ./ctp
执行
```

####部署流程

```
1.预装软件：java 1.8、zookeeper、kafka
2.预装python库：pykafka
3.预装C++库：librdkafka、cppkafka
4.上传主程序：interface_layer
5.编译主程序
6.启动软件
7.启动主程序
```

### 查看消息队列

#### 查看数据流

```
>> cd /opt/kafka												进入kafka所在目录
>> bin/kafka-topics.sh --list --zookeeper localhost:2181	  	查看topic列表
(若出现找不到JAVA的情况，执行source profile，刷新一下环境变量)
>> bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test_main --from-beginning													                            查看topic数据流
>> bin/kafka-configs.sh --zookeeper localhost:2181 --entity-type topics --entity-name $topic$ --alter --add-config retention.ms=86400000                               修改topic的保存时间
```

#### 消息队列topic

| topic | 意义         | 数据格式          |
| ----- | ------------ | ----------------- |
| test  | 测试使用     |                   |
| tick  | 半秒tick k线 | 字符串，以“,”分隔 |

####Tick消息格式

“日内序号,最新价,成交量,交易所时间”

| 参数       | 标识符      | 意义         |
| ---------- | ----------- | ------------ |
| 全局序号   | id          | tick全局序号 |
| 日内序号   | id_intraday | tick日内序号 |
| 最新价     | price       | 最新价格     |
| 成交量     | volume      | 成交量       |
| 交易所时间 | time        | 交易所时间   |

#### K线格式

“日内序号,起始价,结束价,最高价,最低价,成交量,采样时间”

| 参数     | 标识符      | 意义                  |
| -------- | ----------- | --------------------- |
| 日内序号 | id_intraday | k线日内序号           |
| 起始价   | start       | k线起始价格           |
| 结束价   | close       | k线结束价格           |
| 最高价   | high        | k线最高价格           |
| 最低价   | low         | k线最低价格           |
| 成交量   | volume      | k线成交量             |
| 采样时间 | time        | k线时间，以分钟为单位 |

###数据消费接入

不同的应用，应对应不同的消费组(group)，所使用的消费组ID不同。



### 数据库

数据库使用mysql搭建，存储于future_ctp库中

##### Tick表 rb_tick

| 参数       | 标识符      | 意义         |
| ---------- | ----------- | ------------ |
| 全局序号   | id          | tick全局序号 |
| 日内序号   | id_intraday | tick日内序号 |
| 最新价     | price       | 最新价格     |
| 成交量     | volume      | 成交量       |
| 交易所时间 | time        | 交易所时间   |

### 代码说明

#####main.cpp

初始化数据接口

#####md_spi.cpp

定义回调函数，接受ctp数据流，对tick数据采样生成bar数据

注：行情接口无需用户名密码，只需初始化

##### data_engine.py

将tick数据存储到mysql数据库



####常见问题

1. 配置kafka时，报错no broker

   添加一个api_version就好了

2. 无法启动zookeeper或kafka

   检查9092和2181端口是否开防火墙

3. Java报错

   查看$JAVA_HOME环境变量，如不正确修改/etc/profile，并执行source /etc/profile



####备注

1. 本系统以提升速度为前提，做了不重复初始化对象、用指针对象运算等优化。k线用容器函数，出入栈效率高，便于采样

2. 数据库使用mysql储存

3. 

   