/////////////////////////////////////////////////////////////////////////
///@system ÐÂÒ»´ú½»Ò×ËùÏµÍ³
///@company ÉÏº£ÆÚ»õÐÅÏ¢¼¼ÊõÓÐÏÞ¹«Ë¾
///@file ThostFtdcMdApi.h
///@brief ¶¨ÒåÁË¿Í»§¶Ë½Ó¿Ú
///@history 
///20060106	ÕÔºèê»		´´½¨¸ÃÎÄ¼þ
/////////////////////////////////////////////////////////////////////////

#if !defined(THOST_FTDCMDAPI_H)
#define THOST_FTDCMDAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThostFtdcUserApiStruct.h"

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_MD_API_EXPORT
#define MD_API_EXPORT __declspec(dllexport)
#else
#define MD_API_EXPORT __declspec(dllimport)
#endif
#else
#define MD_API_EXPORT 
#endif

class CThostFtdcMdSpi
{
public:
	///µ±¿Í»§¶ËÓë½»Ò×ºóÌ¨½¨Á¢ÆðÍ¨ÐÅÁ¬½ÓÊ±£¨»¹Î´µÇÂ¼Ç°£©£¬¸Ã·½·¨±»µ÷ÓÃ¡£
	virtual void OnFrontConnected(){};
	
	///µ±¿Í»§¶ËÓë½»Ò×ºóÌ¨Í¨ÐÅÁ¬½Ó¶Ï¿ªÊ±£¬¸Ã·½·¨±»µ÷ÓÃ¡£µ±·¢ÉúÕâ¸öÇé¿öºó£¬API»á×Ô¶¯ÖØÐÂÁ¬½Ó£¬¿Í»§¶Ë¿É²»×ö´¦Àí¡£
	///@param nReason ´íÎóÔ­Òò
	///        0x1001 ÍøÂç¶ÁÊ§°Ü
	///        0x1002 ÍøÂçÐ´Ê§°Ü
	///        0x2001 ½ÓÊÕÐÄÌø³¬Ê±
	///        0x2002 ·¢ËÍÐÄÌøÊ§°Ü
	///        0x2003 ÊÕµ½´íÎó±¨ÎÄ
	virtual void OnFrontDisconnected(int nReason){};
		
	///ÐÄÌø³¬Ê±¾¯¸æ¡£µ±³¤Ê±¼äÎ´ÊÕµ½±¨ÎÄÊ±£¬¸Ã·½·¨±»µ÷ÓÃ¡£
	///@param nTimeLapse ¾àÀëÉÏ´Î½ÓÊÕ±¨ÎÄµÄÊ±¼ä
	virtual void OnHeartBeatWarning(int nTimeLapse){};
	

	///µÇÂ¼ÇëÇóÏìÓ¦
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///µÇ³öÇëÇóÏìÓ¦
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ÇëÇó²éÑ¯×é²¥ºÏÔ¼ÏìÓ¦
	virtual void OnRspQryMulticastInstrument(CThostFtdcMulticastInstrumentField *pMulticastInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///´íÎóÓ¦´ð
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///¶©ÔÄÐÐÇéÓ¦´ð
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///È¡Ïû¶©ÔÄÐÐÇéÓ¦´ð
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///¶©ÔÄÑ¯¼ÛÓ¦´ð
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///È¡Ïû¶©ÔÄÑ¯¼ÛÓ¦´ð
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Éî¶ÈÐÐÇéÍ¨Öª
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {};

	///Ñ¯¼ÛÍ¨Öª
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {};
};

class MD_API_EXPORT CThostFtdcMdApi
{
public:
	///´´½¨MdApi
	///@param pszFlowPath ´æÖü¶©ÔÄÐÅÏ¢ÎÄ¼þµÄÄ¿Â¼£¬Ä¬ÈÏÎªµ±Ç°Ä¿Â¼
	///@return ´´½¨³öµÄUserApi
	///modify for udp marketdata
	static CThostFtdcMdApi *CreateFtdcMdApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false, const bool bIsMulticast=false);
	
	///»ñÈ¡APIµÄ°æ±¾ÐÅÏ¢
	///@retrun »ñÈ¡µ½µÄ°æ±¾ºÅ
	static const char *GetApiVersion();
	
	///É¾³ý½Ó¿Ú¶ÔÏó±¾Éí
	///@remark ²»ÔÙÊ¹ÓÃ±¾½Ó¿Ú¶ÔÏóÊ±,µ÷ÓÃ¸Ãº¯ÊýÉ¾³ý½Ó¿Ú¶ÔÏó
	virtual void Release() = 0;
	
