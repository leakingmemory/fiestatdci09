//
// Created by sigsegv on 5/7/25.
//

#include "Fiesta.h"
#include "SerialInterface.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>
#include "seedkey.h"

class BusComException : public std::exception {
private:
    std::string err;
public:
    BusComException(const std::string &err) : err(err) {}
    const char * what() const noexcept override;
};

const char *BusComException::what() const noexcept {
    return err.c_str();
}

Fiesta::Fiesta() {
    serialInterface = std::make_shared<SerialInterface>("/dev/ttyUSB0");
    serialInterface->SetSpeed(SerialSpeed::S38400);
    serialInterface->CommitAttributes();
    {
        auto retry = 10;
        while (true) {
            try {
                Drain();
                Reset();
                break;
            } catch (...) {
                if (--retry == 0) {
                    throw;
                }
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(2s);
                serialInterface->CommitAttributes();
                std::this_thread::sleep_for(1s);
            }
        }
    }
    HeadersOn();
    auto voltage = GetVoltage();
    std::cout << "Port voltage " << voltage << "\n";
    std::cout << "Bus scan on MS-CAN\n";
    SimpleCommand("AT SP6");
    SimpleCommand("AT SH 7DF");
    SimpleCommand("AT CM 000"); // CM(mask)=000, CF(filter)=whatever - Every response visible
    serialInterface->Write("0100\r");
    std::cout << "Responding modules:\n";
    std::string buf{};
    std::string ln{};
    while (WaitForLine(buf, ln, 10000)) {
        std::cout << ln << "\n";
    }
    SimpleCommand("AT SH 7E0");
    serialInterface->Write("3E 00\r");
    while (WaitForLine(buf, ln, 2000)) {
        std::cout << ln << "\n";
    }
    std::cout << "Calib ID:\n";
    serialInterface->Write("09 04\r"); // Extended session
    while (WaitForLine(buf, ln, 2000)) {
        std::cout << ln << "\n";
    }
    std::cout << "Upgrade session:\n";
    serialInterface->Write("10 03\r"); // Extended session
    while (WaitForLine(buf, ln, 2000)) {
        std::cout << ln << "\n";
    }
    // Unrelated notes ->
    // ABS:
    // 8V51-2C405-AE
    // E-A426G
    // <- Unrelated notes
    std::cout << "Security request:\n";
    uint64_t mixkey = 1309871;
    while (true) {
        serialInterface->Write("27 01\r"); // Extended session
        if (WaitForLine(buf, ln, 2000)) {
            std::cout << ln << "\n";
            auto iterator = ln.begin();
            std::string addr{};
            if (iterator != ln.end() && *iterator == '>') {
                iterator = ln.erase(iterator);
            }
            while (iterator != ln.end() &&
                   ((*iterator >= '0' && *iterator <= '9') || (*iterator >= 'A' && *iterator <= 'F') ||
                    (*iterator >= 'a' && *iterator <= 'f'))) {
                addr.append(&(*iterator), 1);
                iterator = ln.erase(iterator);
            }
            while (iterator != ln.end() && (*iterator == ' ')) {
                iterator = ln.erase(iterator);
            }
            auto resp = DecodeHex(ln);
            if (resp.size() == 6 && resp[0] == 5 && resp[1] == 0x67 && resp[2] == 1) {
                auto seed = (((uint32_t) ((unsigned char) resp[3])) << 16) + (((uint32_t) ((unsigned char) resp[4])) << 8) + ((uint32_t) ((unsigned char) resp[5]));
                std::cout << "Mixkey: " << mixkey << "\n";
                std::cout << "Seed: " << seed << "\n";
                auto key = SeedKey(seed, mixkey);
                std::cout << "Key: " << key << "\n";
                const char *keyraw = (const char *) &key;
                std::string keybytes{"   "};
                keybytes[0] = keyraw[2];
                keybytes[1] = keyraw[1];
                keybytes[2] = keyraw[0];
                auto keystr = ToHex(keybytes);
                std::string lnw{"27 02 "};
                lnw.append(keystr);
                std::cout << "Responding: " << lnw << "\n";
                lnw.append("\r");
                serialInterface->Write(lnw);
                if (WaitForLine(buf, ln, 2000)) {
                    std::cout << ln << "\n";
                    if (!ln.empty()) {
                        std::cout << "Hit!: " << seed << " -> " << key << "\n";
                        break;
                    }
                }
                ++mixkey;
            }
        } else {
            break;
        }
    }

    std::cout << "Whatever:\n";
    serialInterface->Write("22 00 01\r");
    while (WaitForLine(buf, ln, 60000)) {
        std::cout << ln << "\n";
    }

}

