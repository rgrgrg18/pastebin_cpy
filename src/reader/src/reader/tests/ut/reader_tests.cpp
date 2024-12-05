#include <gmock/gmock.h>

#include "comparisition_paste_types.hpp"
#include "mocks/factory_mock.hpp"
#include "random_paste_generator.hpp"
#include "reader_interface.hpp"

#include "reader.hpp"

using Reader = pastebin::reader::Reader;

static_assert(pastebin::reader::IReader<Reader>);

MATCHER_P(PasteEquals, expected, "Matches Paste objects") {
    return arg == expected;
}

MATCHER_P(PasteTextEquals, expected, "Matches PasteText objects") {
    return arg == expected;
}

MATCHER_P(PasteMetadataEquals, expected, "Matches PasteMetadata objects") {
    return arg == expected;
}

using ::testing::Return;
using ::testing::ReturnRef;

using utility::testing::GeneratePublicKey;
using utility::testing::GeneratePaste;
using utility::testing::GeneratePasteText;
using utility::testing::GeneratePasteMetadata;

using namespace pastebin::reader::mock;

TEST(ReaderUnitTest, DefaultGet) {
    MockCacheText cache_text;
    MockCacheMetadata cache_metadata;

    MockFactory factory;

    uint64_t seed = 5432;

    for (uint64_t i = 0; i < 1'000; ++i) {
        const std::string public_key = GeneratePublicKey(seed);
        const pastebin::PublicKey key = public_key;

        const pastebin::Paste expected_paste = GeneratePaste(seed);

        ON_CALL(cache_text, get(key))
            .WillByDefault(Return(expected_paste.paste_text));
        EXPECT_CALL(cache_text, get(key))
            .Times(1);

        ON_CALL(cache_metadata, get(key))
            .WillByDefault(Return(expected_paste.paste_metadata));    
        EXPECT_CALL(cache_metadata, get(key))
            .Times(1);

        ON_CALL(factory, getCacheText())
            .WillByDefault(ReturnRef(cache_text));
        EXPECT_CALL(factory, getCacheText())
            .Times(1);
        
        ON_CALL(factory, getCacheMetadata())
            .WillByDefault(ReturnRef(cache_metadata));
        EXPECT_CALL(factory, getCacheMetadata())
            .Times(1);

        Reader reader(std::move(WrapperMockFactory(&factory)));
        
        const pastebin::Paste res = reader.get(key);
        
        EXPECT_THAT(res, PasteEquals(expected_paste));
    }
}

TEST(ReaderUnitTest, DefaultGetText) {
    MockCacheText cache_text;

    MockFactory factory;

    uint64_t seed = 5432;

    for (uint64_t i = 0; i < 1'000; ++i) {
        const std::string public_key = GeneratePublicKey(seed);
        const pastebin::PublicKey key = public_key;

        const pastebin::PasteText expected_text = GeneratePasteText(seed);

        ON_CALL(cache_text, get(key))
            .WillByDefault(Return(expected_text));
        EXPECT_CALL(cache_text, get(key))
            .Times(1);

        ON_CALL(factory, getCacheText())
            .WillByDefault(ReturnRef(cache_text));
        EXPECT_CALL(factory, getCacheText())
            .Times(1);

        Reader reader(std::move(WrapperMockFactory(&factory)));
        
        const pastebin::PasteText res = reader.getText(key);

        EXPECT_THAT(res, PasteTextEquals(expected_text));
    }
}

TEST(ReaderUnitTest, DefaultGetMetadata) {
    MockCacheMetadata cache_metadata;

    MockFactory factory;

    uint64_t seed = 5432;

    for (uint64_t i = 0; i < 500'000; ++i) {
        const std::string public_key = GeneratePublicKey(seed);
        const pastebin::PublicKey key = public_key;

        const pastebin::PasteMetadata expected_metadata = GeneratePasteMetadata(seed); 

        ON_CALL(cache_metadata, get(key))
            .WillByDefault(Return(expected_metadata));    
        EXPECT_CALL(cache_metadata, get(key))
            .Times(1);
        
        ON_CALL(factory, getCacheMetadata())
            .WillByDefault(ReturnRef(cache_metadata));
        EXPECT_CALL(factory, getCacheMetadata())
            .Times(1);

        Reader reader(std::move(WrapperMockFactory(&factory)));

        const pastebin::PasteMetadata res = reader.getMetadata(key);
        
        EXPECT_THAT(res, PasteEquals(expected_metadata));
    }
}