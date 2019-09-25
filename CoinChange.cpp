// CoinChange.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
constexpr auto COINCOUNT = 5;


struct CoinInfo {
	int cCount[COINCOUNT];
	bool achievable;
	bool assigned;
};

int getTotalCoinCount(CoinInfo info) {
	int totalCoinCount = 0;
	for (int i = 0; i < COINCOUNT; i++) {
		totalCoinCount += info.cCount[i];
	}
	return totalCoinCount;
}

CoinInfo getCoinCountDP(int moneyAmount, int coins[], int coinCounts[]) {

	int maxAchievable = 0;
	for (int i = 0; i < COINCOUNT; i++) {
		maxAchievable += coins[i] * coinCounts[i];
	}

	if (moneyAmount > maxAchievable) {
		CoinInfo i = {
			{ 0 },
			false,
			true
		};
		return i;
	}

	CoinInfo* info = new CoinInfo[maxAchievable + 1];

	for (int i = 0; i <= maxAchievable; i++) {
		info[i].assigned = false; // Manual initialization
	}

	for (int i = 0; i < COINCOUNT; i++) {
		if (coinCounts[i] != 0) {
			for (int j = 0; j < COINCOUNT; j++) {
				info[coins[i]].cCount[j] = j == i ? 1 : 0;
			}
			info[coins[i]].achievable = true;
			info[coins[i]].assigned = true;
		} else {
			info[coins[i]] = {
				{ 0 },
				false,
				true
			};
		}
	}

	for (int i = 1; i <= moneyAmount; i++) {
		if (!info[i].assigned) {
			int minTotalCoinCount = std::numeric_limits<int>::max();
			int prevCoinId = -1;
			for (int j = 0; j < COINCOUNT; j++) {
				if (i > coins[j] && info[i - coins[j]].achievable) {
					int prevTotalCoinCount = getTotalCoinCount(info[i - coins[j]]);
					if (minTotalCoinCount > prevTotalCoinCount && coinCounts[j] > info[i - coins[j]].cCount[j]) {
						minTotalCoinCount = prevTotalCoinCount;
						prevCoinId = j;
					}
				}
			}
			info[i].assigned = true;
			info[i].achievable = prevCoinId != -1;
			if (prevCoinId == -1) {
				for (int j = 0; j < COINCOUNT; j++) {
					info[i].cCount[j] = 0;
				}	
			}
			else
			{
				for (int j = 0; j < COINCOUNT; j++) {
					info[i].cCount[j] = prevCoinId == j ? info[i - coins[prevCoinId]].cCount[j] + 1 : info[i - coins[prevCoinId]].cCount[j];
				}
			}
		}
	}

	CoinInfo retVal = info[moneyAmount];

	delete[] info;	
	return retVal;
}

int main()
{
	int coins[COINCOUNT] = { 50, 25, 10, 5, 1 };
	int coinCount[COINCOUNT] = { 5, 2, 3, 1, 4 };

	int coin;

	std::cout << "Enter The Amount Of Money : ";
	std::cin >> coin;

	CoinInfo info = getCoinCountDP(coin, coins, coinCount);

	if (info.achievable) {
		int total = 0;
		std::cout << "In order to get " << coin << " $, you need at least " << std::endl;
		for (int i = 0; i < COINCOUNT; i++) {
			if (info.cCount[i] != 0) {
				std::cout << info.cCount[i] << " " << coins[i] << " $ " << std::endl;
				total += info.cCount[i];
			}
		}
		std::cout << "At least " << total << " coins are needed." << std::endl;
	}
	else {
		std::cout << "With the money you have, its impossible to obtain " << coin << " $." << std::endl;
	}
}
