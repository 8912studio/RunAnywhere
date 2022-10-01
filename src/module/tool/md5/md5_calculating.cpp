#include "module/tool/md5/md5_calculating.h"
#include <fstream>
#include <boost/algorithm/hex.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include <zaf/base/error/check.h>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/encoding_conversion.h>

namespace ra::module::tool::md5 {
namespace {

std::wstring GetMD5String(boost::uuids::detail::md5& md5) {

    boost::uuids::detail::md5::digest_type digest;
    md5.get_digest(digest);

    std::string hex_string;
    boost::algorithm::hex(std::begin(digest), std::end(digest), std::back_inserter(hex_string));

    zaf::Lowercase(hex_string);
    return zaf::FromUtf8String(hex_string);
}

}

zaf::Observable<MD5Result> CalculateFileMD5(const std::filesystem::path& file_path) {

    return zaf::rx::Create<MD5Result>(
        zaf::Scheduler::CreateOnSingleThread(),
        [file_path](zaf::Observer<MD5Result> observer, zaf::CancelToken cancel_token) {

        std::ifstream file_stream{ file_path, std::ios::in | std::ios::binary };
        if (!file_stream) {
            observer.OnError(zaf::Error{ std::make_error_code(std::errc::io_error) });
            return;
        }

        file_stream.seekg(0, std::ios::end);
        std::streamsize remain_size = file_stream.tellg();
        file_stream.seekg(0, std::ios::beg);

        auto one_percent_size = double(remain_size) / 100;
        std::streamsize last_callback_size = 0;

        MD5Result result;
        result.total_size = remain_size;

        boost::uuids::detail::md5 md5;

        constexpr std::size_t buffer_size = 4096;
        auto buffer = std::make_unique<char[]>(buffer_size);

        while (remain_size > 0) {

            std::streamsize need_read_size = std::min(
                static_cast<std::streamsize>(buffer_size),
                remain_size);

            file_stream.read(buffer.get(), need_read_size);

            auto read_size = file_stream.gcount();
            if (read_size <= 0) {
                observer.OnError(zaf::Error{ std::make_error_code(std::errc::io_error) });
                return;
            }

            md5.process_bytes(buffer.get(), static_cast<std::size_t>(read_size));

            result.current_size += read_size;

            if (result.current_size - last_callback_size >= one_percent_size) {
                last_callback_size = result.current_size;
                observer.OnNext(result);
            }

            remain_size -= read_size;

            if (cancel_token.IsCancelled()) {
                return;
            }
        }

        result.md5 = GetMD5String(md5);
        observer.OnNext(result);
        observer.OnCompleted();
    });
}


std::wstring CalculateStringMD5(const std::wstring& string, MD5Encoding encoding) {

    boost::uuids::detail::md5 md5;

    if (encoding == MD5Encoding::UTF8) {
        auto utf8_string = zaf::ToUtf8String(string);
        md5.process_bytes(utf8_string.data(), utf8_string.length());
    }
    else if (encoding == MD5Encoding::UTF16) {
        md5.process_bytes(string.data(), string.length() * sizeof(wchar_t));
    }
    else {
        ZAF_NOT_REACHED();
    }

    return GetMD5String(md5);
}

}