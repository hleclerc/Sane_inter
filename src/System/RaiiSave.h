#pragma once

template<class T>
struct RaiiSave {
    RaiiSave( T &val, const T &nval ) : RaiiSave( val ) { val = nval; }
    RaiiSave( T &val ) : ref( val ), old( val ) {}
    ~RaiiSave() { ref = old; }
    T &ref, old;
};

template<class T>
RaiiSave<T> raii_save( T &val ) {
    return val;
}

template<class T,class U>
RaiiSave<T> raii_save( T &val, const U &nval ) {
    return RaiiSave<T>( val, nval );
}

