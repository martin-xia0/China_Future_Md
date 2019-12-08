##数据模块 文档
[TOC]
### 功能

1. 数据接口：从上期所ctp接口获得tick数据

2. 消息广播：将实时数据置入kafka消息队列

3. 预处理：k线合成

4. 数据库：实时行情存数据库

5. python工具包：供策略调用

  

### 设计思想

1. 稳定：全局推流，内部订阅

2. 低延时：代码层面优化

3. 可扩展：支持多策略订阅

4. 易用：内部封装，外部提供python接口

     

### Python接口

#### 使用方法

1. 导入策略基类

2. 继承并改写虚函数

   ```python
   class MaStrategy(BaseStrategy):
       def __init__(self, trader_id, frequency):
           super().__init__(trader_id, frequency)
       	  ...  此处定义特殊成员
       	
       def on_init(self):
           ... 策略初始化时调用
   
       def on_start(self):
           ... 策略启动时调用
   
       def on_stop(self):
           ... 策略被keyboard interrupt时调用
   
       def on_tick(self, tick):
           ... 推tick时调用
   
       def on_bar(self, bar):
           ... 推bar时调用
   ```

3. 启动策略

   ```python
   s = MaStrategy(trader_id=1, frequency="tick")
   s.subscribe(['IF2001'])
   s.start()
   ```

   

#### 数据结构

**Tick**

| 参数       | 标识符       | 意义             |
| ---------- | ------------ | ---------------- |
| 标的符号   | symbol       | 期货中指合约代码 |
| 交易日     | trading_day  | 交易日           |
| 交易所时间 | time         | 时：分：秒       |
| 交易所毫秒 | millisec     | 0或500的毫秒后缀 |
| 日内总量   | vol_intraday | 当日交易总量     |
| 最新价     | last_price   | 最新价格         |
| 卖一价     | bid_price_1  | 订单簿卖一价     |
| 买一价     | ask_price_1  | 订单簿买一价     |
| 卖一量     | bid_volume_1 | 订单簿卖一量     |
| 买一量     | ask_volume_1 | 订单簿买一量     |

**Bar**

分钟线，1分钟推送一次内部合成的k线

| 参数       | 标识符       | 意义              |
| ---------- | ------------ | ----------------- |
| 标的符号   | symbol       | 期货中指合约代码  |
| 交易日     | trading_day  | 交易日            |
| 交易所时间 | time         | 时：分：秒        |
| 分钟内总量 | volume       | bar内包含的交易量 |
| 日内总量   | vol_intraday | 当日交易总量      |
| 起始价     | open_price   | k线起始价格       |
| 最高价     | high_price   | k线最高价格       |
| 最低价     | low_price    | k线最低价格       |
| 结束价     | close_price  | k线结束价格       |



### 系统运维

#### 预装环境

为kafka创建一个tmux专属管理会话

**zookeeper**

kafka依赖

https://stackoverflow.com/questions/23751708/is-zookeeper-a-must-for-kafka

**kafka**

https://tecadmin.net/install-apache-kafka-ubuntu/

**java 1.8**

mysql（若需同步数据存储则必须安装）

kafka manager（可选）

**开通端口**

需开通端口映射，使zookeeper与kafka正常运行

2181：zookeeper

9092：kafka

**miniconda**

python环境，不是必须的，但安装后能更好管理python相关库

#### Python库

**kafka-python**

https://kafka-python.readthedocs.io/en/master/usage.html

pykafka是kafka的python封装，需要用pip安装。

**pymysql**

pymysql是mysql的python封装，需要用pip安装。

#### C++库

**librdkafka**

librdkafka是基于kafka的封装构件，需要源码在本机安装。

https://github.com/edenhill/librdkafka

不可以用指令安装，版本不够cppkafka要求

**cppkafka**

cppkafka是kafka的C++封装库，需通过源码在本机编译。

```
>> git clone "https://github.com/mfontanini/cppkafka.git"
>> unzip cppkafka.git
>> cd cppkafka
>> mkdir build
>> cd build
>> cmake ..
>> make
此时动态库文件在build 目录下的./src/lib64/libcppkafka
```

**msgpack**

msgpack是高性能二进制传输格式，类似于json但更快。使用msgpack编码为二进制在kafka中传送。

```
>> git clone https://github.com/msgpack/msgpack-c.git
>> cd msgpack-c
>> cmake -DMSGPACK_CXX11=ON .
>> sudo make install
```

**上期所接口**

上期所接口建立在TCP协议上，简称为Thost。Api接口分行情接口和交易接口，接口层只使用行情接口，即只对接广播通讯模式。初始化完成之后，对应Spi的OnFrontConnected函数将会被调用。

