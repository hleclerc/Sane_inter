#include "Gatherer.h"

Gatherer::Gatherer() {
}

Gatherer::Gatherer( AttrClone, const Gatherer *orig ) {
}

int Gatherer::nb_outputs() const {
    return 1;
}

void Gatherer::write_dot( std::ostream &os ) const {
    os << "Gatherer";
}

