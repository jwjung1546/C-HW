#ifndef TRADING_ANALYZER_H
#define TRADING_ANALYZER_H

#include "CandleData.h"

class UpbitPublicClient; 

class TradingAnalyzer {
private:
    static constexpr double trendWeight = 0.7;       
    static constexpr double volatilityWeight = 0.3;  
    static constexpr double k = 0.5;               

    static double minMaxScale(double X, double X_min, double X_max);

public:
  
    void processAllMarkets(UpbitPublicClient& client, CryptoAsset* outResults);
};

#endif
