#pragma once

#include "entities.h"
#include "summing_segment_tree.h"

#include <cstdint>
#include <cmath>

struct BulkMoneyAdder {
    double delta = {};
};

struct BulkMoneySpender {
    double delta = {};
};

struct BulkTaxApplier {
    double factor = 1.0;
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add)
        : add_(add) {
    }

    BulkLinearUpdater(const BulkTaxApplier& tax)
        : tax_(tax) {
    }

    BulkLinearUpdater(const BulkMoneySpender& spend) : spend_(spend) {}

    void CombineWith(const BulkLinearUpdater& other) {
        tax_.factor *= other.tax_.factor;
        add_.delta = add_.delta * other.tax_.factor + other.add_.delta;
        spend_.delta += other.spend_.delta;
    }

    DayInfo Collapse(DayInfo origin, IndexSegment segment) const {
        origin.earned = origin.earned * tax_.factor + add_.delta * static_cast<double>(segment.length());
        origin.spent = origin.spent + spend_.delta * static_cast<double>(segment.length());
        return origin;
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
    BulkMoneySpender spend_;
};