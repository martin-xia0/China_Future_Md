import pymysql
import os
import sys

def build_table(cursor):
    try:
        build_sql = "CREATE TABLE rb_tick " \
                    "(id INT PRIMARY KEY AUTO_INCREMENT, id_intraday INT, price FLOAT(5,2), volume INT, " \
                    "time CHAR(20), time_local CHAR(20)) AUTO_INCREMENT=1;"
        print("Execute {}".format(build_sql))
        cursor.execute(build_sql)
    except:
        print("error: {}", sys.exc_info()[0])


def insert_tick(cursor, tick_info):
    try:
        insert_sql = "INSERT INTO rb_tick " \
                     "(id_intraday, price, volume, time)"
                     "VALUES " \
                     "{};".format(tick_info)

        print("Execute {}".format(insert_sql))
        cursor.execute(insert_sql)
    except:
        print("error: {}", sys.exc_info()[0])


if __name__ == '__main__':
    conn = pymysql.Connect(
        host='localhost',
        port=3306,
        user='root',
        passwd='abcd.1234',
        db='future_ctp',
        charset='utf8'
    )
    cursor = conn.cursor()
    build_table(cursor)
    tick_info = "1, 3989.00, 221983, '23:00:00:500'"
    insert_tick(cursor, tick_info)