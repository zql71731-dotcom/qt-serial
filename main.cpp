#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QDebug>
#include <QSerialPort>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.resize(600, 400);
    window.setWindowTitle("串口调试助手");
    // ===== ③ 创建控件 =====
    // 下拉框：选串口号，波特率
    QComboBox *portBox = new QComboBox(&window);
    QComboBox *baudBox = new QComboBox(&window);

    // 按钮：打开串口，发送
    QPushButton *openBtn = new QPushButton("打开", &window);
    QPushButton *sendBtn = new QPushButton("发送", &window);

    // 多行文本框：显示串口收到的数据
    QTextEdit *recvArea = new QTextEdit(&window);

    // 单行输入框：让用户输入要发送的内容
    QLineEdit *sendEdit = new QLineEdit(&window);

    //  负责真正和设备收发数据
    QSerialPort *serial = new QSerialPort(&window);

    // ===== ④ 配置控件 =====
    // 下拉框功能，串口号，波特率
    portBox->addItem("/tmp/vserial1");
    baudBox->addItem("9600");
    baudBox->addItem("19200");
    baudBox->addItem("38400");
    baudBox->addItem("57600");
    baudBox->addItem("115200");

    // 多行文本框设置为只读
    recvArea->setReadOnly(true);

    // 单行输入框，输入要发送的内容占位
    sendEdit->setPlaceholderText("输入要发送的内容");

    // ===== ⑤ 布局 =====
    // 顶部一行：串口 + 波特率 + 打开按钮
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(portBox);
    topLayout->addWidget(baudBox);
    topLayout->addWidget(openBtn);

    // 底部一行：发送框 + 发送按钮
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(sendEdit);
    bottomLayout->addWidget(sendBtn);

    // 主布局（垂直）：顶部 → 中间接收区 → 底部
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(recvArea);
    mainLayout->addLayout(bottomLayout);

    // ===== ⑥ 信号槽（这里要用 serial）=====
    QObject::connect(openBtn, &QPushButton::clicked, [&]() {

        if (serial->isOpen()) {
        // TODO 1：已经打开 → 关闭串口 + 按钮文字改回 "打开" + 打印日志
        serial->close();
        openBtn->setText("打开");
        qDebug() << "串口已关闭";
        } else {
        // TODO 2：设置端口名（从 portBox 取）
        serial->setPortName(portBox->currentText());
        // TODO 3：设置波特率（从 baudBox 取）
        serial->setBaudRate(baudBox->currentText().toInt());
        // TODO 4：打开串口，判断成功还是失败
        //         成功 → 按钮文字改成 "关闭" + 打印日志
            if (serial->open(QIODevice::ReadWrite)) {
                openBtn->setText("关闭");
                qDebug() << "窗口已打开" << portBox->currentText();
            } else {
        //         失败 → 打印 serial->errorString
                qDebug() << "打开失败:" << serial->errorString();  // 失败原因
            }
            
        }

    });

    QObject::connect(sendBtn, &QPushButton::clicked, [&]() {

        if (!serial->isOpen()) {
            qDebug() << "串口没打开，不能发送";
            return;
        }

        QByteArray data = sendEdit->text().toUtf8();   // 取出文字，转成字节
        serial->write(data);                           // 写进串口
        qDebug() << "发送:" << data;

    });
    
    QObject::connect(serial, &QSerialPort::readyRead, [&]() {

        QByteArray data = serial->readAll();       // 把收到的数据全取出来
        recvArea->append(QString::fromUtf8(data));  // 转成文字，追加到显示区
        qDebug() << "收到:" << data;

    });
    window.show();
    return app.exec();
}
