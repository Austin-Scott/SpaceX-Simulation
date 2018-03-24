#pragma once

#include "rapidjson\rapidjson.h"

const string startDate = "2010-06-04";
const string endDate = "2018-03-24";

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

void getRealData() {

}