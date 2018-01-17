#include "Inst/RessourceInst.h"
#include "Rss.h"

Rss::Rss( const char *desc ) : state( new RefLeaf( make_RessourceInst( this ) ) ), desc( desc ) {
    state->creation_inter_date = 0;
}

void Rss::write_to_stream( std::ostream &os ) const {
    os << desc;
}
