#include "crow.h"
#include <curl/curl.h>
#include <iostream>
#include <string>

// --- Utility: Libcurl Callback ---
// This grabs the raw text from the internet and shoves it into a C++ std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// --- Utility: Fetch Web Data ---
std::string fetch_api(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Required by the National Weather Service
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "FCC-Student-App/1.0 (CIT-66 Project)");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // NWS sometimes redirects
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

// --- Utility: Replace Substrings ---
void replace_all(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

// --- Map Open-Meteo Codes to Human Words ---
std::string get_weather_desc(int code) {
    if (code == 0) return "Clear Sky";
    if (code >= 1 && code <= 3) return "Partly Cloudy";
    if (code == 45 || code == 48) return "Fog";
    if (code >= 51 && code <= 67) return "Rain/Drizzle";
    if (code >= 71 && code <= 77) return "Snow";
    if (code >= 95) return "Thunderstorm";
    return "Variable";
}

int main() {
    crow::SimpleApp app;

    // Our High-End HTML Template (Tailwind CSS)
    const std::string html_template = R"(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Global Weather Dashboard</title>
        <script src="https://cdn.tailwindcss.com"></script>
        <style>
            body { background-color: #0f172a; color: #f8fafc; font-family: sans-serif; }
            .glass-card { background: rgba(30, 41, 59, 0.7); backdrop-filter: blur(10px); border: 1px solid rgba(255, 255, 255, 0.1); }
        </style>
    </head>
    <body class="min-h-screen flex items-center justify-center p-6">
        <div class="max-w-5xl w-full">
            <h1 class="text-4xl font-bold text-center mb-2 text-transparent bg-clip-text bg-gradient-to-r from-blue-400 to-emerald-400">The Vibe Coding Revolution</h1>
            <p class="text-center text-slate-400 mb-10">CIT-66 Final Project • Architected by Gemini & You</p>

            <div class="grid grid-cols-1 md:grid-cols-3 gap-6">
                <!-- Fresno Card -->
                <div class="glass-card rounded-2xl p-8 shadow-2xl">
                    <h2 class="text-2xl font-semibold mb-1">Fresno, CA</h2>
                    <p class="text-sm text-blue-400 mb-6">National Weather Service</p>
                    <div class="text-6xl font-bold mb-4">{{F_TEMP}}</div>
                    <div class="text-xl text-slate-300">{{F_COND}}</div>
                </div>

                <!-- NY Card -->
                <div class="glass-card rounded-2xl p-8 shadow-2xl">
                    <h2 class="text-2xl font-semibold mb-1">New York, NY</h2>
                    <p class="text-sm text-blue-400 mb-6">National Weather Service</p>
                    <div class="text-6xl font-bold mb-4">{{NY_TEMP}}</div>
                    <div class="text-xl text-slate-300">{{NY_COND}}</div>
                </div>

                <!-- London Card -->
                <div class="glass-card rounded-2xl p-8 shadow-2xl border-t-4 border-emerald-500">
                    <div class="flex justify-between items-start">
                        <h2 class="text-2xl font-semibold mb-1">London, UK</h2>
                        <span class="bg-emerald-500/20 text-emerald-400 text-xs px-2 py-1 rounded font-bold uppercase">Bonus</span>
                    </div>
                    <p class="text-sm text-emerald-400 mb-6">Open-Meteo API</p>
                    <div class="text-6xl font-bold mb-4">{{L_TEMP}}</div>
                    <div class="text-xl text-slate-300">{{L_COND}}</div>
                </div>
            </div>
        </div>
    </body>
    </html>
    )";

    CROW_ROUTE(app, "/")([&html_template](){
        std::string html_output = html_template;

        // 1. Fetch Fresno Data (Two-step NWS process)
        std::string f_points_json = fetch_api("https://api.weather.gov/points/36.7378,-119.7871");
        auto f_points = crow::json::load(f_points_json);
        if (f_points && f_points.has("properties")) {
            std::string f_forecast_url = f_points["properties"]["forecast"].s();
            std::string f_json = fetch_api(f_forecast_url);

            // Task A Requirement: Print raw JSON to console
            std::cout << "\n--- RAW FRESNO NWS JSON ---\n" << f_json.substr(0, 500) << "...\n";

            auto f_parsed = crow::json::load(f_json);
            if (f_parsed && f_parsed["properties"]["periods"].size() > 0) {
                auto period = f_parsed["properties"]["periods"][0];
                replace_all(html_output, "{{F_TEMP}}", std::to_string(period["temperature"].i()) + "°" + period["temperatureUnit"].s());
                replace_all(html_output, "{{F_COND}}", period["shortForecast"].s());
            }
        }

        // 2. Fetch New York Data
        std::string ny_points_json = fetch_api("https://api.weather.gov/points/40.7128,-74.0060");
        auto ny_points = crow::json::load(ny_points_json);
        if (ny_points && ny_points.has("properties")) {
            std::string ny_forecast_url = ny_points["properties"]["forecast"].s();
            std::string ny_json = fetch_api(ny_forecast_url);

            auto ny_parsed = crow::json::load(ny_json);
            if (ny_parsed && ny_parsed["properties"]["periods"].size() > 0) {
                auto period = ny_parsed["properties"]["periods"][0];
                replace_all(html_output, "{{NY_TEMP}}", std::to_string(period["temperature"].i()) + "°" + period["temperatureUnit"].s());
                replace_all(html_output, "{{NY_COND}}", period["shortForecast"].s());
            }
        }

        // 3. Fetch London Data (Open-Meteo Bonus)
        std::string london_json = fetch_api("https://api.open-meteo.com/v1/forecast?latitude=51.5085&longitude=-0.1257&current_weather=true");
        auto l_parsed = crow::json::load(london_json);
        if (l_parsed && l_parsed.has("current_weather")) {
            auto current = l_parsed["current_weather"];
            replace_all(html_output, "{{L_TEMP}}", std::to_string((int)current["temperature"].d()) + "°C");
            replace_all(html_output, "{{L_COND}}", get_weather_desc(current["weathercode"].i()));
        }

        // If any API failed, replace the remaining placeholders with "N/A"
        replace_all(html_output, "{{F_TEMP}}", "N/A"); replace_all(html_output, "{{F_COND}}", "Data Error");
        replace_all(html_output, "{{NY_TEMP}}", "N/A"); replace_all(html_output, "{{NY_COND}}", "Data Error");
        replace_all(html_output, "{{L_TEMP}}", "N/A"); replace_all(html_output, "{{L_COND}}", "Data Error");

        // Return to the browser!
        return crow::response(html_output);
    });

    std::cout << "\n✅ VIBE CODING SERVER ENGAGED!\n";
    std::cout << "🌍 Navigate to http://localhost:8080 to view the dashboard.\n\n";

    app.port(8080).multithreaded().run();
}
