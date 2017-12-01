#pragma once

#include "Stream.h"

/**
 *
 */
template<class T, bool use_malloc=false>
struct Rc_ptr {
    Rc_ptr() : data( 0 ) {}
    Rc_ptr( T *obj ) : data( obj ) { if ( data ) ++data->cpt_use; }
    Rc_ptr( Rc_ptr &&obj ) : data( obj.data ) { obj.data = 0; }
    Rc_ptr( const Rc_ptr &obj ) : data( obj.data ) { if ( data ) ++data->cpt_use; }

    template<class U>
    Rc_ptr( const Rc_ptr<U> &obj ) : data( obj.data ) { if ( data ) ++data->cpt_use; }

    template<class U>
    Rc_ptr( Rc_ptr<U> &&obj ) : data( obj.data ) { obj.data = 0; }

    ~Rc_ptr() {
        if ( data && --data->cpt_use == 0 )
            _free( data );
    }

    void _free( T *data ) {
        if ( use_malloc ) {
            data->~T();
            free( data );
        } else
            delete data;
    }

    Rc_ptr &operator=( T *obj ) {
        if ( obj )
            ++obj->cpt_use;
        if ( data && --data->cpt_use == 0 )
            _free( data );
        data = obj;
        return *this;
    }

    template<class U>
    Rc_ptr &operator=( U *obj ) {
        if ( obj )
            ++obj->cpt_use;
        if ( data && --data->cpt_use == 0 )
            _free( data );
        data = obj;
        return *this;
    }

    Rc_ptr &operator=( const Rc_ptr &obj ) {
        if ( obj.data )
            ++obj.data->cpt_use;
        if ( data && --data->cpt_use == 0 )
            _free( data );
        data = obj.data;
        return *this;
    }

    template<class U>
    Rc_ptr &operator=( const Rc_ptr<U> &obj ) {
        if ( obj.data )
            ++obj.data->cpt_use;
        if ( data && --data->cpt_use == 0 )
            _free( data );
        data = obj.data;
        return *this;
    }

    Rc_ptr &operator=( Rc_ptr &&obj ) {
        if ( data && data != obj.data && --data->cpt_use == 0 )
            _free( data );
        data = obj.data;
        obj.data = 0;
        return *this;
    }

    template<class U>
    Rc_ptr &operator=( Rc_ptr<U> &&obj ) {
        if ( data && data != obj.data && --data->cpt_use == 0 )
            _free( data );
        data = obj.data;
        obj.data = 0;
        return *this;
    }


    operator bool() const { return data; }

    void clear() { if ( data ) { if ( --data->cpt_use == 0 ) _free(); data = nullptr; } }

    bool operator==( const T            *p ) const { return data == p;      }
    bool operator==( const Rc_ptr<T>    &p ) const { return data == p.data; }
    // bool operator==( const ConstPtr<T> &p ) const { return data == p.data; }

    bool operator!=( const T            *p ) const { return data != p;      }
    bool operator!=( const Rc_ptr<T>    &p ) const { return data != p.data; }
    // bool operator!=( const ConstPtr<T> &p ) const { return data != p.data; }

    bool operator< ( const T            *p ) const { return data <  p;      }
    bool operator< ( const Rc_ptr<T>    &p ) const { return data <  p.data; }
    // bool operator< ( const ConstPtr<T> &p ) const { return data <  p.data; }

    bool operator<=( const T            *p ) const { return data <= p;      }
    bool operator<=( const Rc_ptr<T>    &p ) const { return data <= p.data; }
    // bool operator<=( const ConstPtr<T> &p ) const { return data <= p.data; }

    bool operator> ( const T            *p ) const { return data >  p;      }
    bool operator> ( const Rc_ptr<T>    &p ) const { return data >  p.data; }
    // bool operator> ( const ConstPtr<T> &p ) const { return data >  p.data; }

    bool operator>=( const T            *p ) const { return data >= p;      }
    bool operator>=( const Rc_ptr<T>    &p ) const { return data >= p.data; }
    // bool operator>=( const ConstPtr<T> &p ) const { return data >= p.data; }

    T *ptr() const { return data; }

    T *operator->() const { return data; }

    T &operator*() const { return *data; }

    void write_to_stream( std::ostream &os ) const {
        if ( data )
            os << *data;
        else
            os << "NULL";
    }

    T *data;
};

/**
 *
 */
struct Rc_obj {
    Rc_obj() : cpt_use( 0 ) {}
    mutable size_t cpt_use;
};

template<class T>
inline const T *inc_ref( const T *p ) {
    ++p->cpt_use;
    return p;
}

template<class T>
inline T *inc_ref( T *p ) {
    ++p->cpt_use;
    return p;
}

template<class T>
inline void dec_ref( const T *ptr ) {
    if ( --ptr->cpt_use == 0 )
        ptr->_free();
}

template<class T>
bool operator==( const T *p, const Rc_ptr<T> &q ) { return p == q.data; }
