#include "daheng_camera_0.h"
#include <QtWidgets\QMessageBox>

daheng_camera_0::daheng_camera_0(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	//函数返回状态
	this->status=GX_STATUS_SUCCESS;
	//设备个数
	this->nDeviceNum = 0;
	//设备句柄
	this->hDevice = NULL;


	//connect
	//connect(this->ui.init_button,SIGNAL(clicked()),this,SLOT(this->on_click_init()));
	connect(this->ui.init_button,SIGNAL(clicked()),this,SLOT(on_click_init()));
	connect(this->ui.capture_button,SIGNAL(clicked()),this,SLOT(on_click_capture()));
	connect(this->ui.save_button,SIGNAL(clicked()),this,SLOT(on_click_save()));
	
	//mode change
	connect(this->ui.Mode,SIGNAL(currentIndexChanged(int)),this,SLOT(on_mode_change()));
	//white balance
	connect(this->ui.WhiteBalance_enable,SIGNAL(stateChanged(int)),this,SLOT(on_balance_change()));
	//color change
	connect(this->ui.ColorCorrect_enable,SIGNAL(stateChanged(int)),this,SLOT(on_color_change()));
	//height
	connect(this->ui.Height_input,SIGNAL(valueChanged(int)),this,SLOT(on_height_changed()));
	//width   
	connect(this->ui.Width_input,SIGNAL(valueChanged(int)),this,SLOT(on_width_changed()));
	//exposetime
	connect(this->ui.expose_bar,SIGNAL(valueChanged(int)),this,SLOT(on_expose_change()));
	//gain
	connect(this->ui.gain_bar,SIGNAL(valueChanged(int)),this,SLOT(on_gain_change()));
	//serial port
	connect(this->ui.open_port,SIGNAL(clicked()),this,SLOT(on_open_port()));

}

daheng_camera_0::~daheng_camera_0()
{

}


//采集模式
//Mode1: SignleFrame + trigger-on
//Mode2: continue    + trigger-on
//Mode3: SignleFrame + trigger-off
//Mode4: continue    + trigger-off
//停止采集的状态下才可用 GXsetEnum()
void daheng_camera_0::on_mode_change()
{
	switch (this->ui.Mode->currentIndex())
	{
		//MODE 1
	case 1:   //设置为软触发
		{
			this->ui.Information->insertPlainText(tr("Mode1"));
			GXSetEnum(this->hDevice,GX_ENUM_ACQUISITION_MODE,GX_ACQ_MODE_SINGLE_FRAME);
			GXSetEnum(this->hDevice,GX_ENUM_TRIGGER_MODE,GX_TRIGGER_MODE_ON);
			GXSetEnum(this->hDevice,GX_ENUM_TRIGGER_SOURCE,GX_TRIGGER_SOURCE_SOFTWARE);
			break;
		}
		//MODE 2
	case 2:   //设置为软触发
		{
			this->ui.Information->insertPlainText(tr("Mode2"));
			GXSetEnum(this->hDevice,GX_ENUM_ACQUISITION_MODE,GX_ACQ_MODE_CONTINUOUS);
			GXSetEnum(this->hDevice,GX_ENUM_TRIGGER_MODE,GX_TRIGGER_MODE_ON);
			GXSetEnum(this->hDevice,GX_ENUM_TRIGGER_SOURCE,GX_TRIGGER_SOURCE_SOFTWARE);
			break;
		}
			//MODE 3
	case 3:
		{
			this->ui.Information->insertPlainText(tr("Mode3"));
			GXSetEnum(this->hDevice,GX_ENUM_ACQUISITION_MODE,GX_ACQ_MODE_SINGLE_FRAME);
			GXSetEnum(this->hDevice,GX_ENUM_TRIGGER_MODE,GX_TRIGGER_MODE_OFF);
			break;
		}
			//MODE 4
	case 4:
		{
			this->ui.Information->insertPlainText(tr("Mode4"));
			GXSetEnum(this->hDevice,GX_ENUM_ACQUISITION_MODE,GX_ACQ_MODE_CONTINUOUS);
			GXSetEnum(this->hDevice,GX_ENUM_TRIGGER_MODE,GX_TRIGGER_MODE_OFF);
			break;
		}
	default:
		{	GXSetEnum(this->hDevice,GX_ENUM_ACQUISITION_MODE,GX_ACQ_MODE_SINGLE_FRAME);
			GXSetEnum(this->hDevice,GX_ENUM_TRIGGER_MODE,GX_TRIGGER_MODE_ON);
			break;
		}
	}	
}

