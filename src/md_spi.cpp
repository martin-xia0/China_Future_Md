#include <iostream>
#include <string.h>
#include <ctime>
#include "md_spi.h"

using cppkafka::Producer;
using cppkafka::MessageBuilder;
using cppkafka::Configuration;

using namespace std;

// ---- 全局参数声明 ---- //
// 账户参数
// 经纪商代码
extern TThostFtdcBrokerIDType BrokerId;
// 投资者账户名
extern TThostFtdcInvestorIDType InvesterId;
// 投资者密码
extern TThostFtdcPasswordType InvesterPassword;

// 行情接口
// 行情指针
extern CThostFtdcMdApi *p_MdApi;
// 行情前置地址
extern char MdFrontAddr[];
// 行情合约代码列表
extern char *p_InstrumentId[];
// 行情合约订阅数量
extern int InstrumentNum;

// Kafka对象指针
string brokers = "127.0.0.1:9092";
// 参数配置
Configuration config = {{ "metadata.broker.list", brokers }};
// 初始化消息生产指针
Producer producer(config);


// ---- 消息数据 ---- //
// 传送消息
char Message[100];
// tick日内序号
int TickIntradayID = 1;
// 上一tick的交易总量
int Volume = 0;
// 上一tick的交易日
char TradingDay[9] = "";
// k线内的tick集合
vector<float> bar; 
// kafka 队列主题
string topic;

// ---- spi回调函数 ---- //
// 连接成功应答
// 当客户端与交易托管系统建立起通信连接时（还未登录前），该方法被调用。本方法在完成初始化后调用，
// 可以在其中完成用户登录任务。
void MdSpi::OnFrontConnected()
{
    cout << "》》》网络连接成功《《《" << endl;
    // 登录准备
    CThostFtdcReqUserLoginField loginReq;
    memset(&loginReq, 0, sizeof(loginReq));
    strcpy(loginReq.BrokerID, BrokerId);
    strcpy(loginReq.UserID, InvesterId);
    strcpy(loginReq.Password, InvesterPassword);
    static int requestID = 0; // 请求编号
    // 登录请求
    int rt = p_MdApi->ReqUserLogin(&loginReq, requestID);
    if (!rt)
        cout << "发送登录请求成功" << endl;
    else
        cerr << "发送登录请求失败" << endl;
}

// 断开连接通知
// 当客户端与交易托管系统通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
// 自动重连地址，可能是原来注册的地址，也可能是注册的其它可用的通信地址，它由程序自动选择。
void MdSpi::OnFrontDisconnected(int nReason)
{
    cerr << "》》》网络连接断开《《《" << endl;
    cerr << "错误码： " << nReason << endl;
}

// 心跳超时警告
// 心跳超时警告。当长时间未收到报文时，该方法被调用。目前该方法暂不启用。
void MdSpi::OnHeartBeatWarning(int nTimeLapse)
{
    cerr << "》》》网络心跳超时《《《" << endl;
    cerr << "距上次连接时间： " << nTimeLapse << endl;
}

// 错误通知
// 针对用户请求的出错通知。
void MdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "》》》出现错误《《《" << endl;
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if (bResult)
        cerr << "返回错误 错误码：" << pRspInfo->ErrorID << ", 错误信息：" << pRspInfo->ErrorMsg << endl;
}

// 订阅询价应答
// 订阅询价应答，当调用SubscribeForQuoteRsp后，通过此接口返回。
void MdSpi::OnRspSubForQuoteRsp(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo,
    int nRequestID,
    bool bIsLast)
{
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if (!bResult)
    {
        cout << "》》》订阅询价成功《《《" << endl;
        cout << "合约代码： " << pSpecificInstrument->InstrumentID << endl;
    }
    else
        cerr << "返回错误 错误码：" << pRspInfo->ErrorID << ", 错误信息：" << pRspInfo->ErrorMsg << endl;
}

// 订阅行情应答
// 订阅行情应答，调用SubscribeMarketData后，通过此接口返回。
void MdSpi::OnRspSubMarketData(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
    CThostFtdcRspInfoField *pRspInfo, 
    int nRequestID, 
    bool bIsLast)
{
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if (!bResult)
    {
        cout << "》》》订阅行情成功《《《" << endl;
        cout << "合约代码： " << pSpecificInstrument->InstrumentID << endl;
        // 如果需要存入文件或者数据库，在这里创建表头,不同的合约单独存储
        // 创建kafka 消息队列
    }
    else
        cerr << "返回错误 错误码：" << pRspInfo->ErrorID << ", 错误信息：" << pRspInfo->ErrorMsg << endl;
}

