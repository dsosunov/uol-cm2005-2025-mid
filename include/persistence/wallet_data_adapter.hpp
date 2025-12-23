#pragma once
#include "core/data/csv_reader.hpp"
#include "core/data/csv_writer.hpp"
#include <map>
#include <memory>
#include <string>

namespace persistence
{

    class WalletDataAdapter
    {
    public:
        explicit WalletDataAdapter(std::shared_ptr<data::CsvReader> reader);

        std::map<std::string, double, std::less<>> ReadBalances(int user_id) const;

        static bool WriteBalances(data::CsvWriter &writer, int user_id,
                                  const std::map<std::string, double, std::less<>> &balances);

    private:
        std::shared_ptr<data::CsvReader> reader_;
    };

}
