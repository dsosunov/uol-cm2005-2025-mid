#pragma once
#include "persistence/base_data_adapter.hpp"
#include "dto/constants.hpp"
#include <functional>
#include <memory>
namespace services
{
    struct OrderRecord;
}
namespace persistence
{
    class TradingDataAdapter : public BaseDataAdapter<services::OrderRecord>
    {
    public:
        explicit TradingDataAdapter(std::shared_ptr<data::CsvReader> reader);
        void ReadWithProcessor(dto::OrderType order_type, const std::function<void(const services::OrderRecord &)> &processor) const;
        void ReadWithProcessor(const std::function<void(const services::OrderRecord &)> &processor) const;
        static bool WriteAll(data::CsvWriter &writer, const std::vector<services::OrderRecord> &records);

    protected:
        std::optional<services::OrderRecord> TransformToEntity(const data::CsvRecord &record) const override;

    private:
        static data::CsvRecord TransformFromOrderRecord(const services::OrderRecord &order);
    };
}