	///³õÊ¼»¯
	///@remark ³õÊ¼»¯ÔËÐÐ»·¾³,Ö»ÓÐµ÷ÓÃºó,½Ó¿Ú²Å¿ªÊ¼¹¤×÷
	virtual void Init() = 0;
	
	///µÈ´ý½Ó¿ÚÏß³Ì½áÊøÔËÐÐ
	///@return Ïß³ÌÍË³ö´úÂë
	virtual int Join() = 0;
	
	///»ñÈ¡µ±Ç°½»Ò×ÈÕ
	///@retrun »ñÈ¡µ½µÄ½»Ò×ÈÕ
	///@remark Ö»ÓÐµÇÂ¼³É¹¦ºó,²ÅÄÜµÃµ½ÕýÈ·µÄ½»Ò×ÈÕ
	virtual const char *GetTradingDay() = 0;
	
	///×¢²áÇ°ÖÃ»úÍøÂçµØÖ·
	///@param pszFrontAddress£ºÇ°ÖÃ»úÍøÂçµØÖ·¡£
	///@remark ÍøÂçµØÖ·µÄ¸ñÊ½Îª£º¡°protocol://ipaddress:port¡±£¬Èç£º¡±tcp://127.0.0.1:17001¡±¡£ 
	///@remark ¡°tcp¡±´ú±í´«ÊäÐ­Òé£¬¡°127.0.0.1¡±´ú±í·þÎñÆ÷µØÖ·¡£¡±17001¡±´ú±í·þÎñÆ÷¶Ë¿ÚºÅ¡£
	virtual void RegisterFront(char *pszFrontAddress) = 0;
	
	///×¢²áÃû×Ö·þÎñÆ÷ÍøÂçµØÖ·
	///@param pszNsAddress£ºÃû×Ö·þÎñÆ÷ÍøÂçµØÖ·¡£
	///@remark ÍøÂçµØÖ·µÄ¸ñÊ½Îª£º¡°protocol://ipaddress:port¡±£¬Èç£º¡±tcp://127.0.0.1:12001¡±¡£ 
	///@remark ¡°tcp¡±´ú±í´«ÊäÐ­Òé£¬¡°127.0.0.1¡±´ú±í·þÎñÆ÷µØÖ·¡£¡±12001¡±´ú±í·þÎñÆ÷¶Ë¿ÚºÅ¡£
	///@remark RegisterNameServerÓÅÏÈÓÚRegisterFront
	virtual void RegisterNameServer(char *pszNsAddress) = 0;
	
	///×¢²áÃû×Ö·þÎñÆ÷ÓÃ»§ÐÅÏ¢
	///@param pFensUserInfo£ºÓÃ»§ÐÅÏ¢¡£
	virtual void RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo) = 0;
	
	///×¢²á»Øµ÷½Ó¿Ú
	///@param pSpi ÅÉÉú×Ô»Øµ÷½Ó¿ÚÀàµÄÊµÀý
	virtual void RegisterSpi(CThostFtdcMdSpi *pSpi) = 0;
	
	///¶©ÔÄÐÐÇé¡£
	///@param ppInstrumentID ºÏÔ¼ID  
	///@param nCount Òª¶©ÔÄ/ÍË¶©ÐÐÇéµÄºÏÔ¼¸öÊý
	///@remark 
	virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount) = 0;

	///ÍË¶©ÐÐÇé¡£
	///@param ppInstrumentID ºÏÔ¼ID  
	///@param nCount Òª¶©ÔÄ/ÍË¶©ÐÐÇéµÄºÏÔ¼¸öÊý
	///@remark 
	virtual int UnSubscribeMarketData(char *ppInstrumentID[], int nCount) = 0;
	
	///¶©ÔÄÑ¯¼Û¡£
	///@param ppInstrumentID ºÏÔ¼ID  
	///@param nCount Òª¶©ÔÄ/ÍË¶©ÐÐÇéµÄºÏÔ¼¸öÊý
	///@remark 
	virtual int SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) = 0;

	///ÍË¶©Ñ¯¼Û¡£
	///@param ppInstrumentID ºÏÔ¼ID  
	///@param nCount Òª¶©ÔÄ/ÍË¶©ÐÐÇéµÄºÏÔ¼¸öÊý
	///@remark 
	virtual int UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) = 0;

	///ÓÃ»§µÇÂ¼ÇëÇó
	virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) = 0;
	

	///µÇ³öÇëÇó
	virtual int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) = 0;

	///ÇëÇó²éÑ¯×é²¥ºÏÔ¼
	virtual int ReqQryMulticastInstrument(CThostFtdcQryMulticastInstrumentField *pQryMulticastInstrument, int nRequestID) = 0;
protected:
	~CThostFtdcMdApi(){};
};

#endif
