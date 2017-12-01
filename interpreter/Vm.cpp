#include "System/File_reader.h"
#include "System/Error_list.h"
#include "System/rcast.h"
#include "Ast/Ast_visitor.h"
#include "Ast/Ast_maker.h"
#include "InterpTypeAttribute.h"
#include "AnonymousRoom.h"
#include "Primitives.h"
#include "Wildcard.h"
#include "Varargs.h"
#include "Import.h"
#include "String.h"
#include "CtMap.h"
#include "AT.h"
#include "Vm.h"
#include "Type_ListOfTypesOrSurdefLists.h"
#include "Type_CallableWithSelfAndArgs.h"
#include "Type_DelayedImportedVar.h"
#include "Type_DelayedVarInScope.h"
#include "Type_AnonymousRoom.h"
#include "Type_SlTrialClass.h"
#include "Type_SlTrialDef.h"
#include "Type_SurdefList.h"
#include "Type_GetSetter.h"
#include "Type_ScopeRef.h"
#include "Type_Wildcard.h"
#include "Type_Varargs.h"
#include "Type_Import.h"
#include "Type_Lambda.h"
#include "Type_Block.h"
#include "Type_Class.h"
#include "Type_Error.h"
#include "Type_Type.h"
#include "Type_Void.h"
#include "Type_Def.h"
#include "Type_BT.h"
#include "Type_AT.h"
#include <algorithm>

Vm::Vm() : main_scope( this ) {
    init_mode = true;
    os        = &std::cerr;
    nb_calls  = 0;
    nb_enum   = 0;

    #define BT( T ) type_##T = 0;
    #include "BaseTypes.h"
    #undef BT

    type_ListOfTypesOrSurdefLists = types.push_back_val( new Type_ListOfTypesOrSurdefLists                        );
    type_CallableWithSelfAndArgs  = types.push_back_val( new Type_CallableWithSelfAndArgs                         );
    type_DelayedImportedVar       = types.push_back_val( new Type_DelayedImportedVar                              );
    type_DelayedVarInScope        = types.push_back_val( new Type_DelayedVarInScope                               );
    type_PrimitiveNumber          = types.push_back_val( new Type( "PrimitiveNumber" )                            );
    type_AnonymousRoom            = types.push_back_val( new Type_AnonymousRoom                                   );
    type_TypeAttribute            = types.push_back_val( new Type_BaseBin<InterpTypeAttribute>( "TypeAttribute" ) );
    type_SlTrialClass             = types.push_back_val( new Type_SlTrialClass                                    );
    type_InterpScope              = types.push_back_val( new Type_BaseBin<Scope *>( "Scope" )                     );
    type_SlTrialDef               = types.push_back_val( new Type_SlTrialDef                                      );
    type_SurdefList               = types.push_back_val( new Type_SurdefList                                      );
    type_UntypedRef               = types.push_back_val( new Type_BaseBin<Variable>( "UntypedRef" )               );
    type_GetSetter                = types.push_back_val( new Type_GetSetter                                       );
    type_Wildcard                 = types.push_back_val( new Type_Wildcard                                        );
    type_ScopeRef                 = types.push_back_val( new Type_ScopeRef                                        );
    type_NullPtr                  = types.push_back_val( new Type( "NullPtr" )                                    );
    type_Varargs                  = types.push_back_val( new Type_Varargs                                         );
    type_Number                   = types.push_back_val( new Type( "Number" )                                     );
    type_Import                   = types.push_back_val( new Type_Import                                          );
    type_String                   = types.push_back_val( new Type_BaseBin<String>( "String" )                     );
    type_Lambda                   = types.push_back_val( new Type_Lambda                                          );
    type_CtVec                    = types.push_back_val( new Type_BaseBin<LVec<Variable>>( "CtVec" )              );
    type_CtMap                    = types.push_back_val( new Type_BaseBin<CtMap>( "CtMap" )                       );
    type_Class                    = types.push_back_val( new Type_Class                                           );
    type_Block                    = types.push_back_val( new Type_Block                                           );
    type_Error                    = types.push_back_val( new Type_Error                                           );
    type_Type                     = types.push_back_val( new Type_Type                                            );
    type_Void                     = types.push_back_val( new Type_Void                                            );
    type_Char                     = types.push_back_val( new Type_BaseBin<char>( "Char" )                         );
    type_Bool                     = types.push_back_val( new Type_BT<PI8 >( "Bool" )                              );
    type_SI64                     = types.push_back_val( new Type_BT<SI64>( "SI64" )                              );
    type_PI64                     = types.push_back_val( new Type_BT<PI64>( "PI64" )                              );
    type_SI32                     = types.push_back_val( new Type_BT<SI32>( "SI32" )                              );
    type_PI32                     = types.push_back_val( new Type_BT<PI32>( "PI32" )                              );
    type_SI16                     = types.push_back_val( new Type_BT<SI16>( "SI16" )                              );
    type_PI16                     = types.push_back_val( new Type_BT<PI16>( "PI16" )                              );
    type_SI8                      = types.push_back_val( new Type_BT<SI8 >( "SI8"  )                              );
    type_PI8                      = types.push_back_val( new Type_BT<PI8 >( "PI8"  )                              );
    type_PT                       = types.push_back_val( new Type_BT<PT  >( "PT"   )                              );
    type_ST                       = types.push_back_val( new Type_BT<ST  >( "ST"   )                              );
    type_AT                       = types.push_back_val( new Type_AT                                              );
    type_Def                      = types.push_back_val( new Type_Def                                             );

    ref_error                     = Variable( this, type_Error );
    ref_void                      = Variable( this, type_Void );

    for( Primitive_decl *pd = last_Primitive_decl; pd; pd = pd->prev )
        predefs[ Rc_string( "__primitive_" ) + pd->name ] = Variable( this, types.push_back_val( pd->func() ) );

    for( Primitive_func *pd = last_Primitive_func; pd; pd = pd->prev )
        predeffs[ pd->name ] = pd->func;

    #define BT( T ) base_types[ #T ] = type_##T; ASSERT( type_##T, "..." );
    #include "BaseTypes.h"
    #undef BT
}

