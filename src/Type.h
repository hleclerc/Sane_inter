#pragma once

#include "TypeContent.h"
#include "KnownRef.h"
/**
*/
class Type {
public:
    Type();


    KnownRef<TypeContent> content;
};