本系统使用上期所最新v6.3.15_P2版本接口

动态链接库：libthostmduserapi.so

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
```

####程序编译

主程序分为行情模块

以行情模块为例：

```
>> cd /home/ctp			进入CTP根目录
>> cd md_ctp			进入行情根目录
#--------------------------编译---------------------------#
>> mkdir build
>> cd build
>> cmake ..
在本目录下生成makfile等文件
>> make
完成编译
>> ./md_cpt
运行行情模块
>> 
可以选择添加软链接，成为服务
#--------------------------检验---------------------------#
新建会话窗口
>> /opt/kafka/bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic tick --from-beginning
查看tick数据流
```

交易模块同理，根目录为trader_ctp

####部署流程

```
1.预装软件：jdk、zookeeper、kafka、cmake
2.预装C++库：librdkafka、boost、cppkafka
3.上传主程序：ctp
4.编译程序
5.启动软件：zookeeper、kafka
6.启动程序
```



### 运行

#### 查看消息队列

```
>> cd /opt/kafka												进入kafka所在目录
>> bin/kafka-topics.sh --list --zookeeper localhost:2181	  	查看topic列表
(若找不到JAVA的情况，在/etc目录下执行source profile，刷新一下环境变量，/etc/profile 存环境变量配置，服务器重启后会失效，source执行生效，注意如果java升级了需要在profile里面更改路径，否则会找不到file）

>> bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test_main --from-beginning													                      
查看topic数据流

>> bin/kafka-configs.sh --zookeeper localhost:2181 --entity-type topics --entity-name $topic$ --alter --add-config retention.ms=86400000                     
修改topic的保存时间
```

#### 消息队列topic

| topic          | 意义       | 数据格式        |
| -------------- | ---------- | --------------- |
| test           | 测试使用   |                 |
| <symbol>       | tick队列   | msgpack二进制流 |
| <symbol>+"min" | 分钟线队列 | msgpack二进制流 |


####数据消费接入

不同的应用，应对应不同的消费组(group)(此处专指策略ID，策略订阅时专属消费)，所使用的消费组ID不同。



###附录

#### 上期所文档注释

1. 最新版本的文档是6.3.15，本系统按此文档编写。文档分为行情接口、交易接口、穿透式监管数据采集，其中有实际意义的是行情接口与交易接口。 目前大部分期货公司及simnow已经更换为19年2月的穿透式版本，部分期货公司测试环境仍然是老版本。穿透式版本在用户端没有区别，在登陆的同时进行一次注册即可

2. 行情与交易接口都分为API与SPI，API由我方调起，SPI由上期所调起。每个SPI创建时都会新建一个线程，这就实现双向通信，SPI就在这个线程内被调用。API在调用后，返回的数据会通过调用对应的SPI回传。例如交易API，ReqQueryAccountMoneyByFuture用于发起查询银行余额请求，对应的响应是OnRtnBankBalanceByFuture，响应 当前账户信息。

3. 行情接口重要API：初始化、登录、订阅行情

   重要SPI：深度行情推送

4. 交易接口重要API：登录、核对、查询资金、下单、撤单

   重要SPI：账户信息核对、下单信息返回

5. 行情与交易由两个单独的进程实现，行情进程是行情获取主线程，交易进程是交易员API的工作线程。由于交易进程只是监听并执行指令，建议使用时先启动交易进程，再启动行情进程。

6. 上期所ctp的API有频率限制的要求，限制每秒只能调用一次，因此1s内无法连续下单，否则会被拒绝。因此本系统将所有命令都从消息队列中读取，按1s的频率执行。如此以来，外部策略无需考虑订单撞车的情况，订单和请求都会被排队执行。

7. 关于报单

   systemID是报单编号，用于唯一识别当天的报单

   orderStatus是报单的状态编号

   

#### 配置问题

1. 配置kafka时，报错no broker

   添加一个api_version就好了

2. 无法启动zookeeper或kafka

   检查9092和2181端口是否开防火墙

3. Java报错

   查看$JAVA_HOME环境变量，如不正确修改/etc/profile，并执行source /etc/profile

####低延时

1. 本系统以提升速度为前提，做了不重复初始化对象、用指针对象运算等优化。k线用容器函数，出入栈效率高，便于采样




#### 一些细节

1. 消息队列数据均通过结构体实现

   所有数据结构定义data_type.h中，使用msgpack进行编解码


1. SPI的头文件、程序文件有对应关系

   为防止程序错误，SPI回调函数在头文件与程序文件中的实现是顺序是一致的，后续开发请按顺序向后添加

   例如：md_spi.cpp与md_spi.h，回调函数顺序依次是连接成功应答、断开连接通知、心跳超时警告...

  
