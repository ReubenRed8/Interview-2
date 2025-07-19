#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;
// using namespace std::filesystem as fs;

string getProperInput(const string& prompt) {
    cin.ignore();
    string input;
    do {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << "Please enter a non-empty value." << endl;
        }
    } while (input.empty());
    return input;
}

void storeFeedback() {
    string name, country, state, district, feedback;
    // cin.ignore();

    cout << "\n ~ Enter feedback ~" << endl;
    name = getProperInput(" User Name : ");
    country = getProperInput(" Country : ");
    state = getProperInput(" State : ");
    district = getProperInput(" District : ");
    feedback = getProperInput(" Feedback : ");

    cout << "\n ---------------------- " << endl;
    cout << "\n ~ Feedback stored ~" << endl;
    cout << " User Name: " << name << endl;
    cout << " Country: " << country << endl;
    cout << " State: " << state << endl;
    cout << " District: " << district << endl;
    cout << " Feedback: " << feedback << endl;
    cout << "\n ---------------------- " << endl;

    std::filesystem::path countryPath = country ;
    std::filesystem::path statePath = countryPath / state ;
    std::filesystem::path districtPath = statePath / district; 
    string file_name = name + ".txt";
    std::filesystem::path filePath = districtPath / file_name; // The file path inside the directory

    try{
        std::filesystem::create_directories(districtPath);
        cout << "\nDirectories created" << districtPath << endl;
    }
    catch (const std::filesystem::filesystem_error& e) {
        cerr << "\nError creating directories: " << e.what() << endl;
    }

    ofstream storefile(filePath);
    if (storefile.is_open()) {
        storefile << "User Name: " << name << endl;
        storefile << "Country: " << country << endl;
        storefile << "State: " << state << endl;
        storefile << "District: " << district << endl;
        storefile << "Feedback: " << feedback << endl;
        storefile.close();
        cout << "\nFile created: " << filePath << endl;
    }
    else {
        cout << "\nError creating file: " << filePath << endl;
    }

    cout << "Continue? (any key):";
    char choice;
    cin >> choice;
}

void readFeedback() {
    string name, country, state, district, feedback;
    // cin.ignore();

    cout << "\n ~ Search feedback to be retrieved ~" << endl;
    name = getProperInput(" User Name : ");
    country = getProperInput(" Country : ");
    state = getProperInput(" State : ");
    district = getProperInput(" District : ");

    std::filesystem::path countryPath = country ;
    std::filesystem::path statePath = countryPath / state ;
    std::filesystem::path districtPath = statePath / district; 
    string file_name = name + ".txt";
    std::filesystem::path filePath = districtPath / file_name; // The file path inside the directory

    // std::filesystem::path filePath = std::filesystem::path(country) / state / district / (name + ".txt");
    
    try {
        ifstream readfile(filePath);
        if (readfile.is_open()) {
            string line;
            cout << "\n ----------------------" << endl;
            cout << " ~ Retrieved Feedback ~ " << endl;
            cout << " ---------------------- " << endl;
            while (getline(readfile, line)) {
                cout << line << endl;
            }
            cout << " ---------------------- " << endl;
            readfile.close();
        } else {
            cout << "\nFile not found.\n" << endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        cout << "\nError accessing file: " << e.what() << endl;
    }

    cout << "Continue? (any key):";
    char choice;
    cin >> choice;
}

int main(){
    int option;
    bool apploop = true;
    string name, country, state, district, feedback;

    while (apploop) {

        cout << "\n ----------~----------- " << endl;
        cout << " |                    | " << endl;
        cout << " - Location Feedbacks - " << endl;
        cout << " |                    | " << endl;
        cout << " ----------~----------- \n" << endl;;


        cout << " 1. Store a feedback " << endl;
        cout << " 2. Retrieve a feedback " << endl;
        cout << " 3. Exit \n" << endl;
        cout << " Choose an option: ";
        cin >> option ;
        // cin.ignore(); 

        switch (option) {
            case 1:
                storeFeedback();
                break;
            case 2:
                readFeedback();
                break;
            case 3:
                cout << "\nExiting the application.\n";
                cout << "Bye." << endl;
                apploop = false;
                break;
            default:
                cout << "\nInvalid choice.\n" << endl;
                break;
        }
    }
}