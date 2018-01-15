#include "RessourceState.h"
#include "Inst/Gatherer.h"
#include "gvm.h"

void RessourceState::write_to_stream(std::ostream &os) const {
    os << "lw: " << last_writer << " lr: [" << last_readers << "]";
}

bool RessourceState::operator==(const RessourceState &that) const {
    return last_readers == that.last_readers && last_writer == that.last_writer;
}

Value RessourceState::make_single_inst() const {
    if ( last_readers.empty() )
        return last_writer;

    if ( last_readers.size() == 1 )
        return last_readers[ 0 ];

    RcPtr<Inst> ga = new Gatherer;
    for( const Value &reader : last_readers )
        ga->add_child( reader );
    return { ga, 0, gvm->type_Ressource };
}
