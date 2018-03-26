#pragma once

#include "Hanger.h"

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

using namespace rapidjson;

const string startDate = "2010-06-04";
const string endDate = "2018-03-24";
int highestMissionNumber = 0;
int highestBoosterNumber = 0;
int highestCapsuleNumber = 0;

//Used for interacting with Curl to download SpaceX api responses
string executeSystemCommand(string cmd) {
	string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	cmd.append(" 2>&1");
	stream = _popen(cmd.c_str(), "r");
	if (stream) {
		while (!feof(stream)) {
			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
		}
		_pclose(stream);
	}
	return data;
}

string downloadMissionData() {
	string command = "curl -k -s ";
	command += "\"https://api.spacexdata.com/v2/launches?start=" + startDate + "&final=" + endDate + "\"";
	return executeSystemCommand(command);
}

string getString(const Value &parent, string name) {
	string result;
	auto iter = parent.FindMember(name.c_str());
	if (iter != parent.MemberEnd()) {
		auto &member = iter->value;
		if (!member.IsNull() && member.IsString()) {
			result = string(member.GetString());
		}
	}
	return result;
}

int getInt(const Value &parent, string name) {
	int result = INT_MIN;
	auto iter = parent.FindMember(name.c_str());
	if (iter != parent.MemberEnd()) {
		auto &member = iter->value;
		if (!member.IsNull() && member.IsInt()) {
			result = member.GetInt();
		}
	}
	return result;
}

bool getBool(const Value &parent, string name) {
	bool result = false;
	auto iter = parent.FindMember(name.c_str());
	if (iter != parent.MemberEnd()) {
		auto &member = iter->value;
		if (!member.IsNull() && member.IsBool()) {
			result = member.GetBool();
		}
	}
	return result;
}

void parseMissionData(const Value &mission) {
	int flight_number = getInt(mission, "flight_number");
	if (flight_number > highestMissionNumber) highestMissionNumber = flight_number;
	Date launch_date(getString(mission, "launch_date_utc").substr(0,10));
	string Description = getString(mission, "details");
	bool launch_success = getBool(mission, "launch_success");
	string rocket_id;
	string Title; //Assigned by name of primary payload
	string site_id;
	string site_description;
	auto launch_site = mission.FindMember("launch_site");
	if (launch_site != mission.MemberEnd()) {
		auto &launch_site_value = launch_site->value;
		site_id = getString(launch_site_value, "site_id");
		site_description = getString(launch_site_value, "site_name_long");
	}
	LaunchSite* current_launch_site = findOrCreateLaunchSite(site_id, site_description);
	Mission* current_mission = createMission(flight_number, "TBD", Description, launch_date, current_launch_site);
	auto rocket = mission.FindMember("rocket");
	if (rocket != mission.MemberEnd()) {
		auto &rocket_value = rocket->value;
		rocket_id = getString(rocket_value, "rocket_id");
		auto first_stage = rocket_value.FindMember("first_stage");
		if (first_stage != rocket_value.MemberEnd()) {
			auto &first_stage_value = first_stage->value;
			auto cores = first_stage_value.FindMember("cores");
			if (cores != first_stage_value.MemberEnd()) {
				auto &cores_value = cores->value;
				if (cores_value.IsArray()) {
					for (int i = 0; i < cores_value.Size(); i++) {
						string core_serial = getString(cores_value[i], "core_serial");
						int block = getInt(cores_value[i], "block");
						bool land_success = getBool(cores_value[i], "land_success");
						string landing_site = getString(cores_value[i], "landing_vehicle");
						Booster* booster = findBooster(core_serial);
						if (booster == nullptr) {
							booster = createBooster(core_serial, land_success ? "Flight Operational" : "Destroyed", block);
							booster->flights = 0;
						}
						booster->flights++;
						createFlownBy(booster, current_mission, landing_site, land_success ? "Booster recovered nominally." : "Rapid unscheduled disassembly.");
					}
				}
			}
		}
		auto second_stage = rocket_value.FindMember("second_stage");
		if (second_stage != rocket_value.MemberEnd()) {
			auto &second_stage_value = second_stage->value;
			auto payloads = second_stage_value.FindMember("payloads");
			if (payloads != second_stage_value.MemberEnd()) {
				auto &payloads_value = payloads->value;
				if (payloads_value.IsArray()) {
					for (int i = 0; i < payloads_value.Size(); i++) {
						string payload_id = getString(payloads_value[i], "payload_id");
						int payload_mass = getInt(payloads_value[i], "payload_mass_kg");
						string orbit = getString(payloads_value[i], "orbit");
						string cap_serial = getString(payloads_value[i], "cap_serial");
						Dragon* capsule = findDragon(cap_serial);
						if (capsule == nullptr && cap_serial.size()>0) {
							capsule = createDragon(cap_serial, "Dragon Capsule");
						}
						string supplier;
						auto customers = payloads_value[i].FindMember("customers");
						if (customers != payloads_value[i].MemberEnd()) {
							auto &customers_value = customers->value;
							if (customers_value.IsArray() && customers_value.Size() > 0 && !customers_value[0].IsNull() && customers_value[0].IsString()) {
								supplier = customers_value[0].GetString();
							}
						}
						if (i == 0) {
							Title = payload_id;
							strcpy(Title, current_mission->Title, 255);
						}
						createPayload(payload_id, orbit, payload_mass, supplier, launch_success ? "Complete success." : "Mission failed.", capsule, current_mission);
					}
				}
			}
		}
	}
}

