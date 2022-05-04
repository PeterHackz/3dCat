#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class objCat
{
  public:
	int Gv, Gvn, Gvt;
	std::vector<std::string> obj;
	objCat()
	{
		Gv = 0;
		Gvn = 0;
		Gvt = 0;
	}
};

std::string
readFile(std::string filep)
{
	std::ifstream file(filep);
	std::string result;
	if (file)
	{
		std::ostringstream ss;
		ss << file.rdbuf();
		result = ss.str();
	}
	return result;
}

bool startsWith(std::string str, std::string key)
{
	if (str.rfind(key, 0) == 0)
	{
		return true;
	}
	return false;
}

std::vector<std::string> splitSpace(std::string str)
{
	std::vector<std::string> result;
	std::stringstream ss(str);
	for (std::string line; std::getline(ss, line, ' ');)
		result.push_back(line);
	result.erase(result.begin());
	return result;
}

std::vector<std::string> splitSlash(std::string str)
{
	std::vector<std::string> result;
	std::stringstream ss(str);
	for (std::string line; std::getline(ss, line, '/');)
		result.push_back(line);
	return result;
}

std::vector<std::string> splitLines(std::string str)
{
	std::vector<std::string> result;
	std::stringstream ss(str);
	for (std::string line; std::getline(ss, line, '\n');)
		result.push_back(line);
	return result;
}

objCat combineChunks(objCat objLion, std::vector<std::string> lines)
{
	std::string line, sub;
	std::vector<std::string> x, y;
	int i, o;
	int v = 0, vn = 0, vt = 0;
	for (i = 0; i < lines.size(); i++)
	{
		line = lines.at(i);
		if (startsWith(line, "v "))
		{
			v += 1;
			objLion.obj.push_back(line);
		}
		else if (startsWith(line, "vn "))
		{
			vn += 1;
			objLion.obj.push_back(line);
		}
		else if (startsWith(line, "vt "))
		{
			vt += 1;
			objLion.obj.push_back(line);
		}
		else if (startsWith(line, "f "))
		{
			std::string sub;
			x = splitSpace(line);
			for (o = 0; o < x.size(); o++)
			{
				y = splitSlash(x.at(o));
				sub += std::to_string(std::stoi(y.at(0)) + objLion.Gv) + "/" + std::to_string(std::stoi(y.at(1)) + objLion.Gvt) + "/" + std::to_string(std::stoi(y.at(2)) + objLion.Gvn) + " ";
			}
			objLion.obj.push_back("f " + sub);
		}
		else
		{
			objLion.obj.push_back(line);
		}
	}
	objLion.Gv += v;
	objLion.Gvn += vn;
	objLion.Gvt += vt;
	return objLion;
}

void combineObj(std::vector<std::string> files, std::string output)
{
	std::string content;
	std::vector<std::string> lines;
	objCat objKitten;
	int c;
	std::ofstream file(output);
	for (int index = 0; index < files.size(); index++)
	{
		content = readFile(files.at(index));
		lines = splitLines(content);
		objKitten = combineChunks(objKitten, lines);
	}
	for (c = 0; c < objKitten.obj.size(); c++)
	{
		file << objKitten.obj.at(c) + "\n";
	}
	file.close();
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		throw std::invalid_argument("Invalid args count!");
	}
	int index = 0;
	std::vector<std::string> files;
	std::string sub = "";
	for (index = 2; index < argc; index++)
	{
		files.push_back(std::string(argv[index]));
		sub += std::string(argv[index]) + " ";
	}
	std::cout << "combining: " << sub;
	combineObj(files, std::string(argv[1]));
	return 0;
}
