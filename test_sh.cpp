#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>

void vulnerable_strcpy(const char* input) {
    char buffer[16];
    // [1] 버퍼 오버플로우 위험: 입력 길이를 체크하지 않고 복사
    strcpy(buffer, input);
    std::cout << "Buffer contents: " << buffer << std::endl;
}

void risky_format(const char* name) {
    char query[256];
    // [2] SQL 인젝션 + 버퍼 오버플로우: sprintf를 쓰며 name 그대로 삽입
    sprintf(query, "SELECT * FROM users WHERE name = '%s';", name);
    std::cout << "Query: " << query << std::endl;
}

void command_injection(const std::string& cmd) {
    // [3] 커맨드 인젝션: system()에 외부 입력을 직접 전달
    std::string full = "sh -c \"" + cmd + "\"";
    system(full.c_str());
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        vulnerable_strcpy(argv[1]);
        risky_format(argv[1]);
        command_injection(argv[1]);
    } else {
        std::cout << "Usage: " << argv[0] << " <input>" << std::endl;
    }
    return 0;
}
