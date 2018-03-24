#pragma once
#include "Tables.h"

struct StoredBooster {
	Booster* booster;
	Date added;
	StoredBooster(Booster* booster, Date added) {
		this->booster = booster;
		this->added = added;
	}
};
class Hanger {
	vector<StoredBooster> storedBoosters;

public:
	bool hasBooster(Booster* booster) {
		for (auto i : storedBoosters) {
			if (i.booster == booster) return true;
		}
		return false;
	}
	void addBooster(Booster* booster, Date added) {
		if (!hasBooster(booster)) {
			storedBoosters.emplace_back(booster, added);
		}
	}
	void addBooster(Booster* booster) {
		Date date;
		addBooster(booster, date);
	}
	void removeBooster(Booster* booster) {
		for (int i = 0; i < storedBoosters.size();i++) {
			if (storedBoosters[i].booster == booster) {
				storedBoosters[i] = storedBoosters.back();
				storedBoosters.pop_back();
				break;
			}
		}
	}
	void removeBoosters(vector<Booster*> boosters) {
		for (auto i : boosters) {
			removeBooster(i);
		}
	}
	vector<Booster*> getBoosters() {
		vector<Booster*> result;
		for (auto i : storedBoosters) {
			result.push_back(i.booster);
		}
		return result;
	}
	vector<Booster*> getBoostersOlderThan(Date today, int days) {
		vector<Booster*> result;
		for (auto i : storedBoosters) {
			if (today - i.added > days) {
				result.push_back(i.booster);
			}
		}
		return result;
	}
};

Hanger flightReadyFalcon9s;
Hanger flightReadyFalconHeavySideBoosters;
Hanger flightReadyFalconHeavyCores;
Hanger falcon9Refurbishment;
Hanger falconHeavySideBoosterRefurbishment;
Hanger falconHeavyCoreRefurbishment;
Hanger falconHeavySideBoosterConversion;