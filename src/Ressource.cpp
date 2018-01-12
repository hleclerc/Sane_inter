#include "Ressource.h"

Ressource::Ressource( const char *desc ) : desc( desc ) {
}

void Ressource::write_to_stream( std::ostream &os ) const {
    os << desc;
}
