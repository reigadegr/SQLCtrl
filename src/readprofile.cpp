#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
// 删除第一列
std::string GetSecondArg(std::string &buf)
{
    size_t pos = buf.find(' ');
    if (pos != std::string::npos) {
        buf.erase(0, pos + 1);  // 删除空格和左边的所有内容
    }
    return buf;
}
std::string GetSecondArg(std::string &buf);
bool readProfile(const char *profile, std::string &elf, std::string &db)
{
    std::ifstream file(profile);
    if (!file.is_open()) [[unlikely]] {
        return false;
    }
    std::string buf = "";

    while (std::getline(file, buf, '\n')) {
        if (buf[0] == '#' || buf.empty()) {
            continue;
        }
        if (buf[0] == '$') {
            elf = GetSecondArg(buf);
            continue;
        }
        if (buf[0] == '&') {
            db = GetSecondArg(buf);
            continue;
        }
    }
    file.close();
    return true;
}