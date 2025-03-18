#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <ctime>

using namespace std;
using json = nlohmann::json;

class WeatherFetcher
{
private:
    string api_key;
    string response;

    // Callback function to handle API response
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *output)
    {
        size_t totalSize = size * nmemb;
        output->append((char *)contents, totalSize);
        return totalSize;
    }

    // Convert Unix timestamp to readable time
    string convertTime(time_t rawTime)
    {
        char buffer[80];
        struct tm *timeInfo = localtime(&rawTime);
        strftime(buffer, sizeof(buffer), "%H:%M:%S", timeInfo);
        return string(buffer);
    }

public:
    WeatherFetcher(string key) : api_key(key) {}

    bool fetchWeather(const string &city)
    {
        string url = "https://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + api_key + "&units=metric";

        CURL *curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (!curl)
        {
            cerr << "CURL initialization failed!" << endl;
            return false;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        if (res != CURLE_OK)
        {
            cerr << "CURL request failed: " << curl_easy_strerror(res) << endl;
            return false;
        }

        return true;
    }

    void displayWeather()
    {
        try
        {
            json data = json::parse(response);

            if (data.contains("cod") && data["cod"] != 200)
            {
                cerr << "API Error: " << data["message"] << endl;
                return;
            }

            string city_name = data["name"];
            string country = data["sys"]["country"];
            float temp = data["main"]["temp"];
            float feels_like = data["main"]["feels_like"];
            int humidity = data["main"]["humidity"];
            int pressure = data["main"]["pressure"];
            string condition = data["weather"][0]["description"];
            float wind_speed = data["wind"]["speed"];
            int cloudiness = data["clouds"]["all"];
            int visibility = data["visibility"];
            time_t sunrise = data["sys"]["sunrise"];
            time_t sunset = data["sys"]["sunset"];

            cout << "\n City: " << city_name << ", " << country << endl;
            cout << " Temperature: " << temp << " \u00B0C" << endl;
            cout << " Feels Like: " << feels_like << " \u00B0C" << endl;
            cout << " Condition: " << condition << endl;
            cout << " Humidity: " << humidity << "%" << endl;
            cout << " Wind Speed: " << wind_speed << " m/s" << endl;
            cout << " Cloudiness: " << cloudiness << "%" << endl;
            cout << " Visibility: " << visibility / 1000.0 << " km" << endl;
            cout << " Pressure: " << pressure << " hPa" << endl;
            cout << " Sunrise: " << convertTime(sunrise) << endl;
            cout << " Sunset: " << convertTime(sunset) << endl;
        }
        catch (json::parse_error &e)
        {
            cerr << "JSON Parsing Error: " << e.what() << endl;
        }
    }
};

int main()
{

    string api_key = "3e5a9e976db7e1d1c268b3528bd1a7de"; // Replace with your actual API key
    WeatherFetcher weather(api_key);
    cout << "****************Real Time Weather Fetcher*****************" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "Using Rest API,cURL,nlohmann json" << endl;

    string city;
    cout << "Enter city name: ";
    cin >> city;

    if (weather.fetchWeather(city))
    {
        weather.displayWeather();
    }

    return 0;
}
/*  g++ weather.cpp -o weather.exe -lcurl -I/usr/include -L/usr/lib
./weather.exe */
