#include <iostream>
#include <fstream>
#include "./nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;


int writeCSVIndividualData(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to Open File !" << "\n";
        return 1;
    }

    std::ofstream csvFile("TeamData.csv");
    csvFile << "Team,Team Email,University,Other University,Member Name,Member NIC,Member Email,Member Food,Member Gender,Member Git,Member NO,Member IEE,Quiz Answer\n";

    std::string line;
    while (std::getline(file, line)) {
        json jsonData = json::parse(line);

        if (jsonData.contains("team")) {
            json teamInfo = jsonData["team"];
            std::string teamName = teamInfo["name"];

            if (jsonData.contains("members")) {
                json members = jsonData["members"];
                for (const auto& member : members) {
                    std::string email = member["email"];
                    std::string food = member["food"];
                    std::string gender = member["gender"];
                    std::string git = member.contains("git") ? member["git"] : "";
                    std::string name = member["name"];
                    std::string nic = member["nic"];
                    std::string no = member.contains("no") ? member["no"] : "";
                    std::string iee = member.contains("iee") ? member["iee"] : "";
                    
                    csvFile << teamName << "," 
                            << teamInfo["email"] << "," 
                            << teamInfo["uni"] << "," 
                            << teamInfo["otherUni"] << "," 
                            << name << "," 
                            << nic << "," 
                            << email << "," 
                            << food << "," 
                            << gender << "," 
                            << git << "," 
                            << no << "," 
                            << iee << "," 
                            << jsonData["quizData"] << "\n";
                }
            }
        }
    }
    csvFile << "\n";
    file.close();
    csvFile.close();
    //system("clear");
    std::cout << "Successfully Wrote Data to CSV File !" <<"\n\n\n";
}



int ValidateData(std::string filename) {
    int invalidCountTeams = 0;
    int invalidCountMembers = 0;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to Open File !" << "\n";
        return 1;
    }

    // std::ofstream csvFile(filename + ".csv");
    // csvFile << "Team,Team Email,University,Other University,Member Name,Member NIC,Member Email,Member Food,Member Gender\n";

    std::string line;
    while (std::getline(file, line)) {
        json jsonData = json::parse(line);

        if (jsonData.contains("team")) {
            json teamInfo = jsonData["team"];
            std::string teamName = teamInfo["name"];

            if (jsonData.contains("members")) {
                json members = jsonData["members"];
                for (const auto& member : members) {
                    std::string email = member["email"];
                    std::string food = member["food"];
                    std::string gender = member["gender"];
                    std::string git = member["git"];
                    std::string name = member["name"];
                    std::string nic = member["nic"];
                    std::string no = member["no"];
                    //Validating Data Before Writing to CSV
                    // if (teamName.find(',') != std::string::npos){
                    //     std::cout << "Invalid Character found in Json file !" << "\n";
                    //     std::cout << "Problematic Value :" << teamName << "\n";
                    //     return 1;
                    // }
                    for (json::iterator it = teamInfo.begin(); it != teamInfo.end(); ++it) {
                        //std::cout << it.key() << ": " << it.value() << "\n";
                        std::string value = it.value();
                        if (value.find(',') != std::string::npos){
                            // std::cout << "--------------------------------" << "\n";
                            // std::cout << "Invalid Value on Team Details !" << "\n";
                            // std::cout << "Problematic Value :" << it.value() <<"\n";
                            // std::cout << "--------------------------------" << "\n";
                            // invalidCountTeams++;
                            std::string arrayTest = it.value();
                            for (int i = 0; i < sizeof(arrayTest)/sizeof(arrayTest[0]); i++) {
                                if (arrayTest[i] == ',') {
                                    arrayTest[i] = NULL;
                                }
                            }
                            std::cout << "Fixed String : " << arrayTest << "\n";
                        }
                    }

                    for (json::const_iterator it = member.begin(); it != member.end(); ++it) {
                            std::string arrayTest = it.value();
                            for (int i = 0; i < sizeof(arrayTest)/sizeof(arrayTest[0]); i++) {
                                if (arrayTest[i] == ',') {
                                    arrayTest[i] = NULL;
                                }
                            }
                            std::cout << "Fixed String : " << arrayTest << "\n";
                    }
                    // csvFile << teamName << "," << teamInfo["email"] << "," << teamInfo["uni"] << "," << teamInfo["otherUni"]<< "," << name << "," << nic << "," << email << "," << food << "," << gender << "\n";
                }
            }
        }
        std::cout << "Total Invalid Team Data : "<< invalidCountTeams <<" | Total Invalid Member Data :" << invalidCountMembers << "\n";
    }

    file.close();
    // csvFile.close();
}


