#include <gmock/gmock.h>

#include "reader_interface.hpp"
#include "mocks/factory_mock.hpp"

class Reader {
public:
    pastebin::Paste get(pastebin::PublicKey key) { return {{"testText"},{1, "test", "test", "test"}}; };
    pastebin::PasteMetadata getMetadata(pastebin::PublicKey key) { return {}; };
    pastebin::PasteText getText(pastebin::PublicKey key) { return {}; };

    Reader() = default;
    Reader(MockFactory&& factory) {};
    Reader(Reader&& other) noexcept = default;
};

bool operator==(const pastebin::Paste& other, const pastebin::Paste& penis) {
    return true;
}

MATCHER_P(PasteEquals, expected, "Matches Paste objects") {
    return arg == expected;
}

using ::testing::Return;
using ::testing::ReturnRef;

static_assert(pastebin::reader::Reader<Reader>);

TEST(ReaderTest, Get) {
    MockCacheText cache_text;
    MockCacheMetadata cache_metadata;

    MockFactory factory;

    const pastebin::PublicKey key = "testKey";

    const pastebin::PasteText text{"testText"};
    const pastebin::PasteMetadata metadata{1, "test", "test", "test"}; 

    const pastebin::PublicKey key_text = key;
    ON_CALL(cache_text, get(key_text))
        .WillByDefault(Return(text));
    EXPECT_CALL(cache_text, get(key_text))
        .Times(1);

    const pastebin::PublicKey key_metadata = key;
    ON_CALL(cache_metadata, get(key_metadata))
        .WillByDefault(Return(metadata));    
    EXPECT_CALL(cache_metadata, get(key_metadata))
        .Times(1);

    ON_CALL(factory, getCacheText())
        .WillByDefault(ReturnRef(cache_text));
    EXPECT_CALL(factory, getCacheText())
        .Times(1)
        .WillOnce(ReturnRef(cache_text));
    
    ON_CALL(factory, getCacheMetadata())
        .WillByDefault(ReturnRef(cache_metadata));
    EXPECT_CALL(factory, getCacheMetadata())
        .Times(1)
        .WillOnce(ReturnRef(cache_metadata));

    Reader reader(std::move(factory)); 

    const auto res = reader.get(key);
    
    const pastebin::Paste expected_paste{text, metadata};
    
    EXPECT_THAT(res, PasteEquals(expected_paste));
}



