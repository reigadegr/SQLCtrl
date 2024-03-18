#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
namespace fs = std::filesystem;
bool readProfile(const char *profile, std::string &elf, std::string &db);

auto execCmdSync(const char *command) -> std::string
{
    // 执行命令并获取输出
    FILE *pipe = popen(command, "r");
    if (pipe == nullptr) [[unlikely]] {
        return {};
    }
    char buffer[2];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

auto getTables(const std::string &elf, const std::string &db)
{
    std::string cmd = elf + " " + db + " .tables";
    std::string rs = execCmdSync(cmd.c_str());
    std::cout << rs << std::endl;
}
auto clearTable(const std::string &elf, const std::string &db)
{
    printf("当前表名: \n");
    getTables(elf, db);
    printf("输入要清空的表名\n");
    std::string target = "";
    std::cin >> target;
    std::string cmd = elf + " " + db + " \"DELETE FROM " + target + ";\"";
    std::string rs = execCmdSync(cmd.c_str());
    std::cout << rs << std::endl;
    // $MODDIR/bin/sqlite3 $TEG_CFG "DELETE FROM rules;"
}
auto lookTable(const std::string &elf, const std::string &db)
{
    printf("当前表名: \n");
    getTables(elf, db);
    printf("输入要查看数据的表名\n");
    std::string target = "";
    std::cin >> target;
    std::string cmd = elf + " " + db + " \"SELECT * FROM " + target + ";\"";
    std::string rs = execCmdSync(cmd.c_str());
    std::cout << rs << std::endl;
}
auto dumpSql(const std::string &elf, const std::string &db)
{
    //./sqlite3 db_game_database.db ".output db.sql" ".dump"

    std::string cmd = elf + " " + db + " " + "\".output db.sql\" \".dump\"";
    std::string rs = execCmdSync(cmd.c_str());
    printf("导出完毕\n");
    std::cout << rs << std::endl;
}
auto packSql(const std::string &elf, const std::string &db)
{
    std::string cmd = elf + " " + "new.db" + " " + "\".read db.sql\"";
    std::string rs = execCmdSync(cmd.c_str());
    printf("打包完毕\n");
    std::cout << rs << std::endl;
    // ./sqlite3 new.db \".read db.sql\"
}
auto print_theme(const std::string &elf, const std::string &db)
{
    printf("1，查看表名\n");
    printf("2，清空指定表数据\n");
    printf("3，查看指定表数据\n");
    printf("4，导出.db为db.sql\n");
    printf("5，打包db.sql为new.db\n");
    printf("输入数字\n");
    std::string input;
    std::cin >> input;
    if (input == "1") {
        getTables(elf, db);
        std::cin.get();
    }
    else if (input == "2") {
        clearTable(elf, db);
        std::cin.get();
    }
    else if (input == "3") {
        lookTable(elf, db);
        std::cin.get();
    }
    else if (input == "4") {
        dumpSql(elf, db);
        std::cin.get();
    }
    else if (input == "5") {
        packSql(elf, db);
        std::cin.get();
    }
    else {
        printf("输入无效\n");
    }
    std::cin.get();
}

int main(int argc, char **argv)
{
    if (argv[1] == nullptr) [[unlikely]] {
        printf("未传递命令行参数\n");
        return 1;
    }

    std::string elf = "", db = "";
    readProfile(argv[1], elf, db);
    if (!fs::exists(elf) && fs::exists(db)) [[unlikely]] {
        printf("需要文件不存在，看看路径是不是写错了？检查argv[1]\n");
        return 2;
    }
    while (true) {
        system("clear");
        printf("\n当前bin文件: %s\n\n数据库文件: %s\n", elf.c_str(),
               db.c_str());
        print_theme(elf, db);
    }
}
