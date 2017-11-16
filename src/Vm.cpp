#include "../lib/globals/var_wrappers.h"
#include <boost/filesystem.hpp>
#include "System/Stream.h"
#include "Inst/Cst.h"
#include "Vm.h"

//// nsmake lib_name boost_filesystem

Vm::Vm() {
    init_mode = false;
    error_stream = &std::cerr;
}

Variable Vm::import( const String &filename, const String &import_dir, bool display_lexems ) {
    String abso = path( filename, import_dir );
    if ( abso.empty() )
        return add_error( "file '{}' not found", filename ), ref_error;

    // already imported ?
    auto iter = imported.find( abso );
    if ( iter != imported.end() )
        return iter->second;

    // prepare a result variable
    Var_Import res;
    imported.insert( iter, std::make_pair( abso, res.var ) );

//    Import *import = rcast( res.ptr() );
//    import->filename = absolute_filename;
//    import->finished = false;

//    // parse
//    Ast_maker am( &el );
//    File_reader fr( path );
//    size_t old_el_size = el.size();
//    am.parse( fr.data, path.c_str(), display_lexems );
//    Ast_crepr ac = am.to_Ast_crepr();

//    if ( display_lexems ) {
//        AstVisitorDisplay ad( std::cout );
//        ast_visit( ad, ac.code );
//    }

//    // if parse error, return an Error var
//    if ( old_el_size != el.size() ) {
//        imported[ path.string() ] = ref_error;
//        return ref_error;
//    }

//    // exec
//    Scope loc( Scope::Scope_type::BLOCK, &main_scope );
//    loc.import = import;
//    loc.visit( ac, false );

    //
    //    for( Scope::NV *nv = loc.last_var; nv; nv = nv->prev )
    //        if ( nv->flags & Scope::VariableFlags::EXPORT )
    //            import->exports << Import::Export{ String( nv->name ), nv->var };
    //    std::reverse( import->exports.begin(), import->exports.end() );

//    import->finished = true;
    return res.var;
}

String Vm::path( const String &filename, const String &import_dir ) {
    // absolute filename
    boost::filesystem::path path = filename;
    if ( import_dir.empty() || path.is_absolute() ) {
        boost::system::error_code ec;
        boost::filesystem::path cano = boost::filesystem::canonical( path, import_dir, ec );
        return ec ? String{} : cano.string();
    }

    // ./... -> relative to import_dir
    if ( import_dir.size() ) {
        auto starts_with = [&]( const char *s ) {
            return filename.size() >= strlen( s ) && strncmp( filename.data(), s, strlen( s ) ) == 0;
        };
        if ( starts_with( "./" ) || starts_with( "../" ) || starts_with( ".\\" ) || starts_with( "..\\" ) ) {
            boost::system::error_code ec;
            boost::filesystem::path cano = boost::filesystem::canonical( path, import_dir, ec );
            return ec ? String{} : cano.string();
        }
    }

    // -> include lookup
    for( const String &include : includes ) {
        boost::system::error_code ec;
        boost::filesystem::path cano = boost::filesystem::canonical( path, include, ec );
        if ( ! ec )
            return cano.string();
    }

    return {};
}

//void Vm::disp_pos( const String &msg ) const {
//    Error_list::Error error;
//    //    bool disp = true;
//    //    for( const Scope *s = this; s; s = s->parent ) {
//    //        if ( s->pos.cur_names && disp ) {
//    //            error.ac( s->src_name( s->pos.cur_src ), s->pos.cur_off );
//    //            disp = false;
//    //        }

//    //        if ( s->type == Scope::Scope_type::CALL )
//    //            disp = true;
//    //    }
//    std::cerr << msg << ": " << error << std::endl;
//}

Error_list::Error &Vm::prep_Error( int nb_calls_to_skip, const String &msg ) {
    Error_list::Error &error = error_list.add( msg );
    //    bool disp = true;
    //    for( const Scope *s = this; s; s = s->parent ) {
    //        if ( s->pos.cur_names && disp ) {
    //            error.ac( s->src_name( s->pos.cur_src ), s->pos.cur_off );
    //            // disp = false;
    //        }

    //        if ( s->type == Scope::Scope_type::CALL )
    //            disp = true;
    //    }
    return error;
}

void Vm::disp_Error( const Error &error ) const {
    *error_stream << error;
}

