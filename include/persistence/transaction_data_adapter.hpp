#pragma once
#include "core/data/csv_reader.hpp"
#include "core/data/csv_writer.hpp"
#include <functional>
#include <memory>

namespace services
{
    struct Transaction;
}

namespace persistence
{
    // Pure transformer: CsvRecord <-> Transaction
    class TransactionDataAdapter
    {
    public:
        explicit TransactionDataAdapter(std::shared_ptr<data::CsvReader> reader);

        // Stream records
        void ReadWithProcessor(std::function<void(const services::Transaction &)> processor) const;

        // Write records
        static bool WriteAll(data::CsvWriter &writer,
                             const std::vector<services::Transaction> &transactions);

    private:
        std::shared_ptr<data::CsvReader> reader_;

        static std::optional<services::Transaction> TransformToTransaction(const data::CsvRecord &record);
    };

}
