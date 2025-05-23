#pragma once


#include <server/RequestType.hpp>
#include <string>
#include <vector>



namespace utils {
RequestType req_type_from_str(std::string_view str);

std::string process_url_str(std::string_view url);


std::vector<std::string> extract_params(std::string_view url);

void trim_r(std::string &s);
void trim_l(std::string &s);
void trim(std::string &s);

std::string find_file(const std::string &filename);

}