constexpr char tohexch(int ch) {
    if (ch < 10) {
        if (ch < 0) {
            return '?';
        }
        return '0' + ch;
    } else if (ch > 15) {
        return '?';
    }
    return 'A' + (ch - 10);
}

static_assert(tohexch(-1) == '?');
static_assert(tohexch(0) == '0');
static_assert(tohexch(1) == '1');
static_assert(tohexch(2) == '2');
static_assert(tohexch(3) == '3');
static_assert(tohexch(4) == '4');
static_assert(tohexch(5) == '5');
static_assert(tohexch(6) == '6');
static_assert(tohexch(7) == '7');
static_assert(tohexch(8) == '8');
static_assert(tohexch(9) == '9');
static_assert(tohexch(10) == 'A');
static_assert(tohexch(11) == 'B');
static_assert(tohexch(12) == 'C');
static_assert(tohexch(13) == 'D');
static_assert(tohexch(14) == 'E');
static_assert(tohexch(15) == 'F');
static_assert(tohexch(16) == '?');

constexpr int hexch(char ch) {
    if (ch >= 'a' && ch <= 'f') {
        return 10 + ch - 'a';
    }
    if (ch >= 'A' && ch <= 'F') {
        return 10 + ch - 'A';
    }
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    return -1;
}

static_assert(hexch('0') == 0);
static_assert(hexch('1') == 1);
static_assert(hexch('2') == 2);
static_assert(hexch('3') == 3);
static_assert(hexch('4') == 4);
static_assert(hexch('5') == 5);
static_assert(hexch('6') == 6);
static_assert(hexch('7') == 7);
static_assert(hexch('8') == 8);
static_assert(hexch('9') == 9);
static_assert(hexch('a') == 10);
static_assert(hexch('b') == 11);
static_assert(hexch('c') == 12);
static_assert(hexch('d') == 13);
static_assert(hexch('e') == 14);
static_assert(hexch('f') == 15);
static_assert(hexch('A') == 10);
static_assert(hexch('B') == 11);
static_assert(hexch('C') == 12);
static_assert(hexch('D') == 13);
static_assert(hexch('E') == 14);
static_assert(hexch('F') == 15);
static_assert(hexch('G') == -1);

std::string Fiesta::ToHex(const std::string &bytes) {
    std::string hex{};
    for (auto ch : bytes) {
        auto ch0 = tohexch((ch >> 4) & 0xF);
        auto ch1 = tohexch(ch & 0xF);
        hex.append(&ch0, 1);
        hex.append(&ch1, 1);
        hex.append(" ");
    }
    if (hex.size() > 0) {
        hex.resize(hex.size() - 1);
    }
    return hex;
}

std::string Fiesta::DecodeHex(const std::string &ihex) {
    std::string hex{ihex};
    {
        auto iterator = hex.begin();
        while (iterator != hex.end()) {
            if (*iterator == ' ') {
                hex.erase(iterator);
                continue;
            }
            ++iterator;
        }
    }
    {
        std::string hhex{hex};
        std::transform(hhex.begin(), hhex.end(), hhex.begin(), [] (auto ch) { return std::tolower(ch); });
        if (hhex == "nodata") {
            return "";
        }
    }
    std::string bytes{};
    {
        auto iterator = hex.begin();
        while (iterator != hex.end()) {
            auto hvu = hexch(*iterator);
            ++iterator;
            if (iterator == hex.end()) {
                throw BusComException("Hex bytes, count error, pair");
            }
            auto hvl = hexch(*iterator);
            if (hvu < 0 || hvl < 0) {
                throw BusComException("Hex bytes invalid");
            }
            ++iterator;
            char hv = (char) ((hvu << 4) + hvl);
            bytes.append((const char *) &hv, 1);
        }
    }
    return bytes;
}

