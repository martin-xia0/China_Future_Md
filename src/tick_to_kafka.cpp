#include <iostream>
#include <string.h>
#include "tick_to_kafka.h"
#include "../cppkafka/cppkafka.h"

using namespace std;
using cppkafka::Producer;
using cppkafka::MessageBuilder;
using cppkafka::Configuration;


// 启动kafka生产者
Kafka::Kafka(void)
{
    string brokers = "127.0.0.1:9092";
    // 参数配置
    Configuration config = {{ "metadata.broker.list", brokers }};
    // 初始化消息生产指针
    this->p_producer = new Producer(config);
}

// 向消息队列写入tick消息
void Kafka::WriteTick(char* Message, string& topic)
{
    string msg = Message;
    // 载入信息
    this->p_producer->produce(MessageBuilder(topic).payload(msg));
    cout << "》》》完成消息生产《《《" << endl;
    cout << msg << endl;
    // 刷新
    // this->p_producer->flush();
    // 写入时间
    time_t now = time(0);
    char* dt = ctime(&now);
    cout << "当前时间" << dt << endl;

}

// 析构函数
Kafka::~Kafka(void)
{
    cout << "》》》关闭消息队列《《《" << endl;
    delete p_producer;
    delete p_builder;
}
