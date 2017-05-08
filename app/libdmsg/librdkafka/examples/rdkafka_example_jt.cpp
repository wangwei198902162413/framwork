/*
 * librdkafka - Apache Kafka C library
 *
 * Copyright (c) 2014, Magnus Edenhill
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Apache Kafka consumer & producer example programs
 * using the Kafka driver from librdkafka
 * (https://github.com/edenhill/librdkafka)
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

#include <getopt.h>
#include "./../../../../cpp/hla/common/Rpc/UserInfo/ailk.UserInfo.BillingMsg.pb.h"
#include "./../../../../cpp/hla/common/include/StdEvent.h"
#include "./../../../../cpp/hla/common/RpcApi/BillingMsgAPI.h"


/*
 * Typically include path in a real application would be
 * #include <librdkafka/rdkafkacpp.h>
 */
#include "rdkafkacpp.h"



static bool run = true;

static void sigterm (int sig) {
  run = false;
}


class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
 public:
  void dr_cb (RdKafka::Message &message) {
    std::cout << "Message delivery for (" << message.len() << " bytes): " <<
        message.errstr() << std::endl;
  }
};


class ExampleEventCb : public RdKafka::EventCb {
 public:
  void event_cb (RdKafka::Event &event) {
    switch (event.type())
    {
      case RdKafka::Event::EVENT_ERROR:
        std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " <<
            event.str() << std::endl;
        if (event.err() == RdKafka::ERR__ALL_BROKERS_DOWN)
          run = false;
        break;

      case RdKafka::Event::EVENT_STATS:
        std::cerr << "\"STATS\": " << event.str() << std::endl;
        break;

      case RdKafka::Event::EVENT_LOG:
        fprintf(stderr, "LOG-%i-%s: %s\n",
                event.severity(), event.fac().c_str(), event.str().c_str());
        break;

      default:
        std::cerr << "EVENT " << event.type() <<
            " (" << RdKafka::err2str(event.err()) << "): " <<
            event.str() << std::endl;
        break;
    }
  }
};





/* Use of this partitioner is pretty pointless since no key is provided
 * in the produce() call. */
class MyHashPartitionerCb : public RdKafka::PartitionerCb {
 public:
  int32_t partitioner_cb (const RdKafka::Topic *topic, const std::string *key,
                          int32_t partition_cnt, void *msg_opaque) {
    return djb_hash(key->c_str(), key->size()) % partition_cnt;
  }
 private:

