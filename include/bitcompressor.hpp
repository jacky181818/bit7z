#ifndef BITCOMPRESSOR_HPP
#define BITCOMPRESSOR_HPP

#include <iostream>
#include <vector>

#include "../include/bit7zlibrary.hpp"
#include "../include/bitformat.hpp"
#include "../include/bittypes.hpp"
#include "../include/bitcompressionlevel.hpp"

namespace bit7z {
    namespace filesystem {
        class FSItem; //avoids inclusion of fsitem.hpp in this header (and then in the release package)
    }

    using std::wstring;
    using std::vector;
    using filesystem::FSItem;

    /**
     * @brief The BitCompressor class allows to compress files and directories.
     *
     * It let decide various properties of the produced archive file, such as the password
     * protection and the compression level desired.
     */
    class BitCompressor {
        public:
            /**
             * @brief Constructs a BitCompressor object
             *
             * The Bit7zLibrary parameter is needed in order to have access to the functionalities
             * of the 7z DLLs. On the other hand, the BitInOutFormat is required in order to know the
             * format of the output archive.
             *
             * @param lib       the 7z library used.
             * @param format    the output archive format.
             */
            BitCompressor( const Bit7zLibrary& lib, const BitInOutFormat& format );

            /**
             * @return the archive format used by the compressor
             */
            const BitInOutFormat& compressionFormat();

            /**
             * @brief Sets up a password for the output archive
             *
             * When setting a password, the produced archive will be encrypted using the default
             * cryptographic method of the output format. If the format is 7z and the option
             * "crypt_headers" is set to true, also the headers of the archive will be encrypted,
             * resulting in a password request everytime the output file will be opened.
             *
             * @note Calling setPassword when the output format doesn't support archive encryption
             * (e.g. GZip, BZip2, etc...) does not have any effects (in other words, it doesn't
             * throw exceptions and it has no effects on compression operations).
             *
             * @note Calling setPassword with "crypt_headers" set to true does not have effects on
             * formats different from 7z.
             *
             * @note After a password has been set, it will be used for every compression operation.
             * To cancel the password, it must be performed a new call to setPassword with argument
             * an empty password.
             *
             * @param password          the password desired.
             * @param crypt_headers     if true, the headers of the output archive will be encrypted
             *                          (valid only with 7z format).
             */
            void setPassword( const wstring& password, bool crypt_headers = false );

            /**
             * @brief Sets the compression level to use when creating an archive
             *
             * @param compression_level the compression level desired.
             */
            void setCompressionLevel( BitCompressionLevel compression_level );

            /**
             * @brief Sets whether to use solid compression or not
             *
             * @note Setting the solid compression mode to true has effect only when using the 7z format.
             *
             * @param solid_mode    if true, it will be used the "solid compression" method.
             */
            void setSolidMode( bool solid_mode );

            /* Compression from file system to file system */

            /**
             * @brief Compresses the given files or directories
             *
             * The items in the first argument must be the relative or absolute paths to files or
             * directories existing on the filesystem.
             *
             * @param in_paths      a vector of paths.
             * @param out_archive   the path (relative or absolute) to the output archive file.
             */
            void compress( const vector< wstring >& in_paths, const wstring& out_archive ) const;

            /**
             * @brief Compresses a single file
             *
             * @param in_file       the path (relative or absolute) to the input file.
             * @param out_archive   the path (relative or absolute) to the output archive file.
             */
            void compressFile( const wstring& in_file, const wstring& out_archive ) const;

            /**
             * @brief Compresses a group of files
             *
             * @note Any path to a directory or to a not-existing file will be ignored!
             *
             * @param in_files      the path (relative or absolute) to the input files.
             * @param out_archive   the path (relative or absolute) to the output archive file.
             */
            void compressFiles( const vector< wstring >& in_files, const wstring& out_archive ) const;

            /**
             * @brief Compresses the files contained in a directory
             *
             * @param in_dir        the path (relative or absolute) to the input directory.
             * @param out_archive   the path (relative or absolute) to the output archive file.
             * @param filter        the filter to use when searching files inside in_dir.
             * @param recursive     if true, it searches files inside the sub-folders of in_dir.
             */
            void compressFiles( const wstring& in_dir, const wstring& out_archive, const wstring& filter = L"*",
                                bool recursive = true ) const;

            /**
             * @brief Compresses an entire directory
             *
             * @note This method is equivalent to compressFiles with filter set to L"*"
             *
             * @param in_dir        the path (relative or absolute) to the input directory.
             * @param out_archive   the path (relative or absolute) to the output archive file.
             * @param recursive     if true, it searches files inside the sub-folders of in_dir.
             */
            void compressDirectory( const wstring& in_dir, const wstring& out_archive, bool recursive = true ) const;

            /* Compression from file system to memory buffer */

            /**
             * @brief Compresses the input file to the output buffer
             *
             * @note If the format of the output archive doesn't support in memory compression, a BitException is thrown
             *
             * @param in_file           the file to be compressed
             * @param out_buffer        the buffer going to contain the output archive
             */
            void compressFile( const wstring& in_file, vector< byte_t >& out_buffer ) const;

        private:
            const Bit7zLibrary& mLibrary;
            const BitInOutFormat& mFormat;
            BitCompressionLevel mCompressionLevel;
            wstring mPassword;
            bool mCryptHeaders;
            bool mSolidMode;

            void compressToFileSystem( const vector< FSItem >& in_items, const wstring& out_archive ) const;
            void compressToMemory( const vector< FSItem >& in_items, vector< byte_t >& out_buffer ) const;
    };
}
#endif // BITCOMPRESSOR_HPP