void Fiesta::Drain() {
    while (serialInterface->Read() != "") {
    }
}

void Fiesta::Reset() {
    Drain();
    serialInterface->Write("ATZ\r");
    std::string buf{};
    WaitForLine(buf, deviceId, 5000);
    if (!deviceId.starts_with("ELM")) {
        WaitForLine(buf, deviceId, 2000);
    }
    WaitForPrompt(buf, 2000);
    Drain();
    buf = "";
    serialInterface->Write("ATE0\r");
    WaitForPrompt(buf, 2000);
    Drain();
    buf = "";
    serialInterface->Write("ATL0\r");
    WaitForPrompt(buf, 2000);
    Drain();
    buf = "";
    EchoOff();
    LinefeedsOff();
}

void Fiesta::SimpleCommand(const std::string &cmd) {
    {
        std::string str{cmd};
        str.append("\r");
        serialInterface->Write(str);
    }
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 200)) {
        throw BusComException("Cmd no resp");
    }
    if (ln != "OK") {
        throw BusComException("Cmd error");
    }
    WaitForPrompt(buf, 1000);
}

void Fiesta::EchoOff() {
    serialInterface->Write("ATE0\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 1000)) {
        throw BusComException("Echo off no resp");
    }
    if (ln == "OK") {
        WaitForPrompt(buf, 1000);
        return;
    }
    if (ln != "ATE0") {
        throw BusComException("Echo off error");
    }
    if (!WaitForLine(buf, ln, 1000)) {
        throw BusComException("Echo off no resp");
    }
    if (ln != "OK") {
        throw BusComException("Echo off error");
    }
    WaitForPrompt(buf, 1000);
}

bool Fiesta::WaitForLine(std::string &buf, std::string &ln, int timeout_ms) const {
    auto timer = std::chrono::system_clock::now();
    {
        std::chrono::milliseconds duration{static_cast<int64_t>(timeout_ms)};
        timer += duration;
    }
    while (true) {
        auto pos = buf.find('\r');
        auto delim = '\r';
        if (pos != std::string::npos) {
            auto lfpos = buf.find('\n');
            if (lfpos != std::string::npos && lfpos < pos) {
                pos = lfpos;
                delim = '\n';
            }
        } else {
            pos = buf.find('\n');
            delim = '\n';
        }
        if (pos != std::string::npos) {
            ln = buf.substr(0, pos);
            auto remaining = buf.substr(pos + 1);
            if (pos < (buf.size() - 1)) {
                auto ch = remaining.at(0);
                if ((ch == '\n' || ch == '\r') && ch != delim) {
                    auto r = remaining.substr(1);
                    remaining = r;
                }
            }
            buf = remaining;
            return true;
        }
        if (std::chrono::system_clock::now() > timer) {
            return false;
        }
        auto str = serialInterface->Read();
        buf.append(str);
    }
}

std::string Fiesta::WaitForPrompt(std::string &buf, int timeout_ms) const {
    auto timer = std::chrono::system_clock::now();
    {
        std::chrono::milliseconds duration{static_cast<int64_t>(timeout_ms)};
        timer += duration;
    }
    std::string msg{};
    while (true) {
        if (buf.ends_with("\r>")) {
            auto m = buf.substr(0, buf.size() - 2);
            buf = "";
            msg.append(m);
            return msg;
        } else {
            msg.append(buf);
            buf = "";
        }
        if (std::chrono::system_clock::now() > timer) {
            buf = msg;
            return "";
        }
        auto str = serialInterface->Read();
        buf.append(str);
    }
}

int Fiesta::GetVoltage() {
    serialInterface->Write("AT RV\r");
    std::string buf{};
    std::string ln{};
    if (!WaitForLine(buf, ln, 100)) {
        throw BusComException("Voltage no resp (adapter)");
    }
    size_t parse;
    float volt = std::stof(ln, &parse);
    if (parse == 0) {
        throw BusComException("Voltage wrong resp (adapter)");
    }
    auto remain = ln.substr(parse);
    if (remain != "v" && remain != "V") {
        throw BusComException("Voltage wrong resp (adapter)");
    }
    WaitForPrompt(buf, 1000);
    return volt;
}

