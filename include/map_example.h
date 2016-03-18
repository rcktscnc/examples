#include <iostream>
#include <fstream>
#include <map>

typedef std::map<std::string, std::string> strstr_map;
strstr_map keyword {
        {"byte ", "unsigned char "},
        {"uint ", "unsigned int "},
        {"int64 ", "long long int "},
        {"uint64 ", "unsigned long long int "},
        {"func ", "template<typename T> decltype(auto) "}
};

void translate(std::string& source, strstr_map& dictionary) {
    strstr_map::iterator iter;
    size_t pos;
    for (iter = dictionary.begin(); iter != dictionary.end(); ++iter) {
        while ((pos = source.find(iter->first)) != std::string::npos) {
            source.replace(pos, iter->first.length(), iter->second);
        }
    }
}

void save_text(const std::string& fname, const std::string& content) {
    std::fstream file;
    file.open(fname, std::fstream::out);
    if (file.is_open() == false) {
        std::cout << "Can't open file." << std::endl;
        return;
    }
    file << content;
}

std::string read_text(const std::string& fname) {
    std::string source, line;
	std::fstream file;
	file.open(fname, std::fstream::in);
	if (file.is_open() == false) {
        return source; // if openning fails, return empty string
	}
    while (std::getline(file, line)) {
        line += "\n";
        source.append(line);
    }
	return source;
}

void ctrl() {
    keyword.insert({{"that ", "nobody"}, {"this ", "WHAT THE FUCK "}});
    // apparently std::string has no move constructor.
    // which means the line below may be creating a whole new copy
    // of the returned value. Requires more research...
    std::string content = read_text("semantics.txt"); 
    translate(content, keyword);
    save_text("edit.txt", content);
}