void daheng_camera_0::on_expose_change()
{
	int value;
	value=this->ui.expose_bar->value();
	this->ui.expose_label->setNum(value);
	GXSetFloat(hDevice,GX_FLOAT_EXPOSURE_TIME,value);
}

void daheng_camera_0::on_gain_change()
{
	int value;
	value=this->ui.expose_bar->value();
	this->ui.gain_label->setNum(value);
	GXSetFloat(hDevice,GX_INT_GAIN,value);
}

void daheng_camera_0::on_width_changed()
{
	int value;
	this->ui.Width_input->value();

	status = GXSetInt(hDevice, GX_INT_WIDTH, value);
	status = GXSetInt(hDevice, GX_INT_OFFSET_X,0);
}


void daheng_camera_0::on_height_changed()
{
	int value;
	this->ui.Height_input->value();

	status = GXSetInt(hDevice, GX_INT_OFFSET_Y,0);
	status = GXSetInt(hDevice, GX_INT_HEIGHT, value);
}


void daheng_camera_0::on_balance_change()
{
	bool checked;
	checked = this->ui.WhiteBalance_enable->isChecked();
	
	if(checked)
		GXSetEnum(hDevice,GX_ENUM_COLOR_CORRECT,GX_COLOR_CORRECT_ON);
	else
		GXSetEnum(hDevice,GX_ENUM_COLOR_CORRECT,GX_COLOR_CORRECT_OFF);
}

void daheng_camera_0::on_color_change()
{
	bool checked;
	checked = this->ui.WhiteBalance_enable->isChecked();
	
	if(checked)
		GXSetEnum(hDevice,GX_ENUM_COLOR_CORRECT,GX_COLOR_CORRECT_ON);
	else
		GXSetEnum(hDevice,GX_ENUM_COLOR_CORRECT,GX_COLOR_CORRECT_OFF);
}

