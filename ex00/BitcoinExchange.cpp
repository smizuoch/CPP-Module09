/* ====================================================================
   File: BitcoinExchange.cpp
   ==================================================================== */

   #include "BitcoinExchange.hpp"
   #include <fstream>
   #include <sstream>
   #include <iostream>
   #include <cstdlib>
   
   static bool parseCsvLine(const std::string &line, std::string &date, double &price)
   {
	   std::size_t comma = line.find(',');
	   if (comma == std::string::npos)
		   return false;
	   date  = line.substr(0, comma);
	   std::string valueStr = line.substr(comma + 1);
	   char *endptr = NULL;
	   price = std::strtod(valueStr.c_str(), &endptr);
	   return (*endptr == '\0');
   }
   
   BitcoinExchange::BitcoinExchange(const std::string &dbPath)
   {
	   std::ifstream infile(dbPath.c_str());
	   if (!infile) {
		   std::cerr << "Error: could not open database file." << std::endl;
		   return;
	   }
	   std::string line;
	   std::getline(infile, line); // ヘッダ行 "date,exchange_rate"
	   while (std::getline(infile, line)) {
		   std::string date;
		   double price;
		   if (parseCsvLine(line, date, price))
			   _rates[date] = price;
	   }
   }
   
   double BitcoinExchange::getRate(const std::string &date, bool &found) const
   {
	   std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);
	   if (it != _rates.end() && it->first == date) {
		   found = true;
		   return it->second;
	   }
	   if (it == _rates.begin()) {
		   found = false;
		   return 0.0;
	   }
	   --it;
	   found = true;
	   return it->second;
   }
