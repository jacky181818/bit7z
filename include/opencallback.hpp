#ifndef OPENCALLBACK_HPP
#define OPENCALLBACK_HPP

#include <string>

#include "7zip/Archive/IArchive.h"
#include "7zip/IPassword.h"
#include "Common/MyCom.h"

#include "../include/callback.hpp"

namespace bit7z {
    class OpenCallback : public IArchiveOpenCallback, ICryptoGetTextPassword, CMyUnknownImp, public Callback {
        public:
            OpenCallback();
            virtual ~OpenCallback();

            MY_UNKNOWN_IMP1( ICryptoGetTextPassword )

            STDMETHOD( SetTotal )( const UInt64 * files, const UInt64 * bytes );
            STDMETHOD( SetCompleted )( const UInt64 * files, const UInt64 * bytes );

            STDMETHOD( CryptoGetTextPassword )( BSTR * password );
    };
}
#endif // OPENCALLBACK_HPP
