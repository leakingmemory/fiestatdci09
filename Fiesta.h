//
// Created by sigsegv on 5/7/25.
//

#ifndef FIESTATDCI09_FIESTA_H
#define FIESTATDCI09_FIESTA_H

#include <memory>

class ElmDevice;

enum class FiestaDevType {
    TTY, BLE
};

class Fiesta {
private:
    std::shared_ptr<ElmDevice> serialInterface;
    std::string deviceId;
public:
    Fiesta() = delete;
    Fiesta(FiestaDevType devType, const std::string &device);
    Fiesta(std::shared_ptr<ElmDevice> elmDevice);
    std::string ToHex(const std::string &bytes);
    std::string DecodeHex(const std::string &ihex);
    void Drain();
    void Reset();
    void EchoOff();
    void SimpleCommand(const std::string &);
    void HeadersOn() {
        SimpleCommand("ATH1");
    }
    void LinefeedsOff() {
        SimpleCommand("ATL0");
    }
    bool WaitForLine(std::string &buf, std::string &ln, int timeout_ms) const;
    std::string WaitForPrompt(std::string &buf, int timeout_ms) const;
    int GetVoltage();
};


#endif //FIESTATDCI09_FIESTA_H
