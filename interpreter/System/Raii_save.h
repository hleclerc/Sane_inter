#ifndef RAIISAVE_H
#define RAIISAVE_H

template<class T>
struct Raii_save {
    Raii_save( T &val, const T &nval ) : Raii_save( val ) { val = nval; }
    Raii_save( T &val ) : ref( val ), old( val ) {}
    ~Raii_save() { ref = old; }
    T &ref, old;
};

template<class T>
Raii_save<T> raii_save( T &val ) {
    return val;
}

template<class T,class U>
Raii_save<T> raii_save( T &val, const U &nval ) {
    return Raii_save<T>( val, nval );
}


#endif // RAIISAVE_H
