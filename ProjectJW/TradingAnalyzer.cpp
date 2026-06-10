#include "TradingAnalyzer.h"
#include "UpbitPublicClient.h"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

double TradingAnalyzer::minMaxScale(double X, double X_min, double X_max) {
    if (abs(X_max - X_min) < 1e-6) {
        return 0.5;
    }
    return (X - X_min) / (X_max - X_min);
}

void TradingAnalyzer::processAllMarkets(UpbitPublicClient& client) {
    vector<CryptoAsset> rankingList; // 구조체 배열 대신 클래스 객체 벡터로 승격
    vector<double> rawTrendScores;
    vector<double> rawVolScores;

    const vector<string>& markets = client.getTickerList();

    for (const string& market : markets) {
        vector<CandleData> history = client.fetchDayCandles(market, 15);

        if (history.size() < 10) {
            rawTrendScores.push_back(0.0);
            rawVolScores.push_back(0.0);
            continue;
        }

        double sum5 = 0.0, sum10 = 0.0;
        for (int i = 0; i < 5; ++i)  sum5 += history[i].tradePrice;
        for (int i = 0; i < 10; ++i) sum10 += history[i].tradePrice;

        double ma5 = sum5 / 5.0;
        double ma10 = sum10 / 10.0;
        double trendScore = (ma5 > ma10) ? (ma5 - ma10) / ma10 : 0.0;
        rawTrendScores.push_back(trendScore);

        double prevHigh = history[1].highPrice;
        double prevLow = history[1].lowPrice;
        double todayOpen = history[0].openingPrice;
        double todayClose = history[0].tradePrice;
        double todayHigh = history[0].highPrice;

        double range = prevHigh - prevLow;
        double targetPrice = todayOpen + (range * k);

        double volatilityScore = 0.0;
        if (todayHigh >= targetPrice) {
            volatilityScore = log(todayClose / targetPrice);
        }
        rawVolScores.push_back(volatilityScore);
    }

    double maxTrend = *max_element(rawTrendScores.begin(), rawTrendScores.end());
    double minTrend = *min_element(rawTrendScores.begin(), rawTrendScores.end());
    double maxVol = *max_element(rawVolScores.begin(), rawVolScores.end());
    double minVol = *min_element(rawVolScores.begin(), rawVolScores.end());

    for (size_t i = 0; i < markets.size(); ++i) {
        double normTrend = minMaxScale(rawTrendScores[i], minTrend, maxTrend);
        double normVol = minMaxScale(rawVolScores[i], minVol, maxVol);

        double finalScore = (normTrend * trendWeight) + (normVol * volatilityWeight);

        // 클래스 생성자를 주입하여 인스턴스 자동 등록
        rankingList.push_back(CryptoAsset(markets[i], finalScore));
    }

    // 람다 식을 통한 클래스 객체 내림차순 정렬 연산
    sort(rankingList.begin(), rankingList.end(), [](const CryptoAsset& a, const CryptoAsset& b) {
        return a.getScore() > b.getScore();
        });

    cout << "\n=========================================" << endl;
    cout << "★ [실시간 업비트 오픈 API 연동] 가상화폐 추천 순위 ★" << endl;
    cout << "=========================================" << endl;
    for (size_t i = 0; i < rankingList.size(); ++i) {
        // 클래스의 getKoreanName() 기능을 써서 우아하게 한글로 전광판 렌더링
        cout << i + 1 << "위: " << rankingList[i].getKoreanName()
            << " (" << rankingList[i].getScore() * 100.0 << "점)" << endl;
    }
    cout << "-----------------------------------------" << endl;
    cout << "📢 실시간 서버 분석 기준 최상위 추천 종목: [" << rankingList[0].getKoreanName() << "]" << endl;
    cout << "=========================================" << endl;
}