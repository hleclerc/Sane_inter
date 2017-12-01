#pragma once

#include "../AT.h"
#include "Vec.h"

/**
  This Vec is compatible with the interpreter (dynamic version)
*/
template<class T>
struct LVec {
    LVec() { _data.ptr->content = 0; _data.ptr->offset_in_bytes = 0; _size = 0; _rese = 0; }
    LVec( T a ) : LVec( Rese(), 1 ) { push_back( a ); }
    LVec( T a, T b ) : LVec( Rese(), 2 ) { push_back( a ); push_back( b ); }
    LVec( T a, T b, T c ) : LVec( Rese(), 3 ) { push_back( a ); push_back( b ); push_back( c ); }
    LVec( T a, T b, T c, T d ) : LVec( Rese(), 4 ) { push_back( a ); push_back( b ); push_back( c ); push_back( d ); }

    template<class... Args>
    LVec( Size, unsigned size, const Args &...args ) : LVec( Rese(), size ) {
        if ( size ) {
            for( size_t i = 0; i < size - 1; ++i )
                new( data() + i ) T( args... );
            new( data() + size - 1 ) T( std::forward<Args>( args )... );
        }
    }

    LVec( Size, unsigned size ) : LVec( Rese(), size ) { _size = size; for( size_t i = 0; i < size; ++i ) new( data() + i ) T; }
    LVec( Rese, unsigned rese ) : _size( 0 ), _rese( rese ) { _data.ptr->content = Variable::Content::alloc( sizeof( T ) * rese ); _data.ptr->content->cpt_use = 1; _data.ptr->offset_in_bytes = 0; }

    template<class U>
    LVec( const LVec<U> &b ) : LVec( Rese(), b.size() ) { _size = b.size(); for( size_t i = 0; i < b.size(); ++i ) new ( data() + i ) T( b[ i ] ); }

    LVec( const LVec<T> &b ) : LVec( Rese(), b.size() ) { _size = b.size(); for( size_t i = 0; i < b.size(); ++i ) new ( data() + i ) T( b[ i ] ); }

    template<class Iter>
    LVec( Iter b, const Iter &e ) : LVec() { for( ; b != e; ++b ) push_back( *b ); }

    ~LVec() {
        for( size_t i = _size; i--; ) data()[ i ].~T();
        if ( _rese && --_data.ptr->content->cpt_use == 0 )
            free( _data.ptr->content );
    }

    template<class U>
    void operator=( const LVec<U> &vec ) { resize( vec.size() ); for( size_t i = 0; i < vec.size(); ++i ) data()[ i ] = vec[ i ]; }

    void operator=( const LVec<T> &vec ) { resize( vec.size() ); for( size_t i = 0; i < vec.size(); ++i ) data()[ i ] = vec[ i ]; }

    operator Vec<T>() const {
        Vec<T> res( Rese(), size() );
        for( const T &val : *this )
            res << val;
        return res;
    }

    void     write_to_stream      ( std::ostream &os ) const;
    PT       size                 () const { return _size; }
    bool     empty                () const { return _size == 0; }
    T       *begin                () const { return data(); }
    T       *end                  () const { return data() + _size; }

    T       *data                 () const { return reinterpret_cast<T *>( _data.ptr->content->data ); }

    LVec    &operator<<           ( const T &val ) { push_back( val ); return *this; }
    LVec    &operator<<           ( T &&val ) { push_back( std::move( val ) ); return *this; }

    LVec    &append               ( const LVec<T> &vec ) { for( const T &val : vec ) push_back( val ); return *this; }

    template<class... Args>
    T       *push_back            ( Args &&...args ) {
        if ( _size == _rese )
            reserve( _size ? 2 * _size : 2 );
        return new ( data() + _size++ ) T( std::forward<Args>( args )... );
    }

    T       *insert               ( PT pos, const T &val ) {
        if ( pos == _size )
            return push_back( val );
        reserve( _size + 1 );
        new ( data() + _size ) T( std::move( data()[ _size - 1 ] ) );
        for( PT i = _size - 1; i > pos; --i )
            data()[ i ] = std::move( data()[ i - 1 ] );
        data()[ pos ] = val;
        ++_size;
        return data() + pos;
    }

    void     erase                ( PT pos ) { for( PT j = pos; j < _size - 1; ++j ) data()[ j ] = std::move( data()[ j + 1 ] ); pop_back(); }

    template<class Arg>
    T       *push_back_unique     ( Arg &&val ) { for( unsigned i = 0; i < size(); ++i ) if ( operator[]( i ) == val ) return &operator[]( i ); return push_back( std::forward<Arg>( val ) ); }

    void     remove_unordered     ( unsigned pos ) { if ( pos + 1 != size() ) operator[]( pos ) = std::move( back() ); pop_back(); }
    void     remove               ( unsigned pos ) { for( PT j = pos; j < _size - 1; ++j ) operator[]( j ) = std::move( operator[]( j + 1 ) ); pop_back(); }
    void     remove_duplicates    () { for( unsigned i = 0; i < size(); ++i ) for( unsigned j = i + 1; j < size(); ++j ) if ( operator[]( i ) == operator[]( j ) ) remove( j-- ); }

