#ifndef CANDLE_DATA_H
#define CANDLE_DATA_H

#include <string>
#include <vector>
using namespace std;

struct CandleData {
    string market = "";
    double openingPrice = 0.0;
    double highPrice = 0.0;
    double lowPrice = 0.0;
    double tradePrice = 0.0;
};

class CryptoAsset {
private:
    string marketCode;
    double finalScore;

public:
    CryptoAsset(string code, double score) : marketCode(code), finalScore(score) {}

    double getScore() const { return finalScore; }
    string getCode() const { return marketCode; }



    string getKoreanName() const {
        if (marketCode == "KRW-BTC")   return "비트코인";
        if (marketCode == "KRW-USDT")  return "테더";
        if (marketCode == "KRW-ETH")   return "이더리움";
        if (marketCode == "KRW-DOGE")  return "도지코인";
        if (marketCode == "KRW-XRP")   return "리플";
        if (marketCode == "KRW-SOL")   return "솔라나";
        if (marketCode == "KRW-TRX")   return "트론";
        if (marketCode == "KRW-BRETT") return "브렛";
        return marketCode;
    }
};
#endif