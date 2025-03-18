<h1 align="center">🌦️ Weather Fetcher C++ 🌦️</h1> <p align="center"> <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" alt="C++17"> <img src="https://img.shields.io/badge/cURL-✔️-green.svg" alt="cURL"> <img src="https://img.shields.io/badge/nlohmann/json-✔️-orange.svg" alt="nlohmann/json"> <img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="MIT License"> </p> <p align="center"> A real-time weather fetching program built using **C++**, **cURL**, and **nlohmann/json**. It fetches live weather data from OpenWeatherMap API and displays detailed weather information! </p>
🚀 Features
✅ Fetches real-time weather data using REST API
✅ Displays detailed weather information:

🌡️ Temperature, Feels Like
🌧️ Humidity, Cloudiness
🌬️ Wind Speed, Pressure
🌅 Sunrise and Sunset Times
✅ Uses cURL for HTTP requests
✅ Parses JSON using nlohmann/json
✅ Clean output with proper formatting
🖥️ Demo
bash
Copy
Edit
> ./weather.exe  
**************** Real-Time Weather Fetcher *****************  
------------------------------------------------------------  
Using REST API, cURL, nlohmann/json  
Enter city name: Patna  
City: Patna, IN  
Temperature: 29.43 °C  
Feels Like: 28.8 °C  
Condition: Clear sky  
Humidity: 40%  
Wind Speed: 3.2 m/s  
Cloudiness: 0%  
Visibility: 10 km  
Pressure: 1012 hPa  
Sunrise: 05:30:21  
Sunset: 18:10:45  
🛠️ Installation
1. Clone the Repository:
bash
Copy
Edit
git clone https://github.com/yourusername/Weather-Fetcher.git  
2. Install Dependencies:
Install cURL:
bash
Copy
Edit
pacman -S mingw-w64-x86_64-curl  
Install nlohmann/json:
bash
Copy
Edit
pacman -S mingw-w64-x86_64-nlohmann-json  
3. Compile:
bash
Copy
Edit
g++ weather.cpp -o weather.exe -lcurl -I/usr/include -L/usr/lib  
4. Run:
bash
Copy
Edit
./weather.exe  
🌐 API Key Setup
Sign up at OpenWeatherMap
Get your API key
Update the api_key value in the code:
cpp
Copy
Edit
string api_key = "YOUR_API_KEY";  
🏗️ How It Works
User inputs the city name
Program sends an HTTP GET request to OpenWeatherMap using cURL
Response is parsed using nlohmann/json
Weather data is extracted and displayed in readable format
📚 Libraries Used
✅ cURL → For HTTP Requests
✅ nlohmann/json → For JSON Parsing

🚀 Future Improvements
🌍 Add support for multiple languages
🖥️ Create a GUI using Qt
🗣️ Add voice input using Speech Recognition

📝 License
This project is licensed under the MIT License.

❤️ Made with C++ by Siddhant
✅ How to Modify:
Replace YOUR_API_KEY with your actual OpenWeatherMap key
Update yourusername with your GitHub username
Update future improvements if you add new features
Let me know if you need to tweak anything! 😎
