 
# -*- coding: utf-8 -*-

'''''
    使用kafka-Python 1.3.3模块
    # pip install kafka==1.3.5
    # pip install kafka-python==1.3.5
'''

import sys
import time

from kafka import KafkaProducer
from kafka import KafkaConsumer
from kafka.errors import KafkaError


class Kafka_consumer():
    '''''
    消费模块: 通过不同groupid消费topic里面的消息
    '''
    def __init__(self, group_id='main', kafkahost="127.0.0.1", kafkaport=9092, kafkatopic="tick"):
        self.consumer = KafkaConsumer(kafkatopic, group_id = group_id,
                    bootstrap_servers = '{kafka_host}:{kafka_port}'.format(
                    kafka_host=kafkahost,
                    kafka_port=kafkaport, 
                    api_version = (0, 10))
                )
    def consume_data(self):
        try:
            for message in self.consumer:
                yield message
        except KeyboardInterrupt as e:
            print (e)


class Kafka_producer():
    '''
    消费模块: 向topic里面生产消息
    '''
    def __init__(self, kafkahost="127.0.0.1", kafkaport=9092, kafkatopic="signal"):
        self.kafkaHost = kafkahost
        self.kafkaPort = kafkaport
        self.kafkatopic = kafkatopic
        self.producer = KafkaProducer(
                    bootstrap_servers = '{kafka_host}:{kafka_port}'.format(
                    kafka_host=self.kafkaHost,
                    kafka_port=self.kafkaPort),
                    api_version = (0, 10)
                )

    def produce_data(self, msg):
        try:
            self.producer.send(self.kafkatopic, bytes(str(msg), "ascii"))
        except KeyboardInterrupt as e:
            print (e)



def test_consumer(group='consumer'):
    '''''
    测试consumer
    '''
    consumer = Kafka_consumer(group)
    print ("===========> consumer:", consumer)
    message = consumer.consume_data()
    bar = []
    print("message type", type(message))
    for msg in message:
        print(type(msg.value))
        s = str(msg.value)
        print ('msg -> ', s)
        bar.append(float(s.split(',')[1]))
        if len(bar) == 30:
            print("生成k线") 
            print(bar)
            print(max(bar))
            print(min(bar))
            bar = []
        # print ('offset---------------->', msg.offset)

def test_producer():
    producer = Kafka_producer()
    for i in range(10):
        producer.produce_data(bytes(str(i), "ascii"))
    consumer = Kafka_consumer('vis')
    message = consumer.consume_data()
    for msg in message:
        print('msg ->', str(msg.value))

if __name__ == '__main__':
    test_producer()