    void     remove_first         ( const T &val ) { for( unsigned i = 0; i < size(); ++i ) if ( operator[]( i ) == val ) return remove( i ); }
    template<class O>
    int      index_first          ( const O &val ) const { for( unsigned i = 0; i < size(); ++i ) if ( operator[]( i ) == val ) return i; return -1; }

    T       &back                 () const { return data()[ _size - 1 ]; }

    void     pop_back             () { data()[ --_size ].~T(); }
    T        pop_back_val         () { T res = std::move( back() ); pop_back(); return res; }

    template<class O>
    const T *find                 ( const O &val ) const { for( unsigned i = 0; i < size(); ++i ) if ( operator[]( i ) == val ) return &operator[]( i ); return 0; }

    template<class O>
    T       *find                 ( const O &val ) { for( unsigned i = 0; i < size(); ++i ) if ( operator[]( i ) == val ) return &operator[]( i ); return 0; }

    T        sum                  ( T init = 0 ) const { for( unsigned i = 0; i < size(); ++i ) init += operator[]( i ); return init; }

    template<class Op>
    void     remove_first_checking( Op &&op ) { for( unsigned i = 0; i < size(); ++i ) if ( op( operator[]( i ) ) ) return remove( i ); }

    template<class Op>
    int      index_first_checking ( Op &&op ) { for( unsigned i = 0; i < size(); ++i ) if ( op( operator[]( i ) ) ) return i; return -1; }

    LVec     subvec               ( unsigned beg, unsigned end ) const { LVec res; res.reserve( end - beg    ); for( unsigned i = beg; i < end   ; ++i ) res << operator[]( i ); return res; }
    LVec     without              ( unsigned index             ) const { LVec res; res.reserve( size() - 1   ); for( unsigned i = 0  ; i < size(); ++i ) if ( i != index ) res << operator[]( i ); return res; }
    LVec     up_to                ( unsigned end               ) const { LVec res; res.reserve( end          ); for( unsigned i = 0  ; i < end   ; ++i ) res << operator[]( i ); return res; }
    LVec     from                 ( unsigned beg               ) const { LVec res; res.reserve( size() - beg ); for( unsigned i = beg; i < size(); ++i ) res << operator[]( i ); return res; }

    bool     all_eq               () const { for( unsigned i = 1; i < size(); ++i ) if ( operator[]( i ) != operator[]( 0 ) ) return false; return true; }

    void     reserve              ( PT rese ) {
        if ( rese > _rese ) {
            _rese = rese;
            Variable::Content *old_data = _data.ptr->content;
            _data.ptr->content = Variable::Content::alloc( sizeof( T ) * rese );
            _data.ptr->content->cpt_use = old_data ? old_data->cpt_use : 1;
            for( PT i = 0; i < _size; ++i )
                new( data() + i ) T( std::move( ((T *)old_data->data)[ i ] ) );
            for( PT i = _size; i--; )
                ((T *)old_data->data)[ i ].~T();
            if ( old_data )
                free( old_data );
        }
    }

    void     resize               ( PT size ) { reserve( size ); for( PT i = _size; i < size; ++i ) new ( data() + i ) T; _size = size; }
    void     resize               ( PT size, const T &value ) { reserve( size ); for( PT i = _size; i < size; ++i ) new ( data() + i ) T( value ); _size = size; }

    const T &operator[]           ( unsigned index ) const { ASSERT_IF_DEBUG( index < size() ); return data()[ index ]; }
    T       &operator[]           ( unsigned index ) { ASSERT_IF_DEBUG( index < size() ); return data()[ index ]; }

    void     secure_set           ( unsigned index, const T &val ) { if ( index >= size() ) resize( index + 1 ); operator[]( index ) = val; }
    T       &secure_get           ( unsigned index, const T &def ) { if ( index >= size() ) resize( index + 1, def ); return operator[]( index ); }
    T       &secure_get           ( unsigned index ) { if ( index >= size() ) resize( index + 1 ); return operator[]( index ); }

    template<class U>
    void     secure_push_back     ( unsigned index, const U &val ) { if ( index >= size() ) resize( index + 1 ); operator[]( index ).push_back( val ); }

    template<class O>
    bool     contains             ( const O &val ) const { for( unsigned i = 0; i < size(); ++i ) if ( data()[ i ] == val ) return true; return false; }
    bool     only_has             ( const T &val ) const { for( unsigned i = 0; i < size(); ++i ) if ( data()[ i ] != val ) return false; return true; }

    LVec     uniqued              () const { LVec res; for( const T &val : *this ) res.push_back_unique( val ); return res; }

    LVec     concat               ( const LVec &v ) const { LVec res = *this; return res.append( v ); }

    template<class O>
    LVec<O>  map                  ( const std::function<O(const T &)> &f ) const { LVec<O> res( Rese(), size() ); for( const T &val : *this ) res << f( val ); return res; }

protected:

    AT _data;
    PT _size;
    PT _rese;
};

template<class T>
LVec<T> range_vec( T s ) {
    LVec<T> res;
    res.reserve( s );
    for( T i = 0; i < s; ++i )
        res << i;
    return res;
}

template<class T>
void LVec<T>::write_to_stream( std::ostream &os ) const {
    for( unsigned i = 0; i < size(); ++i )
        os << ( i ? " " : "" ) << operator[]( i );
}

