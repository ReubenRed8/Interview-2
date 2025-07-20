#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

string getProperInput(const string& prompt) {
    // cin.ignore();
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

const int SHIFT_KEY = 3;
bool processFile(const filesystem::path& filePath, bool mode) {
    ifstream inputFile(filePath, ios::binary);
    if (!inputFile) {
        cerr << "Error: Could not open file for reading: " << filePath << endl;
        return false;
    }

    string content((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    int shift = mode ? SHIFT_KEY : -SHIFT_KEY;

    for (char& c : content) {
        c = c + shift;
    }

    ofstream outputFile(filePath, ios::binary);
    if (!outputFile) {
        cerr << "Error: Could not open file for writing: " << filePath << endl;
        return false;
    }

    outputFile.write(content.c_str(), content.size());
    outputFile.close();

    cout << (mode ? "Encrypted" : "Decrypted") << " file successfully: " << filePath << endl;
    return true;
}


void storeFeedback() {
    string key, country, state, district, feedback;
    // cin.ignore();

    cout << "\n ~ Enter feedback ~" << endl;
    country = getProperInput(" Country : ");
    state = getProperInput(" State : ");
    district = getProperInput(" District : ");
    feedback = getProperInput(" Feedback : ");
    key = getProperInput(" Password : ");

    // cout << "\n ---------------------- " << endl;
    // cout << "\n ~ Feedback stored ~" << endl;
    // cout << " User Name: " << name << endl;
    // cout << " Country: " << country << endl;
    // cout << " State: " << state << endl;
    // cout << " District: " << district << endl;
    // cout << " Feedback: " << feedback << endl;
    // cout << "\n ---------------------- " << endl;

    filesystem::path countryPath = country ;
    filesystem::path statePath = countryPath / state ;
    filesystem::path districtPath = statePath / district; 
    string file_name = key + ".txt";
    filesystem::path filePath = districtPath / file_name; // The file path inside the directory

    try{
        filesystem::create_directories(districtPath);
        cout << "\nDirectories created" << districtPath << endl;
    }
    catch (const filesystem::filesystem_error& e) {
        cerr << "\nError creating directories: " << e.what() << endl;
    }

    ofstream storefile(filePath);
    if (storefile.is_open()) {
        storefile << "Country: " << country << endl;
        storefile << "State: " << state << endl;
        storefile << "District: " << district << endl;
        storefile << "Feedback: " << feedback << endl;
        storefile << "Password: " << key << endl;
        storefile.close();
        cout << "File created: " << filePath << endl;
    }
    else {
        cout << "\nError creating file: " << filePath << endl;
    }
    
    if (!processFile(filePath, true)) {
        cout << "Error encrypting file: " << filePath << endl;
    }


    cout << "\nContinue? (any character):";
    char choice;
    cin >> choice;
}

void readFeedback() {
    string key, country, state, district, feedback;
    // cin.ignore();

    cout << "\n ~ Search feedback to be retrieved ~" << endl;
    country = getProperInput(" Country : ");
    state = getProperInput(" State : ");
    district = getProperInput(" District : ");
    key = getProperInput(" Password : ");

    filesystem::path countryPath = country ;
    filesystem::path statePath = countryPath / state ;
    filesystem::path districtPath = statePath / district; 
    string file_name = key + ".txt";
    filesystem::path filePath = districtPath / file_name; // The file path inside the directory

    // filesystem::path filePath = filesystem::path(country) / state / district / (name + ".txt");
    
    try {
        if (!processFile(filePath, false)) {
            cout << "Error decrypting file: " << filePath << endl;
        }

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
    } catch (const filesystem::filesystem_error& e) {
        cout << "\nError accessing file: " << e.what() << endl;
    }

    if (!processFile(filePath, true)) {
            cout << "Error encrypting file back: " << filePath << endl;
    }

    cout << "\nContinue? (any character):";
    char choice;
    cin >> choice;
}

int main(){
    int option;
    bool apploop = true;

    while (apploop) {

        cout << "\n ----------~----------- " << endl;
        cout << " |                    | " << endl;
        cout << " - LOCATION  MEMORIES - " << endl;
        cout << " |                    | " << endl;
        cout << " ----------~----------- \n" << endl;;


        cout << " 1. Store a feedback " << endl;
        cout << " 2. Retrieve a feedback " << endl;
        cout << " 3. Exit \n" << endl;
        cout << " Choose an option: ";
        cin >> option ;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

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