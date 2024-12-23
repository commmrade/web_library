#pragma once
#include "server/Cookie.hpp"
#include "server/Utils.hpp"
#include <iostream>
#include <optional>
#include<unordered_map>
#include<string>
#include<sstream>


struct HttpRequest {
    std::string request;
    std::unordered_map<std::string, std::string> queries;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, Cookie> cookies;
    
    HttpRequest(const std::string &resp) : request(resp) {
        
        extract_headers();
        extract_queries();
     
    }
    HttpRequest(const HttpRequest&) = delete;
    HttpRequest& operator=(const HttpRequest&) = delete;

    void extract_queries() {
        // Find the start of the query string
        size_t query_start = request.find("?");
        if (query_start == std::string::npos) {
            return; // No query string found
        }

        size_t http_start = request.find(" HTTP", query_start);
        if (http_start == std::string::npos) {
            std::cerr << "Ill formed request\n";
            return; // No HTTP found
        }

        // Extract the query part (everything between '?' and ' HTTP')
        std::string query_part = request.substr(query_start + 1, http_start - query_start - 1);

        // Iterate over the query part and extract each key-value pair
        size_t param_start = 0;
        while (param_start < query_part.length()) {
            // Find the end of the current key-value pair (either '&' or end of string)
            size_t param_end = query_part.find("&", param_start);
            if (param_end == std::string::npos) {
                param_end = query_part.length(); // End of the query string
            }

            // Extract the current key-value pair
            std::string key_value = query_part.substr(param_start, param_end - param_start);

            // Find the '=' that separates the key and value
            size_t equals_pos = key_value.find("=");
            if (equals_pos != std::string::npos) {
                // Extract key and value
                std::string key = key_value.substr(0, equals_pos);
                std::string value = key_value.substr(equals_pos + 1);

                // Insert the key-value pair into the unordered map
                queries[key] = value;
            }

            // Move to the next parameter (skip past '&')
            param_start = param_end + 1;
        }
    }
    void extract_headers() {
       
        std::string headers_cont = request.substr(request.find("\r\n") + 2, request.find("\r\n\r\n") - (request.find("\r\n") + 2));

        if (headers_cont.empty()) {
            std::cerr << "Headers not found\n";
            return;
        }

        std::stringstream strm(headers_cont);
        std::string line;
        while (std::getline(strm, line, '\n')) { //Extracting headers one by one
            if (line.find("\r") != line.npos) { // Remove \r if it is in the line
                line.pop_back();
            } 
            
            std::string name = line.substr(0, line.find(":"));
            std::string value = line.substr(name.size() + 2);; 
            
            if (name != "Cookie") {
                headers[name] = value;

            } else { // Extracting cookies
                std::string name = line.substr(0, line.find(":"));
                std::string vals_str = line.substr(line.find(":") + 2);

                while (!vals_str.empty()) {
                    auto next_pos = vals_str.find(";");

                    // Extracting name and value
                    auto eq_pos = vals_str.find("="); 
                    if (eq_pos == std::string::npos) {
                        std::cerr << "Invalid cookie format: " << vals_str << std::endl;
                        break;
                    }

                    std::string cookie_name = vals_str.substr(0, eq_pos);
                    std::string value = (next_pos == std::string::npos) 
                                            ? vals_str.substr(eq_pos + 1) 
                                            : vals_str.substr(eq_pos + 1, next_pos - eq_pos - 1);

           
                    trim(cookie_name);
                    trim(value);
                    
                    cookies.insert({cookie_name, Cookie{cookie_name, value}});

                    // Update vals_str for the next iteration
                    if (next_pos == std::string::npos) {
                        vals_str.clear(); // No more cookies
                    } else {
                        vals_str = vals_str.substr(next_pos + 1);
                    }
                }
            }

        }
        
    }

    [[nodiscard]]
    inline std::string get_raw() const {
        return request;
    }

    [[nodiscard]]
    std::optional<std::string> get_query(const std::string& query_name) const {
        auto pos = queries.find(query_name);
        if (pos != queries.end()) { //If header exists
            return pos->second;
        }
        return std::nullopt;

    }

    [[nodiscard]]
    std::optional<std::string> get_header(const std::string &header_name) const {
        auto pos = headers.find(header_name);
        if (pos != headers.end()) { //If header exists
            return pos->second;
        }
        return std::nullopt;
    }


    [[nodiscard]]
    std::optional<Cookie> get_cookie(const std::string &name) const {
        auto pos = cookies.find(name);
        if (pos != cookies.end()) {
            return pos->second;
        }
        return std::nullopt;
    }

    [[nodiscard]]
    inline std::string body_as_str() const {
        return request.substr(request.find("\r\n\r\n") + 4);
    }

    [[nodiscard]]
    inline std::string get_method() const {
        return request.substr(0, request.find(" "));
    }

    [[nodiscard]]
    inline std::string get_method_string() const {
        return request.substr(0, request.find("\r\n"));
    }
    
    [[nodiscard]]
    inline std::string get_version() const {
        auto line = request.substr(0, request.find("\r\n"));
        return line.substr(line.find_last_of("/") + 1);
    }
    

    //inline Json::Value get_json_obj() const {}

    // Get message
    
};