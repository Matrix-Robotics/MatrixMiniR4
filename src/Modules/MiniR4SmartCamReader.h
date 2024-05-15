#ifndef MINIR4_SMART_CAM_READER
#define MINIR4_SMART_CAM_READER

#include <Arduino.h>

class MiniR4SmartCamReader
{
private:
public:
    MiniR4SmartCamReader() {}
    ~MiniR4SmartCamReader() {}

    bool hasReceivedValidData = false;   // 记录是否收到过有效数据

    void Begin(void) { Serial1.begin(115200); }

    int SmartCamReader(unsigned int* data, unsigned int timeout = 500)
    {
        byte         length, check;     // 声明局部变量
        byte         data_buffer[40];   // 数据缓冲区
        unsigned int system_time;       // 系统时间

        system_time = millis();   // 获取当前系统时间
        // 等待至少两个字节的数据或超时
        while ((millis() - system_time < timeout) && (Serial1.available() < 2));

        if (Serial1.available() >= 2) {     // 如果至少有两个字节的数据
            if (Serial1.read() == 0xAA) {   // 检查头字节
                length = Serial1.read();    // 读取数据长度
                check  = 0xAA ^ length;     // 初始化校验位

                system_time = millis();   // 重置系统时间
                // 等待足够多的数据或超时
                while ((millis() - system_time < timeout) &&
                       (Serial1.available() < length * 2 + 1)) {}

                if (Serial1.available() >= length * 2 + 1) {   // 如果数据足够
                    for (unsigned char n = 0; n < length * 2; n++) {
                        data_buffer[n] = Serial1.read();           // 读取数据到缓冲区
                        check          = check ^ data_buffer[n];   // 更新校验位
                    }
                    if (check == Serial1.read()) {      // 如果校验成功
                        while (Serial1.available()) {   // 清空缓冲区
                            Serial1.read();
                        }
                        for (unsigned char n = 0; n < length; n++) {   // 解码数据
                            data[n] = (data_buffer[n * 2 + 1] << 8) + data_buffer[n * 2];
                        }
                        hasReceivedValidData = true;
                        return length;   // 返回数据长度
                    } else {
                        hasReceivedValidData = true;
                        return -3;   // 校验失败
                    }
                } else {
                    hasReceivedValidData = true;
                    return -2;   // 数据不完整
                }
            }
        } else {
            if (!hasReceivedValidData) {
                Serial1.write("start");
            }
            return -1;   // 超时或没有数据
        }
    }
};

#endif
