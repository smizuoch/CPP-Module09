/* ====================================================================
   File: main.cpp
   ==================================================================== */

   #include "BitcoinExchange.hpp"
   #include <iostream>
   #include <fstream>
   #include <sstream>
   #include <iomanip>
   #include <cstdlib>
   
   // ------------------------------------------------------------
   //  ヘルパ: うるう年判定 (グレゴリオ暦)
   // ------------------------------------------------------------
   static bool isLeap(int year)
   {
	   return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
   }
   
   static bool isValidDate(const std::string &s)
   {
	   // 形式 YYYY-MM-DD をチェック
	   if (s.size() != 10 || s[4] != '-' || s[7] != '-')
		   return false;
	   for (std::size_t i = 0; i < s.size(); ++i)
		   if (i != 4 && i != 7 && !std::isdigit(s[i]))
			   return false;
   
	   int y = std::atoi(s.substr(0,4).c_str());
	   int m = std::atoi(s.substr(5,2).c_str());
	   int d = std::atoi(s.substr(8,2).c_str());
   
	   if (m < 1 || m > 12)
		   return false;
   
	   static const int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	   int maxDay = days[m-1];
	   if (m == 2 && isLeap(y))
		   maxDay = 29;
   
	   if (d < 1 || d > maxDay)
		   return false;
   
	   return true;
   }
   
   static bool parseValue(const std::string &s, double &val)
   {
	   char *end = NULL;
	   val = std::strtod(s.c_str(), &end);
	   if (*end != '\0')
		   return false;
	   return true;
   }
   
   int main(int argc, char **argv)
   {
	   if (argc != 2 && argc != 3) {
		   std::cerr << "Usage: ./btc <input_file> [database_file]" << std::endl;
		   return 1;
	   }
   
	   const std::string inputPath = argv[1];
	   const std::string dbPath    = (argc == 3) ? argv[2] : "data.csv";
   
	   std::ifstream infile(inputPath.c_str());
	   if (!infile) {
		   std::cerr << "Error: could not open file." << std::endl;
		   return 1;
	   }
   
	   BitcoinExchange exch(dbPath);
   
	   std::string line;
	   std::getline(infile, line); // ヘッダ行 "date | value"
	   while (std::getline(infile, line)) {
		   std::size_t pipe = line.find('|');
		   if (pipe == std::string::npos) {
			   std::cerr << "Error: bad input => " << line << std::endl;
			   continue;
		   }
		   std::string date  = line.substr(0, pipe);
		   std::string value = line.substr(pipe + 1);
		   if (!date.empty() && date[date.size()-1] == ' ')
			   date.erase(date.size()-1);
		   if (!value.empty() && value[0] == ' ')
			   value.erase(0,1);
   
		   if (!isValidDate(date)) {
			   std::cerr << "Error: bad input => " << date << std::endl;
			   continue;
		   }
		   double amount;
		   if (!parseValue(value, amount)) {
			   std::cerr << "Error: bad input => " << value << std::endl;
			   continue;
		   }
		   if (amount < 0) {
			   std::cerr << "Error: not a positive number." << std::endl;
			   continue;
		   }
		   if (amount > 1000) {
			   std::cerr << "Error: too large a number." << std::endl;
			   continue;
		   }
   
		   bool found;
		   double rate = exch.getRate(date, found);
		   if (!found) {
			   std::cerr << "Error: no price available before " << date << std::endl;
			   continue;
		   }
		   double result = rate * amount;
		   std::cout << date << " => " << value << " = "
					 << std::fixed << std::setprecision(2) << result << std::endl;
	   }
	   return 0;
   }
   