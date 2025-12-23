#pragma once
#include "core/data/csv_reader.hpp"
#include "core/data/csv_writer.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include <functional>
#include <memory>

namespace services
{
    struct OrderRecord;
}

namespace persistence
{
    // Pure transformer: CsvRecord <-> OrderRecord
    class TradingDataAdapter
    {
    public:
        explicit TradingDataAdapter(std::shared_ptr<data::CsvReader> reader);

        // Stream records filtered by order type
        void ReadWithProcessor(dto::OrderType order_type, std::function<void(const services::OrderRecord &)> processor) const;

        // Stream all records without filtering
        void ReadWithProcessor(std::function<void(const services::OrderRecord &)> processor) const;

        // Write records
        static bool WriteAll(data::CsvWriter &writer,
                             const std::vector<services::OrderRecord> &records);

    private:
        std::shared_ptr<data::CsvReader> reader_;

        // Pure transformation
        static std::optional<services::OrderRecord> TransformToOrderRecord(const data::CsvRecord &record);
        static data::CsvRecord TransformFromOrderRecord(const services::OrderRecord &order);
    };

}