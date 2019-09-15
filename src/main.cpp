#include <iostream>
#include "md_spi.h"
#include "../ctp_api/ThostFtdcMdApi.h"
using namespace std;

// ---- 全局变量定义 ---- //
// 账户参数
// 经纪公司：海通期货
TThostFtdcBrokerIDType  BrokerId = "8000";
// 经纪公司：simnow
// TThostFtdcBrokerIDType  BrokerId = "9999";
// 投资者代码
TThostFtdcInvestorIDType InvesterId = "";
// 用户密码
TThostFtdcPasswordType  InvesterPassword = "";

// 行情接口参数
// 全局行情指针
CThostFtdcMdApi *p_MdApi = nullptr;
// 前置地址: 海通期货
char MdFrontAddr[] = "tcp://180.168.212.75:41213";
// 前置地址: simnow
// char MdFrontAddr[] = "tcp://180.168.146.187:10031";
// 行情订阅列表
const char *p_InstrumentId[] = {"rb1910", "TA1909"};
// 行情订阅数量
int InstrumentNum = 2;
// 请求编号
int iRequestID = 0;


int main(void)
{
    cout << "》》》初始化行情《《《" << endl;
    // 创建行情实例
    p_MdApi = CThostFtdcMdApi::CreateFtdcMdApi();
    // 创建行情回调Spi实例
    CThostFtdcMdSpi* p_MdSpi = new MdSpi();
    // 注册Spi回调实例
    p_MdApi->RegisterSpi(p_MdSpi);
     // 设置前置服务器地址
    p_MdApi->RegisterFront(MdFrontAddr);

    cout << "》》》开始行情推送《《《" << endl;
    // 初始化 
    p_MdApi->Init();

    // 等待结束
    p_MdApi->Join();
    cout << "》》》退出线程《《《" << endl;
    // 释放空间
    p_MdApi->Release();
    return 0;
}