  static inline unsigned int djb_hash (const char *str, size_t len) {
    unsigned int hash = 5381;
    for (size_t i = 0 ; i < len ; i++)
      hash = ((hash << 5) + hash) + str[i];
    return hash;
  }
};
bool ParserStdEvent(BM35::StdEvent & stStdEvent,ailk::EventInfo::StdEventProto& mStd){
	memset(&stStdEvent,0,sizeof(BM35::StdEvent));
	  try
        {
        if( mStd.has_eventid() )
                stStdEvent.m_lEventID = mStd.eventid();
        #ifdef ORG_BILLINGCYCLE
        if( mStd.has_orgbillingcycleid() )
                stStdEvent.m_iOrgBillingCycleID = mStd.orgbillingcycleid();
        #endif

        if( mStd.has_billingcycleid() )
                stStdEvent.m_iBillingCycleID = mStd.billingcycleid();
        if( mStd.has_eventtypeid() )
                stStdEvent.m_iEventTypeID = mStd.eventtypeid();
        if( mStd.has_billingareacode() )
                strcpy(stStdEvent.m_sBillingAreaCode , mStd.billingareacode().c_str());
        if( mStd.has_billingnbr() )
                strcpy(stStdEvent.m_sBillingNBR,mStd.billingnbr().c_str());
        if( mStd.has_billingvisitareacode() )
                strcpy(stStdEvent.m_sBillingVisitAreaCode,mStd.billingvisitareacode().c_str());
        if( mStd.has_callingareacode() )
                strcpy(stStdEvent.m_sCallingAreaCode,mStd.callingareacode().c_str());
        if( mStd.has_callingnbr() )
                strcpy(stStdEvent.m_sCallingNBR,mStd.callingnbr().c_str());
        if( mStd.has_callingphysicalnbr() )
                strcpy(stStdEvent.m_sCallingPhysicalNBR,mStd.callingphysicalnbr().c_str());

        if( mStd.has_orgcallingnbr() )
                strcpy(stStdEvent.m_sOrgCallingNBR,mStd.orgcallingnbr().c_str());
        if( mStd.has_orgcallingphysicalnbr() )
                strcpy(stStdEvent.m_sOrgCallingPhysicalNBR,mStd.orgcallingphysicalnbr().c_str());
        if( mStd.has_callingsptypeid() )
                stStdEvent.m_iCallingSPTypeID = mStd.callingsptypeid();

        if( mStd.has_callingservicetypeid() )
                stStdEvent.m_iCallingServiceTypeID = mStd.callingservicetypeid();
        if( mStd.has_calledareacode() )
                strcpy(stStdEvent.m_sCalledAreaCode,mStd.calledareacode().c_str());
        if( mStd.has_callednbr() )
                strcpy(stStdEvent.m_sCalledNBR,mStd.callednbr().c_str());

        if( mStd.has_calledphysicalnbr() )
                strcpy(stStdEvent.m_sCalledPhysicalNBR,mStd.calledphysicalnbr().c_str());
        if( mStd.has_orgcallednbr() )
                strcpy(stStdEvent.m_sOrgCalledNBR,mStd.orgcallednbr().c_str());
        if( mStd.has_orgcalledphysicalnbr() )
                strcpy(stStdEvent.m_sOrgCalledPhysicalNBR,mStd.orgcalledphysicalnbr().c_str());
        if( mStd.has_calledsptypeid() )
                stStdEvent.m_iCalledSPTypeID = mStd.calledsptypeid();
        if( mStd.has_calledservicetypeid() )
                stStdEvent.m_iCalledServiceTypeID = mStd.calledservicetypeid();
        if( mStd.has_thirdpartyareacode() )
                strcpy(stStdEvent.m_sThirdPartyAreaCode,mStd.thirdpartyareacode().c_str());

        strcpy(stStdEvent.m_sThirdPartyNBR,mStd.thirdpartynbr().c_str());
        strcpy(stStdEvent.m_sThirdPartyPhysicalNBR,mStd.thirdpartyphysicalnbr().c_str());
        strcpy(stStdEvent.m_sStartDate,mStd.startdate().c_str());

        stStdEvent.m_iUserInfoOffsetAllowed = mStd.userinfooffsetallowed();
        strcpy(stStdEvent.m_sEndDate,mStd.enddate().c_str());
        stStdEvent.m_lDuration = mStd.duration();

        stStdEvent.m_iIPType = mStd.iptype();
        stStdEvent.m_lRecvAmount = mStd.recvamount();
        stStdEvent.m_lSendAmount = mStd.sendamount();

        stStdEvent.m_lRecvAmountV6 = mStd.recvamountv6();
        stStdEvent.m_lSendAmountV6 = mStd.sendamountv6();
        stStdEvent.m_lSumAmountV4 = mStd.sumamountv4();

        stStdEvent.m_lSumAmountV6 = mStd.sumamountv6();
        stStdEvent.m_lSumAmount = mStd.sumamount();
        strcpy(stStdEvent.m_sTrunkIn,mStd.trunkin().c_str());

        stStdEvent.m_iTrunkInTypeID = mStd.trunkintypeid();
        strcpy(stStdEvent.m_sTrunkOut,mStd.trunkout().c_str());
        stStdEvent.m_iTrunkOutTypeID = mStd.trunkouttypeid();

        stStdEvent.m_iFileID = mStd.fileid();
        strcpy(stStdEvent.m_sEventState,mStd.eventstate().c_str());
        strcpy(stStdEvent.m_sCreatedDate,mStd.createddate().c_str());

        strcpy(stStdEvent.m_sStateDate,mStd.statedate().c_str());
        strcpy(stStdEvent.m_sCardID,mStd.cardid().c_str());
        stStdEvent.m_iBillingTypeID = mStd.billingtypeid();

        stStdEvent.m_iParterID=mStd.parterid();
        stStdEvent.m_iServID=mStd.servid();
        stStdEvent.m_iCustID=mStd.custid();

        stStdEvent.m_iProdID = mStd.prodid();
        stStdEvent.m_iServProdID=mStd.servprodid();
        for(int i = 0;i<mStd.offers_size();i++) {
                stStdEvent.m_lOffers[i] = mStd.offers(i);
        }

        strcpy(stStdEvent.m_sBillingRecord,mStd.billingrecord().c_str());
        stStdEvent.m_iErrorID=mStd.errorid();
        strcpy(stStdEvent.m_sAccountAreaCode,mStd.accountareacode().c_str());

        strcpy(stStdEvent.m_sAccountNBR,mStd.accountnbr().c_str());
        strcpy(stStdEvent.m_sNASIp,mStd.nasip().c_str());
        stStdEvent.m_iNASPortID=mStd.nasportid();

        stStdEvent.m_iNegotiatedQoS=mStd.negotiatedqos();
        stStdEvent.m_iExtUsedNum=mStd.extusednum();

        for(int i = 0;i<mStd.extfieldid_size();i++) {
                stStdEvent.m_aiExtFieldID[i] = mStd.extfieldid(i);
        }
        for(int i = 0;i<mStd.extvalue_size();i++) {
                strncpy(stStdEvent.m_asExtValue[i],mStd.extvalue(i).c_str(),MAX_BYTES_PER_STRING);
        }

        stStdEvent.m_iSwitchID=mStd.switchid();
        stStdEvent.m_iBusinessKey=mStd.businesskey();
        for(int i = 0;i<mStd.orgcharge_size();i++) {
                stStdEvent.m_lOrgCharge[i] = mStd.orgcharge(i);
        }

        for(int i = 0;i<mStd.chargeinfo_size();i++) {
                stStdEvent.m_oChargeInfo[i].m_lCharge = mStd.chargeinfo(i).charge();
                stStdEvent.m_oChargeInfo[i].m_iAcctItemTypeID = mStd.chargeinfo(i).acctitemtypeid();
                stStdEvent.m_oChargeInfo[i].m_lBillMeasure = mStd.chargeinfo(i).billmeasure();
                stStdEvent.m_oChargeInfo[i].m_iMeasureType = mStd.chargeinfo(i).measuretype();
                stStdEvent.m_oChargeInfo[i].m_lAcctID = mStd.chargeinfo(i).acctid();
                stStdEvent.m_oChargeInfo[i].m_lOfferInstID = mStd.chargeinfo(i).offerinstid();
                stStdEvent.m_oChargeInfo[i].m_iItemSourceID = mStd.chargeinfo(i).itemsourceid();
        }
        #ifdef ABM_FIELD_EXTEND
        for(int i = 0;i<mStd.chargeinfobak_size();i++) {
                stStdEvent.m_oChargeInfoBak[i].m_lCharge = mStd.chargeinfobak(i).charge();
                stStdEvent.m_oChargeInfoBak[i].m_iAcctItemTypeID = mStd.chargeinfobak(i).acctitemtypeid();
                stStdEvent.m_oChargeInfoBak[i].m_lBillMeasure = mStd.chargeinfobak(i).billmeasure();
                stStdEvent.m_oChargeInfoBak[i].m_lOfferInstID = mStd.chargeinfobak(i).offerinstid();
                stStdEvent.m_oChargeInfoBak[i].m_iItemSourceID = mStd.chargeinfobak(i).itemsourceid();
        }
        #endif
        for(int i = 0;i<mStd.stdchargeinfo_size();i++) {
                stStdEvent.m_oStdChargeInfo[i].m_lStdCharge = mStd.stdchargeinfo(i).stdcharge();
                stStdEvent.m_oStdChargeInfo[i].m_iStdAcctItemTypeID = mStd.stdchargeinfo(i).stdacctitemtypeid();
        }

        stStdEvent.m_iBillingOrgID=mStd.billingorgid();
        stStdEvent.m_iCallTypeID=mStd.calltypeid();
        stStdEvent.m_iPricingCombineID=mStd.pricingcombineid();
        stStdEvent.m_iEventSourceType=mStd.eventsourcetype();
        stStdEvent.m_iNoBillFlag=mStd.nobillflag();

        strcpy(stStdEvent.m_sMSC,mStd.msc().c_str());
        strcpy(stStdEvent.m_sLAC,mStd.lac().c_str());
        strcpy(stStdEvent.m_sCalledMSC,mStd.calledmsc().c_str());
        strcpy(stStdEvent.m_sCalledLAC,mStd.calledlac().c_str());
        strcpy(stStdEvent.m_sCalledCellID,mStd.calledcellid().c_str());
        strcpy(stStdEvent.m_sCellID,mStd.cellid().c_str());
        strcpy(stStdEvent.m_sRomingNBR,mStd.romingnbr().c_str());

        for(int i=0;i<mStd.eventext().extendstrvalue_size();i++) {
                stStdEvent.m_oEventExt.m_lExtendValue[i] = mStd.eventext().extendlongvalue(i);
        }
        stStdEvent.m_oEventExt.m_iRoamTypeID=mStd.eventext().roamtypeid();
        stStdEvent.m_oEventExt.m_iStdRoamTypeID=mStd.eventext().stdroamtypeid();
        stStdEvent.m_oEventExt.m_iStdLongTypeID=mStd.eventext().stdlongtypeid();
        stStdEvent.m_oEventExt.m_iLongTypeID=mStd.eventext().longtypeid();
        strcpy(stStdEvent.m_oEventExt.m_sFileName,mStd.eventext().filename().c_str());

        stStdEvent.m_oEventExt.m_iSourceEventType=mStd.eventext().sourceeventtype();
        stStdEvent.m_oEventExt.m_iCarrierTypeID=mStd.eventext().carriertypeid();
        stStdEvent.m_oEventExt.m_iCallingLongGroupTypeID=mStd.eventext().callinglonggrouptypeid();
        stStdEvent.m_oEventExt.m_iCalledLongGroupTypeID=mStd.eventext().calledlonggrouptypeid();
        stStdEvent.m_oEventExt.m_iCallingOrgID=mStd.eventext().callingorgid();
        stStdEvent.m_oEventExt.m_iCalledOrgID=mStd.eventext().calledorgid();

        strcpy(stStdEvent.m_oEventExt.m_sOrgSequence,mStd.eventext().orgsequence().c_str());
        stStdEvent.m_oEventExt.m_iPartialIndicator = mStd.eventext().partialindicator();
        stStdEvent.m_oEventExt.m_iMiddleSequnce=mStd.eventext().middlesequnce();
        stStdEvent.m_oEventExt.m_iSwitchLongType=mStd.eventext().switchlongtype();
        stStdEvent.m_oEventExt.m_iHotBillingTag=mStd.eventext().hotbillingtag();
        stStdEvent.m_oEventExt.m_iCausefortermination=mStd.eventext().causefortermination();

        strcpy(stStdEvent.m_oEventExt.m_sSwitchAreaCode,mStd.eventext().switchareacode().c_str());
        strcpy(stStdEvent.m_oEventExt.m_sCallingVisitAreaCode,mStd.eventext().callingvisitareacode().c_str());
        strcpy(stStdEvent.m_oEventExt.m_sCalledVisitAreaCode,mStd.eventext().calledvisitareacode().c_str());
        strcpy(stStdEvent.m_oEventExt.m_sThirdPartyVisitAreaCode,mStd.eventext().thirdpartyvisitareacode().c_str());
        strcpy(stStdEvent.m_oEventExt.m_sIMSI,mStd.eventext().imsi().c_str());
        strcpy(stStdEvent.m_oEventExt.m_sIMEI,mStd.eventext().imei().c_str());
        strcpy(stStdEvent.m_oEventExt.m_sAccessPointName,mStd.eventext().accesspointname().c_str());
        stStdEvent.m_oEventExt.m_iNodeID = mStd.eventext().nodeid();

        strcpy(stStdEvent.m_sSPFLAG,mStd.spflag().c_str());
        stStdEvent.m_iSPID=mStd.spid();
        stStdEvent.m_iSuppleServiceCode=mStd.suppleservicecode();
        stStdEvent.m_iCUGCode=mStd.cugcode();
        for(int i=0;i<mStd.reservedfield_size();i++) {
                strcpy(stStdEvent.m_sReservedField[i] , mStd.reservedfield(i).c_str());
        }

        stStdEvent.m_iTimes=mStd.times();
        stStdEvent.m_iNetworkID=mStd.networkid();
        stStdEvent.m_iFlowID=mStd.flowid();

        strcpy(stStdEvent.m_sGateWay,mStd.gateway().c_str());
        stStdEvent.m_iCallingPA=mStd.callingpa();
        stStdEvent.m_iCalledPA=mStd.calledpa();
        strcpy(stStdEvent.m_sCallingRP,mStd.callingrp().c_str());
        strcpy(stStdEvent.m_sCalledRP,mStd.calledrp().c_str());
        stStdEvent.m_iCallingRPArea=mStd.callingrparea();
        stStdEvent.m_iCalledRPArea=mStd.calledrparea();
        strcpy(stStdEvent.m_sSessionID,mStd.sessionid().c_str());
        strcpy(stStdEvent.m_sOrgThirdPartyNBR,mStd.orgthirdpartynbr().c_str());
        strcpy(stStdEvent.m_sOrgThirdPartyPhysicalNBR,mStd.orgthirdpartyphysicalnbr().c_str());

        stStdEvent.m_iEventSeq=mStd.eventseq();
        strcpy(stStdEvent.m_sDomainName,mStd.domainname().c_str());
        stStdEvent.m_iGroupID=mStd.groupid();
        stStdEvent.m_iRoamFlag=mStd.roamflag();
        strcpy(stStdEvent.m_sBindAttr,mStd.bindattr().c_str());
        stStdEvent.m_iServiceType=mStd.servicetype();
        stStdEvent.m_iUserType=mStd.usertype();
        strcpy(stStdEvent.m_sAreaNo,mStd.areano().c_str());
        stStdEvent.m_lSepRecID=mStd.seprecid();
        stStdEvent.m_iSepRecSeq=mStd.seprecseq();
        stStdEvent.m_lResourceID=mStd.resourceid();
        stStdEvent.m_iQueryFlag=mStd.queryflag();
        stStdEvent.m_iProcessID=mStd.processid();
        #ifdef EVENT_CHARGE_ACCU_EXCEEDED
        for(int i=0;i<mStd.accuinfo_size();i++) {
                stStdEvent.m_oAccuInfo[i].m_lEventAggrID=mStd.accuinfo(i).eventaggrid();
                stStdEvent.m_oAccuInfo[i].m_lOfferInstID=mStd.accuinfo(i).offerinstid();
                stStdEvent.m_oAccuInfo[i].m_lValue=mStd.accuinfo(i).value();
        }
        #endif
        strcpy(stStdEvent.m_sAccuRecord,mStd.accurecord().c_str());
        strcpy(stStdEvent.m_sPricingStrategyID,mStd.pricingstrategyid().c_str());
        strcpy(stStdEvent.m_sTrace,mStd.trace().c_str());
        } catch ( std::exception & e ) {
                return false;
        } catch (...)  {
                return false;
        }
        return true;
}
void DisplayUserInfo( ailk::UserInfo::ServResp* pServResp)
{
	std::cout<<"******************ServInfo***********"<<std::endl;	
	ailk::UserInfo::ServInfo_pb* pServInfo =pServResp->mutable_oservinfo();
	if(pServInfo->oprodofferins_size()>0)
	{
		std::cout<<"###############OfferInst##########"<<std::endl;
		ailk::UserInfo::ProdOfferIns_pb* p1 = pServInfo->mutable_oprodofferins(0);
		if(p1!=0){
			std::cout<<"OfferInstID:"<<p1->lprodofferinsid()<<std::endl;
			std::cout<<"OfferID:"<<p1->iofferid()<<std::endl;
		}
	}
	
}
void DisplayStdEvent(char* pValue,int iLen){
	if(0==pValue || pValue[0]==0)
		return;
	ailk::UserInfo::BillingMsg_pb	 billingMsg;
	billingMsg.ParseFromArray(pValue,iLen);
	std::string str1 = billingMsg.DebugString();
	std::cout<<"********BillingMsg***********"<<std::endl<<str1<<std::endl;
	BM35::CSBillingMsgAPI *pBillingMsgAPI =new BM35::CSBillingMsgAPI(&billingMsg);
	ailk::EventInfo::StdEventProto* pStdEventP = billingMsg.mutable_bstdevent();
	str1 = pStdEventP->DebugString();
	std::cout<<"********StdEvent***********"<<std::endl<<str1<<std::endl;
	ailk::UserInfo::ServResp *pServResp = pBillingMsgAPI->GetServResp();
	if(0==pServResp)
		return ;
	str1 =pServResp->DebugString();
	std::cout<<str1<<std::endl;
	std::cout<<"********ServResp***********"<<std::endl<<str1<<std::endl;
	ailk::EventInfo::WriteFileEventProto* pWrieFileEventProto = pBillingMsgAPI->GetWriteFileEvent();
	str1 = pWrieFileEventProto->DebugString();
	std::cout<<"********WriteFileEvent***********"<<std::endl<<str1<<std::endl;
	std::cout<<str1<<std::endl;
	return ;
}
void Display(char* pValue,int iLen)
{
	if(0==pValue||pValue[0]==0){
		std::cout<<"Null Value"<<std::endl;
		return ;
	}
	ailk::KafkaMsg_pb kafkaMsg;
    if (!kafkaMsg.ParseFromArray(pValue, iLen))
    {
	return ;
    }
    iLen = kafkaMsg.lbillingmsglen();
	long lPositionID = kafkaMsg.lposition();
	std::cout<<"Position :"<<lPositionID<<std::endl;
    char* sBuf = (char*)malloc(iLen);
	std::string strBillingMsg = kafkaMsg.bbillingmsgpb();
    memcpy(sBuf, strBillingMsg.c_str(), iLen);
	DisplayStdEvent(sBuf,iLen);
	return ;
};
int main (int argc, char **argv) {
  std::string brokers = "10.20.16.42:8091";
  std::string errstr;
  std::string topic_str;
  std::string mode;
  std::string debug;
  int32_t partition = RdKafka::Topic::PARTITION_UA;
  int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;
  bool exit_eof = false;
  bool do_conf_dump = false;
  char opt;
  MyHashPartitionerCb hash_partitioner;

  /*
   * Create configuration objects
   */
  RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);


  while ((opt = getopt(argc, argv, "PCt:p:b:z:qd:o:eX:AM:")) != -1) {
    switch (opt) {
    case 'P':
    case 'C':
      mode = opt;
      break;
    case 't':
      topic_str = optarg;
      break;
    case 'p':
      if (!strcmp(optarg, "random"))
        /* default */;
      else if (!strcmp(optarg, "hash")) {
        if (tconf->set("partitioner_cb", &hash_partitioner, errstr) !=
            RdKafka::Conf::CONF_OK) {
          std::cerr << errstr << std::endl;
          exit(1);
        }
      } else
        partition = std::atoi(optarg);
      break;
    case 'b':
      brokers = optarg;
      break;
    case 'z':
      if (conf->set("compression.codec", optarg, errstr) !=
	  RdKafka::Conf::CONF_OK) {
	std::cerr << errstr << std::endl;
	exit(1);
      }
      break;
    case 'o':
      if (!strcmp(optarg, "end"))
	start_offset = RdKafka::Topic::OFFSET_END;
      else if (!strcmp(optarg, "beginning"))
	start_offset = RdKafka::Topic::OFFSET_BEGINNING;
      else if (!strcmp(optarg, "stored"))
	start_offset = RdKafka::Topic::OFFSET_STORED;
      else
	start_offset = strtoll(optarg, NULL, 10);
      break;
    case 'e':
      exit_eof = true;
      break;
    case 'd':
      debug = optarg;
      break;
    case 'M':
      if (conf->set("statistics.interval.ms", optarg, errstr) !=
          RdKafka::Conf::CONF_OK) {
        std::cerr << errstr << std::endl;
        exit(1);
      }
      break;
    case 'X':
      {
	char *name, *val;

	if (!strcmp(optarg, "dump")) {
	  do_conf_dump = true;
	  continue;
	}

	name = optarg;
	if (!(val = strchr(name, '='))) {
          std::cerr << "%% Expected -X property=value, not " <<
              name << std::endl;
	  exit(1);
	}

	*val = '\0';
	val++;

	/* Try "topic." prefixed properties on topic
	 * conf first, and then fall through to global if
	 * it didnt match a topic configuration property. */
        RdKafka::Conf::ConfResult res;
	if (!strncmp(name, "topic.", strlen("topic.")))
          res = tconf->set(name+strlen("topic."), val, errstr);
        else
	  res = conf->set(name, val, errstr);

	if (res != RdKafka::Conf::CONF_OK) {
          std::cerr << errstr << std::endl;
	  exit(1);
	}
      }
      break;

    default:
      goto usage;
    }
  }

  if (mode.empty() || topic_str.empty() || optind != argc) {
  usage:
    fprintf(stderr,
            "Usage: %s [-C|-P] -t <topic> "
            "[-p <partition>] [-b <host1:port1,host2:port2,..>]\n"
            "\n"
            "librdkafka version %s (0x%08x)\n"
            "\n"
            " Options:\n"
            "  -C | -P         Consumer or Producer mode\n"
            "  -t <topic>      Topic to fetch / produce\n"
            "  -p <num>        Partition (random partitioner)\n"
            "  -p <func>       Use partitioner:\n"
            "                  random (default), hash\n"
            "  -b <brokers>    Broker address (localhost:9092)\n"
            "  -z <codec>      Enable compression:\n"
            "                  none|gzip|snappy\n"
            "  -o <offset>     Start offset (consumer)\n"
            "  -e              Exit consumer when last message\n"
            "                  in partition has been received.\n"
            "  -d [facs..]     Enable debugging contexts:\n"
            "                  %s\n"
            "  -M <intervalms> Enable statistics\n"
            "  -X <prop=name>  Set arbitrary librdkafka "
            "configuration property\n"
            "                  Properties prefixed with \"topic.\" "
            "will be set on topic object.\n"
            "                  Use '-X list' to see the full list\n"
            "                  of supported properties.\n"
            "\n"
            " In Consumer mode:\n"
            "  writes fetched messages to stdout\n"
            " In Producer mode:\n"
            "  reads messages from stdin and sends to broker\n"
            "\n"
            "\n"
            "\n",
	    argv[0],
	    RdKafka::version_str().c_str(), RdKafka::version(),
	    RdKafka::Conf::DEBUG_CONTEXTS.c_str());
    exit(1);
  }


  /*
   * Set configuration properties
   */
