#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <ctime>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

struct WeatherInfo
{
    string city;
    string country;
    float temperature;
    float feels_like;
    int humidity;
    int pressure;
    string condition;
    float wind_speed;
    int cloudiness;
    float visibility_km;
    string sunrise;
    string sunset;
};

class HistoryManager
{
public:
    void save(const string &city)
    {
        ofstream file("weather_history.txt", ios::app);
        if (file.is_open())
        {
            time_t now = time(nullptr);
            file << city << " at " << ctime(&now);
        }
    }

    void show() const
    {
        ifstream file("weather_history.txt");
        string line;
        cout << "\n--- Search History ---" << endl;
        while (getline(file, line))
        {
            cout << line << endl;
        }
    }
};

class ConsoleWeatherDisplay
{
public:
    void typewriter(const string &text, int delay_ms = 30) const
    {
        for (char c : text)
        {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(delay_ms));
        }
        cout << endl;
    }

    void show(const WeatherInfo &info) const
    {
        typewriter("City: " + info.city + ", " + info.country);
        typewriter("Temperature: " + to_string(info.temperature) + " \u00B0C");
        typewriter("Feels Like: " + to_string(info.feels_like) + " \u00B0C");
        typewriter("Condition: " + info.condition);
        typewriter("Humidity: " + to_string(info.humidity) + "%");
        typewriter("Wind Speed: " + to_string(info.wind_speed) + " m/s");
        typewriter("Cloudiness: " + to_string(info.cloudiness) + "%");
        typewriter("Visibility: " + to_string(info.visibility_km) + " km");
        typewriter("Pressure: " + to_string(info.pressure) + " hPa");
        typewriter("Sunrise: " + info.sunrise);
        typewriter("Sunset: " + info.sunset);
    }
};

class WeatherFetcher
{
private:
    string api_key;
    string response;
    HistoryManager history;
    ConsoleWeatherDisplay display;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *output)
    {
        size_t totalSize = size * nmemb;
        output->append((char *)contents, totalSize);
        return totalSize;
    }

    string formatTime(time_t rawTime) const
    {
        ostringstream oss;
        tm *timeInfo = localtime(&rawTime);
        oss << put_time(timeInfo, "%H:%M:%S");
        return oss.str();
    }

    void loadingAnimation(const string &message, int duration = 3) const
    {
        const char spinner[] = "|/-\\";
        cout << message;
        for (int i = 0; i < duration * 10; ++i)
        {
            cout << "\b" << spinner[i % 4] << flush;
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        cout << "\b \n";
    }

public:
    WeatherFetcher(const string &key) : api_key(key) {}

    bool fetchWeather(const string &city, WeatherInfo &info)
    {
        response.clear();
        string url = "https://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + api_key + "&units=metric";

        CURL *curl = curl_easy_init();
        if (!curl)
        {
            cerr << "CURL initialization failed!" << endl;
            return false;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

        loadingAnimation("Fetching weather data...");

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        if (res != CURLE_OK)
        {
            cerr << "CURL request failed: " << curl_easy_strerror(res) << endl;
            return false;
        }

        try
        {
            json data = json::parse(response);
            if (data.contains("cod") && data["cod"] != 200)
            {
                cerr << "API Error: " << data["message"] << endl;
                return false;
            }

            info.city = data["name"];
            info.country = data["sys"]["country"];
            info.temperature = data["main"]["temp"];
            info.feels_like = data["main"]["feels_like"];
            info.humidity = data["main"]["humidity"];
            info.pressure = data["main"]["pressure"];
            info.condition = data["weather"][0]["description"];
            info.wind_speed = data["wind"]["speed"];
            info.cloudiness = data["clouds"]["all"];
            info.visibility_km = data["visibility"].get<float>() / 1000.0f;
            info.sunrise = formatTime(data["sys"]["sunrise"]);
            info.sunset = formatTime(data["sys"]["sunset"]);

            history.save(city);
            return true;
        }
        catch (const json::parse_error &e)
        {
            cerr << "JSON Parsing Error: " << e.what() << endl;
            return false;
        }
    }

    void displayWeather(const WeatherInfo &info) const
    {
        display.show(info);
    }

    void showHistory() const
    {
        history.show();
    }

    void showLocalTime() const
    {
        time_t now = time(nullptr);
        tm *timeInfo = localtime(&now);
        ostringstream oss;
        oss << put_time(timeInfo, "%c");
        display.typewriter("Local Time: " + oss.str());
    }
};

int main()
{
    string api_key = "3e5a9e976db7e1d1c268b3528bd1a7de"; // Replace with your actual API key
    WeatherFetcher weather(api_key);

    int choice;
    string city;

    while (true)
    {
        cout << "\n**************** Weather App Menu ****************" << endl;
        cout << "1. Fetch Weather" << endl;
        cout << "2. Show Search History" << endl;
        cout << "3. Show Local Time" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            cout << "Enter city name: ";
            getline(cin, city);
            WeatherInfo info;
            if (weather.fetchWeather(city, info))
            {
                weather.displayWeather(info);
            }
        }
        else if (choice == 2)
        {
            weather.showHistory();
        }
        else if (choice == 3)
        {
            weather.showLocalTime();
        }
        else if (choice == 0)
        {
            cout << "Exiting weather fetcher" << endl;
            break;
        }
        else
        {
            cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
