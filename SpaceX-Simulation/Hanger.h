#pragma once

template<class T> struct StoredVehicle {
	T* vehicle;
	Date added;
	StoredVehicle(T* vehicle, Date added) {
		this->vehicle = vehicle;
		this->added = added;
	}
};
template<class T> class Hanger {
	vector<StoredVehicle<T>> storedVehicles;

public:
	bool hasVehicle(T* vehicle) {
		for (auto i : storedVehicles) {
			if (i.vehicle == vehicle) return true;
		}
		return false;
	}
	void addVehicle(T* vehicle, Date added) {
		if (!hasVehicle(vehicle)) {
			storedVehicles.emplace_back(vehicle, added);
		}
	}
	void addVehicle(T* vehicle) {
		Date date;
		addVehicle(vehicle, date);
	}
	void addVehicles(vector<T*> vehicles, Date added) {
		for (auto i : vehicles) {
			addVehicle(i, added);
		}
	}
	void removeVehicle(T* vehicle) {
		for (int i = 0; i < storedVehicles.size();i++) {
			if (storedVehicles[i].vehicle == vehicle) {
				storedVehicles[i] = storedVehicles.back();
				storedVehicles.pop_back();
				break;
			}
		}
	}
	void removeVehicles(vector<T*> vehicles) {
		for (auto i : vehicles) {
			removeVehicle(i);
		}
	}
	vector<T*> getVehicles() {
		vector<T*> result;
		for (auto i : storedVehicles) {
			result.push_back(i.vehicle);
		}
		removeVehicles(result);
		return result;
	}
	int getNumOfVehicles() {
		return storedVehicles.size();
	}
	vector<T*> getRandomVehicles(int n, default_random_engine &e) {
		vector<T*> result;
		while (storedVehicles.size()>0 && n > 0) {
			uniform_int_distribution<int> dist(0, storedVehicles.size() - 1);
			int choice = dist(e);
			result.push_back(storedVehicles[choice].vehicle);
			storedVehicles[choice] = storedVehicles.back();
			storedVehicles.pop_back();
			n--;
		}
		return result;
	}
	vector<T*> getVehiclesOlderThan(Date today, int days) {
		vector<T*> result;
		for (auto i : storedVehicles) {
			if (today - i.added > days) {
				result.push_back(i.vehicle);
			}
		}
		removeVehicles(result);
		return result;
	}
};

Hanger<Booster> flightActiveCores;
Hanger<Dragon> flightActiveDragons;
Hanger<Dragon> dragonsInSpace;