void daheng_camera_0::on_click_init()
{
	
	//---------------workflow---------------//

	//1.
	//在起始位置调用进行初始化，申请资源
	//结束时调用GXCloseLib()释放资源
	status = GXInitLib();

	//2:
	//枚举设备,返回设备个数
	uint32_t unTimeOut=1000;
	//第一个参数：设备个数（指针）；第二个参数：延时时间
	status=GXUpdateDeviceList(&nDeviceNum,unTimeOut);

	if(status)
	{
		QMessageBox::information(this,tr("warning"),tr("cannot capture"));
	}
else
{
		QMessageBox::information(this,tr("warning"),tr("captured!"));

	//3:
	//获取设备基础信息;最后要delete [] baseinfo
	GX_DEVICE_BASE_INFO* baseInfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
	size_t nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);
	status = GXGetAllDeviceBaseInfo(baseInfo,&nSize);//获取所有设备的基础信息
	
	//4:
	//打开相机
	//先判断nDeviceNum，设备个数是否>0
	//第一个参数是打开第n个设备
	//用完之后要关闭设备,GXCloseDevice(hDevice);
	status = GXOpenDeviceByIndex(1,&hDevice);

	//默认为软触发
	status = GXSetEnum(this->hDevice,GX_ENUM_ACQUISITION_MODE,GX_ACQ_MODE_SINGLE_FRAME);
	status = GXSetEnum(this->hDevice,GX_ENUM_TRIGGER_MODE,GX_TRIGGER_MODE_OFF);

	//自动增益
	status = GXSetInt(hDevice,GX_INT_GAIN,8);
	//曝光时间
	status = GXSetFloat(hDevice,GX_FLOAT_EXPOSURE_TIME,50000);

	//this->ui.Information->setText(tr("OK"));
	this->ui.Information->insertPlainText(tr("added"));
	;
}
}
void daheng_camera_0::on_click_save()
{
	if(cvSaveImage("image.bmp",this->cvimage));
	else
	{QMessageBox::information(this,tr("information kuang"),tr("warning"));}
}
void daheng_camera_0::on_click_capture()
{
	//5:
	//开辟存储空间，为数据采集做准备

	//图像帧数据大小
	int64_t nPayLoadSize = 0;
	//数据格式
	int64_t nPixelFormat = 0;
	//Bayer 格式
	int64_t nPixelColorFilter = 0;
	//图像的宽度和高度
	int64_t Width,Height=0;	

	//获取宽高
	this->status = GXGetInt(this->hDevice,GX_INT_WIDTH,&Width);
	this->status = GXGetInt(this->hDevice,GX_INT_HEIGHT,&Height);
	
	//获取图像buffer大小，下面动态申请内存
	status = GXGetInt(hDevice, GX_INT_PAYLOAD_SIZE, &nPayLoadSize);

	//获取图像数据的类型
	status = GXGetEnum(hDevice, GX_ENUM_PIXEL_FORMAT, &nPixelFormat);
	//是否支持colorfilter
	status = GXGetEnum(hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &nPixelColorFilter);

	//为存储RGB数据开辟空间
	BYTE* pImgRGBBuffer = new BYTE[size_t(Width * Height * 3)];
	//为存储Raw8数据开辟空间
	BYTE* pImgRaw8Buffer = new BYTE[size_t(Width * Height)];
	BYTE* pImgRaw8Buffer2 = new BYTE[size_t(Width * Height)];
	BYTE* pImgRaw8Buffer3 = new BYTE[size_t(Width * Height)];

	//定义GXGetImage的传入参数
    GX_FRAME_DATA frameData;

	//根据获取的图像buffer大小m_nPayLoadSize申请buffer
    frameData.pImgBuf = malloc((size_t)nPayLoadSize);

	//6:
	//发送开始采集命令
    status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_START);   
	//延时10ms,等待采集完毕
	Sleep(10);
	
	//调用GXGetImage 获取数据
	status=GXGetImage(hDevice, &frameData, 100);
	
	//发送停止采集命令
    status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);

	//旋转两次90度=180度
	status = DxRotate90CCW8B((BYTE*)frameData.pImgBuf,pImgRaw8Buffer,Width,Height);
	status = DxRotate90CCW8B(pImgRaw8Buffer,pImgRaw8Buffer2,Height,Width);


	//类型转换
//    ProcessData((BYTE*)frameData.pImgBuf, pImgRGBBuffer,pImgRaw8Buffer,
//					frameData.nWidth, frameData.nHeight,
//					nPixelFormat,nPixelColorFilter); 
	ProcessData(pImgRaw8Buffer2, pImgRGBBuffer,pImgRaw8Buffer3,
					frameData.nWidth, frameData.nHeight,
					nPixelFormat,nPixelColorFilter); 
	
	//清除raw数据的缓存
	delete [] pImgRaw8Buffer;
	delete [] pImgRaw8Buffer2;
	delete [] pImgRaw8Buffer3;

	//7:
	//图像获取成功
    //对图像进行处理...


	this->cvimage=cvCreateImageHeader(cvSize(frameData.nWidth,frameData.nHeight),IPL_DEPTH_8U,3);
	this->cvimage->imageData = (char*) pImgRGBBuffer;

	//8:
	//在qlabel上绘图
	//ui.label->setPixmap(QPixmap::fromImage(ConvertToQImage(cvimage)));
	//this 3 for test
	cvNamedWindow("example1",CV_WINDOW_AUTOSIZE);
	cvShowImage("example1",this->cvimage);
	cvWaitKey(0);
	
	cvDestroyWindow("example1");
	//cvReleaseImage(&this->cvimage);
	//释放资源
	delete [] pImgRGBBuffer;
	free (frameData.pImgBuf);
	
}

