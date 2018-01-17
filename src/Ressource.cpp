#include "Inst/RessourceInst.h"
#include "Ressource.h"

Ressource::Ressource( const char *desc ) : state( make_RessourceInst( this ) ), desc( desc ) {
}

void Ressource::write_to_stream( std::ostream &os ) const {
    os << desc;
}
