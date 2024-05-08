#include "module/tool/hash/hash_calculating.h"
#include <fstream>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include <zaf/base/error/check.h>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/encoding_conversion.h>

namespace ra::mod::tool::hash {
namespace {

std::wstring GetHashString(zaf::crypto::HashAlgorithm& hash) {

    auto hex_string = hash.Finish().ToHexString();
    zaf::Lowercase(hex_string);
    return hex_string;
}

}

zaf::Observable<HashResult> CalculateFileHash(
    const std::filesystem::path& file_path,
    HashAlgorithmCreator hash_algorithm_creator) {

    return zaf::rx::Create<HashResult>(
        zaf::Scheduler::CreateOnSingleThread(),
        [file_path, hash_creator = std::move(hash_algorithm_creator)](
            zaf::Observer<HashResult> observer, 
            zaf::CancelToken cancel_token) {

        std::ifstream file_stream{ file_path, std::ios::in | std::ios::binary };
        if (!file_stream) {
            observer.OnError(std::system_error{ std::make_error_code(std::errc::io_error) });
            return;
        }

        file_stream.seekg(0, std::ios::end);
        std::streamsize remain_size = file_stream.tellg();
        file_stream.seekg(0, std::ios::beg);

        auto one_percent_size = double(remain_size) / 100;
        std::streamsize last_callback_size = 0;

        HashResult result;
        result.total_size = remain_size;

        auto hash = hash_creator();

        constexpr std::size_t buffer_size = 4096;
        auto buffer = std::make_unique<char[]>(buffer_size);

        while (remain_size > 0) {

            std::streamsize need_read_size = std::min(
                static_cast<std::streamsize>(buffer_size),
                remain_size);

            file_stream.read(buffer.get(), need_read_size);

            auto read_size = file_stream.gcount();
            if (read_size <= 0) {
                observer.OnError(std::system_error{ std::make_error_code(std::errc::io_error) });
                return;
            }

            hash.Update(buffer.get(), static_cast<std::size_t>(read_size));

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

        result.result = GetHashString(hash);
        observer.OnNext(result);
        observer.OnCompleted();
    });
}


std::wstring CalculateStringHash(
    const std::wstring& string, 
    TextEncoding encoding,
    const HashAlgorithmCreator& hash_algorithm_creator) {

    auto hash = hash_algorithm_creator();

    if (encoding == TextEncoding::UTF8) {
        auto utf8_string = zaf::ToUTF8String(string);
        hash.Update(utf8_string.data(), utf8_string.length());
    }
    else if (encoding == TextEncoding::UTF16) {
        hash.Update(
            reinterpret_cast<const std::byte*>(string.data()),
            string.length() * sizeof(wchar_t));
    }
    else {
        ZAF_NOT_REACHED();
    }

    return GetHashString(hash);
}

}