#include<iostream>
#include <filesystem>
#include <unordered_map>
#include <string>

using namespace std;
namespace fs = filesystem;

// Hash map of the file types 
unordered_map<string, string> fileTypeMap = {
    {".jpg", "Image"},
    {".doc", "Document"},
    {".docx", "Document"},
    {".rtf", "Document"},
    {".mp4", "Video"},
    {".pdf", "Document"},
    {".avi", "Video"},
    {".txt", "Document"},
    {".png", "Image"},
    {".bmp", "Image"},
    {".mp3", "Music"},
    {".wav", "Music"}
};

// Function for identifying file types using the hash map
string identifying(const string& extension) {
    if (fileTypeMap.find(extension) != fileTypeMap.end()) {
        return fileTypeMap[extension];
    }
    else {
        return "Unknown"; // Return "Unknown" for files with extensions not in the map
    }
}

// Function for creating a folder
void creating(const string& folderName) {
    if (!fs::exists(folderName)) {
        fs::create_directory(folderName);
    }
}

// Function for moving files to the folder created
void movingFiles(const fs::path& file, const string& folderName) {
    creating(folderName);  // Ensure the folder is created
    fs::rename(file, folderName + "/" + file.filename().string());
}

// Function for handling unknown file types
void handleUnknownFiles(const fs::path& file, const string& unknownFolderName) {
    creating(unknownFolderName);  // Create a folder for unknown files if it doesn't exist
    fs::rename(file, unknownFolderName + "/" + file.filename().string());
}

// Main function
int main() {
    // Provide the correct path to the desktop or target directory
    string desktopPath = "C:/......"; // enter your path here to clean.

    for (const auto& entry : fs::directory_iterator(desktopPath)) {
        if (entry.is_regular_file()) {
            string extension = entry.path().extension().string();
            string fileType = identifying(extension);
            if (fileType != "Unknown") {
                movingFiles(entry.path(), desktopPath + "/" + fileType);
            }
            else {
                handleUnknownFiles(entry.path(), desktopPath + "/Unknown");
            }
        }
    }

    cout << "Desktop cleanup completed!" << std::endl;
    return 0;
}