void prepareBoosterForSimulation(Booster* booster) {
	Document doc;
	string boosterID(booster->BoosterID, 5);

	cout << "Downloading detailed information on booster " << boosterID << "..." << endl;

	int boosterNum = atoi(boosterID.substr(1, 4).c_str());
	if (boosterNum > highestBoosterNumber) highestBoosterNumber = boosterNum;

	string command = "curl -k -s ";
	command += "\"https://api.spacexdata.com/v2/parts/cores/" + boosterID + "\"";
	string result = executeSystemCommand(command);
	doc.Parse(result.c_str());
	string status = getString(doc, "status");
	if (status == "active") {
		//Currently does not distinguish between Falcon Heavy and Falcon 9 cores. However, this does not matter because currently there are no flight active Falcon Heavy cores.
		flightActiveCores.addVehicle(booster);
	}
	else {
		strcpy(status, booster->FlightStatus, 255);
	}
}

void prepareCapsuleForSimulation(Dragon* capsule) {
	Document doc;
	string capsuleID(capsule->SerialNumber, 4);

	cout << "Downloading detailed information on Dragon " << capsuleID << "..." << endl;

	int capsuleNum = atoi(capsuleID.substr(1, 3).c_str());
	if (capsuleNum > highestCapsuleNumber) highestCapsuleNumber = capsuleNum;

	string command = "curl -k -s ";
	command += "\"https://api.spacexdata.com/v2/parts/caps/" + capsuleID + "\"";
	string result = executeSystemCommand(command);
	doc.Parse(result.c_str());
	string description = getString(doc, "details");
	if (description.size() > 0) {
		strcpy(description, capsule->Description, 500);
	}
	if (getString(doc, "status") == "active") {
		flightActiveDragons.addVehicle(capsule);
	}
}

void getRealData() {
	cout << "Attempting to download mission manifest..." << endl;
	Document doc;
	string rawJson = downloadMissionData();
	doc.Parse(rawJson.c_str());
	if (doc.IsArray()) {
		Value root = doc.GetArray();
		for (int i = 0; i < root.Size(); i++) {
			cout << "Parsing mission #" << (i + 1) << " from manifest..." << endl;
			parseMissionData(root[i]);
		}
	}
	else {
		cout << "Failed to download mission manifest." << endl;
	}
	for (auto i = Boosters.begin(); i.hasNext(); i.operator++()) {
		Booster* b = &i;
		prepareBoosterForSimulation(b);
	}
	for (auto i = Dragons.begin(); i.hasNext(); i.operator++()) {
		Dragon* d = &i;
		prepareCapsuleForSimulation(d);
	}
}