#pragma once

#include <gmock/gmock.h>

#include "reader_interface.hpp"

namespace pastebin::mock {

class MockReader {
public:
    MOCK_METHOD(pastebin::Paste, get, (pastebin::PublicKey));
    MOCK_METHOD(pastebin::PasteText, getText, (pastebin::PublicKey));
    MOCK_METHOD(pastebin::PasteMetadata, getMetadata, (pastebin::PublicKey));

    MockReader() = default;

    // non-copyable
    MockReader(MockReader& other) = delete;
    MockReader& operator=(MockReader& other) = delete;

    // movable
    MockReader(MockReader&& other) noexcept {};
    MockReader& operator=(MockReader&& other) noexcept = default;
    
    ~MockReader() = default;
};

class WrapperMockReader {
public:
    WrapperMockReader(MockReader* reader) 
        : reader_(reader) 
    {}

    // non-copyable
    WrapperMockReader(WrapperMockReader& other) = delete;
    WrapperMockReader& operator=(WrapperMockReader& other) = delete;

    // movable
    WrapperMockReader(WrapperMockReader&& other) noexcept = default;
    WrapperMockReader& operator=(WrapperMockReader&& other) noexcept = default;
    
    ~WrapperMockReader() = default;

    pastebin::Paste get(pastebin::PublicKey public_key) {
        return reader_->get(public_key);
    }

    pastebin::PasteText getText(pastebin::PublicKey public_key) {
        return reader_->getText(public_key);
    }

    pastebin::PasteMetadata getMetadata(pastebin::PublicKey public_key) {
        return reader_->getMetadata(public_key);
    }

private:
   MockReader* reader_;
};

} // namespace pastebin::mock

static_assert(pastebin::reader::IReader<pastebin::mock::MockReader>);
static_assert(pastebin::reader::IReader<pastebin::mock::WrapperMockReader>);