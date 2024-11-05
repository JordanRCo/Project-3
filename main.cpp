#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "DSString.h"

// Function to train the sentiment model
std::unordered_map<DSString, double> trainModel(const DSString& trainingFile, const DSString& sentimentOutputFile) {
    std::unordered_map<DSString, double> wordSentiment;
    std::unordered_map<DSString, int> wordCount;

    std::ifstream file(trainingFile.c_str());
    DSString line;

    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line.c_str());
        DSString sentimentStr, tweet;
        int sentiment;

        // Extract only the Sentiment and Tweet columns from the CSV line
        std::getline(ss, sentimentStr, ',');      // Sentiment
        std::getline(ss, tweet, ',');             // Skip id
        std::getline(ss, tweet, ',');             // Skip Date
        std::getline(ss, tweet, ',');             // Skip Query
        std::getline(ss, tweet, ',');             // Skip User
        std::getline(ss, tweet, ',');             // Tweet

        sentiment = (sentimentStr == "4") ? 1 : 0;

        // Process each word and update its sentiment score
        std::istringstream tweetStream(tweet.c_str());
        DSString word;
        while (tweetStream >> word) {
            // Convert word to lowercase for consistency
            word.toLower();
            wordCount[word]++;
            wordSentiment[word] += sentiment;
        }
    }

    // Calculate average sentiment for each word
    for (auto& pair : wordSentiment) {
        pair.second /= wordCount[pair.first];
    }

    return wordSentiment;
}

// Main function
int main() {
    DSString trainingFile = "training_data.csv";
    DSString sentimentOutputFile = "sentiment_output.txt";

    auto wordSentiment = trainModel(trainingFile, sentimentOutputFile);

    // Output word sentiment values to a file
    std::ofstream outputFile(sentimentOutputFile.c_str());
    for (const auto& pair : wordSentiment) {
        outputFile << pair.first << ": " << pair.second << '\n';
    }

    outputFile.close();
    std::cout << "Sentiment model training completed and saved to " << sentimentOutputFile << std::endl;

    return 0;
}
