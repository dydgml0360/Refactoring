#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>

#include <json-c/json.h>

using namespace std;


json_object *aPerformanceObjFor(json_object *performancesObj, int index);
json_object *playObjFor(json_object *plays, json_object *aPerformanceObj);
json_object *playIDObjFor(json_object *aPerformanceObj);
json_object *typeObjFor(json_object *playObj);
json_object *nameObjFor(json_object *playObj);
json_object *audienceObjFor(json_object *aPerformanceObj);

string playIDFor(json_object *aPerformanceObj);
string typeFor(json_object *playObj);
string nameFor(json_object *playObj);
int audienceFor(json_object *aPerformance);

int amountFor(string type, int audience);
int volumeCreditsFor(json_object *playObj, int audience);

int totalVolumeCredits(json_object *performancesObj, json_object *plays);
int totalAmount(json_object *performancesObj, json_object *plays);


string statement(json_object *invoice, json_object *plays){

	int performanceLength;

	json_object *customerNameObj;
	json_object *performancesObj, *aPerformanceObj;
	json_object *playObj;

	string customerName;
	int audience;

	string result;

	customerNameObj = json_object_object_get(invoice, "customer");
	customerName = json_object_get_string(customerNameObj);

	result = "Bill info (Customer name: " + customerName + ") \n";

	performancesObj = json_object_object_get(invoice, "performances");

	performanceLength = json_object_array_length(performancesObj);


	for (int i = 0; i < performanceLength; i++){
		aPerformanceObj = aPerformanceObjFor(performancesObj, i);
		playObj = playObjFor(plays, aPerformanceObj);
		audience = audienceFor(aPerformanceObj);
		//Print bill
		result += " " + nameFor(playObj) + ": $" + \
			   to_string(amountFor(typeFor(playObj), audience)/100) + \
			   " (" + to_string(audience) + " seats)\n"; 

	}

	result += "Total payment: $" + to_string(totalAmount(performancesObj, plays) / 100) + "\n";
	result += "Saved points: " + to_string(totalVolumeCredits(performancesObj, plays)) + " points\n";

	return result;
}

int totalAmount(json_object *performancesObj, json_object *plays){
	int audience;
	int result = 0;
	int performanceLength = json_object_array_length(performancesObj);

	json_object *playObj;
	json_object *aPerformanceObj;

	for (int i = 0; i < performanceLength; i++){
		aPerformanceObj = aPerformanceObjFor(performancesObj, i);
		playObj = playObjFor(plays, aPerformanceObj);
		audience = audienceFor(aPerformanceObj);

		result += amountFor(typeFor(playObj), audience);

	}

	return result;

}
	

int totalVolumeCredits(json_object *performancesObj, json_object *plays){
	int audience;
	int result = 0;
	int performanceLength = json_object_array_length(performancesObj);

	json_object *playObj;
	json_object *aPerformanceObj;

	for (int i = 0; i < performanceLength; i++){
		aPerformanceObj = aPerformanceObjFor(performancesObj, i);
		playObj = playObjFor(plays, aPerformanceObj);
		audience = audienceFor(aPerformanceObj);

		result += volumeCreditsFor(playObj, audience);
	}

	return result;
}


int volumeCreditsFor(json_object *playObj, int audience){
	int result = 0;
	//Save point
	if (audience - 30 > 0){
		result += audience - 30;
	}
		
	//Offer point for every 5 people about comedy customer
	if (strcmp(typeFor(playObj).c_str(), "comedy") != 0)
	{
		result += audience / 5;
	}

	return result;
}


json_object *aPerformanceObjFor(json_object *performancesObj, int index){
	return json_object_array_get_idx(performancesObj, index);
}

json_object *playObjFor(json_object *plays, json_object *aPerformanceObj){
	return json_object_object_get(plays, playIDFor(aPerformanceObj).c_str());
}

json_object *playIDObjFor(json_object *aPerformanceObj){
	return json_object_object_get(aPerformanceObj, "playID");
}

json_object *typeObjFor(json_object *playObj){
	return json_object_object_get(playObj, "type");
}

json_object *nameObjFor(json_object *playObj){
	return json_object_object_get(playObj, "name");
}

json_object *audienceObjFor(json_object *aPerformanceObj){
	return json_object_object_get(aPerformanceObj, "audience");
}


string playIDFor(json_object *aPerformanceObj){
	return json_object_get_string(playIDObjFor(aPerformanceObj));
}

string typeFor(json_object *playObj){
	return json_object_get_string(typeObjFor(playObj));
}

string nameFor(json_object *playObj){
	return json_object_get_string(nameObjFor(playObj));
}

int audienceFor(json_object *aPerformance){
	return json_object_get_int(audienceObjFor(aPerformance));
}


int amountFor(string type, int audience){
	int result = 0;

	if (strcmp(type.c_str(), "tragedy") == 0){
		result = 40000;
		if (audience > 30){
			result += 1000 * (audience - 30);
		}
	}

	else if (strcmp(type.c_str(), "comedy") == 0){
		result = 30000;
		if (audience > 20){
			result += 10000 + 500 * (audience - 20);
		}
		result += 300 * audience;

	}
	else{
		cout << "Error: Unkown type" << endl;
	}

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
