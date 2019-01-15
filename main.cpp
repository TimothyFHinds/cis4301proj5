/*
Timothy Hinds
CIS4301
Assignment 5
Professor Dobra
*/
#include <iostream>
#include "sqlite3.h"
#include <string>
using namespace std;



static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i=0; i<argc; i++)
	{
		cout<<azColName[i]<<" = " << (argv[i] ? argv[i] : "NULL")<<"\n";
	}
	cout<<"\n";
	return 0;
}

string optionA()
{
    char inputSTR[100]; //this will be used to receive inputs.
    cout << "Please type in the name of the flower you would like to see recent sightings of" << endl;
    string in, updated;
    cin.getline(inputSTR, sizeof(inputSTR));
    in = inputSTR;
    updated = "select name, person, location, sighted from sightings where name = \'";
    updated.append(in);
    updated.append("\' order by sighted desc limit 10");
    return updated;

}
string optionB()
{
    char inputSTR[100]; //this will be used to receive inputs.
    string in, newG, newS, newC;
    cin.getline(inputSTR, sizeof(inputSTR));
    in = inputSTR;

    //in = COMNAME of flower to change
    cout << "You have selected the flower: " << in << endl;
    cout << "Please enter the updated GENUS" << endl;
    cin.getline(inputSTR, sizeof(inputSTR));
    newG = inputSTR;

    cout << "Please enter the updated SPECIES" << endl;
    cin.getline(inputSTR, sizeof(inputSTR));
    newS = inputSTR;
    cout << "Please enther the updated COMNAME" << endl;
    cin.getline(inputSTR, sizeof(inputSTR));
    newC = inputSTR;

    string updated;
    updated = "UPDATE FLOWERS set GENUS = \'";
    updated.append(newG);
    updated.append("\', SPECIES = \'");
    updated.append(newS);
    updated.append("\', COMNAME = \'");
    updated.append(newC);
    updated.append("\' WHERE COMNAME = \'");
    updated.append(in);
    updated.append("\'");

    return updated;
}
string optionC()
{
    cout << "Please type in the name of the flower you would like to insert a sighting of." << endl;
    char inputSTR[100]; //this will be used to receive inputs.
    string in, newP, newL, newS;
    cin.getline(inputSTR, sizeof(inputSTR));
    in = inputSTR;
    //in == COMNAME from FLOWERS which is equal to NAME of SIGHTINGS
    cout << "You have selected the flower: " << in << endl;
    cout << "Please enter the person who sighted the flower" << endl;
    cin.getline(inputSTR, sizeof(inputSTR));
    newP = inputSTR;
    cout << "Please enter the location of where the flower was seen" << endl;
    cin.getline(inputSTR, sizeof(inputSTR));
    newL = inputSTR;
    cout << "Please enter the date at which this flower was seen(2006-07-08 year-month-day)" << endl;
    cin.getline(inputSTR, sizeof(inputSTR));
    newS = inputSTR;
    string updated;
    updated = "INSERT INTO SIGHTINGS (NAME, PERSON, LOCATION, SIGHTED) VALUES (\'";
    updated.append(in);
    updated.append("\', \'");
    updated.append(newP);
    updated.append("\', \'");
    updated.append(newL);
    updated.append("\', \'");
    updated.append(newS);
    updated.append("\'\);");
    return updated;
}


int main()
{
    //Create an interface that:
    //1. Allow the user to select from a list of flowers.
    //When a user selects a flower, it displays the 10 most recent sightings of this flower.
    //This should include information about the sighting: date, location, and who sighted the flower

    //2. Allow a user to select a flower and update its information

    //3. Allow a user to insert a new sighting of a flower.
    sqlite3 *db;
    const int STATEMENTS = 8;
    char *errmsg = 0;
    const char *p[STATEMENTS];
    int test;
    string input;
    test = sqlite3_open("flowers.db", &db);
    if(test)
    {
        cout << "cant open db" << sqlite3_errmsg(db) << endl;
        return 0;
    }
    else {
        cout << "flowers.db has been opened" << endl;
    }

    //implement trigger for part C which checks for foreign key restraints before inserting into the sightings table.
    string fk;
    fk = "create trigger task1 before insert on sightings for each row begin select case when NEW.NAME NOT IN (SELECT COMNAME FROM FLOWERS) THEN RAISE(ABORT, \'Insert into the SIGHTINGS table references a name that is not found in the database\') END; END;";
    test = sqlite3_exec(db, fk.c_str(), callback, 0, &errmsg);



    char selection;
    do{


    cout << "What would you like to do?" << endl << "'A' Select a flower?" << endl << "'B' Update a flower's information?" << endl << "'C' Insert a new sighting of a flower?" << endl << "'Z' End program?" << endl;

    cin >> selection;
    p[0] = "select * from FLOWERS";
    p[1] = "select * from SIGHTINGS";

    //while(answer<=0 || answer > 3)

        switch(selection)
        {
        case 'A':
            test = sqlite3_exec(db, p[0], callback, 0, &errmsg);
            cout << "Displaying a list of flowers, please type in the COMNAME of the flower you would like to see recent sightings from." << endl;
            cin.ignore();
            input = optionA();
            cout << endl;
            test = sqlite3_exec(db, input.c_str(), callback, 0, &errmsg);
            if(errmsg != 0)
            {
                cout << endl << errmsg << endl << "Exiting due to error." << endl;
                return 0;
            }
            //test = sqlite3_exec(db, p[0], callback, 0, &errmsg);
            cout << "Success! The most recent sightings of the flower are being displayed now." << endl;
            break;
        case 'B':
            test = sqlite3_exec(db, p[0], callback, 0, &errmsg);
            cout << "Displaying a list of flowers, please type in the COMNAME of the flower you would like to update." << endl;
            cin.ignore();
            input = optionB();
            test = sqlite3_exec(db, input.c_str(), callback, 0, &errmsg);
            if(errmsg != 0)
            {
                cout << endl << errmsg << endl << "Exiting due to error." << endl;
                return 0;
            }
            test = sqlite3_exec(db, p[0], callback, 0, &errmsg);
            cout << "Success! The updated FLOWERS table is being displayed now." << endl;
            break;
        case 'C':
            test = sqlite3_exec(db, p[0], callback, 0, &errmsg);
            cin.ignore();
            input = optionC();
            test = sqlite3_exec(db, input.c_str(), callback, 0, &errmsg);
            if(errmsg != 0)
            {
                cout << endl << errmsg << endl << "Exiting due to error." << endl;
                return 0;
            }
            test = sqlite3_exec(db, p[1], callback, 0, &errmsg);
            cout << "Success! The updated SIGHTINGS table is being displayed now." << endl;
            break;
        case 'Z':
            return 0;
        default:
            cout << "bad input choice." << endl;
        }
    } while(selection!='Z');


    return 0;
}
