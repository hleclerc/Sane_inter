#include "Gatherer.h"

Gatherer::Gatherer() {
}

Gatherer::Gatherer( AttrClone, const Gatherer *orig ) {
}

void Gatherer::write_dot( std::ostream &os ) const {
    os << "Gatherer";
}