conf->set("fetch.message.max.bytes", "10485760",errstr);
  conf->set("metadata.broker.list", brokers, errstr);

  if (!debug.empty()) {
    if (conf->set("debug", debug, errstr) != RdKafka::Conf::CONF_OK) {
      std::cerr << errstr << std::endl;
      exit(1);
    }
  }

  ExampleEventCb ex_event_cb;
  conf->set("event_cb", &ex_event_cb, errstr);


  if (do_conf_dump) {
    int pass;

    for (pass = 0 ; pass < 2 ; pass++) {
      std::list<std::string> *dump;
      if (pass == 0) {
        dump = conf->dump();
        std::cout << "# Global config" << std::endl;
      } else {
        dump = tconf->dump();
        std::cout << "# Topic config" << std::endl;
      }

      for (std::list<std::string>::iterator it = dump->begin();
           it != dump->end(); ) {
        std::cout << *it << " = ";
        it++;
        std::cout << *it << std::endl;
        it++;
      }
      std::cout << std::endl;
    }
    exit(0);
  }

  signal(SIGINT, sigterm);
  signal(SIGTERM, sigterm);


	std::cout<<"brokers :"<<brokers<<std::endl;
  if (mode == "P") {
    /*
     * Producer mode
     */
    ExampleDeliveryReportCb ex_dr_cb;

    /* Set delivery report callback */
    conf->set("dr_cb", &ex_dr_cb, errstr);

    /*
     * Create producer using accumulated global configuration.
     */
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
      std::cerr << "Failed to create producer: " << errstr << std::endl;
      exit(1);
    }

    std::cout << "% Created producer " << producer->name() << std::endl;

    /*
     * Create topic handle.
     */
    RdKafka::Topic *topic = RdKafka::Topic::create(producer, topic_str,
						   tconf, errstr);
    if (!topic) {
      std::cerr << "Failed to create topic: " << errstr << std::endl;
      exit(1);
    }

    /*
     * Read messages from stdin and produce to broker.
     */
    for (std::string line; run and std::getline(std::cin, line);) {
      if (line.empty()) {
        producer->poll(0);
	continue;
      }

      /*
       * Produce message
       */
      RdKafka::ErrorCode resp =
	producer->produce(topic, partition,
			  RdKafka::Producer::MSG_COPY /* Copy payload */,
			  const_cast<char *>(line.c_str()), line.size(),
			  NULL, NULL);
      if (resp != RdKafka::ERR_NO_ERROR)
	std::cerr << "% Produce failed: " <<
	  RdKafka::err2str(resp) << std::endl;
      else
	std::cerr << "% Produced message (" << line.size() << " bytes)" <<
	  std::endl;

      producer->poll(0);
    }
    run = true;

    while (run and producer->outq_len() > 0) {
      std::cerr << "Waiting for " << producer->outq_len() << std::endl;
      producer->poll(1000);
    }

    delete topic;
    delete producer;


  } else {
    /*
     * Consumer mode
     */

    /*
     * Create consumer using accumulated global configuration.
     */
    RdKafka::Consumer *consumer = RdKafka::Consumer::create(conf, errstr);
    if (!consumer) {
      std::cerr << "Failed to create consumer: " << errstr << std::endl;
      exit(1);
    }

    std::cout << "% Created consumer " << consumer->name() << std::endl;

    /*
     * Create topic handle.
     */
    RdKafka::Topic *topic = RdKafka::Topic::create(consumer, topic_str,
						   tconf, errstr);
    if (!topic) {
      std::cerr << "Failed to create topic: " << errstr << std::endl;
      exit(1);
    }

    /*
     * Start consumer for topic+partition at start offset
     */
    RdKafka::ErrorCode resp = consumer->start(topic, partition, start_offset);
    if (resp != RdKafka::ERR_NO_ERROR) {
      std::cerr << "Failed to start consumer: " <<
	RdKafka::err2str(resp) << std::endl;
      exit(1);
    }

    /*
     * Consume messages
     */
    while (run) {
      RdKafka::Message *msg = consumer->consume(topic, partition, 1000);

      switch (msg->err())
      {
        case RdKafka::ERR__TIMED_OUT:
          break;

        case RdKafka::ERR_NO_ERROR:
	  /* Real message */
	  std::cerr << "Read msg at offset " << msg->offset() <<"Msg Size:"<<static_cast<int>(msg->len())<< std::endl;
		Display(static_cast<char *>(msg->payload()),static_cast<int>(msg->len()));
         /* printf("%.*s\n",
                 static_cast<int>(msg->len()),
                 static_cast<const char *>(msg->payload()));*/
		exit(0);
	  break;

	case RdKafka::ERR__PARTITION_EOF:
	  /* Last message */
	  if (exit_eof)
	    run = false;
	  break;

	default:
	  /* Errors */
	  std::cerr << "Consume failed: " << msg->errstr() << std::endl;
	  run = false;
	}

      delete msg;

      consumer->poll(0);
    }

    /*
     * Stop consumer
     */
    consumer->stop(topic, partition);

    consumer->poll(1000);

    delete topic;
    delete consumer;
  }


  /*
   * Wait for RdKafka to decommission.
   * This is not strictly needed (when check outq_len() above), but
   * allows RdKafka to clean up all its resources before the application
   * exits so that memory profilers such as valgrind wont complain about
   * memory leaks.
   */
  RdKafka::wait_destroyed(5000);

  return 0;
}
