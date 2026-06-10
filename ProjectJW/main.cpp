#include <iostream>
#include "UpbitPublicClient.h"
#include "TradingAnalyzer.h"

using namespace std;

int main() {
    UpbitPublicClient client;
    TradingAnalyzer analyzer;

    analyzer.processAllMarkets(client);

    return 0;
}