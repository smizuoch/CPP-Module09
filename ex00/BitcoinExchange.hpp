/* ====================================================================
   File: BitcoinExchange.hpp
   ==================================================================== */

   #ifndef BITCOINEXCHANGE_HPP
   #define BITCOINEXCHANGE_HPP
   
   #include <map>
   #include <string>
   
   class BitcoinExchange {
   public:
       explicit BitcoinExchange(const std::string &dbPath);
   
       // 与えられた日付に対応するレートを返す。
       // 完全一致がなくてもその日以前の直近レートを検索。
       // 見つからない場合 found=false。
       double getRate(const std::string &date, bool &found) const;
   
   private:
       std::map<std::string, double> _rates; // 昇順保持
   };
   
   #endif // BITCOINEXCHANGE_HPP