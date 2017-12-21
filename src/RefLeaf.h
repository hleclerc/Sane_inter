#pragma once

#include "System/EnumFlags.h"
#include "Ref.h"

/**
*/
class RefLeaf : public Ref {
public:
    enum class Flags: PI32 { NONE = 0, NOT_CONSTRUCTED = 1 };

    RefLeaf( const Value &value, Flags flags = Flags::NONE );

    virtual void  write_to_stream( std::ostream &os ) const override;
    virtual bool  is_shared      () const override;
    virtual Value get            () const override;
    virtual void  set            ( const Value &new_val, SI32 offset ) override;

    Value         value;
    Flags         flags;
};
ENUM_FLAGS( RefLeaf::Flags )