// 取消订阅询价应答
// 取消订阅询价应答，当UnSubscribeForQuoteRsp后，调用此接口。
void MdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if (!bResult)
    {
        cout << "》》》取消订阅询价《《《" << endl;
        cout << "合约代码： " << pSpecificInstrument->InstrumentID << endl;
    }
    else
        cerr << "返回错误 错误码：" << pRspInfo->ErrorID << ", 错误信息：" << pRspInfo->ErrorMsg << endl;
}

// 取消订阅行情应答
// 取消订阅询价应答，当UnSubscribeForQuoteRsp后，调用此接口。
void MdSpi::OnRspUnSubMarketData(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
    CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, 
    bool bIsLast)
{
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if (!bResult)
    {
        cout << "》》》取消订阅行情《《《" << endl;
        cout << "合约代码： " << pSpecificInstrument->InstrumentID << endl;
    }
    else
        cerr << "返回错误 错误码：" << pRspInfo->ErrorID << ", 错误信息：" << pRspInfo->ErrorMsg << endl;
}

// 登录应答
// 登录请求响应，当ReqUserLogin后，该方法被调用。
void MdSpi::OnRspUserLogin(
    CThostFtdcRspUserLoginField *pRspUserLogin, 
    CThostFtdcRspInfoField *pRspInfo, 
    int nRequestID, 
    bool bIsLast)
{
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if (!bResult)
    {
        cout << "》》》账户登录成功《《《" << endl;
        cout << "交易日： " << pRspUserLogin->TradingDay << endl;
        cout << "登录时间： " << pRspUserLogin->LoginTime << endl;
        cout << "经纪商： " << pRspUserLogin->BrokerID << endl;
        cout << "帐户名： " << pRspUserLogin->UserID << endl;
        // 开始订阅行情
        int rt = p_MdApi->SubscribeMarketData(p_InstrumentId, InstrumentNum);
        if (!rt)
            cout << "发送订阅行情请求成功" << endl;
        else
            cerr << "发送订阅行情请求失败" << endl;
    }
    else
        cerr << "返回错误 错误码：" << pRspInfo->ErrorID << ", 错误信息：" << pRspInfo->ErrorMsg << endl;
}

// 登出应答
// 登出请求响应，当ReqUserLogout后，该方法被调用。
void MdSpi::OnRspUserLogout(
    CThostFtdcUserLogoutField *pUserLogout,
    CThostFtdcRspInfoField *pRspInfo, 
    int nRequestID, 
    bool bIsLast)
{
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if (!bResult)
    {
        cout << "》》》账户登出《《《" << endl;
        cout << "经纪商： " << pUserLogout->BrokerID << endl;
        cout << "帐户名： " << pUserLogout->UserID << endl;
    }
    else
        cerr << "返回错误 错误码：" << pRspInfo->ErrorID << ", 错误信息：" << pRspInfo->ErrorMsg << endl;
}


