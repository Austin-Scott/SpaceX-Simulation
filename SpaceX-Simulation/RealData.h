#pragma once

#include "Hanger.h"

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

using namespace rapidjson;

const string startDate = "2010-06-04";
const string endDate = getCurrentDate();
int highestMissionNumber = 0;
int highestBoosterNumber = 0;
int highestCapsuleNumber = 0;

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

int getBool(const Value &parent, string name) {
	int result = -1;
	auto iter = parent.FindMember(name.c_str());
	if (iter != parent.MemberEnd()) {
		auto &member = iter->value;
		if (!member.IsNull() && member.IsBool()) {
			result = member.GetBool()?1:0;
		}
	}
	return result;
}

void parseMissionData(const Value &mission) {
	int flight_number = getInt(mission, "flight_number");
	if (flight_number > highestMissionNumber) highestMissionNumber = flight_number;
	Date launch_date(getString(mission, "launch_date_utc").substr(0,10));
	string Description = getString(mission, "details");
	int launch_success = getBool(mission, "launch_success");
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
	LaunchSite* current_launch_site = findLaunchSite(site_id);
	Mission* current_mission = createMission(flight_number,current_launch_site, "TBD", Description, launch_date, launch_success);
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
						int land_success = getBool(cores_value[i], "land_success");
						string landing_site = getString(cores_value[i], "landing_vehicle");
						Booster* booster = findBooster(core_serial);
						if (booster == nullptr) {
							string status = "";
							if (land_success == 0) status = "Destroyed";
							else if (land_success == 1) status = "Flight Operational";
							else status = "Expended";
							booster = createBooster(core_serial, status, "falcon9", block);
							booster->flights = 0;
						}
						booster->flights++;
						createFlownBy(booster, current_mission, findLandingSite(landing_site), land_success==1 ? "Booster recovered nominally." : "Booster destroyed", land_success);
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
							capsule = createDragon(cap_serial, "Dragon Capsule", 0, 1);
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
						createPayload(payload_id,current_mission, nullptr,capsule, orbit, payload_mass, supplier, launch_success==1 ? "Complete success." : "Mission failed.", 0);
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

	if (booster->BlockNumber != 0) {

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
			strcpy(status, booster->FlightStatus, 50);
		}
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
		strcpy(description, capsule->Description, 400);
	}
	if (getString(doc, "status") == "active") {
		flightActiveDragons.addVehicle(capsule);
	}
	else {
		capsule->FlightActive = 0;
	}
}

void addFalconOneLaunches() {
	LaunchSite* site = findLaunchSite("kwajalein_atoll");
	//Flight One
	Mission* flightOne = createMission(1, site, "Falcon One Flight One", "Engine failure at 33 seconds and loss of vehicle", Date("2006-03-24"), 0);
	Booster* booster1 = createBooster("00001", "Destroyed", "falcon1", 0);
	flownBy* f1 = createFlownBy(booster1, flightOne, nullptr, "", -1);
	Payload* p1 = createPayload("FalconSAT-2", flightOne, nullptr, nullptr, "LEO", 20, "DARPA", "Failed", 0);
	//Flight Two
	Mission* flightTwo = createMission(2, site, "Falcon One Flight Two", "Successful first stage burn and transition to second stage, maximum altitude 289 km, Premature engine shutdown at T+7 min 30 s, Failed to reach orbit, Failed to recover first stage", Date("2007-03-21"), 0);
	Booster* booster2 = createBooster("00002", "Expended", "falcon1", 0);
	flownBy* f2 = createFlownBy(booster2, flightTwo, nullptr, "", -1);
	Payload* p2 = createPayload("DemoSAT",flightTwo, nullptr, nullptr, "LEO", INT_MIN, "DARPA", "Failed", 0);
	//Flight Three
	Mission* flightThree = createMission(3, site, "Falcon One Flight Three", "Residual stage 1 thrust led to collision between stage 1 and stage 2", Date("2008-08-02"), 0);
	Booster* booster3 = createBooster("00003", "Destroyed", "falcon1", 0);
	flownBy* f3 = createFlownBy(booster3, flightThree, nullptr, "", -1);
	Payload* p3 = createPayload("Trailblazer",flightThree,nullptr, nullptr, "LEO", INT_MIN, "NASA", "Failed", 0);
	Payload* p4 = createPayload("PRESat",flightThree, nullptr, nullptr, "LEO", INT_MIN, "ORS", "Failed", 0);
	//Flight Four
	Mission* flightFour = createMission(4, site, "Falcon One Flight Four", "Ratsat was carried to orbit on the first successful orbital launch of any privately funded and developed, liquid-propelled carrier rocket, the SpaceX Falcon 1", Date("2008-09-28"), 1);
	Booster* booster4 = createBooster("00004", "Expended", "falcon1", 0);
	flownBy* f4 = createFlownBy(booster4, flightFour, nullptr, "", -1);
	Payload* p5 = createPayload("RatSat",flightFour, nullptr, nullptr, "LEO", 165, "SpaceX", "Success", 0);
	//Flight Five
	Mission* flightFive = createMission(5, site, "Falcon One Flight Five", "Fifth and final flight of Falcon One", Date("2009-07-13"), 1);
	Booster* booster5 = createBooster("00005", "Expended", "falcon1", 0);
	flownBy* f5 = createFlownBy(booster5, flightFive, nullptr, "", -1);
	Payload* p6 = createPayload("RazakSAT",flightFive, nullptr, nullptr, "LEO", 200, "ATSB", "Success", 0);
}

void addRealWorldLaunchSites() {
	createLaunchSite("kwajalein_atoll", "Kwajalein Atoll Omelek Island", "Earth, Western Pacific, Marshall Islands", 0, 0);
	createLaunchSite("ccafs_slc_40", "Cape Canaveral Air Force Station Space Launch Complex 40", "Earth, North America, Florida", 1, 0);
	createLaunchSite("vafb_slc_4e", "Vandenberg Air Force Base Space Launch Complex 4E", "Earth, North America, California", 1, 0);
	createLaunchSite("ksc_lc_39a", "Kennedy Space Center Historic Launch Complex 39A", "Earth, North America, Florida", 1, 0);
}

void addRealWorldLandingSites() {
	createLandingSite("OCISLY", "Of Course I Still Love You", 0);
	createLandingSite("JRTI", "Just Read The Instructions", 0);
	createLandingSite("LZ-1", "Cape Canaveral Landing Zone One", 0);
	createLandingSite("LZ-2", "Cape Canaveral Landing Zone Two", 0);
}

void getRealData() {

	cout << "Adding Falcon One flights..." << endl;
	addFalconOneLaunches();

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