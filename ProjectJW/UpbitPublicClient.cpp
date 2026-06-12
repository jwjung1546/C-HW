#include "UpbitPublicClient.h"
#include <iostream>
#include <windows.h>
#include <wininet.h> 

#pragma comment(lib, "wininet.lib")

using namespace std;

UpbitPublicClient::UpbitPublicClient() {
    CoinList[0] = "KRW-BTC";  CoinList[1] = "KRW-USDT";
    CoinList[2] = "KRW-ETH";  CoinList[3] = "KRW-DOGE";
    CoinList[4] = "KRW-XRP";  CoinList[5] = "KRW-SOL";
    CoinList[6] = "KRW-TRX";  CoinList[7] = "KRW-ATOM"; 
}

string* UpbitPublicClient::getCoinList() {
    return CoinList;
}

void UpbitPublicClient::fetchDayCandles(string marketCode, int count, CandleData* outHistory) {

    for (int i = 0; i < count; ++i) {
        outHistory[i] = CandleData(marketCode, 0.0, 0.0, 0.0, 0.0);
    }

    string readBuffer = "";

    string targetUrl = "https://api.upbit.com/v1/candles/days?market=" + marketCode + "&count=" + to_string(count);

    HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hConnect = InternetOpenUrlA(hInternet, targetUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hConnect) {
            char buffer[2048];
            DWORD bytesRead;
            while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                readBuffer += buffer;
            }
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }

    if (readBuffer.empty()) return;

    size_t offset = 0;
    int index = 0;
    
    while (index < count) {
        size_t openIdx  = readBuffer.find("\"opening_price\":", offset);
        size_t tradeIdx = readBuffer.find("\"trade_price\":", offset);
        size_t highIdx  = readBuffer.find("\"high_price\":", offset);
        size_t lowIdx   = readBuffer.find("\"low_price\":", offset);

        if (openIdx == string::npos || tradeIdx == string::npos) break;

        try {
            double op = stod(readBuffer.substr(openIdx + 16, readBuffer.find(",", openIdx) - (openIdx + 16)));
            double tp = stod(readBuffer.substr(tradeIdx + 14, readBuffer.find(",", tradeIdx) - (tradeIdx + 14)));
            double hp = stod(readBuffer.substr(highIdx + 13, readBuffer.find(",", highIdx) - (highIdx + 13)));
            double lp = stod(readBuffer.substr(lowIdx + 12, readBuffer.find(",", lowIdx) - (lowIdx + 12)));

            outHistory[index] = CandleData(marketCode, op, hp, lp, tp);
            index++;
        }
        catch (...) {
            break;
        }

        offset = tradeIdx + 30;
    }

    if (index > 0 && index < count) {
        for (int i = index; i < count; ++i) {
            outHistory[i] = outHistory[index - 1];
        }
    }
}