// 深度行情详情通知（重点，公有流）
// 深度行情通知，当SubscribeMarketData订阅行情后，行情通知由此推送。
void MdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    // 打印行情，字段较多，截取部分
    cout << "》》》获得深度行情《《《" << endl;
    cout << "交易日： " << pDepthMarketData->TradingDay << endl;
    cout << "交易所代码： " << pDepthMarketData->ExchangeID << endl;
    cout << "合约代码： " << pDepthMarketData->InstrumentID << endl;
    cout << "最新价： " << pDepthMarketData->LastPrice << endl;
    cout << "交易总量： " << pDepthMarketData->Volume << endl;
    ///最后修改时间
    cout << "最后修改时间 " << pDepthMarketData->UpdateTime << endl;
    ///最后修改毫秒
    cout << "最后修改毫秒 " << pDepthMarketData->UpdateMillisec << endl;
    cout << "最高价 " << pDepthMarketData->HighestPrice << endl;
    cout << "最低价 " << pDepthMarketData->LowestPrice << endl;
    cout << "今收盘 " << pDepthMarketData->ClosePrice << endl;
    cout << "本次结算价 " << pDepthMarketData->SettlementPrice << endl;
    
    cout << "卖五 价 " << pDepthMarketData->AskPrice5 << " 量 " << pDepthMarketData->AskVolume5 << endl;
    cout << "卖四 价 " << pDepthMarketData->AskPrice4 << " 量 " << pDepthMarketData->AskVolume4 << endl;
    cout << "卖三 价 " << pDepthMarketData->AskPrice3 << " 量 " << pDepthMarketData->AskVolume3 << endl;
    cout << "卖二 价 " << pDepthMarketData->AskPrice2 << " 量 " << pDepthMarketData->AskVolume2 << endl;
    cout << "卖一 价 " << pDepthMarketData->AskPrice1 << " 量 " << pDepthMarketData->AskVolume1 << endl;
    cout << "买一 价 " << pDepthMarketData->BidPrice1 << " 量 " << pDepthMarketData->BidVolume1 << endl;
    cout << "买二 价 " << pDepthMarketData->BidPrice2 << " 量 " << pDepthMarketData->BidVolume2 << endl;
    cout << "买三 价 " << pDepthMarketData->BidPrice3 << " 量 " << pDepthMarketData->BidVolume3 << endl;
    cout << "买四 价 " << pDepthMarketData->BidPrice4 << " 量 " << pDepthMarketData->BidVolume4 << endl;
    cout << "买五 价 " << pDepthMarketData->BidPrice5 << " 量 " << pDepthMarketData->BidVolume5 << endl;

    //序号更新
    if(strcmp(pDepthMarketData->TradingDay, TradingDay))
    {
        cout << "》》》新交易日《《《" << endl;
        // 更新交易日TradingDay
        strcpy(TradingDay, pDepthMarketData->TradingDay);
        // 更新日内序号TickIntradayID
        TickIntradayID = 1;
        // 归零交易总量Volume
        Volume = 0;
    }

    // 构造tick信息
    // 依(id_intraday, price, volume, time)生成消息串
    sprintf(Message, "%d, %.2f, %d, %sT%s:%d", TickIntradayID, pDepthMarketData->LastPrice, 
              pDepthMarketData->Volume-Volume, pDepthMarketData->TradingDay,
              pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec);
    cout << "传送信息:" << Message << endl;

    // 创建Kafka实例
    topic = "tick";
    // 写入消息
    string msg = Message;
    // 载入信息
    producer.produce(MessageBuilder(topic).payload(msg));
    cout << "》》》完成消息生产《《《" << endl;
    cout << msg << endl;
    // 写入时间
    time_t now = time(0);
    char* dt = ctime(&now);
    cout << "当前时间" << dt << endl;

    // 日内序号+1
    TickIntradayID += 1;
    // 更新交易总量
    Volume = pDepthMarketData->Volume;

    // // k线采样（目前禁用）
    // bar.push_back(pDepthMarketData->LastPrice);
    // int high = *max_element(bar.begin(), bar.end()); 
    // int low = *min_element(bar.begin(), bar.end()); 
    // int start = *bar.begin(); 
    // int end = *(bar.end()-1); 
    // // 生成k线
    // if (bar.size() == 30)
    // {
    //     cout << "》》》生成k线《《《" << endl;
    //     // 依(id_intraday, price, volume, time)生成消息串
    //     sprintf(Message, "%.2f, %.2f, %.2f, %.2f, %s:%d", start, end, high, low,
    //           pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec);
    //     // 创建Kafka实例
    //     string msg = Message;
    //     // 载入信息
    //     producer.produce(MessageBuilder(topic).payload(msg));
    //     cout << "》》》完成消息生产《《《" << endl;
    //     cout << msg << endl;
    //     // 写入时间
    //     time_t now = time(0);
    //     char* dt = ctime(&now);
    //     cout << "当前时间" << dt << endl;
    //     // k线清空
    //     bar.clear();
    // }
    // cout << "起始价" << start << endl;
    // cout << "结束价" << end << endl;
    // cout << "最高价" << high << endl;
    // cout << "最低价" << low << endl;
}

// 询价详情通知（私有流）
// 询价通知，使用SubscribeForQuoteRsp订阅该询价通知。私有流回报。
void MdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
    // 部分询价结果
    cout << "》》》获得询价结果《《《" << endl;
    cout << "交易日： " << pForQuoteRsp->TradingDay << endl;
    cout << "交易所代码： " << pForQuoteRsp->ExchangeID << endl;
    cout << "合约代码： " << pForQuoteRsp->InstrumentID << endl;
    cout << "询价编号： " << pForQuoteRsp->ForQuoteSysID << endl;
}
