// -*- C++ -*-
/*!
 * @file  joker2.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "joker2.h"

// Module specification
// <rtc-template block="module_spec">
static const char* joker2_spec[] =
  {
    "implementation_id", "joker2",
    "type_name",         "joker2",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "VenderName",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.RGBmode", "1",
    // Widget
    "conf.__widget__.RGBmode", "radio",
    // Constraints
    "conf.__constraints__.RGBmode", "(-1,0,1)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
joker2::joker2(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_fromCameraIn("From_Camera", m_fromCamera),
    m_toKobuki1Out("To_Kobuki_1", m_toKobuki1),
    m_toKobuki2Out("To_Kobuki_2", m_toKobuki2),
    m_toKobuki3Out("To_Kobuki_3", m_toKobuki3),
    m_toCameraViewerOut("To_Camera_Viewer", m_toCameraViewer)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
joker2::~joker2()
{
}



RTC::ReturnCode_t joker2::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("From_Camera", m_fromCameraIn);
  
  // Set OutPort buffer
  addOutPort("To_Kobuki_1", m_toKobuki1Out);
  addOutPort("To_Kobuki_2", m_toKobuki2Out);
  addOutPort("To_Kobuki_3", m_toKobuki3Out);
  addOutPort("To_Camera_Viewer", m_toCameraViewerOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("RGBmode", m_RGBmode, "1");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t joker2::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t joker2::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t joker2::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t joker2::onActivated(RTC::UniqueId ec_id)
{
   // イメージ用メモリの初期化
   m_imageBuff = NULL;
   m_flipImageBuff = NULL;
 
   // OutPortの画面サイズの初期化
   m_toCameraViewer.width = 0;
   m_toCameraViewer.height = 0;

   waitFlag = false;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t joker2::onDeactivated(RTC::UniqueId ec_id)
{
   if(m_imageBuff != NULL)
   {
     // イメージ用メモリの解放
     cvReleaseImage(&m_imageBuff);
     cvReleaseImage(&m_flipImageBuff);
   }
 
  return RTC::RTC_OK;
}


RTC::ReturnCode_t joker2::onExecute(RTC::UniqueId ec_id)
{
	clock_t timerStart, timerEnd;

   // 新しいデータのチェック
   if (m_fromCameraIn.isNew()) {
     // InPortデータの読み込み
     m_fromCameraIn.read();
 
     // InPortとOutPortの画面サイズ処理およびイメージ用メモリの確保
     if( m_fromCamera.width != m_toCameraViewer.width || m_fromCamera.height != m_toCameraViewer.height)
       {
     m_toCameraViewer.width = m_fromCamera.width;
     m_toCameraViewer.height = m_fromCamera.height;
 
     // InPortのイメージサイズが変更された場合
     if(m_imageBuff != NULL)
       {
         cvReleaseImage(&m_imageBuff);
         cvReleaseImage(&m_flipImageBuff);
       }
 
     // イメージ用メモリの確保
     m_imageBuff = cvCreateImage(cvSize(m_fromCamera.width, m_fromCamera.height), IPL_DEPTH_8U, 3);
     m_flipImageBuff = cvCreateImage(cvSize(m_fromCamera.width, m_fromCamera.height), IPL_DEPTH_8U, 3);
       }
 
     // InPortの画像データをIplImageのimageDataにコピー
     memcpy(m_imageBuff->imageData,(void *)&(m_fromCamera.pixels[0]),m_fromCamera.pixels.length());
 
     // InPortからの画像データを反転する。 m_flipMode 0: X軸周り, 1: Y軸周り, -1: 両方の軸周り
     cvFlip(m_imageBuff, m_flipImageBuff, m_RGBmode);
 
     // 画像データのサイズ取得
     int len = m_flipImageBuff->nChannels * m_flipImageBuff->width * m_flipImageBuff->height;
     m_toCameraViewer.pixels.length(len);
 

	 cvRectangle(m_flipImageBuff,cvPoint(250,150),cvPoint(450,350),CV_RGB(255,0,0),3,CV_AA,0);

     // 反転した画像データをOutPortにコピー
     memcpy((void *)&(m_toCameraViewer.pixels[0]),m_flipImageBuff->imageData,len);
	
	 	 
	 int index;
	 for(int yy = 250; yy < 451; yy++){
		 for(int xx =250; xx < 451; xx++){
			index = yy * 200*3 + xx + xx * 3;
			colorR = m_toCameraViewer.pixels[index+2];
			colorG = m_toCameraViewer.pixels[index+1];
			colorB = m_toCameraViewer.pixels[index];
			//colorR = colorR/200;
			//colorG = colorG/200;
			//colorB = colorB/200;
		 }
	 }

	 for(int m=0; m <100; m++ ){
		 aveColorR += colorR;
		 aveColorG += colorG;
		 aveColorB += colorB;
	 }
	 std::cout << aveColorR/100 << "," << aveColorG/100 << "," << aveColorB/100 <<std::endl;
	 
	 if(!waitFlag){
		 if(aveColorR/100 > 200 || aveColorG/100 >160 || aveColorB/100 > 200){
			//条件に応じてポート1,2,3に0,1を出力
			if(aveColorR > aveColorG && aveColorR > aveColorB){
				 m_toKobuki1.data = 1;
				 m_toKobuki2.data = 0;
				 m_toKobuki3.data = 0;
				 std::cout <<  "REEEEEED!!!!!" << std::endl;
				 //Sleep(1000);
			}else if(aveColorG >aveColorR && aveColorG > aveColorB){
				 m_toKobuki1.data = 0;
				 m_toKobuki2.data = 1;
				 m_toKobuki3.data = 0;
				 cout << "GEEEEEEEN!!!" << endl;
				 //Sleep(1000);
			}else if(aveColorB >aveColorG && aveColorB > aveColorR){
				 m_toKobuki1.data = 0;
				 m_toKobuki2.data = 0;
				 m_toKobuki3.data = 1;
				 cout << "BLUEEEEEE!!!" << endl;
				 //Sleep(1000);
			}else{
				cout << "Use your card!" << endl;
			}
			m_toKobuki1Out.write();
			m_toKobuki2Out.write();
			m_toKobuki3Out.write();
		 }
		 timeRecognize = clock();
		 waitFlag = true;
	 }else{
		 clock_t tmp = clock();
		 if(tmp-timeRecognize>5000){
			 waitFlag = false;
		 }
	 }
	

	 // 反転した画像データをOutPortから出力する。
     m_toCameraViewerOut.write();

	 //取得したRBG値を初期化
	 aveColorR = 0;
	 aveColorG = 0;
	 aveColorB = 0;

	 	//30秒ごとに入力を受け付けるように30秒ループする
	//timerStart = clock();
	//timerEnd = clock();
	//while((timerEnd - timerStart) < 10000){
	//	timerEnd = clock();
	//	if((timerEnd - timerStart)%1000 == 0){
	//		cout <<10000 - (timerEnd - timerStart)  << endl;
	//	}
	//	
	//	//30秒入力がないと困るかもしれないのですべてのポートに０を入れる
	//	m_toKobuki1.data = 0;
	//	m_toKobuki2.data = 0;
	//	m_toKobuki3.data = 0;
	//	m_toKobuki1Out.write();
	//	m_toKobuki2Out.write();
	//	m_toKobuki3Out.write();
	//}

   }
 
   return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t joker2::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t joker2::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t joker2::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t joker2::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t joker2::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void joker2Init(RTC::Manager* manager)
  {
    coil::Properties profile(joker2_spec);
    manager->registerFactory(profile,
                             RTC::Create<joker2>,
                             RTC::Delete<joker2>);
  }
  
};


