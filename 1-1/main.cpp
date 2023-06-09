#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>

#include <json-c/json.h>

using namespace std;

#define tragedy "tragedy"
#define comedy "comedy"

string statement(json_object *invoice, json_object *plays){
	int totalAmount = 0;
	int volumeCredits = 0;

	int performanceLength;

	json_object *customerNameObj;
	json_object *performancesObj, *perfObj;
	json_object *playIDObj, *playObj;
	json_object *typeObj, *nameObj;
	json_object *audienceObj;

	string customerName;
	string playID, play;
	string type, name;
	int audience;

	string result;

	customerNameObj = json_object_object_get(invoice, "customer");
	customerName = json_object_get_string(customerNameObj);

	result = "Bill info (Customer name: " + customerName + ") \n";

	performancesObj = json_object_object_get(invoice, "performances");

	performanceLength = json_object_array_length(performancesObj);

	for (int i = 0; i < performanceLength; i++){
		int thisAmount = 0;

		perfObj = json_object_array_get_idx(performancesObj, i);

		playIDObj = json_object_object_get(perfObj, "playID");
		playID = json_object_get_string(playIDObj);

		playObj = json_object_object_get(plays, playID.c_str());
		play = json_object_get_string(playObj);

		typeObj = json_object_object_get(playObj, "type");
		type = json_object_get_string(typeObj);

		nameObj = json_object_object_get(playObj, "name");
		name = json_object_get_string(nameObj);

		audienceObj = json_object_object_get(perfObj, "audience");
		audience = json_object_get_int(audienceObj);

		if (strcmp(type.c_str(), "tragedy") == 0){
			thisAmount = 40000;
			if (audience > 30){
				thisAmount += 1000 * (audience - 30);
			}
		}
		else if (strcmp(type.c_str(), "comedy") == 0){
			thisAmount = 30000;
			if (audience > 20){
				thisAmount += 10000 + 500 * (audience - 20);
			}
			thisAmount += 300 * audience;

		}
		else{
			cout << "Error: Unkown type" << endl;
		}

		//Save point
		if (audience - 30 > 0)
			volumeCredits += audience - 30;
		
		//Offer point for every 5 people about comedy customer
		if (strcmp(type.c_str(), "comedy") != 0)
			volumeCredits += audience / 5;
		
		//Print bill
		result += " " + name + ": $" + to_string(thisAmount/100) + " (" + to_string(audience) + " seats)\n"; 
		totalAmount += thisAmount;
	}

	result += "Total payment: $" + to_string(totalAmount / 100) + "\n";
	result += "Saved points: " + to_string(volumeCredits) + " points\n";

	return result;
}

int main(){


	json_object *plays = json_object_from_file("./plays.json");
	json_object *invoice = json_object_from_file("./invoices.json");

	string result = statement(invoice, plays);

	cout << result << endl;

	json_object_put(plays);
	json_object_put(invoice);
	return 0;
}