Vm::~Vm() {
    imported  .clear();
    main_scope.clear();
    predefs   .clear();
    ref_error .clear();
    ref_void  .clear();

    while ( ! types.empty() )
        delete types.pop_back_val();
}

Variable Vm::import( const Rc_string &filename, const Rc_string &import_dir ) {
    // /...
    std::experimental::filesystem::path path = std::string( filename );
    if ( path.is_absolute() )
        return import( filename );

    if ( filename.begins_with( "./" ) || filename.begins_with( "../" ) || filename.begins_with( ".\\" ) || filename.begins_with( ".\\" ) ) {
        std::error_code ec;
        std::experimental::filesystem::path cano = std::experimental::filesystem::canonical( path, std::string( import_dir ), ec );
        if ( ! ec )
            return import( cano.string() );
    } else {
        for( const Rc_string &include : includes ) {
            std::error_code ec;
            std::experimental::filesystem::path cano = std::experimental::filesystem::canonical( path, std::string( include ), ec );
            if ( ! ec )
                return import( cano.string() );
        }
    }

    return main_scope.add_error( "file {} not found (from {})", filename, import_dir ), ref_error;
}

Variable Vm::import( const Rc_string &absolute_filename, bool display_lexems ) {
    std::error_code ec;
    std::experimental::filesystem::path path = std::experimental::filesystem::canonical( std::string( absolute_filename ), std::experimental::filesystem::current_path(), ec );
    if ( ec )
        return main_scope.add_error( "file {} does not exist", absolute_filename ), ref_error;

    // already imported ?
    auto iter = imported.find( path.string() );
    if ( iter != imported.end() )
        return iter->second;

    // prepare a result variable
    Variable res( this, type_Import );
    imported.insert( iter, std::make_pair( path.string(), res ) );

    Import *import = rcast( res.ptr() );
    import->filename = absolute_filename;
    import->finished = false;

    // parse
    Ast_maker am( &el );
    File_reader fr( path );
    size_t old_el_size = el.size();
    am.parse( fr.data, path.c_str(), display_lexems );
    Ast_crepr ac = am.to_Ast_crepr();

    if ( display_lexems ) {
        AstVisitorDisplay ad( std::cout );
        ast_visit( ad, ac.code );
    }

    // if parse error, return an Error var
    if ( old_el_size != el.size() ) {
        imported[ path.string() ] = ref_error;
        return ref_error;
    }

    // exec
    Scope loc( Scope::Scope_type::BLOCK, &main_scope );
    loc.import = import;
    loc.visit( ac, false );

    //
    //    for( Scope::NV *nv = loc.last_var; nv; nv = nv->prev )
    //        if ( nv->flags & Scope::VariableFlags::EXPORT )
    //            import->exports << Import::Export{ String( nv->name ), nv->var };
    //    std::reverse( import->exports.begin(), import->exports.end() );

    import->finished = true;
    return res;
}

Type *Vm::type_ptr_for( const Rc_string &name, const Vec<Variable> &args ) {
    if ( init_mode ) {
        auto iter = base_types.find( name );
        if ( iter != base_types.end() )
            return iter->second;
    }
    Type *res = types.push_back_val( new Type( name ) );
    for( const Variable &arg : args )
        res->parameters << main_scope.add_static_variable( init_mode ? arg : arg.find_attribute( &main_scope, "operator :=" ).apply( &main_scope, true ) ); // TODO: make a constified copy
    return res;
}

Variable Vm::new_String( const Rc_string &str ) {
    Variable v_res( this, type_String );
    String *res = rcast( v_res.ptr() );
    res->init( str );
    return v_res;
}

Variable Vm::new_Type( Type *type ) { return { this, type_Type, &type  }; }
Variable Vm::new_Bool( Bool value ) { return { this, type_Bool, &value }; }
Variable Vm::new_SI32( SI32 value ) { return { this, type_SI32, &value }; }
Variable Vm::new_PT  ( PT   value ) { return { this, type_PT  , &value }; }
