#ifndef UPBIT_PUBLIC_CLIENT_H
#define UPBIT_PUBLIC_CLIENT_H

#include <string>
#include "CandleData.h"

using namespace std;

class UpbitPublicClient {
private:
    string CoinList[8]; 

public:
    UpbitPublicClient();

    string* getCoinList();

    void fetchDayCandles(string marketCode, int count, CandleData* outHistory);
};

#endif
