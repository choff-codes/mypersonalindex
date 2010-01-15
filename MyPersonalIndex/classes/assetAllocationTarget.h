#ifndef ASSETALLOCATIONTARGET_H
#define ASSETALLOCATIONTARGET_H

class assetAllocationTarget
{
public:
    int id;
    double target;

    assetAllocationTarget(int p_id, double p_target): id(p_id), target(p_target) {}

    bool operator==(const assetAllocationTarget &other) const { return this->id == other.id && this->target == other.target; }
    bool operator!=(const assetAllocationTarget &other) const { return !(*this == other); }

};

#endif // ASSETALLOCATIONTARGET_H
