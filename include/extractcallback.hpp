#ifndef EXTRACTCALLBACK_HPP
#define EXTRACTCALLBACK_HPP

#include <string>

#include "7zip/Archive/IArchive.h"
#include "7zip/Common/FileStreams.h"
#include "7zip/IPassword.h"
#include "Common/MyCom.h"

#include "../include/bitguids.hpp"
#include "../include/callback.hpp"

namespace bit7z {
    using std::wstring;

    class ExtractCallback : public IArchiveExtractCallback, ICryptoGetTextPassword, CMyUnknownImp, public Callback {
        public:
            ExtractCallback( IInArchive* archiveHandler, const wstring& directoryPath );
            virtual ~ExtractCallback();

            MY_UNKNOWN_IMP1( ICryptoGetTextPassword )

            // IProgress
            STDMETHOD( SetTotal )( UInt64 size );
            STDMETHOD( SetCompleted )( const UInt64 * completeValue );

            // IArchiveExtractCallback
            STDMETHOD( GetStream )( UInt32 index, ISequentialOutStream * *outStream, Int32 askExtractMode );
            STDMETHOD( PrepareOperation )( Int32 askExtractMode );
            STDMETHOD( SetOperationResult )( Int32 resultEOperationResult );

            // ICryptoGetTextPassword
            STDMETHOD( CryptoGetTextPassword )( BSTR * aPassword );

        private:
            CMyComPtr< IInArchive > mArchiveHandler;
            wstring mDirectoryPath;  // Output directory
            wstring mFilePath;       // name inside arcvhive
            wstring mDiskFilePath;   // full path to file on disk
            bool mExtractMode;
            struct CProcessedFileInfo {
                FILETIME MTime;
                UInt32 Attrib;
                bool isDir;
                bool AttribDefined;
                bool MTimeDefined;
            } mProcessedFileInfo;

            COutFileStream* mOutFileStreamSpec;
            CMyComPtr< ISequentialOutStream > mOutFileStream;

            UInt64 mNumErrors;
    };
}
#endif // EXTRACTCALLBACK_HPP
