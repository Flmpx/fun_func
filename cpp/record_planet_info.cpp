#include <iostream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <limits>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <stdfloat>
#include <cstdint>

namespace hm {
	auto clearBuf(std::istream& is) -> std::istream&;
}

namespace fl {
	class planet {
	public:
		const static int LEN {32};
	private:
		char name_[LEN];
		std::int64_t population_;
		std::float64_t g_;
	public:
		planet(const char name[LEN] = "None", std::size_t population = 0, std::float64_t g = 0.0);
		~planet() = default;
		
		auto name() -> const char*          {return name_;}
		auto population() -> std::int64_t   {return population_;}
		auto gravity() -> std::float64_t   	{return g_;}
		
		auto name(const char newName[LEN]) -> void;
		auto population(std::int64_t population) -> void;
		auto gravity(std::float64_t gravity) -> void;
		
		friend auto operator<<(std::ostream& os, const planet& p) -> std::ostream&;
	};
}


namespace hm {
	auto clearBuf(std::istream& is) -> std::istream& {
		if (is.fail() && !is.eof()) {
			is.clear();
		}
		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		
		return is;
	}
}

namespace fl {
	planet::planet(const char name[LEN], std::size_t population, std::float64_t g) {
		strncpy(name_, name, LEN - 1);
		name_[LEN - 1] = '\0';
		population_ = population;
		g_ = g;
	}
	
	auto planet::name(const char name[LEN]) -> void {
		strncpy(name_, name, LEN - 1);
		name_[LEN - 1] = '\0';
	}
	
	auto planet::population(std::int64_t population) -> void {
		population_ = population;
	}
	
	auto planet::gravity(std::float64_t gravity) -> void {
		g_ = gravity;
	}
	
	auto operator<<(std::ostream& os, const planet& p) -> std::ostream& {
		os << std::setw(p.LEN)  << p.name_
		   << std::setw(22)     << p.population_
		   << std::setw(22)     << p.g_;
		
		return os;
	}
}


auto input(fl::planet& p) -> bool {
	fl::planet tmp {};
	
	
	std::cout << "Enter the name of planet(a blank line to quit): ";
	char tmp_name[fl::planet::LEN] {'\0'};
	std::cin.get(tmp_name, fl::planet::LEN);
	hm::clearBuf(std::cin);
	if (tmp_name[0] == '\0') {
		return false;
	}
	tmp.name(tmp_name);
	
	
	std::int64_t tmp_population {0};
	std::cout << "Enter " << tmp_name << "'s population: ";
	std::cin >> tmp_population;
	hm::clearBuf(std::cin);
	tmp.population(tmp_population);
	
	std::float64_t tmp_gravity {0.0};
	std::cout << "Enter " << tmp_name << "'s gravity: ";
	std::cin >> tmp_gravity;
	hm::clearBuf(std::cin);
	tmp.gravity(tmp_gravity);
	
	p = tmp;
	return true;
}

auto showFile(std::ifstream& in) -> std::istream& {
	fl::planet tmp {};
	std::cout << std::setw(9)               << "Id"
			  << std::setw(fl::planet::LEN) << "Name" 
			  << std::setw(22)              << "Population" 
			  << std::setw(22)              << "Gravity"; 
	std::cout << "\n";
	
	int cnt {0};
	while (in.read((char*)&tmp, sizeof(fl::planet))) {
		std::cout << std::setw(9) << cnt << tmp << "\n";
		cnt++;
	}
	
	return in;
}


auto add(const std::string& fileName) -> void {
	// write
	std::ofstream fout(fileName.c_str(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
	if (!fout.is_open()) {
		std::cerr << "Can't open " << fileName << " !!!\n";
		std::exit(EXIT_FAILURE);
	}
	
	fl::planet tmp {};
	while (input(tmp)) {
		fout.write((const char*)&tmp, sizeof(fl::planet));
	}
	fout.close();
}

auto change(const std::string& fileName) -> void {
	// change
	std::fstream finout(fileName.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	if (!finout.is_open()) {
		std::cout << "Can't open " << fileName << " !!!\n";
		std::exit(EXIT_FAILURE);
	}
	
	// get the number of elememts
	int cnt = finout.seekg(0, std::ios_base::end).tellg() / sizeof(fl::planet);
	finout.seekg(0, std::ios_base::beg);
	
	// Enter the id
	std::cout << "Enter the id that you want to change: ";
	int id {-1};
	std::cin >> id;
	hm::clearBuf(std::cin);
	if (id < 0 || id >= cnt) {
		std::cerr << "Invalid Id!!!\n";
		exit(EXIT_FAILURE);
	}
	
	// output specialized id
	fl::planet tmp {};
	finout.seekg(id * sizeof(fl::planet));
	finout.read((char*)&tmp, sizeof(fl::planet));
	finout.seekg(0, std::ios_base::beg);
	std::cout << std::setw(9) << id << tmp << "\n";
	
	if (input(tmp)) {
		finout.seekp(id * sizeof(fl::planet));
		finout.write((const char*)&tmp, sizeof(fl::planet));
	}
	finout.close();
}

auto main() -> int
{
	const std::string fileName("bin");
	
	// read
	std::ifstream fin(fileName.c_str(), std::ios_base::in | std::ios_base::binary);;
	
	if (fin.is_open()) {
		std::cout << "Here are the content of " << fileName << ":\n";
		showFile(fin);
		fin.close();
	}
	
	
	std::cout << "Switch the choice<a/c/d>: ";
	char choice {'\0'};
	std::cin >> choice;
	hm::clearBuf(std::cin);
	switch (choice) {
		case 'a': case 'A': add(fileName);      break;
		case 'c': case 'C': change(fileName);   break;
		default: 
		std::cerr << "Invalid Choice!!!\n"; 
		exit(EXIT_FAILURE);
	}
	
	
	// read twice
	fin.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
	
	if (fin.is_open()) {
		std::cout << "Here are the new content of " << fileName << ":\n";
		showFile(fin);
		fin.close();
	}
	
	return 0;
}