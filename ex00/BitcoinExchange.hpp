/* ====================================================================
   File: BitcoinExchange.hpp
   ==================================================================== */

   #ifndef BITCOINEXCHANGE_HPP
   #define BITCOINEXCHANGE_HPP
   
   // --- 標準ライブラリ ---------------------------------------------- //
   #include <map>      // 日付 → レート (ソート済連想配列)
   #include <string>
   
   class BitcoinExchange {
   public:
	   // コンストラクタ: データベース(csv)をパースして _rates に格納
	   explicit BitcoinExchange(const std::string &dbPath);
   
	   // 指定日付のレート取得。
	   // 見つかった場合 found=true を返し、rate を返す。
	   // 見つからない場合 (入力がデータベースより前) は found=false。
	   double getRate(const std::string &date, bool &found) const;
   
   private:
	   std::map<std::string, double> _rates; // キー昇順保持で lower_bound 利用
   };
   
   #endif // BITCOINEXCHANGE_HPP