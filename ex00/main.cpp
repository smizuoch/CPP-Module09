/* ====================================================================
   File: main.cpp
   ==================================================================== */

   #include "BitcoinExchange.hpp"
   #include <iostream>
   #include <fstream>
   #include <sstream>
   #include <iomanip>   // setprecision
   #include <cstdlib>   // strtod
   
   // ------------------- 入力検証ユーティリティ --------------------- //
   static bool isValidDate(const std::string &s)
   {
	   if (s.size() != 10 || s[4] != '-' || s[7] != '-')
		   return false;
	   for (std::size_t i = 0; i < s.size(); ++i)
		   if (i != 4 && i != 7 && !std::isdigit(s[i]))
			   return false;
	   int y = std::atoi(s.substr(0,4).c_str());
	   int m = std::atoi(s.substr(5,2).c_str());
	   int d = std::atoi(s.substr(8,2).c_str());
	   if (m < 1 || m > 12 || d < 1 || d > 31)
		   return false; // 厳密な日数チェックは省略
	   (void)y; // y は 0000〜9999 范囲とみなす
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
   
   // --------------------------- main ------------------------------ //
   int main(int argc, char **argv)
   {
	   if (argc != 2) {
		   std::cerr << "Error: could not open file." << std::endl;
		   return 1;
	   }
	   std::ifstream infile(argv[1]);
	   if (!infile) {
		   std::cerr << "Error: could not open file." << std::endl;
		   return 1;
	   }
   
	   BitcoinExchange exch("data.csv");  // デフォルト DB 名
   
	   std::string line;
	   std::getline(infile, line); // ヘッダ行 "date | value" をスキップ
	   while (std::getline(infile, line)) {
		   // 行を "date | value" に分割
		   std::size_t pipe = line.find('|');
		   if (pipe == std::string::npos) {
			   std::cerr << "Error: bad input => " << line << std::endl;
			   continue;
		   }
		   std::string date  = line.substr(0, pipe);
		   std::string value = line.substr(pipe + 1);
		   // 余分な空白をトリム
		   if (!date.empty() && date[date.size()-1] == ' ')
			   date.erase(date.size()-1);
		   if (!value.empty() && value[0] == ' ')
			   value.erase(0,1);
   
		   // --- 妥当性チェック --- //
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
		   // 結果を小数点以下 2 桁で表示
		   std::cout << date << " => " << value << " = "
					 << std::fixed << std::setprecision(2) << result << std::endl;
	   }
	   return 0;
   }