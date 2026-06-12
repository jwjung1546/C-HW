#ifndef CANDLE_DATA_H
#define CANDLE_DATA_H

#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

class CandleData {
private:
    string market = "";
    double openingPrice = 0.0;
    double highPrice = 0.0;
    double lowPrice = 0.0;
    double tradePrice = 0.0;

public:
    CandleData() {}
    CandleData(string m, double o, double h, double l, double t)
        : market(m), openingPrice(o), highPrice(h), lowPrice(l), tradePrice(t) {}

    string getMarket() { return market; }
    double getOpeningPrice() { return openingPrice; }
    double getHighPrice() { return highPrice; }
    double getLowPrice() { return lowPrice; }
    double getTradePrice() { return tradePrice; }
};


class CryptoAsset {
private:
    string marketCode;
    double finalScore;
    CandleData priceHistory[15]; 

public:
    CryptoAsset() : marketCode(""), finalScore(0.0) {}

    CryptoAsset(string code, double score, CandleData* history)
        : marketCode(code), finalScore(score) {
        for (int i = 0; i < 15; ++i) {
            priceHistory[i] = history[i];
        }
    }

    double getScore() { return finalScore; }
    string getCode() { return marketCode; }


    string getKoreanName() {
        if (marketCode == "KRW-BTC")   return "비트코인";
        if (marketCode == "KRW-USDT")  return "테더(USDT)";
        if (marketCode == "KRW-ETH")   return "이더리움";
        if (marketCode == "KRW-DOGE")  return "도지코인";
        if (marketCode == "KRW-XRP")   return "리플";
        if (marketCode == "KRW-SOL")   return "솔라나";
        if (marketCode == "KRW-TRX")   return "트론";
        if (marketCode == "KRW-ATOM")  return "코스모스"; 
        return marketCode;
    }

    void printQuantReport() {
        double sum5 = 0.0, sum10 = 0.0;
        for (int i = 0; i < 5; ++i)  sum5 += priceHistory[i].getTradePrice();
        for (int i = 0; i < 10; ++i) sum10 += priceHistory[i].getTradePrice();

        double ma5 = sum5 / 5.0;
        double ma10 = sum10 / 10.0;

        double changePercent = ((ma5 - ma10) / ma10) * 100.0;

        cout << fixed << setprecision(2);
        cout << "   - 5시간 이동평균선  : " << ma5 << " 원" << endl;
        cout << "   - 10시간 이동평균선 : " << ma10 << " 원" << endl;

        if (changePercent > 0) {
            cout << "   - 두 평균선 변화량  : +" << changePercent << " % (5일선이 위로 이격)" << endl;
        } else if (changePercent < 0) {
            cout << "   - 두 평균선 변화량  : " << changePercent << " % (5일선이 아래로 이격)" << endl;
        } else {
            cout << "   - 두 평균선 변화량  : 0.00 % (두 평균선 수치 일치)" << endl;
        }
        cout << "   -----------------------------------------------------------------" << endl;

        if (ma5 > ma10) {
            cout << "   - 최종 추세 판정    : [추천] (상승 추세 진입 완료)" << endl;
        } else {
            cout << "   - 최종 추세 판정    : [비추천] (하락 또는 역배열 진행 중)" << endl;
        }
    }
};

#endif
