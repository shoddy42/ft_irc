#include <iostream>   // For input/output operations
#include <string>     // For std::string
#include <curl/curl.h>  // For libcurl functionality
#include "../include/Bot.hpp"


// Hugging Face API endpoint for text generation
// const std::string API_URL = "https://api-inference.huggingface.co/models/gpt2";

// Callback function to write response data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *response) {
    response->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string ask_question(const std::string& question)
{
    std::string endpoint = "https://api-inference.huggingface.co/models/openai-community/gpt2";

    // Initialize CURL
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return "";
    }

    // Set up the request headers
	std::string header = "Authorization: Bearer " + std::string(API_KEY);
	std::cout << PURPLE << "ASKING GPT!" << END_LINE;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, header.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Set up the request body containing the question prompt
	std::string message = R"({
		"inputs": "How do I bake a cake?",
		"options": {
			"num_return_sequences": 1,
			"max_length": 50
		}
	})";

    // Perform the POST request
    std::string response_data;
    curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Error performing request: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return "";
    }

    // Clean up
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // Parse the response JSON

    // Extract the generated text from the response
    std::string generated_text = response_data;
	std::cout << response_data << END_LINE;
    return generated_text;
}
