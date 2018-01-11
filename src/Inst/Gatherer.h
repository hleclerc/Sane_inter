#pragma once

#include "Clonable.h"

/**
*/
class Gatherer : public Clonable<Gatherer> {
public:
    Gatherer();
    Gatherer( AttrClone, const Gatherer *orig );

    virtual void  write_dot        ( std::ostream &os ) const override;
};

