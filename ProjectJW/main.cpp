#include <iostream>
#include "UpbitPublicClient.h"
#include "TradingAnalyzer.h"

using namespace std;

int main() {
    UpbitPublicClient client;
    TradingAnalyzer analyzer;

    CryptoAsset results[8];

    analyzer.processAllMarkets(client, results);

    while (true) {
        cout << "\n[메뉴] 추세 상태를 볼 코인 번호(1~8) 입력 (종료: 0) -> ";
        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "프로그램을 종료합니다. 성공적인 투자 되세요!" << endl;
            break;
        }



        CryptoAsset& selected = results[choice - 1];

        cout << "\n=================================================================" << endl;
        cout << " [추세 분석] " << selected.getKoreanName() << " (" << selected.getCode() << ")" << endl;
        cout << "-----------------------------------------------------------------" << endl;
        
        selected.printQuantReport(); // 5시간/10시간선 이평선 보고서 즉각 출력
        
        cout << "=================================================================" << endl;
    }

    return 0;
}
