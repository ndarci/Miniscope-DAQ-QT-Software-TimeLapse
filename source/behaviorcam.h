#ifndef BEHAVIORCAM_H
#define BEHAVIORCAM_H

#include <QObject>
#include <QThread>
#include <QSemaphore>
#include <QTimer>
#include <QAtomicInt>
#include <QJsonObject>
#include <QQuickView>
#include <QMap>
#include <QVector>
#include <QQuickItem>
#include <QVariant>

#include "videostreamocv.h"
#include "videodisplay.h"
#include "newquickview.h"
#include <opencv2/opencv.hpp>

#define PROTOCOL_I2C            -2
#define PROTOCOL_SPI            -3
#define SEND_COMMAND_VALUE_H    -5
#define SEND_COMMAND_VALUE_L    -6
#define SEND_COMMAND_VALUE      -6
#define SEND_COMMAND_VALUE_H16  -7
#define SEND_COMMAND_VALUE_H24  -8
#define SEND_COMMAND_VALUE2_H   -9
#define SEND_COMMAND_VALUE2_L   -10
#define SEND_COMMAND_ERROR      -20

#define FRAME_BUFFER_SIZE   128

class BehaviorCam : public QObject
{
    Q_OBJECT
public:
    explicit BehaviorCam(QObject *parent = nullptr, QJsonObject ucBehavCam = QJsonObject());
    void createView();
    void connectSnS();
    void parseUserConfigBehavCam();
    void sendInitCommands();
    QString getCompressionType();
    cv::Mat* getFrameBufferPointer(){return frameBuffer;}
    qint64* getTimeStampBufferPointer(){return timeStampBuffer;}
    int getBufferSize() {return FRAME_BUFFER_SIZE;}
    QSemaphore* getFreeFramesPointer(){return freeFrames;}
    QSemaphore* getUsedFramesPointer(){return usedFrames;}
    QAtomicInt* getAcqFrameNumPointer(){return m_acqFrameNum;}
//    QAtomicInt* getDAQFrameNumPointer() { return m_daqFrameNum; }
    QString getDeviceName() {return m_deviceName;}
    int* getROI() { return m_roiBoundingBox; }



signals:
    // TODO: setup signals to configure camera in thread
    void setPropertyI2C(long preambleKey, QVector<quint8> packet);
    void onPropertyChanged(QString devieName, QString propName, QVariant propValue);
    void sendMessage(QString msg);
    void takeScreenShot(QString type);

    // THINK THIS IS UNUSED!!
    void newFrameAvailable(QString name, int frameNum);

    // THIS STAYS WITH BEHAV CLASS!!
    void openCamPropsDialog();

public slots:
    void sendNewFrame();
    void testSlot(QString, double);
    void handlePropChangedSignal(QString type, double displayValue, double i2cValue, double i2cValue2);
    void handleTakeScreenShotSignal();
    void close();
    void handleInitCommandsRequest();
    void handleSaturationSwitchChanged(bool checked);
    void handleSetRoiClicked();
    // Setting new ROI
    void handleNewROI(int leftEdge, int topEdge, int width, int height);


    // LEAVE THIS!!!!
    void handleCamPropsClicked() { emit openCamPropsDialog();}

    // NOT SURE WHAT TO DO WITH THESE!!
    // Camera calibration slots
    void handleCamCalibClicked();
    void handleCamCalibStart();
    void handleCamCalibQuit();


private:
    void getBehavCamConfig(QString deviceType);
    void configureBehavCamControls();
    QVector<QMap<QString, int>> parseSendCommand(QJsonArray sendCommand);
    int processString2Int(QString s);

    int m_camConnected;
    NewQuickView *view;
    VideoStreamOCV *behavCamStream;
    QThread *videoStreamThread;
    cv::Mat frameBuffer[FRAME_BUFFER_SIZE];
    cv::Mat tempFrame;
    qint64 timeStampBuffer[FRAME_BUFFER_SIZE];
    QSemaphore *freeFrames;
    QSemaphore *usedFrames;
    QObject *rootObject;
    VideoDisplay *vidDisplay;
    QTimer *timer;
    int m_previousDisplayFrameNum;
    QAtomicInt *m_acqFrameNum;
    QAtomicInt *m_daqFrameNum;

    // User Config parameters
    QJsonObject m_ucBehavCam;
    int m_deviceID;
    QString m_deviceName;
    QString m_deviceType;


    QJsonObject m_cBehavCam; // Consider renaming to not confuse with ucMiniscopes
    QMap<QString,QVector<QMap<QString, int>>> m_controlSendCommand;

//    bool m_streamHeadOrientationState;
    QString m_compressionType;

    // Handle MiniCAM stuff
    bool isMiniCAM;



    // Camera Calibration Vars
    bool m_camCalibWindowOpen;
    bool m_camCalibRunning;


    // CAN GET RID OF ONCE IN VIDEODEVICE!!!
    // ROI
    bool m_roiIsDefined;
    int m_roiBoundingBox[4]; // left, top, width, height


};

#endif // BEHAVIORCAM_H
