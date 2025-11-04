#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
using namespace std;

const double RON95_SUB = 2.05, RON97_MARKET = 3.70, DIESEL_SUB = 2.15, DIESEL_MARKET = 3.35;
const bool ALLOW_PRIVATE_DIESEL_SUBSIDY = false;
bool mainLoop, restart;
int fuelChoice, liters;
double unitPrice, savings, total;
string idNo, plateNo, category, fuelType;
bool eligibility;

void FuelChoiceEntry();
void EligibilityCheck();
void UnitPriceCheck();
void QuantityEntry();
void Subtotal();
void ConfirmPayment();
void Dispense();
void Receipt();

int main(){
	mainLoop = true;
    while (mainLoop){
    	restart = false;
	    FuelChoiceEntry();
	    if (!mainLoop) break;
	    if (restart) continue;
	    EligibilityCheck();
	    UnitPriceCheck();
	    if (restart) continue;
	    QuantityEntry();
	    Subtotal();
	    ConfirmPayment();
	    if (restart) continue;
	    Dispense();
	    Receipt();
    }
}

void FuelChoiceEntry(){
	cout<<"Welcome to self-service pump kiosk. Here's the main menu:"<<endl;
	cout<< "1 = RON95 (RM" << RON95_SUB << "/L, subsidised for eligible users)" <<endl;
	cout<< "2 = RON97 (RM" << RON97_MARKET <<"/L)" <<endl;
	cout<< "3 = Diesel (RM" << DIESEL_SUB << "/L, subsidised for eligible users. For non-subsidised users, RM" << DIESEL_MARKET << "/L)" <<endl;
	cout<< "9999 = Exit" <<endl;
	cin>>fuelChoice;
	if (fuelChoice == 9999){
		cout<<"Thank you. Session end.";
		mainLoop = false;
		return;
	}
	else if (fuelChoice < 1 || fuelChoice > 3){
		cout<<"Input ERROR. Please try again.";
		restart = true;
		return;
	}
	switch(fuelChoice){
		case 1: fuelType = "RON95"; break;
		case 2: fuelType = "RON97"; break;
		case 3: fuelType = "Diesel"; break;
	}
}

void EligibilityCheck(){
	cout<<"\nEnter MyKad or Passport number: "<<endl;
	cin>>idNo;
	cout<<"Enter vehicle plate number: "<<endl;
	cin>>plateNo;
	cout<<"Enter vehicle category (Motorcycle, PrivateCar, Commercial, Government, Foreign): "<<endl;
	cin>>category;
	cout<<"Citizen? Y/N "<<endl;
	string citizen;
	cin>>citizen;
	if (idNo.length() > 16  ||  idNo.length() < 8 ||  !(category == "Motorcycle" || category == "PrivateCar" || category == "Commercial" || category == "Government" || category == "Foreign")){
		cout<<"Input ERROR. Please try again.";
		EligibilityCheck();
    }
	else{
	    if (fuelChoice == 1){
			if (citizen == "Y" && (category == "Motorcycle" || category == "PrivateCar")) eligibility = true;
		}
		else if (fuelChoice == 3){
			if ((citizen == "Y" && category == "Commercial") || (ALLOW_PRIVATE_DIESEL_SUBSIDY)) eligibility = true;
		}
	}
}

void UnitPriceCheck(){
	if (fuelChoice == 1){
		if (eligibility) unitPrice = RON95_SUB;
		else {
			cout<<"Dear customer, you're not eligible for this fuel choice. You're suggested to opt for 2 = RON97."<<endl;
			restart = true;
    	}
    }
	else if (fuelChoice == 2) unitPrice = RON97_MARKET;
	else if (fuelChoice == 3){
		if (eligibility) unitPrice = DIESEL_SUB;
		else unitPrice = DIESEL_MARKET;
	}
}

void QuantityEntry(){
	int maxLiters = 0;
	if (fuelChoice == 1){
		if (category == "Motorcycle") maxLiters = 10;
		else maxLiters = 30;
    }
	else maxLiters = 60;
	cout<<"\nEnter whole liters to purchase. Maximum: "<<maxLiters<<"L"<<endl;
	cin>>liters;
	if (typeid(liters) != typeid(int) || liters <= 0 || liters > maxLiters){
		cout<<"Invalid quantity entry. Please try again."<<endl;
		QuantityEntry();
	}
}

void Subtotal(){
	total = liters * unitPrice;
	switch(fuelChoice){
		case 1: savings = liters * (RON97_MARKET - RON95_SUB); break;
		case 3: savings = liters * (DIESEL_MARKET - DIESEL_SUB); break;
		case 2: savings = 0; break;
	}
	cout<<"\nSUMMARY:"<<endl;
	cout<<"Fuel Type: "<<fuelType<<endl;
	cout<<"Quantity (Liters): "<<liters<<endl;
	cout<<"Unit Price (RM): "<<unitPrice<<endl;
	cout<<"Total (RM): "<<total<<endl;
	if (savings > 0){
		cout<<"Savings (RM): "<<savings<<endl;
	}
}

void ConfirmPayment(){
	cout<<"\nConfirm purchase? (Y/N)"<<endl;
	string decision;
	cin>>decision;
	if (decision == "N"){
		restart = true;
		return;
	}
	else if (decision == "Y"){
		cout<<"Payment approved. Dispensing fuel..."<<endl;
	}
	else{
		cout<<"Invalid confirmation. Please try again."<<endl;
		restart = true;
		return;
    }
}

void Dispense(){
	double commonDispenseRate = 0.75, dieselDispenseRate = 1.5;
	double dispensed = 0.00;
	cout<<" "<<endl;
	while (dispensed <= liters){
    	cout<<"\rDispensing Progress (liters): "<<dispensed<<flush;
		this_thread::sleep_for(chrono::milliseconds(100));
		if (fuelChoice == 3){
    		dispensed = dispensed + (dieselDispenseRate * 0.1);
	    }
	    else dispensed = dispensed + (commonDispenseRate * 0.1);
    }
    cout<<"\rDispensing Progress (liters): "<<liters<<".000"<<endl;
}

void Receipt(){
	cout<<"\nSUMMARY:"<<endl;
	cout<<"02/11/2025 01:36:20a.m."<<endl;
	string maskedId = idNo;
	for (int i=1; i<=4; i++){
		maskedId[idNo.length() - i] = 'X';
	}
	cout<<"MyKad/Passport: "<<maskedId<<endl;
	cout<<"Vehicle: "<<plateNo<<"  "<<category<<endl;
	cout<<"Fuel Type: "<<fuelType<<endl;
	cout<<"Quantity (Liters): "<<liters<<endl;
	cout<<"Unit Price (RM): "<<unitPrice<<endl;
	cout<<"Total (RM): "<<total<<endl;
	if (savings > 0){
		cout<<"Subsidy Flag: Yes"<<endl;
		cout<<"Savings (RM): "<<savings<<endl;
	}
	else cout<<"Subsidy Flag: No"<<endl;
	cout<<"Thank you. Have a safe journey!\n\n\n";	
	cout<<"---------------------------------"<<endl;
}
