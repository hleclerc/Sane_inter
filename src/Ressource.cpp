#include "Inst/RessourceInst.h"
#include "Ressource.h"

Ressource::Ressource( const char *desc ) : state( new RefLeaf( make_RessourceInst( this ) ) ), desc( desc ) {
    state->creation_inter_date = 0;
}

void Ressource::write_to_stream( std::ostream &os ) const {
    os << desc;
}
