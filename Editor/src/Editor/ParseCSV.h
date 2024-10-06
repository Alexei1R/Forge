
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>  // for std::pair


std::vector<std::pair<int, int>> loadDataFromCSV(const std::string& filename)
{
    std::vector<std::pair<int, int>> data;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string timestampStr, valueStr;

        // Assuming the CSV has two columns separated by a comma
        if (std::getline(ss, timestampStr, ',') && std::getline(ss, valueStr, ','))
        {
            try
            {
                // Convert strings to integers
                int timestamp = static_cast<int>(std::stod(timestampStr));  // cast float to int
                int value = std::stoi(valueStr);

                // Add to vector as a pair
                data.emplace_back(timestamp, value);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error parsing line: " << line << " - " << e.what() << std::endl;
            }
        }
    }

    file.close();
    return data;
}
