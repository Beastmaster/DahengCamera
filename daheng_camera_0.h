#ifndef DAHENG_CAMERA_0_H
#define DAHENG_CAMERA_0_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

//head files for Camera function
#include <GxIAPI.h>
//head files for data process
#include <DxImageProc.h>

//header for opencv
#include "highgui.h"
#include "opencv2/core/core.hpp"

//header for serialport
#include "serialport/qextserialbase.h"
#include "serialport/qextserialport.h"
#include "serialport/win_qextserialport.h"

class daheng_camera_0 : public QMainWindow
{
	Q_OBJECT

public:
	daheng_camera_0(QWidget *parent = 0);
	~daheng_camera_0();

public slots:
	void on_mode_change();
	void on_expose_change();
	void on_gain_change();
	void on_balance_change();
	void on_color_change();
	void on_click_init();
	void on_click_save();
	void on_click_capture();
	void on_width_changed();
	void on_height_changed();

	void on_open_port();


private:
	Ui::MainWindow ui;
	
	//函数返回状态
	GX_STATUS status;
	//设备个数
	uint32_t nDeviceNum;
	//设备句柄
	GX_DEV_HANDLE hDevice;

	//cvimage
	IplImage* cvimage;

	Win_QextSerialPort* myCom;
	void trigger();
};

#endif // DAHENG_CAMERA_0_H

void ProcessData(BYTE *pImageBuf, BYTE *pImageRGBBuf,BYTE* m_pImgRaw8Buffer,int64_t nImageWidth, 
				 int64_t nImageHeight,int64_t m_nPixelFormat,int64_t m_nPixelColorFilter);


QImage ConvertToQImage(IplImage *iplImg);