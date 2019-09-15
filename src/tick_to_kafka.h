#ifndef TICKTOKAFKA_H
#define TICKTOKAFKA_H

#include <iostream>
#include <vector>
#include "../cppkafka/producer.h"
#include "../cppkafka/configuration.h"

using namespace std;
using cppkafka::Producer;
using cppkafka::MessageBuilder;
using cppkafka::Configuration;

class Kafka
{
public:
    // 构造函数
    Kafka();
    // 析构函数
    ~Kafka();
    // 向消息队列写入tick消息
    void WriteTick(char* Message, string& topic);
public:
    // 消息生产指针
    Producer *p_producer;
    // 消息队列指针
    MessageBuilder *p_builder;
};

#endif
