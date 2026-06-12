#include "TradingAnalyzer.h"
#include "UpbitPublicClient.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <windows.h> 

using namespace std;


double TradingAnalyzer::minMaxScale(double X, double X_min, double X_max) {
    if (abs(X_max - X_min) < 1e-4 || X_max <= X_min) {
        return 0.5; 
    }
    double result = (X - X_min) / (X_max - X_min);
    if (result < 0.0 || isnan(result)) return 0.0;
    if (result > 1.0) return 1.0;
    return result;
}

void TradingAnalyzer::processAllMarkets(UpbitPublicClient& client, CryptoAsset* outResults) {
    double rawTrendScores[8] = { 0 };
    double rawVolScores[8] = { 0 };
    CandleData allHistories[8][15];

    string* markets = client.getCoinList();

    for (int i = 0; i < 8; ++i) {
        Sleep(100); 

        client.fetchDayCandles(markets[i], 15, allHistories[i]);

        double sum5 = 0.0, sum10 = 0.0;
        for (int j = 0; j < 5; ++j)  sum5 += allHistories[i][j].getTradePrice();
        for (int j = 0; j < 10; ++j) sum10 += allHistories[i][j].getTradePrice();

        double ma5 = sum5 / 5.0;
        double ma10 = sum10 / 10.0;
        
  
        double trendScore = 0.0;
        if (ma10 > 1e-6 && ma5 > ma10) {
            trendScore = (ma5 - ma10) / ma10;
        }
        rawTrendScores[i] = trendScore;


        double prevHigh   = allHistories[i][1].getHighPrice();
        double prevLow    = allHistories[i][1].getLowPrice();
        double todayOpen  = allHistories[i][0].getOpeningPrice();
        double todayClose = allHistories[i][0].getTradePrice();
        double todayHigh  = allHistories[i][0].getHighPrice();

        double range = prevHigh - prevLow;
        double targetPrice = todayOpen + (range * k);


        double volatilityScore = 0.0;
        if (targetPrice > 1e-6 && todayClose > 1e-6 && todayHigh >= targetPrice) {
            volatilityScore = log(todayClose / targetPrice);
            if (volatilityScore < 0.0 || isnan(volatilityScore)) {
                volatilityScore = 0.0;
            }
        }
        rawVolScores[i] = volatilityScore;
    }

    double maxTrend = rawTrendScores[0], minTrend = rawTrendScores[0];
    double maxVol = rawVolScores[0], minVol = rawVolScores[0];
    for (int i = 1; i < 8; ++i) {
        if (rawTrendScores[i] > maxTrend) maxTrend = rawTrendScores[i];
        if (rawTrendScores[i] < minTrend) minTrend = rawTrendScores[i];
        if (rawVolScores[i] > maxVol) maxVol = rawVolScores[i];
        if (rawVolScores[i] < minVol) minVol = rawVolScores[i];
    }

  
    for (int i = 0; i < 8; ++i) {
        double normTrend = minMaxScale(rawTrendScores[i], minTrend, maxTrend);
        double normVol = minMaxScale(rawVolScores[i], minVol, maxVol);
        double finalScore = (normTrend * trendWeight) + (normVol * volatilityWeight);
        
        if (isnan(finalScore)) finalScore = 0.0; 

        outResults[i] = CryptoAsset(markets[i], finalScore, allHistories[i]);
    }

  
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7 - i; ++j) {
            if (outResults[j].getScore() < outResults[j + 1].getScore()) {
                CryptoAsset temp = outResults[j];
                outResults[j] = outResults[j + 1];
                outResults[j + 1] = temp;
            }
        }
    }

    cout << "\n=========================================" << endl;
    cout << " [실시간 업비트 오픈 API 연동] 가상화폐 추천 순위 " << endl;
    cout << "=========================================" << endl;
    for (int i = 0; i < 8; ++i) {
        cout << i + 1 << "위: " << outResults[i].getKoreanName()
             << " (" << outResults[i].getScore() * 100.0 << "점)" << endl;
    }
    cout << "-----------------------------------------" << endl;
    cout << " [안내] 현재 최고 점수 종목: [" << outResults[0].getKoreanName() << "]" << endl;
    cout << "=========================================" << endl;
}
