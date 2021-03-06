#include "../include/bitmemextractor.hpp"

#include "7zip/Archive/IArchive.h"
#include "Windows/COM.h"
#include "Windows/PropVariant.h"

#include "../include/bitexception.hpp"
#include "../include/opencallback.hpp"
#include "../include/memextractcallback.hpp"
#include "../include/extractcallback.hpp"

using namespace bit7z;
using namespace std;
using namespace NWindows;

// NOTE: this function is not a method of BitMemExtractor because it would dirty the header with extra dependencies
CMyComPtr< IInArchive > openArchive( const Bit7zLibrary &lib, const BitInFormat &format,
                                     const vector< byte_t >& in_buffer, const wstring &password ) {
    CMyComPtr< IInArchive > inArchive;
    const GUID formatGUID = format.guid();
    lib.createArchiveObject( &formatGUID, &::IID_IInArchive, reinterpret_cast< void** >( &inArchive ) );

    CBufInStream* bufStreamSpec = new CBufInStream;
    CMyComPtr< IInStream > bufStream( bufStreamSpec );
    bufStreamSpec->Init( &in_buffer[0], in_buffer.size() );

    OpenCallback* openCallbackSpec = new OpenCallback();
    openCallbackSpec->setPassword( password );

    CMyComPtr< IArchiveOpenCallback > openCallback( openCallbackSpec );
    if ( inArchive->Open( bufStream, 0, openCallback ) != S_OK ) {
        throw BitException( L"Cannot open archive buffer" );
    }
    return inArchive;
}

BitMemExtractor::BitMemExtractor(const Bit7zLibrary& lib, const BitInFormat &format ) :
    mLibrary( lib ),
    mFormat( format ),
    mPassword( L"" ) {}

const BitInFormat& BitMemExtractor::extractionFormat() {
    return mFormat;
}

void BitMemExtractor::extract( const vector< byte_t >& in_buffer, const wstring& out_dir ) const {
    CMyComPtr< IInArchive > inArchive = openArchive( mLibrary, mFormat, in_buffer, mPassword );

    ExtractCallback* extractCallbackSpec = new ExtractCallback( inArchive, out_dir );
    extractCallbackSpec->setPassword( mPassword );

    CMyComPtr< IArchiveExtractCallback > extractCallback( extractCallbackSpec );
    if ( inArchive->Extract( NULL, static_cast< UInt32 >( -1 ), false, extractCallback ) != S_OK ) {
        throw BitException( extractCallbackSpec->getErrorMessage() );
    }
}

void BitMemExtractor::extract( const vector< byte_t >& in_buffer, vector< byte_t >& out_buffer,
                               unsigned int index ) const {
    CMyComPtr< IInArchive > inArchive = openArchive( mLibrary, mFormat, in_buffer, mPassword );

    NCOM::CPropVariant prop;
    inArchive->GetProperty( index, kpidSize, &prop );

    MemExtractCallback* extractCallbackSpec = new MemExtractCallback( inArchive, out_buffer );
    extractCallbackSpec->setPassword( mPassword );

    const UInt32 indices[] = { index };

    CMyComPtr< IArchiveExtractCallback > extractCallback( extractCallbackSpec );
    if ( inArchive->Extract( indices, 1, false, extractCallback ) != S_OK ) {
        throw BitException( extractCallbackSpec->getErrorMessage() );
    }
}
