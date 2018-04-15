#pragma once
#include "Utility.h"



Date BFR_FLIGHT_OPERATIONAL = Date("2020-12-26");


struct BFRPlatform {
	LaunchSite* launchSite;
	LandingSite* landingSite;

	Hanger<Dragon> BFSs;
	Hanger<Booster> BFRs;

	BFRPlatform(string SiteID, string Name, string Location) {
		launchSite = createLaunchSite(SiteID + "_lch", Name + " Launch Platform", Location, 1, 1);
		landingSite = createLandingSite(SiteID + "_lnd", Name + " Landing Platform", 1);


	}
};