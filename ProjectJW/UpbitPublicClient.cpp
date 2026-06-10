#define _HAS_STD_BYTE 0

#include "UpbitPublicClient.h"
#include <iostream>
#include <windows.h>
#include <wininet.h> 

#pragma comment(lib, "wininet.lib")

using namespace std;

UpbitPublicClient::UpbitPublicClient() {
    tickerList = {
        "KRW-BTC", "KRW-USDT", "KRW-ETH", "KRW-DOGE",
        "KRW-XRP", "KRW-SOL", "KRW-TRX", "KRW-BRETT"
    };
}

const vector<string>& UpbitPublicClient::getTickerList() const {
    return tickerList;
}

vector<CandleData> UpbitPublicClient::fetchDayCandles(string marketCode, int count) {
    vector<CandleData> candleHistory;
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

    if (readBuffer.empty()) {
        return candleHistory;
    }

    size_t offset = 0;
    while (true) {
        size_t openIdx = readBuffer.find("\"opening_price\":", offset);
        size_t tradeIdx = readBuffer.find("\"trade_price\":", offset);
        size_t highIdx = readBuffer.find("\"high_price\":", offset);
        size_t lowIdx = readBuffer.find("\"low_price\":", offset);

        if (openIdx == string::npos || tradeIdx == string::npos) {
            break;
        }

        CandleData day;
        day.market = marketCode;

        day.openingPrice = stod(readBuffer.substr(openIdx + 16, readBuffer.find(",", openIdx) - (openIdx + 16)));
        day.tradePrice = stod(readBuffer.substr(tradeIdx + 14, readBuffer.find(",", tradeIdx) - (tradeIdx + 14)));
        day.highPrice = stod(readBuffer.substr(highIdx + 13, readBuffer.find(",", highIdx) - (highIdx + 13)));
        day.lowPrice = stod(readBuffer.substr(lowIdx + 12, readBuffer.find(",", lowIdx) - (lowIdx + 12)));

        candleHistory.push_back(day);
        offset = tradeIdx + 30;
    }

    return candleHistory;
}