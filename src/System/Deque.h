#pragma once

#include <iostream>
#include <deque>

template<class T>
struct Deque : std::deque<T> {
    void     write_to_stream ( std::ostream &os ) const;
    Deque   &operator<<      ( const T &val ) { this->emplace_back( val ); return *this; }
    Deque   &operator<<      ( T &&val ) { this->emplace_back( std::move( val ) ); return *this; }

    template<class... Args>
    T       *new_back_item   ( Args &&...args ) { this->emplace_back( std::forward<Args>( args )... ); return &this->back(); }

    T        pop_front_val   () { T res = this->front(); this->pop_front(); return res; }
    T        pop_back_val    () { T res = this->back (); this->pop_back (); return res; }

    T       *push_back_unique( const T &val ) { for( auto &v : *this ) if ( v == val ) return &v; return new_back_item( val );  }
    T       &push_back_val   ( const T &val ) { this->push_back( val ); return this->back(); }
};


template<class T>
void Deque<T>::write_to_stream( std::ostream &os ) const {
    for( unsigned i = 0; i < this->size(); ++i )
        os << ( i ? " " : "" ) << this->operator[]( i );
}