void daheng_camera_0::on_open_port()
{
	//store parameters
	struct PortSettings myComSetting = {BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
	//define target pass parameters
	myCom = new Win_QextSerialPort("com1",myComSetting,QextSerialBase::EventDriven);
	myCom->open(QIODevice::WriteOnly);
}

void daheng_camera_0::trigger()
{
	myCom->write("t");
}

//----------------------------------------------------------------------------------
/**
\brief  将相机输出的原始数据转换为RGB数据
\param  pImageBuf  指向图像缓冲区的指针
\param  pImageRGBBuf  指向RGB数据缓冲区的指针
\param  nImageWidth 图像宽
\param  nImageHeight 图像高
\return 无返回值
*/
//----------------------------------------------------------------------------------
void ProcessData(BYTE *pImageBuf, BYTE *pImageRGBBuf,BYTE* m_pImgRaw8Buffer, 
				 int64_t nImageWidth, int64_t nImageHeight,int64_t m_nPixelFormat,int64_t m_nPixelColorFilter)
{
	switch(m_nPixelFormat)
	{
		//当数据格式为12位时，位数转换为4-11
	case GX_PIXEL_FORMAT_BAYER_GR12:
	case GX_PIXEL_FORMAT_BAYER_RG12:
	case GX_PIXEL_FORMAT_BAYER_GB12:
	case GX_PIXEL_FORMAT_BAYER_BG12:
		//将12位格式的图像转换为8位格式
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_4_11);	

		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(m_pImgRaw8Buffer,pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
		   	          DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);		        
		break;
		
		//当数据格式为12位时，位数转换为2-9
	case GX_PIXEL_FORMAT_BAYER_GR10:
	case GX_PIXEL_FORMAT_BAYER_RG10:
	case GX_PIXEL_FORMAT_BAYER_GB10:
	case GX_PIXEL_FORMAT_BAYER_BG10:
		////将12位格式的图像转换为8位格式,有效位数2-9
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_2_9);

		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(m_pImgRaw8Buffer, pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
			          DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);	
		break;
		
	case GX_PIXEL_FORMAT_BAYER_GR8:
	case GX_PIXEL_FORMAT_BAYER_RG8:
	case GX_PIXEL_FORMAT_BAYER_GB8:
	case GX_PIXEL_FORMAT_BAYER_BG8:
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(pImageBuf, pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
			          DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);	
		break;
		
	case GX_PIXEL_FORMAT_MONO12:
		//将12位格式的图像转换为8位格式
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_4_11);	

		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(m_pImgRaw8Buffer, pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
		   	          DX_PIXEL_COLOR_FILTER(NONE), TRUE);		        
		break;
		
	case GX_PIXEL_FORMAT_MONO10:
		//将10位格式的图像转换为8位格式
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_4_11);	

		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(m_pImgRaw8Buffer, pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
			          DX_PIXEL_COLOR_FILTER(NONE), TRUE);		        
		break;
		
	case GX_PIXEL_FORMAT_MONO8:
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(pImageBuf, pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
		          	  DX_PIXEL_COLOR_FILTER(NONE), TRUE);		        
		break;
		
	default:
		break;
	}
}



//iplimage ->>> qtimage
QImage ConvertToQImage(IplImage *iplImg)
{
	QImage img;
	int nChannel=iplImg->nChannels;
	if(nChannel==3)
		{
		cvConvertImage(iplImg,iplImg,CV_CVTIMG_SWAP_RB);
		img = QImage((const unsigned char*)iplImg->imageData,iplImg->width,iplImg->height,QImage::Format_RGB888);
		}
	else if(nChannel==4||nChannel==1)
		{
		img = QImage((const unsigned char*)iplImg->imageData,iplImg->width,iplImg->height,QImage::Format_ARGB32);
		}
	return img;
}
