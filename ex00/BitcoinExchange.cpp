/* ====================================================================
   File: BitcoinExchange.cpp
   ==================================================================== */

   #include "BitcoinExchange.hpp"
   #include <fstream>
   #include <sstream>
   #include <iostream>
   #include <cstdlib>   // strtod
   
   // ------------------------ ヘルパ ------------------------------- //
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
   
   // --------------------- 実装本体 ------------------------------- //
   BitcoinExchange::BitcoinExchange(const std::string &dbPath)
   {
	   std::ifstream infile(dbPath.c_str());
	   if (!infile) {
		   std::cerr << "Error: could not open database file." << std::endl;
		   return;
	   }
   
	   std::string line;
	   std::getline(infile, line); // ヘッダ行を読み飛ばす
	   while (std::getline(infile, line)) {
		   std::string date;
		   double price;
		   if (parseCsvLine(line, date, price))
			   _rates[date] = price;   // map なら重複日付は最後が有効
	   }
   }
   
   // 指定日付 date のレートを返す。見つからない場合は直前の日付を探す。
   double BitcoinExchange::getRate(const std::string &date, bool &found) const
   {
	   std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);
	   if (it != _rates.end() && it->first == date) {
		   found = true;              // 完全一致
		   return it->second;
	   }
	   if (it == _rates.begin()) {    // date が DB の最小日付より前
		   found = false;
		   return 0.0;
	   }
	   --it;                          // 1 つ前 = 直近の過去日付
	   found = true;
	   return it->second;
   }
   