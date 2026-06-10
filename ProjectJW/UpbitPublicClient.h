#ifndef UPBIT_PUBLIC_CLIENT_H
#define UPBIT_PUBLIC_CLIENT_H

#include <vector>
#include <string>
#include "CandleData.h"

using namespace std;

class UpbitPublicClient {
private:
    vector<string> tickerList;

public:
    UpbitPublicClient();
    const vector<string>& getTickerList() const;
    vector<CandleData> fetchDayCandles(string marketCode, int count);
};

#endif