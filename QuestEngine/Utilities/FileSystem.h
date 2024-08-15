#ifndef  _FILESYSTEM_H_
#define _FILESYSTEM_H_
#include <fstream>
#include <sstream>
#include <string>


class FileSystem
{
public:
	static std::string get_file_contents(const std::string& filename);
};

#endif