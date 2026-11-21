# 串口调试助手（Qt5）

基于 Qt5 的串口调试工具，用于和串口设备收发数据。工控、嵌入式场景下的常用工具。

## 功能

- 选择串口、波特率（9600 ~ 115200）
- 打开 / 关闭串口，按钮文字随状态切换
- 发送数据（以 UTF-8 编码写入串口）
- 实时显示接收到的数据（`readyRead` 信号驱动）

## 编译

需要 Qt5 开发库：

```bash
sudo apt install -y qtbase5-dev libqt5serialport5-dev
```

编译：

```bash
qmake serial.pro
make
```

产物为当前目录下的 `serial`。

## 运行

```bash
./serial
```

## 没有真实串口时如何测试

用 `socat` 造一对互通的虚拟串口：

```bash
sudo apt install -y socat
socat -d -d pty,raw,echo=0,link=/tmp/vserial1 pty,raw,echo=0,link=/tmp/vserial2
```

这条命令会创建两个固定路径的虚拟串口（`/tmp/vserial1` 和 `/tmp/vserial2`），
它们互相连通。**socat 需保持运行，关闭则虚拟串口消失。**

测试步骤：

1. 程序中选中 `/tmp/vserial1`，点击「打开」
2. 另开一个终端，模拟设备发数据：

```bash
echo "hello" > /tmp/vserial2
```

3. 程序接收区应显示 `hello`

## 实现要点

| 技术点 | 说明 |
|---|---|
| `QSerialPort` | 串口读写，继承自 `QIODevice` |
| `readyRead` 信号 | 串口收到数据时由 Qt 发出，驱动界面更新 |
| 信号槽 + lambda | 三个 `connect` 覆盖全部交互逻辑 |
| 布局嵌套 | `QVBoxLayout` 内嵌两个 `QHBoxLayout` |

## 已知不足

- 串口列表为硬编码（`/tmp/vserial1`），未使用 `QSerialPortInfo` 自动枚举
- 仅支持 UTF-8 文本，未提供 Hex 收发模式
- 未显示收发字节计数