int readIndividualData(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to Open File !" << "\n";
        return 1;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    size_t start = 0;
    while (start < content.size()) {
        size_t end = content.find("\n", start);
        if (end == std::string::npos) {
            end = content.size();
        }

        std::string line = content.substr(start, end - start);
        json jsonData = json::parse(line);

        if (jsonData.contains("team")) {
            json teamInfo = jsonData["team"];
            std::string teamName = teamInfo["name"];
            std::cout << "Team :" << teamName << "\n";

            if (jsonData.contains("members")) {
                json members = jsonData["members"];
                std::cout << "Members:\n";

                for (const auto& member : members) {
                    std::cout << "  Name: " << member["name"] << "\n";
                    std::cout << "  NIC: " << member["nic"] << "\n";
                    std::cout << "  Email: " << member["email"] << "\n";
                    std::cout << "  Food: " << member["food"] << "\n";
                    std::cout << "  Gender: " << member["gender"] << "\n";
                    std::cout << "------------------------\n";
                }
            }
        }

        start = end + 1;
    }

}

int displayJson(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()){
        std::cerr << "Failed to Open File !" << "\n";
        return 1;
    }
    
    std::cout << "Displaying Json Data...\n";
    
    std::string line;
    while (std::getline(file, line)) {
        json jsonData = json::parse(line);
        std::cout << jsonData.dump(4) << std::endl;
    }
    
    file.close();
    return 0;
}

int mainDisplay() {
    std::cout << "=============================================" << std::endl;
    std::cout << "DUOTHAN 4.0 JSON DATA PARSER                " << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "1) Show json file" << std::endl;
    std::cout << "2) Show Grouped Data" << std::endl;
    std::cout << "3) Write Grouped Data as CSV" << std::endl;
    std::cout << "4) Check File's Availability" << std::endl;
    std::cout << "5) Validate Data" << std::endl;
    std::cout << "6) Automatically Count & Display Teams" << std::endl;
    std::cout << "7) Exit" << std::endl;
    std::cout << "=============================================" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string jsonFileName;
    if (argc < 2) {
        std::cout << "Enter Json FileName :";
        // std::getline(std::cin, jsonFileName);
        std::cin >> jsonFileName;
        std::ifstream file(jsonFileName);
        if (!file.is_open()){
            std::cerr << "Failed to Open File !" << "\n";
            return 1;
        }
        while (jsonFileName != "Exit" || jsonFileName != "exit") {
            std::cin.clear();
            int choice;
            mainDisplay();
            std::cout << "Enter Your Choice :";
            std::cin >> choice;
            switch (choice) {
                case 1:
                    if (displayJson(jsonFileName) == 0){
                        std::cout << "Process Completed Successfully..." <<"\n";
                    } else {
                        std::cout << "An error occured !" <<"\n";
                    }
                    break;
                case 2:
                    readIndividualData(jsonFileName);
                    break;
                case 3:
                    writeCSVIndividualData(jsonFileName);
                    break;
                case 5:
                    ValidateData(jsonFileName);
                case 6:
                    return 0;
                    break;
                default:
                    std::cout << "Input Invalid !";
            }
            std::cin.clear();
        }
    } else {
        std::cout << "! Parser Automatic Override !\n";
        writeCSVIndividualData("teamsForP.json");
        system("cat TeamData.csv");
    }
}
