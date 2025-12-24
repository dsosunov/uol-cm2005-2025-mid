#include "forms/shared/fields/currency_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/transaction_query.hpp"
#include "forms/shared/validators/currency_validator.hpp"

namespace forms::shared
{

template <typename TDTO>
CurrencyField<TDTO>::CurrencyField(std::set<std::string, std::less<>> allowed_currencies,
                                   MemberPtr member_ptr)
    : form::TextField("currency", "Currency",
                      form::SimpleFieldBinder<TDTO, std::string>(member_ptr),
                      std::make_shared<CurrencyValidator>(std::move(allowed_currencies)))
{
}

template class CurrencyField<dto::TransactionQuery>;

} // namespace forms::shared
