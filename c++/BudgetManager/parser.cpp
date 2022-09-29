#include "parser.h"

using namespace std;

namespace queries {
class ComputeIncome : public ComputeQuery {
public:
    using ComputeQuery::ComputeQuery;
    ReadResult Process(const BudgetManager& budget) const override {
        return {budget.ComputeSum(GetFrom(), GetTo())};
    }

    class Factory : public QueryFactory {
    public:
        std::unique_ptr<Query> Construct(std::string_view config) const override {
            auto parts = Split(config, ' ');
            return std::make_unique<ComputeIncome>(Date::FromString(parts[0]), Date::FromString(parts[1]));
        }
    };
};

class Alter : public ModifyQuery {
public:
    Alter(Date from, Date to, double amount)
        : ModifyQuery(from, to)
        , amount_(amount) {
    }

    void Process(BudgetManager& budget) const override {
        double day_income = amount_ / (Date::ComputeDistance(GetFrom(), GetTo()) + 1);

        budget.AddBulkOperation(GetFrom(), GetTo(), BulkMoneyAdder{day_income});
    }

    class Factory : public QueryFactory {
    public:
        std::unique_ptr<Query> Construct(std::string_view config) const override {
            auto parts = Split(config, ' ');
            double payload = std::stod(std::string(parts[2]));
            return std::make_unique<Alter>(Date::FromString(parts[0]), Date::FromString(parts[1]), payload);
        }
    };

private:
    double amount_;
};

class PayTax : public ModifyQuery {
public:
    using ModifyQuery::ModifyQuery;

    void Process(BudgetManager& budget) const override {
        budget.AddBulkOperation(GetFrom(), GetTo(), BulkTaxApplier{1});
    }

    class Factory : public QueryFactory {
    public:
        std::unique_ptr<Query> Construct(std::string_view config) const override {
            auto parts = Split(config, ' ');
            return std::make_unique<PayTax>(Date::FromString(parts[0]), Date::FromString(parts[1]));
        }
    };

private:
};

class Spend : public ModifyQuery {
public:
    using ModifyQuery::ModifyQuery;

    void Process(BudgetManager& budget) const override {
        budget.AddBulkOperation(GetFrom(), GetTo(), BulkTaxApplier{ 1 });
    }

    class Factory : public QueryFactory {
    public:
        std::unique_ptr<Query> Construct(std::string_view config) const override {
            auto parts = Split(config, ' ');
            return std::make_unique<Spend>(Date::FromString(parts[0]), Date::FromString(parts[1]));
        }
    };

private:
};

}  // namespace queries

const QueryFactory& QueryFactory::GetFactory(std::string_view id) {
    static queries::ComputeIncome::Factory compute_income;
    static queries::Alter::Factory earn;
    static queries::PayTax::Factory pay_tax;
    static std::unordered_map<std::string_view, const QueryFactory&> factories
        = {{"ComputeIncome"sv, compute_income}, {"Earn"sv, earn}, {"PayTax"sv, pay_tax}};

    return factories.at(id);
}
